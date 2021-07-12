//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_CURRENTTHREAD_H
#define MYWEBSERVER_CURRENTTHREAD_H

#include "cstdint"

namespace CurrentThread {
    extern __thread int t_cachedTid;
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    extern __thread const char *t_threadName;

    void cacheTid();

    // 获取线程tid
    inline int tid() {
        if (__builtin_expect(t_cachedTid == 0, 0)) {
            cacheTid();
        }
        return t_cachedTid;
    }

}

#endif //MYWEBSERVER_CURRENTTHREAD_H
