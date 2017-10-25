/**
 * @file common.h
 * @brief common utilities
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-12
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <limits.h>
#include <stdlib.h>

namespace jlf
{

static size_t normalize(size_t size)
{
    size_t actual_size = 1;
    while (actual_size && actual_size < size) // in case of overflow
    {
        actual_size *= 2;
    }
    if (!actual_size)
        actual_size = ULONG_MAX;
    return actual_size;
}

}

#endif

