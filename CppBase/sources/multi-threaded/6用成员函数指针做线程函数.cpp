
#include <iostream>
#include <thread>
#include <string>
using namespace std;

class A
{
public:
  int m_i;
  // 类型转换构造函数，可以把一个int转换成一个类A对象。
  A(int a) :m_i(a) { cout << "[A::A(int a)构造函数执行]" << this << "thread_id = " << std::this_thread::get_id() << endl; 
    cout<<a<<endl;
  }
  A(const A &a):m_i(a.m_i){ cout << "[A::A(int a)拷贝构造函数执行]" << this << "thread_id = " << std::this_thread::get_id() << endl; }
  ~A() { cout << "[A::~A(int a)析构函数执行]" << this << "thread_id = " << std::this_thread::get_id() << endl; }

  void thread_work(int num)
{
    cout << "[子线程thread_work执行]" << this << "thread_id = " << std::this_thread::get_id() << endl;
    cout<<num<<endl;
  }
};

int main()
{
  A myobj(10); // 生成一个类对象
  thread mytobj(&A::thread_work, myobj, 15);

  mytobj.join();
  cout << "main over" << endl;
  return 0;
}