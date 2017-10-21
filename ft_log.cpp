/**
 * @file ut_log.cpp
 * @brief unit test for log
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-13
 */
#include <stdio.h>
#include "log.h"

using namespace jlf;

class LogThread : public Thread
{
protected:
    virtual void run()
    {
        char buf[200];
        Log* log = Log::instance();
        for (int i = 0; i < 1000; ++i)
        {
            sprintf(buf, "%d\n", i);
            int ret = log->log(Log::INFO, buf);
            if (ret < 0)
            {
                printf("error\n");
            }
        }
    }

};

int main()
{
    Log *log = Log::instance();
    if (log == NULL)
        return -1;
    int ret = log->init("log.txt", 2, 1024);
    if (ret < 0)
        return -1;

    unsigned thd_num = 4;
    LogThread *log_thd = new LogThread[thd_num];
    for (unsigned i = 0; i < thd_num; ++i)
    {
        log_thd[i].start();
    }
    for (unsigned i = 0; i < thd_num; ++i)
    {
        log_thd[i].join();
    }
    log->flush();
    sleep(2);
    return 0;
}
