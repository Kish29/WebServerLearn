//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// Server启动
//

#include "getopt.h"
#include "string"
#include "cstring"
#include "thread"

static const uint32_t CORE_NUM = std::thread::hardware_concurrency();
#define DEFAULT_PORT 80

int main2(int argc, char *argv[]) {
    int threadNum = CORE_NUM;
    int port = DEFAULT_PORT;

    std::string logPath = "./WebServer.log";

    // parse arguments
    int opt{};
    const char *str = "t:l:p";
    while ((opt = getopt(argc, argv, str)) != -1) {
        switch (opt) {
            case 't': {
                threadNum = atoi(optarg);
                break;
            }
            case 'l': {
                logPath = optarg;
                if (logPath.size() < 2 || optarg[0] != '/') {
                    fprintf(stderr, "logPath should be start with \"/\"\n");
                    abort();
                }
                break;
            }
            case 'p': {
                port = atoi(optarg);
                break;
            }
            default:
                break;
        }
    }

    return 0;
}

#include "iostream"

int main(int argc, char *argv[]) {
//    char str[32] = "my name";
//    int id = 123;
//    int len = snprintf(str, sizeof str, "%5d", id);
//    std::cout << str << std::endl;
//    std::cout << len << std::endl;
    /*std::string logPath = "./WebServer.log";
    FILE *fp = fopen(logPath.c_str(), "ae");
    if (fp) {
        printf("write data\n");
        fwrite_unlocked("hello logPath\n", 1, strlen("hello logPath\n"), fp);
    }*/
    std::cout << sizeof "(null)\n" << std::endl;
    std::cout << strlen("(null)\n") << std::endl;
    return 0;
}