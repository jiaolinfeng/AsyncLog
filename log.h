/**
 * @file log.h
 * @brief log declaration
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-12
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include "buffer.h"
#include "thread.h"
#include "mutex.h"

namespace jlf
{

class Log
{
public:
    enum Level
    {
        DEBUG = 0,
        INFO  = 1,
        ERROR = 2,
        FATAL = 3
    };
public:
    static Log* instance()
    {
        if (inst == NULL)
            inst = new Log;
        return inst;
    }

    int init(const char *log_path, unsigned buffer_level = 4,
            size_t buffer_size = 1024);

    int log(Level level, const char *str, ...);

    void flush();

private:
    Log() : head_(0), tail_(0), buffer_level_(0), buffer_size_(0),
            buffers_(NULL), mtx_(), cond_(&mtx_) {}

    ~Log();

    int append(const char *str, size_t len = 0);

    int switchBuffer();

    int expandBuffer();

private:
    class FlushThread : public Thread
    {
    public:
        FlushThread() : fd_(-1), log_(NULL)
        {

        }

        int init(Log *log);

    protected:
        virtual void run();

    private:
        int fd_;
        Log* log_;
    };

private:
    unsigned head_;
    unsigned tail_;
    unsigned buffer_level_;
    size_t buffer_size_;
    Buffer *buffers_;

    Mutex mtx_;
    Condition cond_;
    std::string log_path_;

    Mutex read_mtx_;

    FlushThread flush_thd_;

    static Log* inst;
};

}

#endif
