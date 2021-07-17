//
// Created by 蒋澳然 on 2021/7/14.
// email: 875691208@qq.com
// $desc
//

#include "Utils.h"
#include "unistd.h"
#include "cerrno"
#include "csignal"
#include "cstring"
#include "fcntl.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netinet/tcp.h"

const int MAX_BUFF = 4 * 1024;

ssize_t readn(int fd, void *buff, size_t n) {
    size_t nleft = n;
    ssize_t nread = 0;
    ssize_t read_sum = 0;
    char *ptr = (char *) buff;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            switch (errno) {
                case EINTR:
                    nread = 0;
                    continue;
                case EAGAIN:
                    return read_sum;
                default:
                    return -1;
            }
        } else if (nread == 0) {
            break;
        }
        nleft -= nread;
        ptr += nread;
        read_sum += nread;
    }
    return read_sum;
}

ssize_t readn(int fd, std::string &inbuff, bool &zero) {
    ssize_t nread = 0;
    ssize_t read_sum = 0;
    while (true) {
        char buff[MAX_BUFF];
        if ((nread = read(fd, buff, MAX_BUFF)) < 0) {
            switch (errno) {
                case EINTR:
                    continue;
                case EAGAIN:
                    return read_sum;
                default:
                    perror("read error\n");
                    return -1;
            }
        } else if (nread == 0) {
            zero = true;
            break;
        }

        read_sum += nread;
        inbuff += std::string(buff, buff + nread);
    }
    return read_sum;
}

ssize_t readn(int fd, std::string &inbuff) {
    bool zero = false;
    return readn(fd, inbuff, zero);
}

ssize_t writen(int fd, void *buff, size_t n) {
    size_t nleft = n;
    ssize_t nwritten = 0;
    ssize_t write_sum = 0;
    char *ptr = (char *) buff;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            switch (errno) {
                case EINTR:
                    nwritten = 0;
                    continue;
                case EAGAIN:
                    return write_sum;
                default:
                    return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
        write_sum += nwritten;
    }
    return write_sum;
}

ssize_t writen(int fd, std::string &sbuff) {
    const char *ptr = sbuff.c_str();
    ssize_t write_sum = writen(fd, (void *) ptr, sbuff.size());
    if (write_sum == static_cast<int >(sbuff.size())) {
        sbuff.clear();
    } else {
        sbuff.substr(write_sum);
    }
    return write_sum;
}

void handle_ignore_sigpipe() {
    struct sigaction sa{};
    memset(&sa, '\0', sizeof sa);
    sa.sa_handler = SIG_IGN;    // ignore signal
    sa.sa_flags = 0;
    if (sigaction(SIGPIPE, &sa, nullptr) == -1) {
        perror("sigaction: ignore pipe failed!\n");
    }
}

int set_socket_non_blocking(int fd) {
    int flag = fcntl(fd, F_GETFL);
    if (flag == -1) return -1;
    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) == -1) return -1;
    return 0;
}

void set_socket_no_delay(int fd) {
    int enable = 1;
    // TCP_NODELAY 禁用Nagle算法，即tcp数据段即使传送，即使数据包很小，不会等到内核缓冲区满才发送
    // 在socket层进行设置: SOL_SOCKET，其他层如tcp层通过 IPPROTO_TCP 指定
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *) &enable, sizeof enable);
}

void set_socket_no_linger(int fd) {
    struct linger linger_{};
    linger_.l_onoff = 1;
    linger_.l_linger = 30;
    setsockopt(fd, SOL_SOCKET, SO_LINGER, &linger_, sizeof linger_);
}

void shutdown_write(int fd) {
    // shutdown socket
    shutdown(fd, SHUT_WR);
}

int socket_bind_listen(int port) {
    // 检查port
#define MAX_PORT_NU ((1 << 16) - 1)
#define ERROR_CLOSE_RET(msg, fd)    \
    perror(msg);                    \
    close(fd);                      \
    return -1;

    if (port < 0 || port > MAX_PORT_NU) return -1;

    // ipv4(AF_INET) + tcp(SOCK_STREAM)
    // 向内核申请socket资源
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) return -1;

    // 检查地址是否被占用
    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) == -1) {
        ERROR_CLOSE_RET("socket address reuse failed!\n", sock_fd)
    }

    // 端口被占用会报错
    struct sockaddr_in server_add{};
    bzero(&server_add, sizeof server_add);
    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr = htonl(INADDR_ANY); // (h)ost (to)to (n)et long
    server_add.sin_port = htons((unsigned short) port);
    if (bind(sock_fd, (struct sockaddr *) &server_add, sizeof server_add) == -1) {
        ERROR_CLOSE_RET("socket bind failed!\n", sock_fd)
    }

    // 开始监听，backlog为2048，即最大等待队列为2048个
    if (listen(sock_fd, 2048) == -1) {
        ERROR_CLOSE_RET("socket listen failed!\n", sock_fd)
    }

    return sock_fd;
}





