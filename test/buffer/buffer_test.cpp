#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      // open
#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>

#include "buffer/buffer.h"

using namespace std;

void check(Buffer *buf) {
    cout << endl;
    cout << "used: " << buf->UsedBytesToString() << endl;
    cout << "read: " << buf->ReadableBytesToString() << endl;
    cout << "buff: " << buf->BufToString() << endl;
    cout << "readable bytes: " << buf->ReadableBytes() << endl;
    cout << "writable bytes: " << buf->WritableBytes() << endl;
    cout << "buff size: " << buf->buffsz() << endl;
    cout << endl;
}

int main() {
    Buffer *buf = new Buffer(static_cast<size_t>(5));
    // // 123456789
    // buf->Readfd(0);
    // check(buf);
    // buf->Writefd(1);
    // check(buf);
    // // 1
    // buf->Readfd(0);
    // check(buf);
    // buf->Writefd(1);
    // check(buf);
    int fd = open("/home/mayor/practical_training2/covid_springboot/src/main/resources/static/pages/index.html", O_RDONLY);
    if (fd < 0) {
        cout << "open is failed" << endl;
    }

    buf->Readfd(fd);
    check(buf);
}