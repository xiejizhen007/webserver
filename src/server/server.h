#ifndef SERVER_H
#define SERVER_H

#include <memory>   // smart pointer
#include <atomic>

#include "pool/threadpool.h"

class Server {
public:
    Server(int port = 8000);
    ~Server();

    void SimpleWork();
    void Work();    // 服务启动的接口
private:
    bool InitSorket();
private:
    int port_;      // 所监听的端口
    int listenfd_;  // fd
    int closed_;    // 是否关闭

    std::atomic<size_t> usercount_;

    std::unique_ptr<ThreadPool> threadpool_;
};

#endif