#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unordered_map>
#include <string>

#include "buffer/buffer.h"
#include "http/httprequest.h"

using namespace std;

int main() {
    Buffer buff(1024);
    int fd = open("/home/mayor/webserver/build/http.txt", O_RDONLY);
    buff.Readfd(fd, nullptr);
    HttpRequest http;
    http.init();
    http.parse(buff);

    cout << "buff size: " << buff.buffsz() << endl;
    cout << "method: " << http.method() << endl;
    cout << "path: " << http.path() << endl;
    cout << "version: " << http.version() << endl;
    cout << "body: " << http.body().size() << endl;
    cout << http.head()["Host"] << endl;

    unordered_map<string, string> maps;
    maps["abc"] = "abd";
}