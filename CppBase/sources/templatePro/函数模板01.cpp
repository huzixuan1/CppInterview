#include <iostream>
using namespace std;

template<class T>
void MySwap(T &a,T &b)
{
    T temp=b;
    b=a;
    a=temp;
}

int main()
{
    // 自动推导的方式MySwap();
    // 显式调用MySwap<int,int>();
    // 测试函数模板的使用情况(int)
    int a=1;
    int b=2;
    cout<<"a="<<a<<" "<<"b="<<b<<endl;
    MySwap(a,b);
    cout<<"a="<<a<<" "<<"b="<<b<<endl;

    // 测试函数模板的使用情况(char)
    char c='u';
    char d='p';
    cout<<"c="<<c<<" "<<"d="<<d<<endl;
    MySwap(c,d);
    cout<<"c="<<c<<" "<<"d="<<d<<endl;

    // 测试函数模板的使用情况(double)
    double d1=12.89;
    double d2=34.78;
    cout<<"d1="<<d1<<" "<<"d2="<<d2<<endl;
    MySwap(d1,d2);
    cout<<"d1="<<d1<<" "<<"d2="<<d2<<endl;

    // 显式调用
    double q1=12.89;
    double q2=34.78;
    cout<<"q1="<<q1<<" "<<"q2="<<q2<<endl;
    MySwap<double>(q1,q2);
    cout<<"q1="<<q1<<" "<<"q2="<<q2<<endl;

    

    return 0;
}