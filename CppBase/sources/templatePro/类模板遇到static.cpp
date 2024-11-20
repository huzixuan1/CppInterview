#include <iostream>
using namespace std;

// 类模板遇到static
// static变量是属于每一个具体类的

template<class T>
class Person
{
    public:
        static int a;
};

// 类外实现
template<class T>
int Person<T>::a=0;

int main(){
    Person<int> p1,p2,p3;
    Person<char> pp1,pp2,pp3;
    p1.a=10;
    pp1.a=100;
    cout<<p1.a<<" "<<p2.a<<" "<<p3.a<<endl; // 10 10 10 static变量是属于每一个具体类的
    cout<<pp1.a<<" "<<pp2.a<<" "<<pp3.a<<endl;  // 100 100 100  static变量是属于每一个具体类的

    return 0;
}
