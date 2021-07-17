//
// Created by 蒋澳然 on 2021/7/17.
// email: 875691208@qq.com
// $desc
//
#include "threadpool/ThreadPool.h"
#include "thread"

void *thread_func(const std::shared_ptr<void> &args) {
    int arg = *((int *) args.get());
    printf("run in %s, arg is %d\n", ThreadPool::current_thread_name().c_str(), arg);
    return nullptr;
}

int main() {
    ThreadPool::instance()->create(cpucore);
    /*std::shared_ptr<void> s(new int{2});
    for (int i = 0; i < 1000000; ++i) {
        ThreadPool::instance()->submit(s, thread_func);
    }*/
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ThreadPool::shutdown(GRACEFUL_SHUTDOWN);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}