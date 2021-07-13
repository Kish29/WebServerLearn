//
// Created by 蒋澳然 on 2021/7/13.
// email: 875691208@qq.com
// $desc
//

#include "ArgsParser.h"
#include "getopt.h"

void ArgsParser::parse(int argc, char *const argv[]) {
    // parse arguments
    int opt{};
    while ((opt = getopt(argc, argv, opt_)) != -1) {
        switch (opt) {
            case 't': {
                threadNum_ = atoi(optarg);
                break;
            }
            case 'l': {
                logPath_ = optarg;
                if (logPath_.size() < 2 || optarg[0] != '/') {
                    fprintf(stderr, "logPath should be start with \"/\"\n");
                    abort();
                }
                break;
            }
            case 'p': {
                port_ = atoi(optarg);
                break;
            }
            default:
                break;
        }
    }
}

int ArgsParser::threadNum() const {
    return threadNum_;
}

int ArgsParser::port() const {
    return port_;
}

const std::string &ArgsParser::logPath() const {
    return logPath_;
}
