/**
 * @file mutex.h
 * @brief mutex implementation
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-12
 */

#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <pthread.h>
#include <assert.h>

namespace jlf
{

#define RET_CHECK(ret) ({ __typeof__(ret) errnum = (ret); \
                          assert(errnum == 0);            \
                          (void) errnum;})

class Mutex
{
public:
    Mutex()
    {
        RET_CHECK(pthread_mutex_init(&mtx_, NULL));
    }

    ~Mutex()
    {
        RET_CHECK(pthread_mutex_destroy(&mtx_));
    }

    void lock()
    {
        RET_CHECK(pthread_mutex_lock(&mtx_));
    }

    void unlock()
    {
        RET_CHECK(pthread_mutex_unlock(&mtx_));
    }

private:
    friend class Condition;

    pthread_mutex_t* getMutex()
    {
        return &mtx_;
    }

private:
    pthread_mutex_t mtx_;
};

class ScopedLock
{
public:
    ScopedLock(Mutex& mtx) : mtx_(mtx)
    {
        mtx_.lock();
    }

    ~ScopedLock()
    {
        mtx_.unlock();
    }
private:
    Mutex &mtx_;
};

class Condition
{
public:
    Condition(Mutex& mtx) : mutex_(mtx)
    {
        pthread_condattr_t cond_attr;
        RET_CHECK(pthread_condattr_init(&cond_attr));
        RET_CHECK(pthread_condattr_setclock(&cond_attr, CLOCK_MONOTONIC));
        RET_CHECK(pthread_cond_init(&cond_, &cond_attr));
    }

    ~Condition()
    {
        RET_CHECK(pthread_cond_destroy(&cond_));
    }

    void wait(unsigned usec = 0)
    {
        if (!usec)
            RET_CHECK(pthread_cond_wait(&cond_, mutex_.getMutex()));
        else
        {
            struct timespec tp;
            clock_gettime(CLOCK_MONOTONIC, &tp);
            tp.tv_sec += usec / 1000000;
            tp.tv_nsec += usec % 1000000 * 1000;
            if (tp.tv_nsec > 1000000000)
            {
                tp.tv_nsec -= 1000000000;
                tp.tv_sec += 1;
            }
            RET_CHECK(pthread_cond_timedwait(&cond_, mutex_.getMutex(), &tp));
        }
    }

    void wakeup()
    {
        RET_CHECK(pthread_cond_signal(&cond_));
    }

    void wakeupAll()
    {
        RET_CHECK(pthread_cond_broadcast(&cond_));
    }

private:
    Mutex &mutex_;
    pthread_cond_t cond_;
};

}

#endif
