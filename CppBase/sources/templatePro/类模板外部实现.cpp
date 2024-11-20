// 类模板的外部实现

#include <iostream>
using namespace std;

template <class T>
class Person
{
public:
    // 重载左移操作符
    template<class U>
    friend ostream& operator<<(ostream& os,Person<U>& p);
    
    // 或者这样写
    // friend ostream& operator<<<T>(ostream& os,Person<U>& p);


    Person(T id, T age);
    void show();

private:
    T mId;
    T mAge;
};

// 类模板外部实现
template <class T>
Person<T>::Person(T age, T id)
{
    this->mId = id;
    this->mAge = age;
}

template<class T>
void Person<T>::show()
{
    cout << "ID:" << mId << " "
         << "Age:" << mAge << endl;
}

// 实现重载左移操作符
template<class U>
ostream& operator<<(ostream& os,Person<U>& p)
{
    os<< "ID:" << p.mId << " "
         << "Age:" << p.mAge << endl;

         return os;
}


int main()
{
    Person<int> p(10,20);
    // p.show();
    cout<<p;

    return 0;
}


// note:    类模板的.h文件和.cpp文件分开写会出现的问题，以及原因
// 出现的问题:不在main函数调用的时候，可以编译通过，但是在调用的时候，就会出现编译不通过的问题
// 出现问题的原因:因为C++编译器是独立编译的，而.h文件中是表示申明的，不参与编译
// 在编译main函数中的对应类模板构造函数的时候，编译器没有找到对应的方法实现的时候，
// 会认为这个方法的实现是在其他的文件中的，所以编译器会跳过这个函数的编译，而让链接器去找
// 当链接器对应的去找的时候，由于它是一个类模板，而类模板的编译是需要进行两次的，
// 此时的链接器没有办法找到对应的模板类中的方法的实现，所以编译就会导致编译不通过

// 类模板在头文件和cpp文件分开写的时候，都会出现问题，
// 友元的话，出现的问题就会更多了

// 解决方案:在main函数文件中将#include "sample.h"改为#include"sample.cpp"这样写的话，就相当于，将
// .cpp文件中的代码都拷贝到main函数文件中了
// 一般建议：给sample.cpp修改为sample.hpp(为了提示别人，你写的是函数模板、类模板),同时将申明和实现都写在hpp文件中