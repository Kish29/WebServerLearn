//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_LOGGER_H
#define MYWEBSERVER_LOGGER_H

#include "AsyncLogging.h"

//  logger的作用是记录哪个文件的哪行代码写下了什么
// 所有的 "<<" 操作都是往 logger的 logstream缓冲区里放数据，然后再西沟函数里
// 利用AsyncLogging异步写入文件
class Logger {
public:
    Logger(const char *filename, int line);

    ~Logger();

    LogStream &stream() {
        return impl_.stream();
    }

    static void setLogfilename(std::string filename) {
        logfilename_ = std::move(filename);
    }

    static std::string getLogfilename() {
        return logfilename_;
    }

private:

    class Impl {
    public:
        Impl(const char *filename, int line);

        void formatTime();

        LogStream &stream() {
            return stream_;
        }

        int line() const {
            return line_;
        }

        std::string &filename() {
            return filename_;
        }

    private:
        LogStream stream_;
        int line_;
        std::string filename_;
    };

    Impl impl_;
    static std::string logfilename_;

};

// __FILE__ 和 __LINE__ 都是内置宏
// __FILE__ 表示源文件名，__LINE__表示代码所在行号
// 如在Main.cpp的第55行代码中，使用 LOG << "I'm main"
// 在日志文件中就会写下 /绝对路径/Main.cpp:55
#define LOG Logger(__FILE__, __LINE__).stream()

#endif  // MYWEBSERVER_LOGGER_H