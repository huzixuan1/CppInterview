/**
 * 有读有写的解决方法
 */

/**
  1 互斥量（mutex）的基本概念
        保护共享数据，操作时，用代码把共享数据锁住，其他想操作共享数据的线程必须等待解锁。
        互斥量是一个类对象。理解成一把锁，多个线程尝试用lock()成员函数来加锁这把锁头，只有一个线程能锁定成功（成功的标志是lock()函数返回）.
        如果没锁成功，那么流程卡在Lock()这里不断地尝试去锁这把锁头。
        互斥量使用要小心，保护数据不多也不少，少了，没达到保护效果；多了，影响效率。
*/

/**
 * 2 互斥量的用法
        2.1 lock(), unlock()
        使用时，需要引入mutex库。
    步骤：先lock()->操作共享数据->unlock()
    注意：Lock()和unlock()要成对使用！不允许非对称数量的调用。
*/

// 对有读有写的案例代码

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
        for (int i = 0; i < 5; ++i)
        {
            cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
            my_mutex.lock();
            msgRecvQueue.push_back(i);
            my_mutex.unlock();
        }
        return;
    }
    bool outMsgLULPro(int &command)
    {
        my_mutex.lock();
        if (!msgRecvQueue.empty())
        {
            // 取数据 & 去除这个数据
            int commond = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            my_mutex.unlock();
            return true;
        }
        my_mutex.unlock();
        return false;
    }

    // 取出数据的线程
    void outMsgRescQueue()
    {
        int command = 0;

        for (int i = 0; i < 5; ++i)
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
    std::mutex my_mutex;         // 私有互斥量
};

int main()
{
    A myobja;
    std::thread myOutMsgobj(&A::outMsgRescQueue, &myobja);
    std::thread myInMsgobj(&A::inMsgRecvQueue, &myobja);
    myOutMsgobj.join();
    myInMsgobj.join();

    cout << "main over" << endl;
    return 0;
}
