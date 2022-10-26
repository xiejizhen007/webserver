#include <algorithm>
#include <string>
#include <regex>
#include <unordered_set>
#include <unordered_map>

#include "http/httprequest.h"

const std::unordered_set<std::string> HttpRequest::DEFAULT_HTML {
    "/index",
};

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
                if (!ParseRequestLine(line) || !ParsePath()) {
                    return false;
                }
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

        if (end == buff.WritePtr()) {
            break;
        }
        // 修改读指针
        buff.RetrieveUntil(end + 2);
    }

    return true;
}

bool HttpRequest::IsKeepAlive() const {
    auto iter = head_.find("Connection");
    return iter != head_.end() && iter->second == "keep-alive";
}

std::string HttpRequest::method() const {
    return method_;
}

std::string HttpRequest::path() const {
    return path_;
}

std::string HttpRequest::version() const {
    return version_;
}

std::string HttpRequest::body() const {
    return body_;
}

std::unordered_map<std::string, std::string> HttpRequest::head() const {
    return head_;
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

bool HttpRequest::ParsePath() {
    if (path_.empty()) {
        return false;
    } else if (path_ == "/") {
        path_ = "/index.html";
    } else {
        // 处理路径不是 "/" 的情况
    }

    return true;
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