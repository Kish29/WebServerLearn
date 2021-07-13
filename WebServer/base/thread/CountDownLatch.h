//
// Created by 蒋澳然 on 2021/7/13.
// email: 875691208@qq.com
// 使用计数的方式保证 Thread中传进去的func启动后，start才返回
//

#ifndef MYWEBSERVER_COUNTDOWNLATCH_H
#define MYWEBSERVER_COUNTDOWNLATCH_H

#include "NoCopyable.h"
#include "MutexLock.h"
#include "Condition.h"

class CountDownLatch : NoCopyable {
public:
    explicit CountDownLatch(int count);

    void wait();

    // 计数
    void countDown();

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};


#endif //MYWEBSERVER_COUNTDOWNLATCH_H
