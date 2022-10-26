#ifndef HTTP_CONNECT_H
#define HTTP_CONNECT_H

#include <sys/types.h>
#include <netinet/in.h>

#include "buffer/buffer.h"
#include "http/httprequest.h"
#include "http/httpresponse.h"

class HttpConnect {
public:
    HttpConnect();
    ~HttpConnect();

    void init(int fd, const sockaddr_in &addr);
private:
    int fd_;
    sockaddr_in addr_;

    Buffer rbuf_;   // 读缓冲
    Buffer wbuf_;   // 写缓冲

    HttpRequest httprequest_;
    HttpResponse httpresponse_;
};

#endif