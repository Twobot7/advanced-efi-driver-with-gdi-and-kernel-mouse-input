#pragma once
#include "Common.h"

namespace Driver::Input {
    class MouseDriver {
    private:
        HANDLE deviceHandle;
        DWORD pollingRate;
        
    public:
        MouseDriver();
        ~MouseDriver();
        
        bool Initialize(DWORD pollRate);
        MouseState GetState();
        void Shutdown();
        
    protected:
        bool RegisterMouseDevice();
        void ProcessRawInput(PRAWINPUT raw);
    };
} 