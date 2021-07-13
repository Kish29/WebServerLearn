//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#include "Logger.h"
#include "sys/time.h"

static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsyncLogging *AsyncLogger_;

std::string Logger::logfilename_ = "./WebServer.log";

void once_init() {
    AsyncLogger_ = new AsyncLogging(Logger::getLogfilename());
    // 后台线程跑起来了
    AsyncLogger_->start();
}

void output(const char *msg, int len) {
    pthread_once(&once_control_, once_init);
    // 贴上logger的标志
    AsyncLogger_->append(msg, len);
}

void Logger::Impl::formatTime() {
    struct timeval tv{};
    time_t time;
    char str_time[26]{};
    gettimeofday(&tv, nullptr);
    time = tv.tv_sec;
    struct tm *p_time = localtime(&time);
    strftime(str_time, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
    stream_ << str_time;
}

Logger::Impl::Impl(const char *filename, int line) :
        stream_(),
        line_(line),
        filename_(filename) {
    formatTime();
}

Logger::Logger(const char *filename, int line) : impl_(filename, line) {}

// 析构函数这些代码是为了Logger.h的LOG宏定义，写下文件名和行数，并记录log了什么东西
Logger::~Logger() {
    impl_.stream() << " -- " << impl_.filename() << ':' << impl_.line() << '\n';
    const LogStream::Buffer &buf(stream().buffer());
    // 向AsyncLogging提交本logger所记录的东西
    output(buf.data(), buf.length());
}