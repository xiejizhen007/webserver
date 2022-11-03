#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>     // close
#include <fcntl.h>      // open, fcntl
#include <iostream>
#include <string>
#include <atomic>
#include <filesystem>   // c++17

#include "buffer/buffer.h"
#include "http/httpresponse.h"
#include "http/httpconnect.h"
#include "server/server.h"
#include "log/log.h"

Server::Server(int port): port_(port) {
    threadpool_ = std::unique_ptr<ThreadPool>(new ThreadPool(8));
    // 当前目录 /xx/xx/parent/build
    // 获取父目录，resources 放在父目录下
    const std::string src = std::filesystem::current_path().parent_path().string();
    // 日志存放位置
    Log::Instance()->Init((src + "/log/").c_str(), ".log");
    // http 页面存放位置
    HttpConnect::InitSrcDir(src + "/resources");
    LOG_DEBUG("Server::Server src: %s", src.c_str());

    // 初始化监听端口
    if (!InitSorket()) {
        closed_ = true;
    }
}

Server::~Server() {
    close(listenfd_);
    closed_ = true;
}

// 简陋的 tcp 服务端
void Server::SimpleWork() {
    int comfd;
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if ((comfd = accept(listenfd_, (sockaddr *)&addr, &len)) >= 0) {
        usercount_++;
        LOG_INFO("user count: %ld", usercount_.load());

        threadpool_->AddTask([&]() {
            HttpConnect conn;
            conn.init(comfd, addr);
            conn.Process();
        });
    } else {
        LOG_DEBUG("accept no fd");
    }
}

void Server::Work() {
    if (!closed_) {
        LOG_INFO("Server start to work!");
    } else {
        LOG_ERROR("Server unable to work!");
    }

    while (!closed_) {
        SimpleWork();
    }
}

bool Server::InitSorket() {
    LOG_INFO("server start init, listen port: %d", port_);

    if (port_ > 65535 || port_ < 1024) {
        LOG_ERROR("port %d is not in the suitable range!!!");
        return false;
    }

    if ((listenfd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR("unable to create listen sorket!!!");
        return false;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd_, (sockaddr *)&addr, sizeof(addr)) < 0) {
        LOG_ERROR("unable to bind port %d!", port_);
        return false;
    }

    if (listen(listenfd_, 5) < 0) {
        LOG_ERROR("unable to listen fd.");
        return false;
    }

    LOG_INFO("server has completed intialization.");
    return true;
}