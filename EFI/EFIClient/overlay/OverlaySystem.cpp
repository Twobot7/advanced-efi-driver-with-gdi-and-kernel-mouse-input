#include "OverlaySystem.h"
#include <sstream>
#include <iomanip>
#include <format>

OverlaySystem::OverlaySystem() 
    : overlayWindow(nullptr)
    , memoryDC(nullptr)
    , memoryBitmap(nullptr)
    , oldBitmap(nullptr)
    , width(0)
    , height(0)
    , targetWindow(nullptr)
    , colorKey(RGB(0, 0, 0))
    , currentBlendMode(BlendMode::ColorKey)
    , targetFPS(60)
    , isActive(true) {
    
    lastFrameTime = std::chrono::steady_clock::now();
    logFile.open("overlay_log.txt", std::ios::app);
    LogInfo("OverlaySystem initialized");
}

OverlaySystem::~OverlaySystem() {
    Cleanup();
}

LRESULT CALLBACK OverlaySystem::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    OverlaySystem* overlay = reinterpret_cast<OverlaySystem*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    
    switch (uMsg) {
        case WM_CREATE: {
            CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_TIMER: {
            if (overlay) {
                overlay->UpdateOverlayPosition();
            }
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool OverlaySystem::CreateOverlayWindow() {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"OverlayWindowClass";
    
    if (!RegisterClassEx(&wc)) {
        return false;
    }

    overlayWindow = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        L"OverlayWindowClass",
        L"Overlay",
        WS_POPUP,
        0, 0, width, height,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        this
    );

    if (!overlayWindow) {
        return false;
    }

    SetTimer(overlayWindow, 1, 16, NULL);

    SetLayeredWindowAttributes(overlayWindow, RGB(0, 0, 0), 255, LWA_COLORKEY);
    ShowWindow(overlayWindow, SW_SHOW);
    
    return true;
}

bool OverlaySystem::Initialize(int w, int h) {
    width = w;
    height = h;

    if (!CreateOverlayWindow()) {
        LogError("Failed to create overlay window");
        return false;
    }

    HDC windowDC = GetDC(overlayWindow);
    if (!windowDC) {
        LogError("Failed to get window DC");
        return false;
    }

    memoryDC = CreateCompatibleDC(windowDC);
    if (!memoryDC) {
        LogError("Failed to create memory DC");
        ReleaseDC(overlayWindow, windowDC);
        return false;
    }

    memoryBitmap = CreateCompatibleBitmap(windowDC, width, height);
    if (!memoryBitmap) {
        LogError("Failed to create memory bitmap");
        DeleteDC(memoryDC);
        ReleaseDC(overlayWindow, windowDC);
        return false;
    }

    oldBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitmap);
    ReleaseDC(overlayWindow, windowDC);

    LogInfo("Overlay initialized successfully");
    return true;
}

bool OverlaySystem::RenderFrame(const void* data, size_t size) {
    if (!isActive) return true;
    if (!overlayWindow || !memoryDC) {
        LogError("Cannot render frame - overlay not properly initialized");
        return false;
    }

    WaitForNextFrame();

    // Copy the data to the memory DC
    BITMAPINFO* bmi = (BITMAPINFO*)data;
    if (!SetDIBitsToDevice(
        memoryDC,
        0, 0, width, height,
        0, 0, 0, height,
        (BYTE*)data + sizeof(BITMAPINFOHEADER),
        bmi,
        DIB_RGB_COLORS
    )) {
        LogError("Failed to set DIB bits to device");
        return false;
    }

    HDC windowDC = GetDC(overlayWindow);
    if (!windowDC) {
        LogError("Failed to get window DC for rendering");
        return false;
    }

    bool success = true;
    if (!BitBlt(windowDC, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY)) {
        LogError("Failed to BitBlt to window");
        success = false;
    }

    ReleaseDC(overlayWindow, windowDC);
    return success;
}

void OverlaySystem::Cleanup() {
    if (oldBitmap) {
        SelectObject(memoryDC, oldBitmap);
        oldBitmap = nullptr;
    }
    if (memoryBitmap) {
        DeleteObject(memoryBitmap);
        memoryBitmap = nullptr;
    }
    if (memoryDC) {
        DeleteDC(memoryDC);
        memoryDC = nullptr;
    }
    if (overlayWindow) {
        DestroyWindow(overlayWindow);
        overlayWindow = nullptr;
    }
}

bool OverlaySystem::AttachToWindow(const wchar_t* windowName) {
    targetWindow = FindWindow(NULL, windowName);
    if (!targetWindow) {
        return false;
    }
    
    // Initial position update
    return UpdateOverlayPosition();
}

bool OverlaySystem::UpdateOverlayPosition() {
    if (!targetWindow || !overlayWindow) {
        return false;
    }

    RECT targetRect;
    if (!GetWindowRect(targetWindow, &targetRect)) {
        return false;
    }

    // Check if the window is minimized
    if (IsIconic(targetWindow)) {
        ShowWindow(overlayWindow, SW_HIDE);
        return true;
    }

    // Update overlay size and position
    SetWindowPos(
        overlayWindow,
        HWND_TOPMOST,
        targetRect.left,
        targetRect.top,
        targetRect.right - targetRect.left,
        targetRect.bottom - targetRect.top,
        SWP_NOACTIVATE
    );

    ShowWindow(overlayWindow, SW_SHOW);
    return true;
}

bool OverlaySystem::SetTransparency(BYTE alpha) {
    if (!overlayWindow) {
        return false;
    }
    return SetLayeredWindowAttributes(overlayWindow, RGB(0, 0, 0), alpha, LWA_COLORKEY | LWA_ALPHA);
}

bool OverlaySystem::SetColorKey(COLORREF color) {
    if (!overlayWindow) {
        LogError("Cannot set color key - window not initialized");
        return false;
    }
    colorKey = color;
    return SetLayeredWindowAttributes(overlayWindow, colorKey, 255, LWA_COLORKEY);
}

bool OverlaySystem::SetBlendMode(BlendMode mode) {
    currentBlendMode = mode;
    if (!overlayWindow) {
        LogError("Cannot set blend mode - window not initialized");
        return false;
    }

    DWORD flags = 0;
    switch (mode) {
        case BlendMode::ColorKey:
            flags = LWA_COLORKEY;
            break;
        case BlendMode::Normal:
            flags = LWA_ALPHA;
            break;
        case BlendMode::Additive:
            // Custom blend function could be set here using UpdateLayeredWindow
            break;
        case BlendMode::Multiply:
            // Custom blend function could be set here using UpdateLayeredWindow
            break;
    }

    return SetLayeredWindowAttributes(overlayWindow, colorKey, 255, flags);
}

bool OverlaySystem::SetTargetFPS(int fps) {
    if (fps <= 0) {
        LogError("Invalid FPS value");
        return false;
    }
    targetFPS = fps;
    return true;
}

bool OverlaySystem::WaitForNextFrame() {
    if (targetFPS <= 0) return true;

    auto now = std::chrono::steady_clock::now();
    auto frameTime = std::chrono::microseconds(1000000 / targetFPS);
    auto elapsed = now - lastFrameTime;

    if (elapsed < frameTime) {
        auto sleepTime = frameTime - elapsed;
        std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::microseconds>(sleepTime));
    }

    lastFrameTime = std::chrono::steady_clock::now();
    return true;
}

bool OverlaySystem::BringToFront() {
    if (!overlayWindow) {
        LogError("Cannot bring to front - window not initialized");
        return false;
    }

    if (!SetWindowPos(overlayWindow, HWND_TOPMOST, 0, 0, 0, 0, 
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE)) {
        LogError("Failed to bring window to front");
        return false;
    }

    return true;
}

bool OverlaySystem::SetActive(bool active) {
    isActive = active;
    if (!overlayWindow) {
        LogError("Cannot set active state - window not initialized");
        return false;
    }

    ShowWindow(overlayWindow, active ? SW_SHOW : SW_HIDE);
    return true;
}

void OverlaySystem::LogError(const std::string& message) {
    if (!logFile.is_open()) return;
    
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << "[ERROR][" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "] " << message;
    
    logFile << ss.str() << std::endl;
    logFile.flush();
}

void OverlaySystem::LogInfo(const std::string& message) {
    if (!logFile.is_open()) return;
    
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << "[INFO][" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "] " << message;
    
    logFile << ss.str() << std::endl;
    logFile.flush();
} 