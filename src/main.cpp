#include "server/server.h"

int main() {
    Server *server = new Server();
    server->Work();
    delete server;
}