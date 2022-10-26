#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

/**
 * http 的应答
*/

#include <sys/stat.h>
#include <string>
#include <unordered_map>

#include "buffer/buffer.h"

class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();

    void Init(const std::string &str, const std::string &path);
    void Response(Buffer &buf);
private:
    void AddStateLine(Buffer &buf);
    void AddHeader(Buffer &buf);
    void AddContent(Buffer &buf);

    std::string path_;  // 相对路径，类似于 "/index.html"
    std::string src_;   // 资源所处的目录

    struct stat fileStat;

    int code_;          // 状态码

    static const std::unordered_map<int, std::string> CODE_STATUS;
};

#endif