//
// Created by 蒋澳然 on 2021/7/13.
// email: 875691208@qq.com
// $desc
//

#include "LogFile.h"

LogFile::LogFile(const std::string &filename, int fN)
        : filename_(filename),
          flushEveryN_(fN),
          count_(0),
          mutex_(new MutexLock) {
    file_.reset(new AppendFile(filename));
}

LogFile::~LogFile() = default;

void LogFile::append(const char *logline, size_t len) {
    MutexLockGuard lock(*mutex_);
    append_unlocked(logline, len);
}

void LogFile::flush() {
    MutexLockGuard lock(*mutex_);
    file_->flush();
}

void LogFile::append_unlocked(const char *logline, size_t len) {
    file_->append(logline, len);
    count_++;
    if (count_ >= flushEveryN_) {
        count_ = 0;
        file_->flush();
    }
}