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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include "thread.h"
#include "mutex.h"

namespace log
{

class Buffer
{
public:
    /**
     * @brief buffer type
     */
    enum Type
    {
        internal = 0, // heap
        shared = 1 // shm
    };

public:
    Buffer() : size_(0), type_(internal), buf_(NULL)
    {

    }

    ~Buffer();

    /**
     * @brief buffer init
     *
     * @return 0 for success, otherwises -1
     */
    int init(size_t size, Type type = internal, bool renew = false);

    /**
     * @brief release used memory
     */
    void release();

    /**
     * @brief append char to buffer
     *
     * @param char
     *
     * @return 0 for success, otherwise -1
     */
    int append(char c);

    /**
     * @brief append string to buffer
     *
     * @param str
     *
     * @return 0 for success, otherwise -1
     */
    int append(const char* str, size_t len = 0);

    /**
     * @brief get current data position
     *
     * @return data position
     */
    size_t getPos()
    {
        if (!buf_)
            return 0;
        return *(size_t *)buf_;
    }

    /**
     * @brief get current buffer addr
     *
     * @return buffer addr
     */
    char* getBuf()
    {
        if (!buf_)
            return NULL;
        return buf_ + POS_LEN;
    }

    /**
     * @brief get spare capacity
     *
     * @return spare capacity
     */
    size_t getSpareCap()
    {
        if (!buf_)
            return 0;
        return size_ - getPos();
    }

    void clear()
    {
        memset(buf_, 0, size_ + POS_LEN);
    }

    void swap(Buffer* buf)
    {
        if (buf == this)
            return;
        std::swap(buf_, buf->buf_);
        std::swap(type_, buf->type_);
        std::swap(size_, buf->size_);
    }

private:
    void incPos(size_t len)
    {
        *(size_t*)(buf_) += len;
    }

private:
    size_t size_; ///< buffer size
    Type type_; ///< buffer type(unused)
    char *buf_; ///< buffer

    static const unsigned POS_LEN = sizeof(size_t); ///< data position len
};

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
