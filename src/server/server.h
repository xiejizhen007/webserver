#ifndef SERVER_H
#define SERVER_H

class Server {
public:
    Server(int port = 8000);
    ~Server();

    void SimpleWork();
private:
    int port_;      // 所监听的端口
    int listenfd_;  // fd
};

#endif