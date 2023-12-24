#include "ThreadPool.h"

ThreadPool::ThreadPool(int size)
{
    _initSize = size;
    addThreadSize(size); //添加线程
}

ThreadPool::~ThreadPool()
{
    _run = false;
    not_empty.notify_all(); //唤醒所有阻塞的进程
    for(thread& thread:workers)
    {
        if (thread.joinable())  //一个线程对象可结合意味着它代表了一个正在运行的线程或者一个已经完成但尚未被合并的线程。
            thread.join(); // 等待任务结束， 前提：线程一定会执行完
    }
}

// 添加工作线程
void ThreadPool::addThreadSize(int size) 
{
    _run = true;
    for(int i = 0; i < size && workers.size() < THREADPOOL_MAX_NUM; i++)
    {
        workers.emplace_back([this]{this->DoTask(); });
        {
            unique_lock<mutex> lock{_lock};
            _idlThrNum++;
        }
        // workers.back().detach();
    }
}

void ThreadPool::DoTask()
{
    while(1)
    {
        Task task;
        {
            unique_lock<mutex> lock{ _lock};
            while(_run && tasks.empty())
                not_empty.wait(lock); 
            // 关闭线程池且任务队列为空，结束
            if(!_run && tasks.empty())
                return; //lock自行解锁
            _idlThrNum--;
            task = move(tasks.front());
            tasks.pop();
            
        }
        task(); //执行任务
#ifdef THREADPOOL_AUTO_GROW
        if(_idlThrNum>0 && workers.size() > _initSize)
            return;
#endif
        {
            unique_lock<mutex> lock{ _lock }; // 为什么加锁
            _idlThrNum++;
        }

        // 不需要唤醒生产者生产任务吗?
        // 不需要，因为使用的是任务队列，不会出现生产者被阻塞的情况
        
    }
}
