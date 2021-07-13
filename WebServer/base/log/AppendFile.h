//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// FileUtil提供最底层的写文件
//

#ifndef MYWEBSERVER_APPENDFILE_H
#define MYWEBSERVER_APPENDFILE_H

#include "NoCopyable.h"
#include "string"

constexpr int FILE_BUFFER_SIZE(int k) {
    return k * 1024;
}

class AppendFile : NoCopyable {

public:
    explicit AppendFile(const std::string &filename);

    ~AppendFile();

    // append是底层系统调用封装，向文件写入数据
    void append(const char *logline, size_t len);

    // 刷新文件的缓冲区
    void flush();

private:
    // 二层封装
    size_t write(const char *logline, size_t len);

    FILE *fp_;
    // 64K的文件写缓冲区
    char buffer_[FILE_BUFFER_SIZE(64)]{};
};


#endif //MYWEBSERVER_APPENDFILE_H
