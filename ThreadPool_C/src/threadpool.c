#include "threadpool.h"
const int INCREASE_NUMBER = 2;
const int KILLNUMBER = 2;
ThreadPool *threadpool(int queueSize, int minNum, int maxNum)
{
    //malloc返回的是void类型，要强制类型转换
    ThreadPool* threadpool = (ThreadPool*)malloc(sizeof(ThreadPool));
    do
    {
        if(threadpool == NULL)
        {
            printf("malloc threadpoll error!\n");
            break;
        }

        // 初始化任务队列
        threadpool->taskQ = (Task*)malloc(sizeof(Task) * queueSize);
        if(threadpool->taskQ == NULL)
        {
            printf("malloc taskQ error\n");
            break;
        }
        threadpool->queueSize = queueSize;
        threadpool->queue_taskNum = 0;
        threadpool->queueFront = 0;
        threadpool->queueTail = 0;

        // 分配maxNum个pthread_t大小的连续内存
        threadpool->threadIDs = (pthread_t *)malloc(sizeof(pthread_t)*maxNum);
        if(threadpool->threadIDs == NULL)
        {
            printf("malloc threadIDs error!\n");
            break;
        }
        memset(threadpool->threadIDs, 0, sizeof(pthread_t) * maxNum);

        // 初始化线程相关变量
        threadpool->maxNum = maxNum;
        threadpool->minNum = minNum;
        threadpool->now_thread_Num = minNum; //设置当前存活数为minNum
        threadpool->busy_thread_Num = 0;
        threadpool->killNum = 0;

        // 初始化锁和条件变量
        if(pthread_mutex_init(&threadpool->mutexPool,NULL)!=0 ||
           pthread_mutex_init(&threadpool->mutexBusyNum, NULL)!=0 ||
           pthread_cond_init(&threadpool->notEmpty,NULL)!=0 ||
           pthread_cond_init(&threadpool->notFull,NULL)!=0)
        {
            printf("mutex or cond error\n");
            break;
        }

        threadpool->shutdown  = 0;

        // 创建线程，创建后立即执行线程
        pthread_create(&threadpool->manageID, NULL, manager, threadpool); 
        for(int i=0;i<minNum;i++)
        {
            pthread_create(&threadpool->threadIDs[i], NULL, worker, threadpool);
        } 
        return threadpool;
    } while (0);
    
    // 请求内存失败
    if(threadpool&&threadpool->threadIDs) free(threadpool->threadIDs);
    if(threadpool&&threadpool->taskQ) free(threadpool->taskQ);
    if(threadpool) free(threadpool); //最后释放threadpool
}

void* worker(void* arg)
{
    ThreadPool* threadpool = (ThreadPool*)arg;
    while (1) //不断访问线程池,去任务队列取任务
    {
        pthread_mutex_lock(&threadpool->mutexPool); //把对线程池的操作上锁
        while(threadpool->queue_taskNum==0 && !threadpool->shutdown)
        {
            pthread_cond_wait(&threadpool->notEmpty, &threadpool->mutexPool); //阻塞
            // 判断是否要销毁线程, 使用threadpool->killNum判断
            if(threadpool->killNum)
            {
                threadpool->killNum--;
                threadpool->now_thread_Num--;
                pthread_mutex_unlock(&threadpool->mutexPool);
                threadExit(threadpool); //直接退出，这个线程就结束了，但是退出前一定要解锁
            }
        }

        if(threadpool->shutdown)
        {
            pthread_mutex_unlock(&threadpool->mutexPool);
            pthread_exit(NULL);
        }

        Task task;
        task.function = threadpool->taskQ[threadpool->queueFront].function;
        task.arg = threadpool->taskQ[threadpool->queueFront].arg;
        // printf("index is %d,",threadpool->queueFront);
        threadpool->queueFront = (threadpool->queueFront+1) % threadpool->queueSize; //循环队列
        threadpool->queue_taskNum--;
        pthread_cond_signal(&threadpool->notFull);  //唤醒阻塞在添加任务的线程
        pthread_mutex_unlock(&threadpool->mutexPool);

        // 给busyNum+1,需要加锁，其实可以在上边的mutexPool加锁内部对busyNum的值修改
        // 但是这个操作只是设计busyNum，不影响其它临界资源的占用，因此最好是再单独设置一个控制busyNum值的锁
        // 同时，因为对busynum的访问比较频繁，需要额外加锁
        pthread_mutex_lock(&threadpool->mutexBusyNum);
        threadpool->busy_thread_Num++;
        pthread_mutex_unlock(&threadpool->mutexBusyNum);

        task.function(task.arg); //执行函数
        free(task.arg);
        task.arg = NULL;

        pthread_mutex_lock(&threadpool->mutexBusyNum);
        threadpool->busy_thread_Num--;
        pthread_mutex_unlock(&threadpool->mutexBusyNum);
    }
    return;
}

void* manager(void* arg)
{
    ThreadPool* threadpool = arg;

    while(!threadpool->shutdown)  // shutdown = 1时，manager函数就终止了
    {
        sleep(3);

        // 取出线程池中的任务数量和当前线程的数量,因为任务数量可能会被worker线程改变
        pthread_mutex_lock(&threadpool->mutexPool);
 
        int maxNum = threadpool->maxNum;


        // 满足条件就增加线程，不满足直接unlock
        // 当任务队列中的任务个数 > 当前总的线程 && 当前总的线程数 < 最大线程数
        if(threadpool->queue_taskNum > threadpool->now_thread_Num && threadpool->now_thread_Num < maxNum)
        {
            int counter = 0; // 本次增加的线程数，上限是2
            for(int i = 0;i<maxNum && counter< INCREASE_NUMBER && threadpool->now_thread_Num < maxNum;i++)
            {
                //两种情况，
                //开始时这个数组被初始化成0，或者是某个位置的线程指针因为被kill掉而置为0
               if(threadpool->threadIDs[i] == 0) 
               {
                    pthread_create(&threadpool->threadIDs[i], NULL, worker, threadpool);
                    counter++;
                    threadpool->now_thread_Num++;
               }
            }
            pthread_mutex_unlock(&threadpool->mutexPool);
        }
        else 
            pthread_mutex_unlock(&threadpool->mutexPool);

        // 杀死线程
        // 规则：忙的线程*2 < 总的线程数  && 总的线程数>最小线程数
        pthread_mutex_lock(&threadpool->mutexPool);
        if(threadpool->busy_thread_Num * 2 < threadpool->now_thread_Num && threadpool->now_thread_Num < threadpool->minNum)
        {
            threadpool->killNum = KILLNUMBER;
            pthread_mutex_unlock(&threadpool->mutexPool);
            // 要杀死线程的前提是线程数比任务数多，那就说明有很多线程因为notEmpty条件变量未达到而阻塞
            // 因此要杀死这些进程，就要首先让这些进程不阻塞，然后杀死
            for(int i=0; i<KILLNUMBER;i++)
                pthread_cond_signal(&threadpool->notEmpty);
        }
        else
            pthread_mutex_unlock(&threadpool->mutexPool);
    }
}


void threadExit(ThreadPool* threadpool)
{
    pthread_t thread_id = pthread_self(); //获取当前执行这个函数的进程id
    for(int i=0;i<threadpool->maxNum;i++)
    {
        if(threadpool->threadIDs[i] == thread_id) //遍历
        {
            threadpool->threadIDs[i] = 0;//重置
            printf("threadExit() called, %ld exiting...\n",thread_id);
            pthread_exit(NULL);
        }
    }

}

void threadPoolAdd(ThreadPool* threadpool, void(*func)(void*), void* arg)
{
    pthread_mutex_lock(&threadpool->mutexPool);
    while(threadpool->queueSize == threadpool->queue_taskNum && !threadpool->shutdown) //任务队列满了
    {
        pthread_cond_wait(&threadpool->notFull, &threadpool->mutexPool);
    }
    if(threadpool->shutdown)
    {
        pthread_mutex_unlock(&threadpool->mutexPool);
        return;
    }
    // 添加任务
    threadpool->taskQ[threadpool->queueTail].function = func;
    threadpool->taskQ[threadpool->queueTail].arg = arg;
    threadpool->queueTail = (threadpool->queueTail+1) % threadpool->queueSize;
    threadpool->queue_taskNum++;
    pthread_cond_signal(&threadpool->notEmpty);  //唤醒阻塞的工作线程
    pthread_mutex_unlock(&threadpool->mutexPool);
}



int threadBusyNum(ThreadPool* threadpool)
{
    pthread_mutex_lock(&threadpool->mutexBusyNum);  //获取busynum的值，需要加锁
    int busynum = threadpool->busy_thread_Num;
    pthread_mutex_unlock(&threadpool->mutexBusyNum);
    return busynum;
}

int threadAliveNum(ThreadPool* threadpool)  
{
    pthread_mutex_lock(&threadpool->mutexPool);  //获取busynum的值，需要加锁
    int alivenum = threadpool->now_thread_Num;
    pthread_mutex_unlock(&threadpool->mutexPool);
    return alivenum;
}

bool threadpollDestroy(ThreadPool* threadpool)
{
    if(threadpool == NULL)
        return false; //传过来的threadpool指针为空，有错误
    // 关闭线程池
    threadpool->shutdown = 1;
    // 阻塞回收管理线程
    pthread_join(threadpool->manageID,NULL);
    // 回收工作线程
    for(int i=0;i<threadpool->now_thread_Num;i++)
        pthread_cond_signal(&threadpool->notEmpty);
    // 释放堆内存
    if(threadpool->taskQ)
        free(threadpool->taskQ);
    if(threadpool->threadIDs)
        free(threadpool->threadIDs);
    pthread_mutex_destroy(&threadpool->mutexBusyNum);
    pthread_mutex_destroy(&threadpool->mutexPool);
    pthread_cond_destroy(&threadpool->notEmpty);
    pthread_cond_destroy(&threadpool->notFull);

    free(threadpool);
    threadpool = NULL;
}