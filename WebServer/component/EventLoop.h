//
// Created by 蒋澳然 on 2021/7/12.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_EVENTLOOP_H
#define MYWEBSERVER_EVENTLOOP_H

#include "functional"

using namespace std;

class EventLoop {
 public:
  typedef std::function<void()> Functor;
  EventLoop();
  ~EventLoop();
  void loop();
  void quit();
  void runInLoop(Functor &cb);
  void queueInLoop(Functor &cb);
  bool isInLoopThread() const ;

 private:
  bool looping_;
};

#endif  // MYWEBSERVER_EVENTLOOP_H
