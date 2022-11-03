#include <sys/types.h>
#include <netinet/in.h>     // struct sockaddr_in
#include <unistd.h>         // close
#include <string>

#include "httpconnect.h"
#include "log/log.h"

const std::string HttpConnect::src_;

HttpConnect::HttpConnect() {}

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

    if (rbuf_.ReadableBytes() <= 0) {
        return false;
    } else if (httprequest_.parse(rbuf_)) {
        httpresponse_.Init(src_, httprequest_.path());
    }

    LOG_DEBUG("==== method: %s, path: %s, version: %s, body: %s", 
        httprequest_.method().c_str(), httprequest_.path().c_str(), 
        httprequest_.version().c_str(), httprequest_.body().c_str());

    httpresponse_.Response(wbuf_);
    wbuf_.Writefd(fd_);
    return true;
}