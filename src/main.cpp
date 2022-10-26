#include "server/server.h"

int main() {
    Server *server = new Server();
    server->SimpleWork();
    delete server;
}