/**
 * 2 数据共享问题分析
 *      2.1 只读的数据
 * 
 * 
 *      2.2 有读有写
 *          如果有2个线程写，8个线程读，如果代码没有什么特别的处理，那程序肯定崩溃。
 *          最简单的不崩溃处理方法——读的时候不能写，写的时候不能读。2个线程不能同时写，8个线程不能同时读。
 * 
 * 
 * 
*/


#include <iostream>
#include <thread>
#include <vector>
using namespace std;

vector<int> g_v = { 1,2,3 };// 共享数据=>只读

void myprint(int inum)
{
  //cout << "myprint线程开始执行了，线程编号 = " << inum << endl;
  //// 
  //cout << "myprint线程结束执行了，线程编号 = " << inum << endl;
  cout << "id为" << std::this_thread::get_id() << "的线程打印g_v值" << g_v[0] << g_v[1] << g_v[2] << endl;
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