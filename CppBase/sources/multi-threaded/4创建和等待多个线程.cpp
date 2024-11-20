/**
 * 创建多个线程时数据共享的问题:
 * 借助vector存放多个线程thread对象，借助vector和它的迭代器
 * 实现创建和运行多个线程
 * 
 * 
 * 运行结果，发现有些乱，因为各个线程是并发执行的,
 * 与操作系统内部的运行调度机制有关。主线程也是等待了所有子线程运行结束才结束。
 * 如果是只读数据的话,就没有太大的问题
*/

#include <iostream>
#include <thread>
#include <vector>
using namespace std;

void myprint(int inum)
{
  cout << "myprint线程开始执行了，线程编号 = " << inum << endl;
  cout << "myprint线程结束执行了，线程编号 = " << inum << endl;
  return;
}

int main()
{
  vector<thread> mythreads;
  // 创建10个线程，线程入口函数统一使用 myprint
  for (int i = 0; i < 10; i++)
  {
    // 创建线程，并开始执行
    mythreads.push_back(thread(myprint, i));
  }
  for (auto iter = mythreads.begin(); iter != mythreads.end(); ++iter)
  {
    iter->join();// 等待线程返回
  }

  cout << "main over" << endl;
  return 0;
}