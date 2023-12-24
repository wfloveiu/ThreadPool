#include <pthread.h>
#include <stdbool.h>
#ifndef _THREADPOOL_H

typedef struct Task
{
    void (*function) (void * arg); //函数指针
    void * arg;    //函数参数
}Task;

typedef struct ThreadPool
{
    Task * taskQ;//任务队列
    int queueSize; //任务队列的最大容量（自定义）
    int queue_taskNum;  //当前任务队列中的任务
    int queueFront;  //队头
    int queueTail;   //队尾

    pthread_t manageID; //管理者线程ID
    pthread_t *threadIDs; //工作者线程ID数组指针

    int minNum;  //线程的最小值（自定义）
    int maxNum;  //线程的最大值（自定义）
    int now_thread_Num;  //当前总的存活的线程数量
    int busy_thread_Num; //当前正在工作的线程数量
    int killNum; //当前需要杀死的线程数量（自定义）

    pthread_mutex_t mutexPool; //锁住整个线程池
    pthread_mutex_t mutexBusyNum;
    pthread_cond_t notFull; //任务队列满
    pthread_cond_t notEmpty;  //任务队列空

    int shutdown; //是否销毁线程池
}ThreadPool;


/*函数声明*/

// 初始化线程池
ThreadPool* threadpool(int queueSize, int minNum, int maxNum);

// 给线程池添加任务
void threadPoolAdd(ThreadPool* threadpool, void(*func)(void*), void* arg);
// 获取线程池中工作的线程个数
int threadBusyNum(ThreadPool* threadpool);
// 获取活着的总的线程个数
int threadAliveNum(ThreadPool* threadpool);
// 销毁线程池
bool threadpollDestroy(ThreadPool* threadpool);

void* worker(void * arg);//为什么是void

void* manager(void* arg);

void threadExit(ThreadPool* threadpool);
#define _THREADPOOL_H
#endif