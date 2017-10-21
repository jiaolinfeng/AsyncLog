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

namespace log
{

class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&mtx_, NULL);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&mtx_);
    }

    void lock()
    {
        pthread_mutex_lock(&mtx_);
    }

    void unlock()
    {
        pthread_mutex_unlock(&mtx_);
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
    ScopedLock(Mutex *mtx) : mtx_(mtx)
    {
        mtx_->lock();
    }

    ~ScopedLock()
    {
        mtx_->unlock();
    }
private:
    Mutex *mtx_;
};

class Condition
{
public:
    Condition(Mutex *mtx) : mutex_(mtx)
    {
        pthread_condattr_t cond_attr;
        pthread_condattr_init(&cond_attr);
        pthread_condattr_setclock(&cond_attr, CLOCK_MONOTONIC);
        pthread_cond_init(&cond_, &cond_attr);
    }

    ~Condition()
    {
        pthread_cond_destroy(&cond_);
    }

    void wait(unsigned usec = 0)
    {
        if (!usec)
            pthread_cond_wait(&cond_, mutex_->getMutex());
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
            pthread_cond_timedwait(&cond_, mutex_->getMutex(), &tp);
        }
    }

    void wakeup()
    {
        pthread_cond_signal(&cond_);
    }

    void wakeupAll()
    {
        pthread_cond_broadcast(&cond_);
    }

private:
    Mutex *mutex_;
    pthread_cond_t cond_;
};

}

#endif
