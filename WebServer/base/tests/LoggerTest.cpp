//
// Created by 蒋澳然 on 2021/7/13.
// email: 875691208@qq.com
// $desc
//

#include "log/Logger.h"
#include "thread"
#include "iostream"

using namespace std;

void thread_func();

void stress_single_thread();

void other();

void stress_multi_thread();


void type_test() {
    cout << "----------- type test -----------\n";
    LOG << true;
    LOG << false;
    LOG << (short) 100;
    LOG << (unsigned short) 200;
    LOG << 12414;
    LOG << (unsigned int) 12414;
    LOG << (long) 1001234;
    LOG << (unsigned long) 1001234;
    LOG << (long long) 10012434;
    LOG << (unsigned long long) 141431001234;
    LOG << (float) 12.12412521;
    LOG << (double) 12.12412521;
    LOG << (long double) 12.12412521;
    LOG << 'c';
    LOG << string("safsadf");
}

void thread_func() {
    for (int i = 0; i < 100000; i++) {
        LOG << i;
    }
}

void stress_single_thread() {
    cout << "-------- stress single thread --------\n";
    shared_ptr<Thread> pthread(new Thread(thread_func, "single-thread"));
    pthread->start();
    pthread->join();
}

void stress_multi_thread() {
    cout << "-------- stress multi thread --------\n";
    typedef std::vector<unique_ptr<Thread>> thread_pool;
    thread_pool tp(4);
    for (int i = 0; i < 4; ++i) {
        tp.at(i).reset(new Thread(thread_func, "multi-thread"));
    }
    for (unique_ptr<Thread> &pt: tp) {
        pt->start();
        pt->join();
    }
}

void other() {
    cout << "-------- other --------\n";
    LOG << "jiang ao ran" << 234.2151235123 << 'j' << string("string from string()\n");
}

int main() {
    type_test();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    stress_single_thread();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    other();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    stress_multi_thread();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    return 0;
}