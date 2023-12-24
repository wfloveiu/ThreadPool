#include "ThreadPool.h"
#include <iostream>


void fun1(int slp)
{
    printf("  hello, fun1 !  %d\n" , this_thread::get_id());
    if (slp>0) {
        printf(" ======= fun1 sleep %d  =========  %d\n",slp,  this_thread::get_id());
        this_thread::sleep_for(chrono::milliseconds(slp));
    }
}

struct gfun {
    int operator()(int n) {
        printf("%d  hello, gfun !  %d\n" ,n, this_thread::get_id() );
        return 42;
    }
};

class A {
public:
    static int Afun(int n = 0) {   //函数必须是 static 的才能直接使用线程池
        cout << n << "  hello, Afun !  " << this_thread::get_id() << endl;
        return n;
    }

    static string Bfun(int n, string str, char c) {
        cout << n << "  hello, Bfun !  "<< str.c_str() <<"  " << (int)c <<"  " << this_thread::get_id() << endl;
        return str;
    }
};

void test1()  //使用简单的lamda表达式作为任务函数
{
    ThreadPool pool(4);
    vector< future<int> > results;
    // 可以通过future来异步获取任务的执行结果，但是cout这样的打印结果还是在任务执行时打印输出
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.commit([i] {
                cout << "hello " << i << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "world " << i << endl;
                return i*i;
            })
        );
    }
    cout << " =======  commit all2 ========= " << this_thread::get_id() << endl;

    for (auto && result : results)
        cout << result.get() << ' ';
    cout << endl;
}

void test_func1()
{
    ThreadPool executor{ 50 };
    future<void> ff = executor.commit(fun1,0);
    cout << " =======  sleep ========= " << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::microseconds(900));
    ff.get(); //调用.get()获取返回值会等待线程执行完,获取返回值,void类型无返回值
}
void test_gfun()
{
    ThreadPool executor{ 50 };
    future<int> fg = executor.commit(gfun{},0);
    this_thread::sleep_for(chrono::microseconds(900));
    cout << fg.get() << "  " << this_thread::get_id() << endl;
}
void test_classA()
{
    ThreadPool executor{ 50 };
    A a;
    future<int> gg = executor.commit(a.Afun, 9999); //IDE提示错误,但可以编译运行
    future<string> gh = executor.commit(A::Bfun, 9998,"mult args", 123);
    this_thread::sleep_for(chrono::microseconds(900));
    cout<<gg.get()<<endl;
    cout<<gh.get()<<endl;
}

int main()
{
    // test1();
    // test_func1();
    // test_gfun();
    test_classA();
    return 0;
}

