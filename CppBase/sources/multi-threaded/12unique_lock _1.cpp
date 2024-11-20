// unique_lock取代lock_guard

// unique_lock是个类模板，工作中，一般lock_guard（推荐使用）;
// lock_guard取代了mutex的lock()和unlock()
// unique_lock比lock_guard灵活很多；但是效率差一点，内存占用多一点。
// 使用方法很简单，修改上次的代码：

void inMsgRecvQueue()
{
    for (int i = 0; i < 100000; ++i)
    {
        cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
        std::unique_lock<std::mutex> sbguard1(my_mutex1);
        msgRecvQueue.push_back(i);
    }
    return;
}

// unique_lock的第二个参数
// 像lock_guard一样，unique_lock支持很多标记，例如adopt_lock.  std::adopt_lock：
// 该参数表示这个互斥量已经被lock了（你必须把互斥量提前lock了，否则会报异常）。这个标记的效果就是“假设调用方
// 线程已经拥有了互斥的所有权（已经lock成功了）”
//    通知lock_guard不需要在构造函数中lock这个互斥量了。unique_lock也可以带std::adopt_lock标记，
//    含义相同，就是不希望unique_lock()的构造函数中lock这个mutex了。
// 修改代码，其中一个线程设置等待20s：

bool outMsgLULPro(int& command)
{
  std::unique_lock<std::mutex> sbguard(my_mutex1);
  std::chrono::milliseconds dura(20000);  // 20000ms = 20s
  std::this_thread::sleep_for(dura);    // 休息一段时间
//  这样等待一段时间的时候，另一个线程也在等待这个锁头，就会造成阻塞。

  if (!msgRecvQueue.empty())
  {
    // 取数据 & 去除这个数据
    int commond = msgRecvQueue.front();
    msgRecvQueue.pop_front();

    return true;
  }
  return false;
}


// std::try_to_lock
// 我们会尝试用mutex的lock()去锁定这个mutex，但如果没有锁定成功，我也会立即返回，并不会阻塞在那里
// 用这个try_to_lock的前提是你自己不能先lock

#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>
using namespace std;

class A {
public:
  // 收集数据的函数
  void inMsgRecvQueue()
{
    for (int i = 0; i < 10; ++i)
    {
      //my_mutex1.lock();
      std::unique_lock<std::mutex> sbguard1(my_mutex1,std::try_to_lock);
      if (sbguard1.owns_lock())
      {
        cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
        // 拿到了锁
        msgRecvQueue.push_back(i);
      }
      else
      {
        cout << "inMsgRecvQueue()执行，但是没有拿到锁" << i << endl;
      }
    }
    return;
  }
  bool outMsgLULPro(int& command)
{
    std::unique_lock<std::mutex> sbguard(my_mutex1);

    std::chrono::milliseconds dura(200);  // 200ms
    std::this_thread::sleep_for(dura);    // 休息一段时间

    if (!msgRecvQueue.empty())
    {
      // 取数据 & 去除这个数据
      int commond = msgRecvQueue.front();
      msgRecvQueue.pop_front();

      return true;
    }
    return false;
  }

  // 取出数据的线程
  void outMsgRescQueue()
{
    int command = 0;
    
    for (int i = 0; i < 10; ++i)
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

  cout << "main over" << endl;
  return 0;
}