//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// 使用线程进行异步日志输出
//

#ifndef MYWEBSERVER_ASYNCLOGGING_H
#define MYWEBSERVER_ASYNCLOGGING_H

#include "NoCopyable.h"
#include "LogStream.h"
#include "thread/Thread.h"

class AsyncLogging : NoCopyable {
public:
    explicit AsyncLogging(std::string filename, int flushInterval = 2);

    ~AsyncLogging() {
        if (running_) {
            stop();
        }
    }

    void append(const char *logline, int len);

    void start() {
        running_ = true;
        logThread_.start();
        latch_.wait();
    }

    void stop() {
        running_ = false;
        condition_.notify();
        // log线程的声明周期和程序一样了，将剩下的数据打印
        logThread_.join();
    }

private:
    void logInThread();

    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::shared_ptr<Buffer> BufferPtr;
    typedef std::vector<BufferPtr> BufferGroup;
    const int flushInterval_;
    bool running_;
    std::string filename_;

    // log线程
    Thread logThread_;
    MutexLock mutex_;
    Condition condition_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferGroup buffers_;
    CountDownLatch latch_;
};

#endif  // MYWEBSERVER_ASYNCLOGGING_H
