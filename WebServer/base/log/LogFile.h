//
// Created by 蒋澳然 on 2021/7/13.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_LOGFILE_H
#define MYWEBSERVER_LOGFILE_H

#include "FileUtil.h"
#include "MutexLock.h"
#include "memory"

class LogFile : NoCopyable {
public:
    explicit LogFile(const std::string &bn, int fN = 1024);

    ~LogFile();

    void append(const char *logline, size_t len);

    void flush();

    // 归档
    bool rollFile();

private:
    void append_unlocked(const char *logline, size_t len);

    const std::string basename_;

    // count_用来记录append的次数，当append达到flushEveryN_时，flush一次
    int count_;
    const int flushEveryN_;

    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;
};


#endif //MYWEBSERVER_LOGFILE_H
