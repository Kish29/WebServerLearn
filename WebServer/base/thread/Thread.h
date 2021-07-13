//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_THREAD_H
#define MYWEBSERVER_THREAD_H

#include "pthread.h"
#include "string"
#include "NoCopyable.h"
#include "sys/syscall.h"
#include "unistd.h"
#include "functional"
#include "CountDownLatch.h"

class Thread : NoCopyable {
public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(const ThreadFunc &, const std::string &name = "Thread-Default");

    ~Thread();

    void start();

    int join();

    bool started() const { return started_; }

    bool joined() const { return joined_; }

private:
    void setDefaultName();

    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;
};


#endif //MYWEBSERVER_THREAD_H
