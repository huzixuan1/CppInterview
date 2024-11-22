/**
 * 3 共享数据的保护案例代码
    案例对象——网络游戏服务器。两个自己创建的线程，一个线程是手机玩家命令（用一个数字代表玩家发来的命令），并把命令数据写到一个队列；
    另外一个线程从队列中取出玩家发送来的命令，解析，然后执行玩家需要的动作。
        vector ,list=>
            list：频繁地按顺序插入和删除数据时效率高。
            vector随机地插入和删除数据效率高。
            应用成员函数来写一个示例：
 *
 *
*/

#include <iostream>
#include <thread>
#include <vector>
#include <list>
using namespace std;

class A
{
public:
    // 构造函数
    A(){
        cout<<"A的构造函数"<<endl;
    }

    A(const A &a){
        cout<<"A的拷贝构造函数"<<endl;
    }

    ~A(){
        cout<<"A的析构函数"<<endl;
    }


    // 收集数据的函数
    void isMsgRecvQueue()
    {
        for (int i = 0; i < 2; ++i)
        {
            cout << "isMsgRecvQueue()执行,插入一个元素" << i << endl;
            msgRecvQueue.push_back(i);
        }
    }

    // 取出数据的线程
    void outMsgRescQueue()
    {
        for (int i = 0; i < 2; ++i)
        {
            if (!msgRecvQueue.empty())
            {
                // 取数据 & 去除这个数据
                int commond = msgRecvQueue.front();
                msgRecvQueue.pop_front();
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
};

int main()
{
    A myobja;
    std::thread myOutMsgobj(&A::outMsgRescQueue, &myobja);
    std::thread myInMsgobj(&A::inMsgRecvQueue, &myobja);
    myOutMsgobj.join();
    myInMsgobj.join();

  // 两个线程一个写数据，一个读数据。运行后报错。这说明程序是不稳定的。解决办法就是需要互相锁定——“互斥量”。

  cout << "main over" << endl;
  return 0;
}
