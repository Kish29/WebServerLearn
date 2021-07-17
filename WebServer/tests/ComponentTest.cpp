//
// Created by 蒋澳然 on 2021/7/17.
// email: 875691208@qq.com
// $desc
//
#include "threadpool/ThreadPool.h"
#include "thread"

#include "random"

std::random_device dv{};
std::default_random_engine dre = std::default_random_engine(dv());
std::uniform_int_distribution<int> uidis(1, 3);

void *thread_func(const std::shared_ptr<void> &args) {
    int arg = *((int *) args.get());
    printf("run in %s, arg is %d\n", ThreadPool::current_thread_name().c_str(), arg);
    sleep(uidis(dre));
    return nullptr;
}

int main() {
    ThreadPool::instance()->create(cpucore, 10000);
    std::shared_ptr<void> s(new int{2});
    for (int i = 0; i < 10000; ++i) {
        int res = ThreadPool::instance()->submit(s, thread_func);
        if (res != 0) {
            printf("submit task failed - %d\n", res);
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(3600));
    ThreadPool::shutdown(GRACEFUL_SHUTDOWN);
    return 0;
}