//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_LOGGER_H
#define MYWEBSERVER_LOGGER_H

#include "AsyncLogging.h"

class Logger {
 public:
  Logger(const char *fileName, int line);
  ~Logger();

};

#endif  // MYWEBSERVER_LOGGER_H
