#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      // open
#include <string>

#include "buffer/buffer.h"
#include "http/httpresponse.h"

HttpResponse::HttpResponse() {

}

HttpResponse::~HttpResponse() {

}

void HttpResponse::Response(Buffer &buf) {
    AddStateLine(buf);
    AddHeader(buf);
    AddContent(buf);
}

void HttpResponse::AddStateLine(Buffer &buf) {
    buf.Append("HTTP/1.1 " + std::to_string(200) + " " + "OK" + "\r\n");
}

void HttpResponse::AddHeader(Buffer &buf) {
    buf.Append("Connection: keep-alive\r\n");
}

void HttpResponse::AddContent(Buffer &buf) {
    buf.Append("hello world\r\n");
}