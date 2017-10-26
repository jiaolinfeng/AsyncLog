/**
 * @file flow_limit.h
 * @brief flow limit
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-25
 */

#ifndef _FLOW_LIMIT_H_
#define _FLOW_LIMIT_H_

#include <sys/time.h>
#include <stdio.h>

class FlowLimit
{
public:
    FlowLimit(unsigned max_req_per_sec)
    {
        last_sec_ = last_usec_ = offset_ = req_cnt_ = 0;
        max_speed_ = 1.0 * max_req_per_sec / 1000;
    }

    bool shouldDo()
    {
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        if (!last_sec_ && !last_usec_) // init
        {
            last_sec_ = tv.tv_sec;
            offset_ = last_usec_ = tv.tv_usec;
        }
        else if (tv.tv_sec == last_sec_) // same seconds
        {
            double cur_speed = 1000.0 * req_cnt_ / (tv.tv_usec - offset_);
            last_usec_ = tv.tv_usec;
            if (cur_speed > max_speed_)
                return false;
        }
        else // second switch
        {
            double cur_speed = 1000.0 * req_cnt_ /
                (tv.tv_usec - last_usec_ + (tv.tv_sec - last_sec_) * 1000000);
            offset_ = last_usec_ = tv.tv_usec;
            last_sec_ = tv.tv_sec;
            req_cnt_ = 0;
            if (cur_speed > max_speed_)
                return false;
        }
        return true;
    }

    void done()
    {
        ++req_cnt_;
    }

private:
    long last_sec_;
    long last_usec_;
    long offset_;
    long req_cnt_;
    double max_speed_;
};

#endif
