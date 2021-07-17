//
// Created by 蒋澳然 on 2021/7/14.
// email: 875691208@qq.com
// http数据的定义
//

#ifndef MYWEBSERVER_HTTPDATA_H
#define MYWEBSERVER_HTTPDATA_H

#include "sys/epoll.h"
#include "unordered_map"
#include "memory"
#include "unistd.h"
#include "string"
#include "map"

class EventLoop;

class TimerNode;

class Channel;

enum ProcessState {
    STATE_PARSE_URI = 1,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH
};

enum URIState {
    PARSE_URI_AGAIN = 1,
    PARSE_URI_ERROR,
    PARSE_URI_SUCCESS
};

enum HeaderState {
    PARSE_HEADER_SUCCESS = 1,
    PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR
};

enum AnalysisState {
    ANALYSIS_SUCCESS = 1,
    ANALYSIS_ERROR
};

enum ParseState {
    H_START = 0,
    H_KEY,
    H_COLON,
    H_PARSE_AFTER_COLON,
    H_VALUE,
    H_CR,
    H_LF,
    H_END_CR,
    H_END_LF
};

enum ConnectionState {
    h_connected = 0,
    h_disconnecting,
    h_disconnected
};

enum HttpMethod {
    METHOD_POST = 1,
    METHOD_GET,
    METHOD_HEAD
};

enum HttpVersion {
    HTTP_10 = 1,
    HTTP_11
};

class MimeType {
public:
    static std::string getMime(const std::string &suffix);

private:

    MimeType();

    MimeType(const MimeType &other);

    static void init();

    static std::unordered_map<std::string, std::string> mime;

    static pthread_once_t once_control;
};

class HttpData : public std::enable_shared_from_this<HttpData> {
public:
    HttpData(EventLoop *loop, int connfd);

    ~HttpData() {
        close(fd_);
    }

    void reset();

    void separateTimer();

    void linkTimer(const std::shared_ptr<TimerNode> &mTimer) {
        timer_ = mTimer;
    }

    std::shared_ptr<Channel> getChannel() { return channel_; }

    EventLoop *getLoop() { return loop_; }

    void handleClose();

    void nextEvent();


private:
    EventLoop *loop_;
    std::shared_ptr<Channel> channel_;
    int fd_;
    std::string inBuff_;
    std::string outBuff_;
    bool error_;
    ConnectionState conState_;

    HttpMethod method_;
    HttpVersion version_;
    std::string filename_;
    std::string path_;
    int nowReadPos_;
    ProcessState state_;
    ParseState parseState_;
    bool keepAlive_;
    std::map<std::string, std::string> headers_;
    std::weak_ptr<TimerNode> timer_;

    void handleRead();

    void handleWrite();

    void handleConn();

    void handleError();

    URIState parseURI();

    HeaderState parseHeaders();

    AnalysisState analysisRequest();

};


#endif //MYWEBSERVER_HTTPDATA_H
