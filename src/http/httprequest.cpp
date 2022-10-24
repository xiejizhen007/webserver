#include <algorithm>
#include <string>
#include <regex>

#include "httprequest.h"

void HttpRequest::init() {
    method_ = path_ = version_ = body_ = std::string("");
    state_ = STATE::RELINE;
}

bool HttpRequest::parse(Buffer &buff) {
    const std::string CRLF = "\r\n";
    // 缓冲区没有可读的数据了
    if (buff.ReadableBytes() <= 0) {
        return false;
    }

    // 将数据解析完
    while (buff.ReadableBytes() && state_ != STATE::DONE) {
        // 去掉 http 的换行 "\r\n"
        auto end = std::search(buff.ReadPtr(), buff.WritePtr(), CRLF.begin(), CRLF.end());
        auto line = std::string(buff.ReadPtr(), end);

        switch (state_) {
            case STATE::RELINE:
                ParseRequestLine(line);
                break;
            case STATE::HEAD:
                ParseHead(line);
                break;
            case STATE::BODY:
                ParseBody(line);
                break;
            default:
                break;
        }
    }
}

bool HttpRequest::ParseRequestLine(const std::string &line) {
    std::regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::smatch subMatch;
    if (std::regex_match(line, subMatch, patten)) {   
        method_ = subMatch[1];
        path_ = subMatch[2];
        version_ = subMatch[3];
        state_ = STATE::HEAD;
        return true;
    }
    return false;
}

bool HttpRequest::ParseHead(const std::string &line) {
    std::regex patten("^([^:]*): ?(.*)$");
    std::smatch subMatch;
    if (std::regex_match(line, subMatch, patten)) {
        head_[subMatch[1]] = subMatch[2];
    } else {
        state_ = STATE::BODY;
    }
    return true;
}

bool HttpRequest::ParseBody(const std::string &line) {
    body_ = line;
    state_ = STATE::DONE;
    return true;
}