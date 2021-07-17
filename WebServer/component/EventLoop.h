//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_EVENTLOOP_H
#define MYWEBSERVER_EVENTLOOP_H

#include "vector"
#include "MutexLock.h"
#include "functional"
#include "Channel.h"
#include "memory"
#include "cassert"
#include "Epoll.h"

using namespace std;

// EventLoop 开启，检测IO事件
class EventLoop {
public:
    typedef std::function<void()> Functor;

    EventLoop();

    ~EventLoop();

    void loop();

    void quit();

    void runInLoop(Functor &cb);

    void queueInLoop(Functor &cb);

    bool isInLoopThread() const { return }

    void assertInLoopThread() const { assert(isInLoopThread()); }

    void shutdown();

private:
    bool looping_;
    std::shared_ptr<Epoll> poller_;
    int wakeupFd_;
    bool quit_;
    bool eventHandling_;
    mutable MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;
    bool callingPendingFunctors_;
    const pid_t threadId_;
    std::shared_ptr<Channel> spWakeupChannel_;

    void wakeup();

    void handleRead();

    void doPendingFunctors();

    void handleConn();
};

#endif  // MYWEBSERVER_EVENTLOOP_H
