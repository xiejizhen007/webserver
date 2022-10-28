#include <thread>
#include <chrono>

#include "log/log.h"

int main() {
    Log::Instance()->Init("./");
    using namespace std::chrono_literals;

    std::thread threads[2];

    threads[0] = std::thread([&]() {
        for (int i = 0; i < 1000; i++) {
            LOG_INFO("%s-%d", "hello world", 20);
            std::this_thread::sleep_for(100ms);
        }
    });

    threads[1] = std::thread([&]() {
        for (int i = 0; i < 1000; i++) {
            LOG_DEBUG("%s-%d", "hello world", 20);
            std::this_thread::sleep_for(100ms);
        }
    });

    for (int i = 0; i < 2; i++) {
        threads[i].join();
    }
}