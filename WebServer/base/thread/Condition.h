//
// Created by 蒋澳然 on 2021/7/13.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_CONDITION_H
#define MYWEBSERVER_CONDITION_H

#include "NoCopyable.h"
#include "MutexLock.h"
#include "cstdint"
#include "ctime"
#include "cerrno"

class Condition : NoCopyable {
public:
    explicit Condition(MutexLock &mutex) : mutex_(mutex) {
        pthread_cond_init(&cond_, nullptr);
    }

    ~Condition() {
        pthread_cond_destroy(&cond_);
    }

    void wait() {
        pthread_cond_wait(&cond_, mutex_.get());
    }

    void notify() {
        pthread_cond_signal(&cond_);
    }

    void notifyAll() {
        pthread_cond_broadcast(&cond_);
    }

    bool waitForSeconds(uint32_t seconds) {
        struct timespec abstime{};
        clock_gettime(CLOCK_REALTIME, &abstime);
        // 设定未来等待时间
        abstime.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.get(), &abstime);
    }

private:
    // 引用保证线程都是拿到同一个mutex对象
    MutexLock &mutex_;
    pthread_cond_t cond_{};
};

#endif //MYWEBSERVER_CONDITION_H
