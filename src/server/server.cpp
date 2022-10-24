#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include "server.h"

Server::Server(int port): port_(port) {}

Server::~Server() {
    close(listenfd_);
}

// 简陋的 tcp 服务端
void Server::SimpleWork() {
    using namespace std;

    if ((listenfd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "create sockrt failed" << endl;
        return;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    cout << "pid: " << getpid() << endl;
    cout << "port: " << addr.sin_port << endl;
    cout << "ip: " << std::hex << addr.sin_addr.s_addr << endl;

    if (bind(listenfd_, (sockaddr *)&addr, sizeof(addr)) < 0) {
        cout << "err: bind" << endl;
        return;
    }

    if (listen(listenfd_, 5) < 0) {
        cout << "err: listen" << endl;
        return;
    }

    cout << "开始监听" << endl;
    int comfd = -1;

    while ((comfd = accept(listenfd_, nullptr, nullptr)) >= 0) {
        cout << "connect" << endl;
        char buf;

        while ((read(comfd, &buf, 1)) != -1) {
            cout << buf;
        }
        
        close(comfd);
    }
}