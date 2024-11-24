#pragma once
#include "Common.h"

namespace Driver::Input {
    class InputManager {
    public:
        struct MouseState {
            int32_t x;
            int32_t y;
            int32_t deltaX;
            int32_t deltaY;
            uint32_t buttons;
            bool HasChanged() const;
        };

        struct KeyboardState {
            uint8_t keys[256];
            uint8_t previousKeys[256];
            bool HasNewKeys() const;
        };

        virtual bool Initialize() = 0;
        virtual void ProcessInput() = 0;
        virtual void Shutdown() = 0;
    };
} 