//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#include "LogFile.h"
#include "AsyncLogging.h"

AsyncLogging::AsyncLogging(std::string filename, int flushInterval)
        : flushInterval_(flushInterval),
          running_(false),
          filename_(std::move(filename)),
          logThread_(std::bind(&AsyncLogging::logInThread, this), "Thread-Logging"),
          mutex_(),
          condition_(mutex_),
          currentBuffer_(new Buffer),
          nextBuffer_(new Buffer),
          buffers_(),
          latch_(1) {
    assert(filename_.size() > 1);
    currentBuffer_->bzero();
    nextBuffer_->bzero();
    buffers_.reserve(16);
}

void AsyncLogging::append(const char *logline, int len) {
    MutexLockGuard lock(mutex_);
    if (currentBuffer_->avail() > len) {
        // 往缓冲区放入数据
        currentBuffer_->append(logline, len);
    } else {
        // 暂存缓冲区
        buffers_.push_back(currentBuffer_);
        currentBuffer_.reset();
        if (nextBuffer_) {
            currentBuffer_ = std::move(nextBuffer_);
        } else {
            currentBuffer_.reset(new Buffer);
        }
        // 往缓冲区放入数据
        currentBuffer_->append(logline, len);
        // 唤醒线程，缓冲区此时有数据了
        condition_.notify();
    }
}

void AsyncLogging::logInThread() {
    assert(running_ == true);
    latch_.countDown();
    LogFile output(filename_);
    BufferPtr bf1(new Buffer);
    BufferPtr bf2(new Buffer);
    bf1->bzero();
    bf2->bzero();
    BufferGroup buffersInThisThread;
    buffersInThisThread.reserve(16);
    while (running_) {
        assert(bf1 && bf1->length() == 0);
        assert(bf2 && bf2->length() == 0);
        assert(buffersInThisThread.empty());

        // 获取全局buffer
        {
            MutexLockGuard lock(mutex_);
            if (buffers_.empty()) {
                // 等待缓冲区的数据 2S
                condition_.waitForSeconds(flushInterval_);
            }
            buffers_.push_back(currentBuffer_);
            currentBuffer_.reset();

            currentBuffer_ = std::move(bf1);
            buffersInThisThread.swap(buffers_);

            if (!nextBuffer_) {
                nextBuffer_ = std::move(bf2);
            }
        }

        assert(!buffersInThisThread.empty());

        // 如果buffers堆积，清除
        if (buffersInThisThread.size() > 25) {
            buffersInThisThread.erase(buffersInThisThread.begin() + 2, buffersInThisThread.end());
        }

        // 线程写入数据
        for (const BufferPtr &buf : buffersInThisThread) {
//            printf("write data!\n");
//            printf("data is \n%s", buf->data());
            output.append(buf->data(), buf->length());
        }

        if (buffersInThisThread.size() > 2) {
            buffersInThisThread.resize(2);
        }

        // 获取新的缓冲数据
        if (!bf1) {
            assert(!buffersInThisThread.empty());
            bf1 = buffersInThisThread.back();
            buffersInThisThread.pop_back();
            bf1->clear();
        }

        if (!bf2) {
            assert(!buffersInThisThread.empty());
            bf2 = buffersInThisThread.back();
            buffersInThisThread.pop_back();
            bf2->clear();
        }

        // 没有写完的数据在本线程丢弃
        buffersInThisThread.clear();
        output.flush();
    }
    output.flush();
}
