#pragma once
#include <Windows.h>
#include <chrono>
#include <string>
#include <fstream>

enum class BlendMode {
    Normal,
    Additive,
    Multiply,
    ColorKey
};

class OverlaySystem {
private:
    HWND overlayWindow;
    HDC memoryDC;
    HBITMAP memoryBitmap;
    HBITMAP oldBitmap;
    int width;
    int height;
    HWND targetWindow;
    
    // State members
    COLORREF colorKey;
    BlendMode currentBlendMode;
    int targetFPS;
    std::chrono::steady_clock::time_point lastFrameTime;
    bool isActive;
    std::ofstream logFile;

    bool CreateOverlayWindow();
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool UpdateOverlayPosition();
    void LogError(const std::string& message);
    void LogInfo(const std::string& message);
    bool WaitForNextFrame();

public:
    OverlaySystem();
    ~OverlaySystem();
    bool Initialize(int width, int height);
    bool RenderFrame(const void* data, size_t size);
    void Cleanup();
    bool AttachToWindow(const wchar_t* windowName);
    bool SetTransparency(BYTE alpha);
    
    // New methods
    bool SetColorKey(COLORREF color);
    bool SetBlendMode(BlendMode mode);
    bool SetTargetFPS(int fps);
    bool BringToFront();
    bool SetActive(bool active);
    bool IsActive() const { return isActive; }
}; 