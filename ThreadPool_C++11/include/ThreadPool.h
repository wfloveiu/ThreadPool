#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>

using namespace std;
using Task = function<void()>; // 函数类型为void，无参数

#define THREADPOOL_MAX_NUM 16 // 线程池中线程的最大数量
// #define THREADPOOL_AUTO_GROW    // 是否可以自动增长
class ThreadPool
{
public:
    ThreadPool(int size = 4);
    ~ThreadPool();
    int idlCount() { return _idlThrNum; };     // 空闲线程数量
    int thrCount() { return workers.size(); }; // 总的线程数量
    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) -> future<decltype(f(args...))>
    {
        if(!_run)
            throw runtime_error("commit on ThreadPool is stopped.");
        using RetType = decltype(f(args...));
        auto task = make_shared<packaged_task<RetType()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );// 把函数入口即参数，打包绑定

        // 使用future来访问任务的执行结果
        future<RetType> future = task->get_future();
        {
            lock_guard<mutex> lock{_lock};
            tasks.emplace([task](){
                (*task)();
            });
        }
    #ifdef THREADPOOL_AUTO_GROW
        // 没有空闲线程并且未达到最多的线程数，可以再生成1个线程
        if(_idlThrNum < 1 && workers.size() < THREADPOOL_MAX_NUM)
            addThreadSize(1);
    #endif
        not_empty.notify_one();

        return future;
    }
private:
    int _initSize; // 初始化线程数量
    vector<thread> workers;
    queue<Task> tasks;
    mutex _lock; // 任务队列同步锁
#ifdef THREADPOOL_AUTO_GROW
    mutex _lock_grow;
#endif
    condition_variable not_empty;
    condition_variable nor_full;
    atomic<bool> _run{true};   // 线程池是否运行
    atomic<int> _idlThrNum{0}; // 空闲的线程数量

private:
    void addThreadSize(int size);
    void DoTask();
};
