/**
 * @file buffer.h
 * @brief buffer declaration
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-21
 */

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <unistd.h>
#include <string.h>
#include <algorithm>

namespace jlf
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

public:
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

}

#endif

