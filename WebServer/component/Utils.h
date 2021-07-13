//
// Created by 蒋澳然 on 2021/7/14.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_UTILS_H
#define MYWEBSERVER_UTILS_H

#include "cstdlib"
#include "string"

ssize_t readn(int fd, void *buff, size_t n);

ssize_t readn(int fd, std::string &inbuff, bool &zero);

ssize_t readn(int fd, std::string &inbuff);

ssize_t writen(int fd, void *buff, size_t n);

ssize_t writen(int fd, std::string &sbuff);

void handle_for_sigpipe();

int set_socket_non_blocking(int fd);

void set_socket_no_delay(int fd);

void set_socket_no_linger(int fd);

void shutdown_write(int fd);

int socket_bind_listen(int port);

#endif //MYWEBSERVER_UTILS_H
