/**
 * 1、示例演示多线程运行和结束
 *  1.1、thread
 *  1.2、join()
 *  1.3、detach()
 *  1.4、joinable()
 *
 * 2、其他创建多线程的手法
 *  2.1、类、以及一个问题的范例
 *  2.2、用lambda表达式
 */

// 1、示例演示多线程运行和结束
#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <string>
using namespace std;

// 自定义的线程也需要从一个函数(初始函数)开始运行
void myprint()
{
    cout << "我的线程开始执行了" << endl;

    cout << "我的线程执行完毕了" << endl;
}


// 其他创建线程的方式：类
class Ta{
    public:
        void operator()(){          // 不带参数,是可以调用的对象，其实质就是仿函数
            cout << "我的线程operator()开始执行了" << endl;
            cout << "我的线程operator()执行完毕了" << endl;
        }
};

int main()
{

    // 程序运行起来，生成一个进程，该进程所属的主线程开始自动的运行

    // 主线线程是从main()开始执行的,自定义的线程是
    // 从一个函数开始运行的(初始函数),一旦初始函数运行完毕,就代表着自定义线程结束
    // 整个进程是否执行完毕的标志是:主线程是否执行完毕,如果主线程执行
    // 完毕,就代表着进程执行完毕了，此时一般情况下如果其他子线程没有执行完毕，那么这些子线程
    // 也会被操作系统强行终止，所以想要保持子线程的运行状态，就需要让主线程一直保持运行状态

    // 创建一个多线程:
    // 1、需要引入头文件thread
    // 2、创建一个初始函数
    // 3、main()开始写代码

    /**
     *  需要明确一点，整个程序是两条线在运行即使有一条线被阻塞了也不影响其他的
     */

    // myobj是可调用对象
    thread myobj(myprint); // 1、创建了线程、线程的执行起点(入口)myprint; 2、myprint线程开始执行
    // join():起到阻塞的作用，让主线程等待子线程执行完毕，然后等待子线程和主线程的汇合，然后主线程再往下走
    myobj.join();
    // myobj.detach();  // 一旦使用了detach()的话,就没有办法使用join对其进行接管了,否则系统会异常

    // detach():分离，也就是主线程不等子线程汇合了
    /**
     * 引入detach()的原因：
     *  1、创建了比较多的子线程，让主线程逐个的等待子线程不太合适，所以使用detach()
     *  2、一旦detach()之后，与整个主线程关联的thread对象就会失去与这个主线程的关联,此时这个子线程就会驻留在
     *     后台运行,这个子线程就相当于被C++运行时库接管了，当子线程执行完成后，由运行时库负责清理该线程的相关资源(守护线程)
     * */

    // joinable():判断是否可以成功使用join()或者detach():返回true或false
    if (myobj.joinable())
    {
        cout << "myobj.joinable()==true" << endl;
    }
    else
    {
        cout << "myobj.joinable()==false" << endl;
    }

    cout << "main结束执行1" << endl;

    cout<<"-------------"<<endl;

    /**
     * 其他创建线程的方法：
     *  1、类：
    */
   // 类
   Ta ta;
   thread myobj2(ta);
   myobj2.join();   // 等待子线程执行结束
   cout<<"main函数执行结束2"<<endl;

   // lambda表达式
   auto myldathread=[]{
    cout<<"我的线程3开始了"<<endl;
    cout<<"我的线程3执行结束了"<<endl;
   };
   // 调用lambda
   thread myobj3(myldathread);
   myobj3.join();
   cout<<"main函数执行结束3"<<endl;

    return 0;
}