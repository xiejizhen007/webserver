#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <unordered_set>
#include <unordered_map>

#include "buffer/buffer.h"

/**
 * 解析 http 请求
*/

class HttpRequest {
public:
    // http 请求的状态
    enum class STATE {
        RELINE,     // 解析请求行
        HEAD,       // 解析请求头
        BODY,       // 解析消息体
        DONE,
    };

    void init();

    // 对报文进行解析
    bool parse(Buffer &buff);

    bool IsKeepAlive() const;

    std::string method() const;
    std::string path() const;
    std::string version() const;
    std::string body() const;
    std::unordered_map<std::string, std::string> head() const;

private:
    STATE state_;
    std::string method_;
    std::string path_;
    std::string version_;
    std::string body_;

    std::unordered_map<std::string, std::string> head_;

    bool ParseRequestLine(const std::string &line);
    bool ParsePath();
    bool ParseHead(const std::string &line);
    bool ParseBody(const std::string &line);

    static const std::unordered_set<std::string> DEFAULT_HTML;
};

#endif