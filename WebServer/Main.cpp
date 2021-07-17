//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// Server启动
//

#include "string"
#include "ArgsParser.h"
#include "log/Logger.h"
#include "EventLoop.h"
#include "Server.h"

int main(int argc, char *argv[]) {
    // parse args
    ArgsParser argsParser("t:l:p:");
    argsParser.parse(argc, argv);
    Logger::setLogfilename(argsParser.logPath());

#ifndef _PTHREADS
    LOG << "_PTHREADS is not defined!\n";
#endif
    // set loop and server
    EventLoop mainLoop;
    Server httpServer(&mainLoop, argsParser.threadnum(), argsParser.port());
    // start server
    httpServer.start();
    mainLoop.loop();
    return 0;
}