#pragma once
#include <thread>
#include <atomic>
#include <vector>
#include <functional>
#include <mutex>

namespace EFIClient {

    class SpinLock {
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
    public:
        void lock() {
            while (flag.test_and_set(std::memory_order_acquire));
        }
        void unlock() {
            flag.clear(std::memory_order_release);
        }
    };

    class ThreadManager {
    private:
        std::vector<std::thread> threads;
        mutable SpinLock lock;

    public:
        template<typename Func>
        void CreateThread(Func&& function) {
            std::lock_guard<SpinLock> guard(lock);
            threads.emplace_back(std::forward<Func>(function));
        }

        void JoinAll() {
            std::lock_guard<SpinLock> guard(lock);
            for (auto& thread : threads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
            threads.clear();
        }

        ~ThreadManager() {
            JoinAll();
        }
    };

}  // namespace EFIClient 