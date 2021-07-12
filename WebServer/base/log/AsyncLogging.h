//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// 异步日志输出
//

#ifndef MYWEBSERVER_ASYNCLOGGING_H
#define MYWEBSERVER_ASYNCLOGGING_H

#include "NoCopyable.h"
#include "LogStream.h"

class AsyncLogging : NoCopyable {
public:
    AsyncLogging(const std::string basename, int flushInterval = 2);

    ~AsyncLogging() {

    }

private:
    void threadFunc();

    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::shared_ptr<Buffer>> BufferGroup;
    typedef std::shared_ptr<Buffer> BufferPtr;
    const int flushInterval_{};
    bool running_{};
    std::string basename_;
};

#endif  // MYWEBSERVER_ASYNCLOGGING_H
