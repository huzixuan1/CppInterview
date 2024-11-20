#include <iostream>
using namespace std;

/**
 * 1、函数模板可以像普通函数那样被重载
 * 2、C++优先考虑普通函数
 * 3、如果函数模板可以产生一个更好的匹配，那么选择函数模板
 * 4、可以通过空模板实参列表的语法限定编译器只能通过模板匹配 MyAdd<>()
*/

template<typename T>
int MyAdd(T a,T b)
{
    return a+b;
}

int MyAdd(int a,char c)
{
    return a+c;
} 

int main()
{
    int a=1;
    int b=3;
    char c='b';
    char d='d';
    cout<<MyAdd(a,b)<<endl; // 调用模板函数(不会进行类型转换)
    cout<<MyAdd(a,c)<<endl; // 调用自定义函数(会进行类型转换)
    cout<<MyAdd(c,d)<<endl; // 调用自定义函数

    return 0;
}

// 函数模板->模板函数->被调用
// 编译器会对函数模板进行两次编译，
// 编辑器并不是把函数模板处理成能够处理任何类型的函数
// 函数模板通过具体的类型产生不同的函数
// 编译器会对函数模板进行两次编译，在声明的地方对模板代码进行编译，
// 在调用的地方对参数替换后的代码进行编译