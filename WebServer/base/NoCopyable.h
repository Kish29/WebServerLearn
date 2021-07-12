//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// 该类无法复制
//

#ifndef MYWEBSERVER_NOCOPYABLE_H
#define MYWEBSERVER_NOCOPYABLE_H

class NoCopyable {
 protected:
  NoCopyable() {}
  ~NoCopyable() {}

 private:
  NoCopyable(const NoCopyable &);
  const NoCopyable &operator=(const NoCopyable &);
};

#endif  // MYWEBSERVER_NOCOPYABLE_H
