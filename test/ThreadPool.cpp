#include <iostream>
#include <memory>
#include "../src/pool/ThreadPool.h"
#include <thread>
#include <atomic>
#include <chrono>
#include <condition_variable>

int main() {
    std::atomic<int> x(0);
    using std::chrono::operator""ms;

    {
        std::shared_ptr<ThreadPool> pool = std::make_shared<ThreadPool>(8);
        for (int i = 0; i < 100000; i++) {
            pool->AddTask([&]() {
                x++;
            });
        }
    }

    // 可能线程执行速度比较慢，等待线程任务执行结束
    // using std::chrono::operator""ms;
    std::this_thread::sleep_for(1000ms);
    
    std::cout << "x: " << x << std::endl;
    std::condition_variable cond;
    cond.notify_one();
    cond.wait()
    std::unique_lock<std::mutex> lock1();
}