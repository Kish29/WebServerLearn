//
// Created by 蒋澳然 on 2021/7/17.
// email: 875691208@qq.com
// $desc
//

#ifndef MYWEBSERVER_THREADPOOL_H
#define MYWEBSERVER_THREADPOOL_H

#include "atomic"
#include "mutex"
#include "memory"
#include "functional"
#include "vector"
#include "unistd.h"
#include "queue"

const int cpucore = ::sysconf(_SC_NPROCESSORS_CONF);

const int THREADPOOL_INVALID = -1;
const int THREADPOOL_LOCK_FAILURE = -2;
const int THREADPOOL_QUEUE_FULL = -3;
const int THREADPOOL_SHUTDOWN = -4;
const int THREADPOOL_THREAD_FAILURE = -5;
const int THREADPOOL_GRACEFUL = -6;

const int MAX_THREADS = 1 << 10;
const int MAX_QUEUE = (1 << 16) - 1;

typedef enum {
    IMMEDIATE_SHUTDOWN = 1,
    GRACEFUL_SHUTDOWN = 2
} ShutdownMode;

struct ThreadPoolTask {
    std::function<void(std::shared_ptr<void>)> fun;
    std::shared_ptr<void> args;

    void run() const {
        fun(args);
    }
};


class ThreadPool {
private:
    static ThreadPool *m_threadpool_;
    static std::mutex mtx_;
public:

    ThreadPool(const ThreadPool &other) = delete;

    ThreadPool &operator=(const ThreadPool &other) = delete;

    int create(int _thread_count, int _queue_size = 1 << 11);

    int submit(
            const std::shared_ptr<void> &args,
            const std::function<void(std::shared_ptr<void>)> &func
    );

    int free_resource();

    static ThreadPool *instance();

    static void shutdown(ShutdownMode mode = GRACEFUL_SHUTDOWN);

    static std::string current_thread_name();

private:
    ThreadPool();

    virtual ~ThreadPool() = default;

    static void *thread_routine(void *args);

    int shutdown_interval(ShutdownMode mode);

    pthread_mutex_t lock_;
    pthread_cond_t cond_;

    std::vector<pthread_t> threads_;
    std::vector<ThreadPoolTask> work_queue;
    std::atomic_int16_t thread_count_;
    int queue_size_;
    int head_;
    int tail_;
    int work_count;
    std::atomic_bool shutdown_;
    volatile int shutdown_mode_;
    std::atomic_int16_t started_num_;
};

#endif //MYWEBSERVER_THREADPOOL_H
