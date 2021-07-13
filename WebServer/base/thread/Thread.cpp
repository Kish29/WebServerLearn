//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#include "Thread.h"

#include <utility>
#include "cassert"
#include "CountDownLatch.h"
#include "sys/prctl.h"

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

// 保留线程的name，tid等数据
struct ThreadData {
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    string name_;
    pid_t *tid_;
    CountDownLatch *latch_;

    ThreadData(ThreadFunc &func, string &name, pid_t *tid, CountDownLatch *latch)
            : func_(func),
              name_(name),
              tid_(tid),
              latch_(latch) {}

    void runInThread() {
        //  保存数据
        *tid_ = CurrentThread::tid();
        latch_->countDown();
        tid_ = nullptr;
        latch_ = nullptr;

        CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
        // 等效于pthread_setname_np(pthread_self(), CurrentThread::t_threadName);
        prctl(PR_SET_NAME, CurrentThread::t_threadName);

        // 执行
        func_();
        CurrentThread::t_threadName = "Finished";
    }
};

void *startThread(void *obj) {
    auto data = static_cast<ThreadData *>(obj);
    data->runInThread();
    // ???
    delete data;
    return nullptr;
}

Thread::Thread(const ThreadFunc &func, const string &name)
        : started_(false),
          joined_(false),
          pthreadId_(0),
          tid_(0),
          latch_(1),
          func_(func),
          name_(name) {
    setDefaultName();
}

Thread::~Thread() {
    if (started_ && !joined_) {
        pthread_detach(pthreadId_);
    }
}

void Thread::setDefaultName() {
    if (name_.empty()) {
        // 保留32个字符长度
        name_.reserve(32);
        name_ = "Thread";
    }
}

void Thread::start() {
    assert(!started_);
    started_ = true;
    auto data = new ThreadData(func_, name_, &tid_, &latch_);
    // pthread_create success return 0
    if (pthread_create(&pthreadId_, nullptr, &startThread, data)) {
        started_ = false;
        delete data;
    } else {
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, nullptr);
}
