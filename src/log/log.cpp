#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <cstdio>       // fflush
#include <ctime>        // time_t
#include <cstdarg>      // va_list
#include <string>
#include <mutex>

#include "log/log.h"

Log::Log() {
    lineCount_ = 0;
    MAXLINE_ = 1000;
    fp_ = nullptr;
}

Log::~Log() {
    if (fp_) {
        std::unique_lock<std::mutex> ulock(lock_);
        Flush();
        fclose(fp_);
    }
}

Log * Log::Instance() {
    static Log log;
    return &log;
}

void Log::Init(const char *path, const char *suffix) {
    path_ = path;
    suffix_ = suffix;
    lineCount_ = 0;
    MAXLINE_ = 10000;

    time_t timer = time(nullptr);
    tm t = *(localtime(&timer));

    today_ = t.tm_mday;

    char fileName[FILENAME_LEN] = {0};
    snprintf(fileName, FILENAME_LEN - 1, "%s%04d-%02d-%02d%s", 
        path_, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, suffix_);
    // snprintf(fileName, FILENAME_LEN - 1, "%s%04d-%02d-%02d_%02d:%02d%s", 
    //     path_, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, suffix_);

    {
        std::lock_guard<std::mutex> lg(lock_);

        fp_ = fopen(fileName, "a");
        if (!fp_) {
            mkdir(path_, 0777);
            fp_ = fopen(fileName, "a");
        }
    }
}

void Log::Write(LEVEL level, const char *format, ...) {
    timeval now = {0, 0};
    gettimeofday(&now, nullptr);
    time_t tSec = now.tv_sec;
    tm t = *(localtime(&tSec));
    va_list vaList;

    {
        std::unique_lock<std::mutex> ulock(lock_);
        if (today_ != t.tm_mday) {
            char newFileName[FILENAME_LEN] = {0};
            snprintf(newFileName, FILENAME_LEN - 1, "%s%04d-%02d-%02d%s", 
                path_, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, suffix_);
            // snprintf(newFileName, FILENAME_LEN - 1, "%s%04d-%02d-%02d_%02d:%02d%s", 
            //     path_, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, suffix_);
            today_ = t.tm_mday;
            Flush();
            fclose(fp_);
            fp_ = fopen(newFileName, "a");
        }
    }

    {
        std::unique_lock<std::mutex> ulock(lock_);
        int n = snprintf(buf_.WritePtr(), 128, "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
                    t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                    t.tm_hour, t.tm_min, t.tm_sec, now.tv_usec);

        buf_.HasWritten(n);
        AppendLogLevelTitle(level);

        va_start(vaList, format);
        int m = vsnprintf(buf_.WritePtr(), buf_.WritableBytes(), format, vaList);
        va_end(vaList);

        buf_.HasWritten(m);
        buf_.Append("\n\0");

        fputs(buf_.ReadPtr(), fp_);
        buf_.Reset();
    }
}

void Log::Flush() {
    fflush(fp_);
}

void Log::AppendLogLevelTitle(LEVEL level) {
    switch (level)
    {
    case LEVEL::DEBUG:
        buf_.Append("[DEBUG]: ");
        break;
    case LEVEL::INFO:
        buf_.Append("[INFO]: ");
        break;
    case LEVEL::WARN:
        buf_.Append("[WARN]: ");
        break;
    case LEVEL::ERROR:
        buf_.Append("[ERROR]: ");
        break;
    default:
        buf_.Append("[INFO]: ");
        break;
    }
}