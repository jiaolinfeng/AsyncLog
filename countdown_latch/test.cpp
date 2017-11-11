/**
 * @file test.cpp
 * @brief test countdown latch
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-11-11
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "countdown_latch.h"

jlf::CountDownLatch *latch = NULL;

void* worker_func(void* arg)
{
    size_t num = reinterpret_cast<size_t>(arg);
    if (num)
        sleep(num);
    printf("%ld thread signal\n", num + 1);
    latch->signal();
    pthread_exit(NULL);
}

int main()
{
    const size_t thread_num = 5;
    latch = new jlf::CountDownLatch(thread_num);
    pthread_t thds[thread_num];
    for (size_t i = 0; i < thread_num; ++i)
    {
        pthread_create(&thds[i], NULL, worker_func, reinterpret_cast<void*>(i));
    }
    latch->wait();
    for (size_t i = 0; i < thread_num; ++i)
    {
        pthread_join(thds[i], NULL);
    }
    delete latch;
    return 0;
}
