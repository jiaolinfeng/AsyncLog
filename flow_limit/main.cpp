/**
 * @file main.cpp
 * @brief flow limit test
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-25
 */

#include <unistd.h>
#include <iostream>
#include <memory>
#include "timer.h"
#include "flow_limit.h"

int main()
{

    Timer timer;
    timer.init();
    FlowLimit flow_limit(20);

    for (int i = 0; i < 100; )
    {
        if (flow_limit.shouldDo())
        {
            std::cout << i++ + 1 << std::endl;
            flow_limit.done();
        }
        else
        {
            usleep(10000);
        }
    }
    timer.end();
    printf("%lfms\n", 1.0 * timer.diff() / 1000);

    return 0;
}
