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

class Thread : NoCopyable {
public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(const ThreadFunc &, const std::string &name = std::string());

    ~Thread();

    void start();

    int join();

    bool started();

private:
    void setDefaultName();

    bool started_{};
    bool joined_{};
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;

};


#endif //MYWEBSERVER_THREAD_H
