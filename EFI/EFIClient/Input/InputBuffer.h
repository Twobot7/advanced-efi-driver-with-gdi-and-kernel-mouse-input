#pragma once
#include "Common.h"
#include <queue>

namespace Driver::Input {
    struct InputEvent {
        enum class Type {
            Mouse,
            Keyboard
        } eventType;
        
        union {
            MouseState mouseState;
            KeyboardState keyboardState;
        } data;
        
        uint64_t timestamp;
    };

    class InputBuffer {
    private:
        static constexpr size_t MAX_BUFFER_SIZE = 1024;
        std::queue<InputEvent> eventQueue;
        SpinLock bufferLock;

    public:
        bool PushEvent(const InputEvent& event);
        bool PopEvent(InputEvent& outEvent);
        void Clear();
    };
} 