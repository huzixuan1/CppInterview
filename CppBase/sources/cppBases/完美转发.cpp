#include <iostream>
using namespace std;

/**
 *  完美转发:
 *      在函数模板中，可以将参数“完美”转发给其他函数。所谓的完美
 * 即不仅能准确的转发参数值，还能保证被转发参数的左右属性保持不变
 * C++11标准引入了右值引用和移动语义，所以能否实现完美转发，决定了
 * 该参数在传递过程使用的是拷贝语义还是移动语义。
 *
 *
 * 为了支持完美转发，C++11提供了一下方案：
 *  1、如果模板中(包括类模板和函数模板)，函数的参数书写成为T&&
 * 那么，函数既可以接受左值引用，又可以接受右值引用
 *
 * 2、提供了模板函数std::forword<T>(参数)，用于转发参数，如果参数是一个右值
 * 转发后仍是一个右值引用，如果参数是一个左值，转发后仍是一个左值引用。
 *
 *
 *
 */

void func1(int &ii)
{ // 如果参数是左值，调用此函数
    cout << "参数是左值=" << ii << endl;
}

void func1(int &&ii)
{ // 如果参数是右值，调用此函数
    cout << "参数是右值=" << ii << endl;
}

void func2(int &ii)
{ // 左值版本
    func1(ii);
}

void func2(int &&ii)
{                    // 右值版本
    func1(move(ii)); // 将左值转化为右值
}

// 模板方法
template <typename T>
void func2(T &ii)
{ // 左值版本
    func1(ii);
}

template <typename T>
void func2(T &&ii)
{ // 右值版本
    func1(move(ii));
}

/* 为了支持完美转发，C++11提供了一下方案：
 *  1、如果模板中(包括类模板和函数模板)，函数的参数书写成为T&&
 * 那么，函数既可以接受左值引用，又可以接受右值引用
 *
 * 2、提供了模板函数std::forword<T>(参数)，用于转发参数，如果参数是一个右值
 * 转发后仍是一个右值引用，如果参数是一个左值，转发后仍是一个左值引用。
 *
 */

// 解决方案一：
template <typename TT>
void func(TT &&ii)
{
    func1(forward<TT>(ii));
}

int main()
{
    int ii = 3;
    func1(ii); // 将调用左值版本
    func1(8);  // 将调用右值版本

    cout << "------" << endl;
    func2(ii);
    func2(8);

    cout<<"------解决方案1-----"<<endl;
    func(ii);
    func(7);

    return 0;
}