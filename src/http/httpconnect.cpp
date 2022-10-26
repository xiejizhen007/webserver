#include <sys/types.h>
#include <netinet/in.h>     // struct sockaddr_in
#include <unistd.h>         // close
#include <string>

#include <iostream>         // io for debug

#include "httpconnect.h"

const std::string src = std::string("/home/mayor/webserver/resource");

HttpConnect::HttpConnect() {

}

HttpConnect::~HttpConnect() {
    close(fd_);
}

void HttpConnect::init(int fd, const sockaddr_in &addr) {
    fd_ = fd;
    addr_ = addr;
}

bool HttpConnect::Process() {
    rbuf_.Readfd(fd_);
    httprequest_.init();
    std::cout << "readable: " << rbuf_.ReadableBytes() << std::endl;
    std::cout << "rbuf info: \n" << rbuf_.ReadableBytesToString() << std::endl;
    if (rbuf_.ReadableBytes() <= 0) {
        return false;
    } else if (httprequest_.parse(rbuf_)) {
        httpresponse_.Init(src, httprequest_.path());
    }

    httpresponse_.Response(wbuf_);
    std::cout << "\n\nwbuf info: \n" << wbuf_.ReadableBytesToString() << std::endl;
    wbuf_.Writefd(fd_);
    return true;
}