#include <sys/uio.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <iostream>

#include "buffer.h"

Buffer::Buffer(size_t bufsz): buf_(bufsz), wptr_(0), rptr_(0) {}

size_t Buffer::ReadableBytes() const {
    return wptr_ - rptr_;
}

size_t Buffer::WritableBytes() const {
    return buf_.size() - wptr_;
}

size_t Buffer::UsedBytes() const {
    return rptr_;
}

void Buffer::reset(char c) {
    buf_.assign(buf_.size(), c);
}

void Buffer::Append(std::string &str) {
    this->Append(str.data(), str.length());
}

void Buffer::Append(const void *str, size_t len) {
    assert(str);
    Append(static_cast<const char *>(str), len);
}

void Buffer::Append(const char *str, size_t len) {
    assert(str);
    MakeLenToWrite(len);
    // std::cout << "wptr: " << wptr_ << std::endl;
    std::copy(str, str + len, BeginPtr_() + wptr_);
    wptr_ += len;
}

// 从 fd 读取数据，写入到缓冲区
ssize_t Buffer::Readfd(int fd, int *err) {
    char tbuf[4096];
    struct iovec iov[2];
    // 当前缓冲区可写入的字节数
    const size_t writesz = WritableBytes();

    iov[0].iov_base = (void *)(BeginPtr_() + wptr_);
    iov[0].iov_len = writesz;
    iov[1].iov_base = tbuf;
    iov[1].iov_len = sizeof(tbuf);

    // 从 fd 中读数据，先把数据放到 iov[0]，溢出后把数据放到 iov[1]
    const ssize_t len = readv(fd, iov, 2);
    if (len < 0) {
        *err = errno;
    } else if (static_cast<size_t>(len) <= writesz) {
        // buf 刚刚好够装数据
        wptr_ += len;
    } else {
        // TODO: 数据溢出到 tbuf 了，需要处理
        wptr_ = buf_.size();
        Append(tbuf, len - writesz);
    }

    return len;
}

// 读取缓冲区的数据，写入到 fd 中
ssize_t Buffer::Writefd(int fd, int *err) {
    const size_t readsz = ReadableBytes();
    ssize_t len = write(fd, (void *)(BeginPtr_() + rptr_), readsz);
    if (len >= 0) {
        rptr_ += len;
    }
    return len;
}

std::string Buffer::ReadableBytesToString() {
    return std::string(BeginPtr_() + rptr_, ReadableBytes());
}

std::string Buffer::UsedBytesToString() {
    return std::string(BeginPtr_(), rptr_);
}

std::string Buffer::bufToString() {
    return std::string(buf_.data());
}

char *Buffer::BeginPtr_() {
    return &(*buf_.begin());
}

const char *Buffer::BeginPtr_() const {
    return &(*buf_.begin());
}

void Buffer::MakeLenToWrite(size_t len) {
    if (WritableBytes() < len) {
        // std::cout <<"need to resize" << std::endl;
        ResizeWritable(len);
    }
    assert(WritableBytes() >= len);
}

void Buffer::ResizeWritable(size_t len) {
    // 确保 buf 有 len 的空间可写
    if (WritableBytes() + UsedBytes() < len) {
        // 在可写的字节和用过的字节少于 len 的情况下，通过扩展 buf 获得足够空间
        buf_.resize(wptr_ + len);
    } else {
        // 在空闲的字节数多余 len 时，将迁移一下数据，空出 len 空间来写
        size_t readable = ReadableBytes();
        std::copy(BeginPtr_() + rptr_, BeginPtr_() + wptr_, BeginPtr_());
        rptr_ = 0;
        wptr_ = readable;
    }
}