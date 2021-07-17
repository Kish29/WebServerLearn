//
// Created by 蒋澳然 on 2021/7/17.
// email: 875691208@qq.com
// $desc
//

#include "ThreadPool.h"

#define coerce_in(a, l, r) a < l ? l : (a > r ? r : a)

std::mutex ThreadPool::mtx_{};
ThreadPool *ThreadPool::m_threadpool_ = nullptr;

/*pthread_mutex_t ThreadPool::lock_ = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ThreadPool::cond_ = PTHREAD_COND_INITIALIZER;

std::vector<pthread_t> ThreadPool::threads_{};
std::vector<ThreadPoolTask> ThreadPool::work_queue{};
int ThreadPool::thread_count_ = 0;
int ThreadPool::queue_size_ = 0;
int ThreadPool::curr_ = 0;
int ThreadPool::tail_ = 0;
int ThreadPool::work_count = 0;
int ThreadPool::shutdown_mode_ = 0;
int ThreadPool::started_num_ = 0;*/

ThreadPool::ThreadPool() :
        lock_(PTHREAD_MUTEX_INITIALIZER),
        cond_(PTHREAD_COND_INITIALIZER),
        threads_({}),
        work_queue({}),
        thread_count_(0),
        queue_size_(0),
        curr_(0),
        tail_(0),
        work_count(0),
        shutdown_(false),
        shutdown_mode_(GRACEFUL_SHUTDOWN),
        started_num_(0) {}

int ThreadPool::create(int _thread_count, int _queue_size) {
    // 参数合法化
    _thread_count = coerce_in(_thread_count, cpucore, MAX_THREADS);
    queue_size_ = coerce_in(_queue_size, 1024, MAX_QUEUE);

    thread_count_ = 0;
    threads_.resize(_thread_count);
    work_queue.resize(queue_size_);

    curr_ = tail_ = work_count = 0;
    started_num_ = 0;

    for (int i = 0; i < _thread_count; ++i) {
        if (pthread_create(
                &threads_.at(i),
                nullptr,
                thread_routine,
                this) != 0) {
            return -1;
        }
    }

//    args->instance = nullptr;
//    delete args;
//    args = nullptr;

    printf("ThreadPool create! thread_num is %d\n", _thread_count);

    return 0;
}

ThreadPool *ThreadPool::instance() {
    if (m_threadpool_ == nullptr) {
        std::unique_lock<std::mutex> lck(mtx_);
        if (m_threadpool_ == nullptr) {
            m_threadpool_ = new ThreadPool;
        }
        lck.unlock();
    }
    return m_threadpool_;
}

void ThreadPool::shutdown(ShutdownMode mode) {
    if (m_threadpool_ != nullptr) {
        std::unique_lock<std::mutex> lck(mtx_);
        if (m_threadpool_ != nullptr) {
            m_threadpool_->shutdown_interval(mode);
            delete m_threadpool_;
            m_threadpool_ = nullptr;
        }
        lck.unlock();
    }
}

int ThreadPool::submit(
        const std::shared_ptr<void> &args,
        const std::function<void(std::shared_ptr<void>)> &func
) {
    int next{}, err = 0;
    if (pthread_mutex_lock(&lock_) != 0) {
        return THREADPOOL_LOCK_FAILURE;
    }

    do {
        // 工作队列已满
        if (work_count == queue_size_) {
            err = THREADPOOL_QUEUE_FULL;
            break;
        }
        // 线程池已经关闭
        if (shutdown_) {
            err = THREADPOOL_SHUTDOWN;
            break;
        }
        next = (tail_ + 1) % queue_size_;
        work_queue.at(tail_).fun = func;
        work_queue.at(tail_).args = args;

        tail_ = next;
        work_count++;

        // 唤醒一个线程
//        printf("signal one thread\n");
        if (pthread_cond_signal(&cond_) != 0) {
            err = THREADPOOL_LOCK_FAILURE;
            break;
        }
    } while (false);

    // 释放锁
    if (pthread_mutex_unlock(&lock_) != 0) {
        err = THREADPOOL_LOCK_FAILURE;
    }
    return err;
}

int ThreadPool::shutdown_interval(ShutdownMode mode) {
    int err = 0;
    if (pthread_mutex_lock(&lock_) != 0) {
        return THREADPOOL_LOCK_FAILURE;
    }

    do {
        if (shutdown_) {
            err = THREADPOOL_SHUTDOWN;
            break;
        }

        // 设置shutdown模式
        shutdown_mode_ = mode;
        shutdown_ = true;
        // 通知所有线程shutdown
        if (pthread_cond_broadcast(&cond_) != 0 ||
            pthread_mutex_unlock(&lock_) != 0) {
            err = THREADPOOL_LOCK_FAILURE;
            break;
        }

        for (int i = 0; i < thread_count_; ++i) {
            if (pthread_join(threads_.at(i), nullptr) != 0) {
                err = THREADPOOL_THREAD_FAILURE;
                break;
            }
        }

    } while (false);

    if (!err) {
        free_resource();
    }
    return 0;
}

void *ThreadPool::thread_routine(void *args) {
    auto *instance = (ThreadPool *) args;

    // set name
    instance->started_num_++;
    std::string name = "thread-" + std::to_string(instance->thread_count_++);
    pthread_setname_np(pthread_self(), name.c_str());

    while (true) {
//        printf("thread is running...\n");
        ThreadPoolTask task;
        // 加锁，从工作队列中拿任务
        pthread_mutex_lock(&instance->lock_);
        while (!instance->shutdown_ && instance->work_count == 0) {
//            printf("thread wait\n");
            pthread_cond_wait(&instance->cond_, &(instance->lock_));
        }
        if (instance->shutdown_) {
            if (instance->shutdown_mode_ == IMMEDIATE_SHUTDOWN) {
                // 关闭线程池
                instance->started_num_--;
                pthread_mutex_unlock(&(instance->lock_));
                break;
            } else {
                printf("graceful shutdown threadpool\n");
                /* TODO: 优雅关闭应当等待任务队列中的任务都完成再关闭，并设置最大超时时间 */
                // 关闭线程池
                instance->started_num_--;
                pthread_mutex_unlock(&(instance->lock_));
                break;
            }
        }
        task.fun = instance->work_queue[instance->curr_].fun;
        task.args = instance->work_queue[instance->curr_].args;

        instance->work_queue[instance->curr_].fun = nullptr;
        instance->work_queue[instance->curr_].args.reset();

        instance->curr_ = (instance->curr_ + 1) % instance->queue_size_;
        instance->work_count--;
        // 释放锁
        pthread_mutex_unlock(&(instance->lock_));
        task.run();
//        sleep(1);
    }
    pthread_exit(nullptr);
}

int ThreadPool::free_resource() {
    if (started_num_ > 0) {
        return -1;
    }
    pthread_mutex_lock(&lock_);
    pthread_mutex_destroy(&lock_);
    pthread_cond_destroy(&cond_);
    return 0;
}

std::string ThreadPool::current_thread_name() {
#define BUFF_SIZE 255
    char buff[BUFF_SIZE];
    pthread_getname_np(pthread_self(), buff, BUFF_SIZE);
    return buff;
}
