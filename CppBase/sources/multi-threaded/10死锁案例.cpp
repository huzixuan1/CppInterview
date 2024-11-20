// 死锁案例
/**
 *
 *     两个锁——金锁，银锁和两个线程A，B
        （1）A执行时，这个线程先锁金锁，把金锁Lock()成功了，然后它去lock银锁。
        出现了上下文切换
        （2）B执行了，这个线程先锁银锁，因为银锁还没有被锁，所以银锁lock()成功了，B然后就去lock金锁。
                此时，死锁就产生了，两个线程互相锁住。
        （3）A因为拿不到银锁头，流程走不下去；
        （4）B因为拿不到金锁头，流程也走不下去。
 *
 * */

#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>
using namespace std;

class A
{
public:
    // 收集数据的函数
    void inMsgRecvQueue()
    {
        for (int i = 0; i < 100000; ++i)
        {
            cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
            my_mutex1.lock();
            my_mutex2.lock();

            msgRecvQueue.push_back(i);

            my_mutex2.unlock();
            my_mutex1.unlock();
        }
        return;
    }
    bool outMsgLULPro(int &command)
    {
        // std::lock_guard<std::mutex> sbguard(my_mutex); //
        my_mutex2.lock();
        my_mutex1.lock();
        if (!msgRecvQueue.empty())
        {
            // 取数据 & 去除这个数据
            int commond = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            my_mutex1.unlock();
            my_mutex2.unlock();

            return true;
        }
        my_mutex1.unlock();
        my_mutex2.unlock();
        return false;
    }

    // 取出数据的线程
    void outMsgRescQueue()
    {
        int command = 0;

        for (int i = 0; i < 100000; ++i)
        {
            bool result = outMsgLULPro(command);
            if (result == true)
            {
                cout << "outMsgRescQueue()执行，取出一个元素" << command << endl;
            }
            else
            {
                cout << "outMsgRescQueue()执行，但目前消息队列为空" << i << endl;
            }
        }
        cout << "end" << endl;
    }

private:
    std::list<int> msgRecvQueue; // 命令接收list
    std::mutex my_mutex1;
    std::mutex my_mutex2;
};

int main()
{
    A myobja;
    std::thread myOutMsgobj(&A::outMsgRescQueue, &myobja);
    std::thread myInMsgobj(&A::inMsgRecvQueue, &myobja);
    myOutMsgobj.join();
    myInMsgobj.join();

    // 这个程序在执行一段时间后就会停止

    cout << "main over" << endl;
    return 0;
}