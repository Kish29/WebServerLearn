//
// Created by 蒋澳然 on 2021/7/14.
// email: 875691208@qq.com
// $desc
//

#include "Channel.h"

Channel::Channel(EventLoop *loop) : loop_(loop) {

}

Channel::Channel(EventLoop *loop, int fd) : loop_(loop), fd_(fd) {

}
