//
// Created by 蒋澳然 on 2021/7/13.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_ARGSPARSER_H
#define MYWEBSERVER_ARGSPARSER_H

#include "string"
#include "thread"
#include "log/Logger.h"

const int CORE_NUM = static_cast<int>(std::thread::hardware_concurrency());
const int DEFAULT_PORT = 80;

struct IllegalArgs : std::exception {
public:
    const char *what() const noexcept override {
        return "IllegalArguments!";
    }
};

class ArgsParser {

public:

    explicit ArgsParser(const char *const opt)
            : opt_(opt),
              threadNum_(CORE_NUM),
              port_(DEFAULT_PORT),
              logPath_(Logger::getLogfilename()) {}

    void parse(int argc, char *const argv[]);

    int threadNum() const;

    int port() const;

    const std::string &logPath() const;

private:
    const char *opt_;
    int threadNum_;
    int port_;
    std::string logPath_;
};


#endif //MYWEBSERVER_ARGSPARSER_H
