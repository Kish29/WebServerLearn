//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#include "Thread.h"
#include "CurrentThread.h"

using namespace std;

// 使用__thread 关键字，告诉g++，这些变量是每个线程独有的
namespace CurrentThread {
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char *t_threadName = "default";
}

pid_t gettid() {
    return static_cast<pid_t > (::syscall(SYS_gettid));
}


void CurrentThread::cacheTid() {
    if (t_cachedTid == 0) {
        t_cachedTid = gettid();
        // tid字符串占位5位
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}