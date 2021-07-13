//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_EVENTLOOP_H
#define MYWEBSERVER_EVENTLOOP_H

#include "functional"
#include "cassert"

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

    bool isInLoopThread() const;

    void assertInLoopThread() const { assert(isInLoopThread()); }

    void shutdown();

private:
    bool looping_;
};

#endif  // MYWEBSERVER_EVENTLOOP_H
