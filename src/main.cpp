// #include <iostream>
// #include <memory>
// #include <thread>
// #include <atomic>
// #include <chrono>

// // #include "pool/ThreadPool.h"
// #include "buffer/buffer.h"

// int main() {
//     // std::atomic<int> x(0);
//     // using std::chrono::operator""ms;

//     // {
//     //     std::shared_ptr<ThreadPool> pool = std::make_shared<ThreadPool>(8);
//     //     for (int i = 0; i < 100000; i++) {
//     //         pool->AddTask([&]() {
//     //             x++;
//     //         });
//     //     }
//     // }

//     // // 可能线程执行速度比较慢，等待线程任务执行结束
//     // // using std::chrono::operator""ms;
//     // std::this_thread::sleep_for(1000ms);
    
//     // std::cout << "x: " << x << std::endl;
//     Buffer *buf = new Buffer(static_cast<size_t>(10));
//     int *err;
//     buf->Readfd(0, err);
//     std::cout << "buf: " << buf->BufToString() << std::endl;
//     std::cout << "readable: " << buf->ReadableBytesToString() << std::endl;
//     std::cout << "used: " << buf->UsedBytesToString() << std::endl;
//     buf->Writefd(1, err);
//     std::cout << "buf: " << buf->BufToString() << std::endl;
//     std::cout << "readable: " << buf->ReadableBytesToString() << std::endl;
//     std::cout << "used: " << buf->UsedBytesToString() << std::endl;
//     std::string str = "hello,world";
//     buf->Append(str);
//     std::cout << "buf: " << buf->BufToString() << std::endl;
//     std::cout << "readable: " << buf->ReadableBytesToString() << std::endl;
//     std::cout << "used: " << buf->UsedBytesToString() << std::endl;
// }

#include "server/server.h"

int main() {
    Server *server = new Server();
    server->SimpleWork();
    delete server;
}


// #include <iostream>
// #include <memory>
// #include <thread>
// #include <atomic>
// #include <chrono>
// #include <condition_variable>

// #include "pool/threadpool.h"

// int main() {
//     std::atomic<int> x(0);
//     using std::chrono::operator""ms;

//     {
//         std::shared_ptr<ThreadPool> pool = std::make_shared<ThreadPool>(8);
//         for (int i = 0; i < 100000; i++) {
//             pool->AddTask([&]() {
//                 x++;
//             });
//         }
//     }

//     // 可能线程执行速度比较慢，等待线程任务执行结束
//     // using std::chrono::operator""ms;
//     std::this_thread::sleep_for(1000ms);
    
//     std::cout << "x: " << x << std::endl;
// }