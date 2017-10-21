/**
 * @file thread.h
 * @brief thread declaration
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-11
 */
#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

namespace jlf
{

typedef void* (*ThreadFunc) (void*);

class Thread
{
public:
    explicit
    Thread(ThreadFunc func = NULL, void *arg = NULL)
        : func_(func), arg_(arg), is_running_(false), start_(false)
    {

    }
    virtual ~Thread();

    /**
     * @brief threads start running
     *
     * @return 0 for success, otherwise -1
     */
    int start();

    /**
     * @brief  wait for thread stopped
     */
    void join();

    /**
     * @brief stop thread
     */
    void stop();

    /**
     * @brief get current thread id
     *
     * @return 线程id
     */
    static pid_t currentThread();
protected:
    virtual void run() {}

protected:
    ThreadFunc func_; ///< thread function
    void *arg_; ///< thread function arguments
    bool is_running_; ///< thread is running
    pthread_t thread_; ///< thread id

private:
    bool start_; ///< thread is start or not
    static void* _thread_func_(void *arg);
};

}

#endif
