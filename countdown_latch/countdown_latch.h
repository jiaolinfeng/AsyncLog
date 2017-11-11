/**
 * @file countdown_latch.h
 * @brief countdown latch
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-11-11
 */

#ifndef _COUNTDOWN_LATCH_H_
#define _COUNTDOWN_LATCH_H_

#if __cplusplus >= 201103L
#include <atomic>
#include <mutex>
#include <condition_variable>
#else
#include "mutex.h"
typedef unsigned long long uint64_t;
#endif

namespace jlf
{

class CountDownLatch
{
public:
    CountDownLatch(uint64_t count) : counter_(count), mtx_(),
#if __cplusplus >= 201103L
    cond_()
#else
    cond_(mtx_)
#endif
    {
    }

    void signal()
    {
#if __cplusplus >= 201103L
        std::unique_lock<std::mutex> lock(mtx_);
#else
        jlf::ScopedLock lock(mtx_);
#endif
        --counter_;
        if (counter_ <= 0)
        {
#if __cplusplus >= 201103L
            cond_.notify_all();
#else
            cond_.wakeupAll();
#endif
        }
    }

    void wait()
    {
        if (counter_)
        {
#if __cplusplus >= 201103L
            std::unique_lock<std::mutex> lock(mtx_);
            cond_.wait(lock);
#else
            jlf::ScopedLock lock(mtx_);
            cond_.wait();
#endif
        }
    }
private:
    uint64_t counter_;
#if __cplusplus >= 201103L
    std::mutex mtx_;
    std::condition_variable cond_;
#else
    jlf::Mutex mtx_;
    jlf::Condition cond_;
#endif
};

}

#endif

