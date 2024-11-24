#pragma once
#include "Common.h"

namespace Driver::Input {
    class KeyboardDriver {
    private:
        HANDLE deviceHandle;
        DWORD pollingRate;
        KeyboardState currentState;
        
    public:
        KeyboardDriver();
        ~KeyboardDriver();
        
        bool Initialize(DWORD pollRate);
        KeyboardState GetState();
        void Shutdown();
        
    protected:
        bool RegisterKeyboardDevice();
        void ProcessKeyboardInput(PRAWKEYBOARD raw);
    };
} 