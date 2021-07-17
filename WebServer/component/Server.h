//
// Created by 蒋澳然 on 2021/7/15.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_SERVER_H
#define MYWEBSERVER_SERVER_H

#include "EventLoop.h"

class Server {
public:
    Server(EventLoop *loop, int threadnum, int port);

    ~Server();

    EventLoop *getLoop() const { return loop_; }

    void start();

    void handleNewConn();

    void handleThisConn();

private:
    EventLoop *loop_;
    std::unique_ptr<EventLoopThreadPool>
    int sockListenFd_;
    bool started_;
    int threadnum_;
    int port_;
    Channel *acceptChannel_;
    // 最大的连接数
    static const int MAX_FDS = 100000;
};


#endif //MYWEBSERVER_SERVER_H
