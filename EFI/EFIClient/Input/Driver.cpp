#include "Driver.h"
#include "InputManager.h"
#include "MouseDriver.h"
#include "KeyboardDriver.h"

namespace Driver::Input {
    namespace {
        constexpr DWORD MOUSE_POLLING_RATE = 1000;    // 1000Hz polling rate
        constexpr DWORD KB_POLLING_RATE = 1000;       // 1000Hz polling rate
        
        InputManager* g_InputManager = nullptr;
        SpinLock inputLock;
        volatile bool isDriverActive = false;
    }

    class InputSystemManager {
    private:
        MouseDriver mouseDriver;
        KeyboardDriver keyboardDriver;
        InputBuffer inputBuffer;

    public:
        bool Initialize() {
            SpinLock::lock_guard guard(inputLock);
            
            if (!mouseDriver.Initialize(MOUSE_POLLING_RATE) ||
                !keyboardDriver.Initialize(KB_POLLING_RATE)) {
                return false;
            }

            isDriverActive = true;
            return true;
        }

        void ProcessInput() {
            if (!isDriverActive) return;

            SpinLock::lock_guard guard(inputLock);
            
            // Process mouse input
            MouseState mouseState = mouseDriver.GetState();
            if (mouseState.HasChanged()) {
                inputBuffer.PushMouseEvent(mouseState);
            }

            // Process keyboard input
            KeyboardState kbState = keyboardDriver.GetState();
            if (kbState.HasNewKeys()) {
                inputBuffer.PushKeyboardEvent(kbState);
            }
        }

        void Shutdown() {
            SpinLock::lock_guard guard(inputLock);
            isDriverActive = false;
            mouseDriver.Shutdown();
            keyboardDriver.Shutdown();
        }
    };
} 