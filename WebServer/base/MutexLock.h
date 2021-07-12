//
// Created by 蒋澳然 on 2021/7/13.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_MUTEXLOCK_H
#define MYWEBSERVER_MUTEXLOCK_H

#include "NoCopyable.h"
#include "pthread.h"

class MutexLock : NoCopyable {
public:
    MutexLock() {
        pthread_mutex_init(&mutex_, nullptr);
    }

    ~MutexLock() {
        pthread_mutex_lock(&mutex_);
        pthread_mutex_destroy(&mutex_);
    }

    void lock() {
        pthread_mutex_lock(&mutex_);
    }

    void unlock() {
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t *get() {
        return &mutex_;
    }

private:
    pthread_mutex_t mutex_{};

private:
    // 友元类不受访问权限的影响
    friend class Condition;
};

// 加锁
class MutexLockGuard : NoCopyable {
public:
    explicit MutexLockGuard(MutexLock &_mutex) : mutex_(_mutex) {
        mutex_.lock();
    }

    ~MutexLockGuard() {
        mutex_.unlock();
    }

private:
    // 引用类型只能赋值一次且赋值即初始化，只能在构造函数中初始化
    // 故 MutexLock &mutex_{}; 是错误的，因为已经初始化了，导致mutex无法在构造函数中初始化
    MutexLock &mutex_;
};


#endif //MYWEBSERVER_MUTEXLOCK_H
