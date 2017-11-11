/**
 * @file test.cpp
 * @brief test countdown latch
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-11-11
 */

#include <unistd.h>
#include <iostream>
#include <thread>
#include <memory>
#include "countdown_latch.h"


int main()
{
    constexpr size_t thread_num = 5;
    std::unique_ptr<jlf::CountDownLatch> latch(new
        jlf::CountDownLatch(thread_num));
    std::thread thds[thread_num];
    for (size_t i = 0; i < thread_num; ++i)
    {
        thds[i] = std::thread([&latch, i]
        {
            if (i)
                sleep(i);
            std::cout << i + 1 << " thread signal\n";
            latch->signal();
        });
    }
    latch->wait();
    for (size_t i = 0; i < thread_num; ++i)
    {
        thds[i].join();
    }
    return 0;
}
