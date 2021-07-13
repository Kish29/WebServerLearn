//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
//

#include "AppendFile.h"

// modes为"ae"表示append，O_CLOEXE (close-on-exec)
// 具体见 man 2 open
// O_CLOEXE条目
AppendFile::AppendFile(const std::string &filename) : fp_(fopen(filename.c_str(), "ae")) {
    // 为文件设置文件流缓冲区
    setbuffer(fp_, buffer_, sizeof buffer_);
}

AppendFile::~AppendFile() {
    fclose(fp_);
}

void AppendFile::append(const char *logline, const size_t len) {
    // 注意这里的write
    size_t n = this->write(logline, len);
    size_t remain = len - n;
    while (remain > 0) {
        size_t x = this->write(logline + n, remain);
        if (x == 0) {
            int err = ferror(fp_);
            if (err) {
                fprintf(stderr, "AppendFile::append() failed!\n");
                break;
            }
        }
        n += x;
        // remain = len - n;
        remain -= x;
    }
}

void AppendFile::flush() {
    fflush(fp_);
}

size_t AppendFile::write(const char *logline, size_t len) {
    // 当文件不需要考虑并发的情况下，使用 fwrite_unlocked 速度会更快
    // 函数原型是fwrite：表示从ptr里面写nmemb个数据到stream中，其中，每个nmemb占size个字节
    // size_t fwrite (const void * ptr, size_t size, size_t nmemb, FILE *stream);
//    printf("fwrite_unlocked\n%s\n", logline);
    return fwrite_unlocked(logline, 1, len, fp_);
}




