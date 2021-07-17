//
// Created by 蒋澳然 on 2021/7/14.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_CHANNEL_H
#define MYWEBSERVER_CHANNEL_H

#include "functional"
#include "Timer.h"
#include "sys/epoll.h"

class EventLoop;

class HttpData;

/* Channel是对epoll系统调用的封装 */
class Channel {

private:
    typedef std::function<void()> Callback;
    EventLoop *loop_;
    int fd_;
    uint32_t events_;
    uint32_t revents_;
    uint32_t lastEvents_;

    std::weak_ptr<HttpData> holder_;

    int parseURI();

    int parseHeaders();

    int analysisRequest();

    Callback readHandler_;
    Callback writeHandler_;
    Callback errorHandler_;
    Callback connHandler_;

public:
    explicit Channel(EventLoop *loop);

    Channel(EventLoop *loop, int fd);

    ~Channel();

    int getFd();

    void setFd();

    void setHolder(const std::shared_ptr<HttpData> &holder) {
        holder_ = holder;
    }

    std::shared_ptr<HttpData> getHolder() {
        return std::shared_ptr<HttpData>(holder_.lock());
    }

    void setReadHandler(Callback &&readHandler) {
        readHandler_ = readHandler;
    }

    void setWriteHandler(Callback &&writeHandler) {
        readHandler_ = writeHandler;
    }

    void setErrorHandler(Callback &&errorHandler) {
        readHandler_ = errorHandler;
    }

    void setConnHandler(Callback &&connHandler) {
        readHandler_ = connHandler;
    }

    void handleEvents() {
        events_ = 0;
        if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {

        }
        if (revents_ & EPOLLERR) {

        }
        if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {

        }
        if (revents_ & EPOLLOUT) {

        }
    }

    void handleRead();

    void handleWrite();

    void handleError(int fd, int err_num, std::string short_msg);

    void handleConn();

    void setRevents(uint32_t ev) {
        revents_ = ev;
    }

    void setEvents(uint32_t ev) {
        events_ = ev;
    }

    uint32_t &getEvents() { return events_; }

    bool EqualAndUpdateLastEvents() {
        bool ret = (lastEvents_ == events_);
        lastEvents_ = events_;
        return ret;
    }

    uint32_t &getLastEvents() { return lastEvents_; }

};

typedef std::shared_ptr<Channel> SP_Channel;

#endif //MYWEBSERVER_CHANNEL_H
