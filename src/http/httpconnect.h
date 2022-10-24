#ifndef HTTP_CONNECT_H
#define HTTP_CONNECT_H

#include "buffer.h"

class HttpConnect {
public:

private:
    Buffer rbuf_;   // 读缓冲
    Buffer wbuf_;   // 写缓冲
};

#endif