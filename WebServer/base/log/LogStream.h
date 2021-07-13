//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_LOGSTREAM_H
#define MYWEBSERVER_LOGSTREAM_H

#include "NoCopyable.h"
#include "cassert"
#include "cstring"
#include "string"
#include "vector"
#include "memory"

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

// 缓冲区
template<int SIZE>
class FixedBuffer : NoCopyable {
public:
    FixedBuffer() : cur_(data_) {}

    ~FixedBuffer() = default;

    // 调用append之前，记得调用reset
    void append(const char *buf, size_t len) {
        if (avail() > static_cast<int >(len)) {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }

    const char *data() const {
        return data_;
    }

    int length() const {
        return static_cast<int >(cur_ - data_);
    }

    char *current() {
        return cur_;
    }

    // 缓冲区剩余大小
    int avail() const {
        return static_cast<int > (end() - cur_);
    }

    void add(size_t len) {
        cur_ += len;
    }

    // 准备开始输入数据或重置缓冲区
    void clear() {
        cur_ = data_;
    }

    // 设置为0
    void bzero() {
        memset(data_, 0, sizeof data_);
    }

private:
    // 返回缓冲区末尾地址地址
    const char *end() const {
        return data_ + sizeof data_;
    }

    // 缓冲区
    char data_[SIZE]{};
    // cur指向data_空闲区域地址
    char *cur_{};
};

// stream流，有一个小的缓冲区，给宏定义LOG(每次会创建一个logger)使用
class LogStream : NoCopyable {

public:

    typedef FixedBuffer<kSmallBuffer> Buffer;

    // --verbose
    LogStream &operator<<(bool v) {
        buffer_.append(v ? "true" : "false", v ? strlen("true") : strlen("false"));
        return *this;
    }

    LogStream &operator<<(short);

    LogStream &operator<<(unsigned short);

    LogStream &operator<<(int);

    LogStream &operator<<(unsigned int);

    LogStream &operator<<(long);

    LogStream &operator<<(unsigned long);

    LogStream &operator<<(long long);

    LogStream &operator<<(unsigned long long);

    LogStream &operator<<(const void *);

    LogStream &operator<<(float v) {
        *this << static_cast<double >(v);
        return *this;
    }

    LogStream &operator<<(double);

    LogStream &operator<<(long double);

    LogStream &operator<<(char v) {
        buffer_.append(&v, 1);
        return *this;
    }

    LogStream &operator<<(const char *str) {
        if (str) {
            buffer_.append(str, strlen(str));
        } else {
            // 对字符串常量求长度时，用strlen，去掉对'\0'的计数
            // buffer_.append("(null)", strlen("(null)"));
            buffer_.append("(null)", 6);
        }
        return *this;
    }

    LogStream &operator<<(const unsigned char *str) {
        return operator<<(reinterpret_cast<const char *>(str));
    }

    LogStream &operator<<(const std::string &v) {
        buffer_.append(v.c_str(), v.size());
        return *this;
    }

    void append(const char *data, int len) {
        buffer_.append(data, len);
    }

    const Buffer &buffer() const {
        return buffer_;
    }

    void resetBuffer() {
        buffer_.clear();
    }

private:
    void staticCheck();

    template<class T>
    void formatInteger(T);

    Buffer buffer_{};
    // 转换数字的最大长度
    static const int kMaxNumericSize = 32;
};


#endif  // MYWEBSERVER_LOGSTREAM_H
