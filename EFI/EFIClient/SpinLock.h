#pragma once
#include <atomic>

class SpinLock {
private:
    std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
    // Add nested lock_guard class
    class lock_guard {
    private:
        SpinLock& lock;
    public:
        explicit lock_guard(SpinLock& lock) : lock(lock) {
            lock.lock();
        }
        ~lock_guard() {
            lock.unlock();
        }
        // Prevent copying and moving
        lock_guard(const lock_guard&) = delete;
        lock_guard& operator=(const lock_guard&) = delete;
    };

    SpinLock() = default;
    ~SpinLock() = default;

    // Prevent copying and moving
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;
    SpinLock(SpinLock&&) = delete;
    SpinLock& operator=(SpinLock&&) = delete;

    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) {
            #if defined(_MSC_VER)
                _mm_pause();
            #elif defined(__GNUC__)
                __builtin_ia32_pause();
            #endif
        }
    }

    void unlock() {
        locked.clear(std::memory_order_release);
    }

    bool try_lock() {
        return !locked.test_and_set(std::memory_order_acquire);
    }
}; 