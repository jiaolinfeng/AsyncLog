/**
 * @file log.cpp
 * @brief log implementation
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-12
 */

#include <stdlib.h>
#include <stdarg.h>
#include "log.h"
#include "common.h"

namespace jlf
{

static const unsigned BUF_SIZE = 200;

static const char* level_name[] =
{
    "DEBUG",
    "INFO",
    "ERROR",
    "FATAL"
};

Log* Log::inst = NULL;

int Log::FlushThread::init(Log *log)
{
    log_ = log;
    if (fd_)
    {
        close(fd_);
    }
    fd_ = open(log_->log_path_.c_str(), O_WRONLY | O_CREAT | O_APPEND,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd_ < 0)
        return -1;
    return 0;
}

void Log::FlushThread::run()
{
    if (!log_ || fd_ == -1)
        return;
    while (true)
    {
        if (log_->head_ == log_->tail_) // empty
        {
            ScopedLock lock(&log_->mtx_);
            if (log_->head_ == log_->tail_) // determine again after locked
                log_->cond_.wait(2000 * 1000);
            // if wakeup and still empty, switch buffer to flush
            if (log_->head_ == log_->tail_ &&
                log_->buffers_[log_->head_].getPos())
                log_->switchBuffer();
        }
        {
            ScopedLock lock(&log_->read_mtx_);
            Buffer &buf = log_->buffers_[log_->head_];
            write(fd_, buf.getBuf(), buf.getPos());
            buf.clear();
            log_->head_ = (log_->head_ + 1) % log_->buffer_level_;
        }
    }
}

int Log::init(const char* log_path, unsigned buffer_level, size_t buffer_size)
{
    if (!log_path)
        return -1;
    log_path_ = log_path;
    if (buffer_level_ < 4)
        buffer_level_ = 4;
    if (buffer_size < 1024)
        buffer_size = 1024;
    buffer_size_ = normalize(buffer_size);
    buffer_level_ = normalize(buffer_level);
    buffers_ = new(std::nothrow) Buffer[buffer_level_];
    if (buffers_ == NULL)
        return -1;
    for (unsigned idx = 0; idx < buffer_level_; ++idx)
    {
        int ret = buffers_[idx].init(buffer_size_);
        if (ret < 0)
        {
            delete[] buffers_;
            return -1;
        }
    }
    int ret = flush_thd_.init(this);
    if (ret < 0)
    {
        delete[] buffers_;
        return -1;
    }
    flush_thd_.start();

    return 0;
}

int Log::log(Level level, const char *str, ...)
{
    char buf[BUF_SIZE];

    time_t now;
    time(&now);
    struct tm time_val;
    localtime_r(&now, &time_val);
    int ret = snprintf(buf, BUF_SIZE, "%d-%02d-%02d %02d:%02d:%02d [%s]: ",
        time_val.tm_year + 1900, time_val.tm_mon + 1, time_val.tm_mday,
        time_val.tm_hour, time_val.tm_min, time_val.tm_sec, level_name[level]);
    if (ret < 0)
        return -1;

    int len = ret;

    ::va_list va_args;
    ::va_start(va_args, str);
    ret = ::vsnprintf(buf + ret, BUF_SIZE - ret, str, va_args);
    ::va_end(va_args);
    if (ret < 0)
        return -1;
    len += ret;
    if (len == BUF_SIZE)
        return -1;
    buf[len] = '\n';
    return append(buf, len);
}

int Log::append(const char* str, size_t len)
{
    if (!buffers_ || !str)
        return -1;
    size_t str_len = len;
    if (!str_len)
        str_len = strlen(str);
    {
        ScopedLock lock(&mtx_);
        Buffer *buf = &buffers_[tail_];
        if (buf->getSpareCap() < str_len)
        {
            // change buffer
            int ret = switchBuffer();
            if (ret < 0)
                return -1;
            buf = &buffers_[tail_];
            if (buf->getSpareCap() < str_len) // still can't store
                return -1;
        }
        buf->append(str, str_len);
    }
    return 0;
}

void Log::flush()
{
    if (!buffers_) // uninit
        return;
    ScopedLock lock(&mtx_);
    switchBuffer();
}

int Log::switchBuffer()
{
    if ((tail_ + 1) % buffer_level_ == head_) // full?
    {
        if (expandBuffer() < 0)
            return -1;
    }
    tail_ = (tail_ + 1) % buffer_level_;
    if (tail_ == (head_ + 1) % buffer_level_)
    {
        cond_.wakeup();
    }
    return 0;
}

int Log::expandBuffer()
{
    unsigned buffer_level = buffer_level_ * 2;
    Buffer *new_buffers = new(std::nothrow) Buffer[buffer_level];
    if (new_buffers == NULL)
        return -1;
    for (unsigned idx = tail_ + 1; idx <= tail_ + buffer_level_; ++idx)
    {
        int ret = new_buffers[idx].init(buffer_size_);
        if (ret < 0)
        {
            delete[] buffers_;
            return -1;
        }
    }
    {
        ScopedLock read_lock(&read_mtx_);
        for (unsigned idx = 0; idx <= tail_ ; ++idx)
        {
            new_buffers[idx].swap(&buffers_[idx]);
        }
        for (unsigned idx = tail_ + buffer_level_ + 1; idx < buffer_level;
             ++idx)
        {
            new_buffers[idx].swap(&buffers_[idx - buffer_level_]);
        }
        if (head_ > tail_)
        {
            head_ += buffer_level_;
        }
        buffer_level_ = buffer_level;
        buffers_ = new_buffers;
    }
    return 0;
}

Log::~Log()
{
    //TODO: clean memory
}

}
