#ifndef LOG_H
#define LOG_H

#include <mutex>

#include "buffer/buffer.h"

class Log {
public:
    enum class LEVEL {
        DEBUG,
        INFO,
        WARN,
        ERROR,
    };

    static Log *Instance();

    void Init(const char *path, const char *suffix = ".log");
    void Write(LEVEL level, const char *format, ...);
    void Flush();

private:
    Log();
    ~Log();

    const char *path_;      // log 路径
    const char *suffix_;    // log 后缀

    int MAXLINE_;
    int lineCount_;
    int today_;             // 以日期划分日志

    FILE *fp_;
    std::mutex lock_;
    Buffer buf_;

    static const int FILENAME_LEN = 256;

    void AppendLogLevelTitle(LEVEL level);
};

#define LOG_DEBUG(format, ...) { Log::Instance()->Write(Log::LEVEL::DEBUG, format, ##__VA_ARGS__); Log::Instance()->Flush(); }
#define LOG_INFO(format, ...) { Log::Instance()->Write(Log::LEVEL::INFO, format, ##__VA_ARGS__); Log::Instance()->Flush(); }
#define LOG_WARN(format, ...) { Log::Instance()->Write(Log::LEVEL::WARN, format, ##__VA_ARGS__); Log::Instance()->Flush(); }
#define LOG_ERROR(format, ...) { Log::Instance()->Write(Log::LEVEL::ERROR, format, ##__VA_ARGS__); Log::Instance()->Flush(); }

#endif  // LOG_H