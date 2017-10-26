/**
 * @file timer.h
 * @brief timer
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-26
 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include <string.h>
#include <sys/time.h>

struct Timer
{
    struct timeval tm_start;
    struct timeval tm_end;

    void init()
    {
        gettimeofday(&tm_start, NULL);
        memcpy(&tm_end, &tm_start, sizeof(tm_end));
    }

    void restart()
    {
        init();
    }

    void end()
    {
        gettimeofday(&tm_end, NULL);
    }

    long diff()
    {
        return ((tm_end.tv_sec - tm_start.tv_sec) * 1000000 +
            (tm_end.tv_usec - tm_start.tv_usec));
    }
};

#endif
