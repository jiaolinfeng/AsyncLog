/**
 * @file buffer.cpp
 * @brief buffer implementation
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-21
 */

#include "buffer.h"

namespace jlf
{

Buffer::~Buffer()
{
    release();
}

int Buffer::init(size_t size, Type type, bool renew)
{
    if (buf_)
    {
        if (renew)
            release();
        else
            return -1;
    }
    size_ = size;
    type_ = type;
    buf_ = (char *)malloc(POS_LEN + size_ + 1); // pos + data + \0
    if (buf_ == NULL)
        return -1;
    return 0;
}

void Buffer::release()
{
    if (buf_)
    {
        free(buf_);
        buf_ = NULL;
    }
}

int Buffer::append(char c)
{
    if (getSpareCap() < 1)
        return -1; // no memory
    *(getBuf() + getPos()) = c;
    incPos(1);
    return 0;
}

int Buffer::append(const char* str, size_t len)
{
    if (!str)
        return -1;
    size_t str_len = len;
    if (!str_len)
        str_len = strlen(str);
    if (getSpareCap() < str_len)
        return -1;
    memcpy(getBuf() + getPos(), str, str_len);
    incPos(str_len);
    return 0;
}

}
