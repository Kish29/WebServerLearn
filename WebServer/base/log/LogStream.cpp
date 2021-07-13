//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#include "LogStream.h"
#include "cstring"
#include "algorithm"

const char *digits = "9876543210123456789";
const char *zero_start = digits + 9;

template
class FixedBuffer<kSmallBuffer>;

template
class FixedBuffer<kLargeBuffer>;

// convert value to char
template<class T>
size_t convert(char *buf, T value) {
    T t = value;
    char *p = buf;

    do {
        int remainder = static_cast<int>(t % 10);
        t /= 10;
        *p++ = zero_start[remainder];
    } while (t != 0);

    if (value < 0) {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);
    return p - buf;
}

template<class T>
void LogStream::formatInteger(T v) {
    // buffer容不下 kMaxNumericSize 个字符的话会被直接丢弃
    if (buffer_.avail() >= kMaxNumericSize) {
        size_t len = convert(buffer_.current(), v);
        buffer_.add(len);
    }
}

LogStream &LogStream::operator<<(short s) {
    *this << static_cast<int>(s);
    return *this;
}

LogStream &LogStream::operator<<(unsigned short us) {
    *this << static_cast<unsigned int>(us);
    return *this;
}

LogStream &LogStream::operator<<(int i) {
    formatInteger(i);
    return *this;
}

LogStream &LogStream::operator<<(unsigned int ui) {
    formatInteger(ui);
    return *this;
}

LogStream &LogStream::operator<<(long l) {
    formatInteger(l);
    return *this;
}

LogStream &LogStream::operator<<(unsigned long ul) {
    formatInteger(ul);
    return *this;
}

LogStream &LogStream::operator<<(long long int lli) {
    formatInteger(lli);
    return *this;
}

LogStream &LogStream::operator<<(unsigned long long int ulli) {
    formatInteger(ulli);
    return *this;
}

LogStream &LogStream::operator<<(double d) {
    if (buffer_.avail() > kMaxNumericSize) {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", d);
        buffer_.add(len);
    }
    return *this;
}

LogStream &LogStream::operator<<(long double ld) {
    if (buffer_.avail() > kMaxNumericSize) {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12Lg", ld);
        buffer_.add(len);
    }
    return *this;
}