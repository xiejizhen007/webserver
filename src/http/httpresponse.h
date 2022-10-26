#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

/**
 * http 的应答
*/

#include "buffer/buffer.h"

class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();

    void Response(Buffer &buf);
private:
    void AddStateLine(Buffer &buf);
    void AddHeader(Buffer &buf);
    void AddContent(Buffer &buf);
};

#endif