#ifndef THREADPOOL
#define THREADPOOL

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

/*
* 将任务放入任务队列当中，空闲的线程去任务队列当中取
*/

class ThreadPool {
public:
    explicit ThreadPool(size_t threadCount = 8): pool_(std::make_shared<Pool>()) {
        for (size_t i = 0; i < threadCount; i++) {
            std::thread([pool = pool_] {
                std::unique_lock<std::mutex> ulock(pool->lock);
                while (1) {
                    if (!pool->tasks.empty()) {
                        // 从任务队列当中取任务
                        auto task = std::move(pool->tasks.front());
                        pool->tasks.pop();
                        // 要去执行所获取的任务了，释放掉锁
                        ulock.unlock();
                        task();
                        // 重新获取锁
                        ulock.lock();
                    } else if (pool->isClosed) {
                        break;
                    } else {
                        // 等待任务的到来
                        pool->cond.wait(ulock);
                    }
                }
            }).detach();
        }
    }

    ~ThreadPool() {
        if (static_cast<bool>(pool_)) {
            {
                std::unique_lock<std::mutex> ulocker(pool_->lock);
                pool_->isClosed = true;
            }
            // 现在线程池要销毁了，通知所有睡眠中的线程退出
            // 如果说任务队列当中还有任务，那么线程仍去指向任务
            pool_->cond.notify_all();
        }
    }

    template<typename F>
    void AddTask(F&& task) {
        {
            std::unique_lock<std::mutex> ulocker(pool_->lock);
            pool_->tasks.emplace(std::forward<F>(task));
        }
        // 每次只加入了一个任务，通知一个线程即可
        pool_->cond.notify_one();
    }

private:
    struct Pool {
        std::mutex lock;
        std::condition_variable cond;
        bool isClosed;
        std::queue<std::function<void()>> tasks;

        Pool(): isClosed(false) {}
    };

    std::shared_ptr<Pool> pool_;
};

#endif