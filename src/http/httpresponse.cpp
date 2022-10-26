#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>   // stat
#include <fcntl.h>      // open
#include <string>
#include <unordered_map>
#include <iostream>

#include "buffer/buffer.h"
#include "http/httpresponse.h"

const std::unordered_map<int, std::string> HttpResponse::CODE_STATUS {
    {200, "OK"},
    {400, "Bad Requese"},
    {404, "Not Found"},
};

HttpResponse::HttpResponse() {

}

HttpResponse::~HttpResponse() {

}

void HttpResponse::Init(const std::string &src, const std::string &path) {
    code_ = 200;
    src_ = src;
    path_ = path;
}

void HttpResponse::Response(Buffer &buf) {
    // 检查是否存在相应的资源，从而生成相对应的状态码
    if (stat((src_ + path_).data(), &fileStat) < 0) {
        code_ = 404;
    } else {
        code_ = 200;
    }

    AddStateLine(buf);
    AddHeader(buf);
    AddContent(buf);
}

void HttpResponse::AddStateLine(Buffer &buf) {
    std::cout << "code: " << code_ << std::endl;
    code_ = CODE_STATUS.count(code_) ? code_ : 400;
    buf.Append("HTTP/1.1 " + std::to_string(code_) + " " + CODE_STATUS.find(code_)->second + "\r\n");
}

void HttpResponse::AddHeader(Buffer &buf) {
    buf.Append("Connection: keep-alive\r\n");
    // 仅支持 html？
    buf.Append("Content-type: text/html\r\n");
}

void HttpResponse::AddContent(Buffer &buf) {
    int filefd = open((src_ + path_).data(), O_RDONLY);
    if (filefd < 0) {
        std::cout << "\nerr: AddContent can't open file\n";
        return;
    }

    buf.Append("Content-length: " + std::to_string(fileStat.st_size) + "\r\n\r\n");
    buf.Readfd(filefd);
    buf.Append("\r\n");
}