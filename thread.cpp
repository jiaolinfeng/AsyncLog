/**
 * @file thread.cpp
 * @brief thread implementation
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-11
 */
#include <sys/syscall.h>
#include <string>
#include <stdio.h>
#include "thread.h"

namespace log
{

Thread::~Thread()
{
    if (start_)
        stop();
}

pid_t Thread::currentThread()
{
    return (pid_t)syscall(SYS_gettid);
}

void* Thread::_thread_func_(void *arg)
{
    Thread *p_self = (Thread*)arg;
    if (p_self)
    {
        p_self->run();
        p_self->start_ = false;
    }
    else
    {
        throw std::string("Thread::_thread_func_ arg(this) is NULL");
    }
    pthread_exit(NULL);
}

int Thread::start()
{
    int res = 0;
    if (start_)
        return -1;
    if (func_)
        res = pthread_create(&thread_, NULL, func_, arg_);
    else
        res = pthread_create(&thread_, NULL, _thread_func_, this);
    if (res)
        return -1;
    is_running_ = start_ = true;
    return 0;
}

void Thread::stop()
{
    is_running_ = false;
    join();
    start_ = false;
}

void Thread::join()
{
    if (start_)
        pthread_join(thread_, NULL);
}

}
