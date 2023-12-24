#include <stdio.h>
#include "threadpool.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
void taskfunction(void* arg)
{
    int num = *(int*)arg;
    printf("work %d is working, tid = %ld\n", num,pthread_self());
    usleep(1000);
}

int main()
{
    ThreadPool* pool = threadpool(3,10,100);//任务队列，最小线程数，最大线程数
    for(int i=0;i<100;i++)
    {
        int * num = (int *)malloc(sizeof(int));
        *num = i+100;
        threadPoolAdd(pool, taskfunction, num);
    }

    sleep(3);

    threadpollDestroy(pool);
    return 0;
}