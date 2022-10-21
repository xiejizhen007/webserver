#ifndef BUFFER_H
#define BUFFER_H

/**
 * 有关于缓冲区的操作
 * 将数据写入缓冲区
 * 读出缓冲区中的数据
*/

#include <vector>
#include <string>
#include <atomic>

class Buffer {
public:
    Buffer(size_t bufsz = 1024);

    size_t ReadableBytes() const;
    size_t WritableBytes() const;
    size_t UsedBytes() const;

    void reset(char c = 0);

    void Append(std::string &str);
    void Append(const void *str, size_t len);
    void Append(const char *str, size_t len);

    ssize_t Readfd(int fd, int *err);
    ssize_t Writefd(int fd, int *err);

    std::string ReadableBytesToString();
    std::string UsedBytesToString();
    std::string bufToString();
private:
    /**
     * 缓冲区类似于窗口
     * 0...rptr...wptr...\0
     * [0, rptr) 是以及读写过的
     * [rptr, wptr) 是已经写入，但未读取
     * [wptr, n) 仍未写入
    */
    std::vector<char> buf_;
    std::atomic<int> wptr_;
    std::atomic<int> rptr_;

    // 缓冲区起始指针
    char *BeginPtr_();
    const char *BeginPtr_() const;

    void MakeLenToWrite(size_t len);
    void ResizeWritable(size_t len);
};

#endif