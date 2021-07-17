//
// Created by 蒋澳然 on 2021/7/15.
// email: 875691208@qq.com
// $desc
//

#include "Server.h"
#include "Utils.h"
#include "sys/socket.h"
#include "log/Logger.h"
#include "arpa/inet.h"
#include "netinet/in.h"

Server::Server(EventLoop *loop, int threadnum, int port)
        : loop_(loop),
          threadnum_(threadnum),
          port_(port),
          started_(false), {
    sockListenFd_ = socket_bind_listen(port);
    acceptChannel_->setFd();
    handle_ignore_sigpipe();
}

void Server::start() {

    acceptChannel_->setEvents(EPOLLIN | EPOLLET);
    acceptChannel_->setReadHandler(std::bind(&Server::handleNewConn, this));
    acceptChannel_->setConnHandler(std::bind(&Server::handleThisConn, this));
    started_ = true;
}

void Server::handleNewConn() {
    struct sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof client_addr;
    int accept_fd = 0;
    while ((accept_fd = accept(sockListenFd_, (struct sockaddr *) &client_addr, &client_addr_len)) > 0) {
        LOG << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << '\n';
        // 限制服务器的最大并发连接数
        if (accept_fd >= MAX_FDS) {
            close(accept_fd);
            continue;
        }
        // 非阻塞方式查询IO事件
        if (set_socket_non_blocking(accept_fd) < 0) {
            LOG << "Set accept_fd nonblocking mode failed!\n";
            return;
        }
        // 数据即使发送
        set_socket_no_delay(accept_fd);
        shared_ptr<HttpData> requestInfo(new HttpData(loop, accept_fd));
        requestInfo->getChannel()->setHolder(requestInfo);
        loop->queueInLoop(std::bind(&HttpData::newEvent, requestInfo));
    }
    acceptChannel_->setEvents(EPOLLIN | EPOLLET);
}
