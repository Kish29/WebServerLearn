//
// Created by 蒋澳然 on 2021/7/13.
// email: 875691208@qq.com
// $desc
//

#include "CountDownLatch.h"

CountDownLatch::CountDownLatch(int count)
        : count_(count),
          mutex_(),
          condition_(mutex_) {
}

void CountDownLatch::wait() {
    MutexLockGuard lock(mutex_);
    while (count_ > 0) condition_.wait();
}

void CountDownLatch::countDown() {
    MutexLockGuard lock(mutex_);
    count_--;
    if (count_ == 0) {
        condition_.notifyAll();
    }
}

