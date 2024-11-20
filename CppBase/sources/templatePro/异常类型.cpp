#include <iostream>
using namespace std;

// 异常类型和异常变量的生命周期
// throw的异常是有类型的，可以是数字、字符串、类对象
// throw的异常是有类型的，catch需要严格匹配类型
void func01()
{
    throw -1;
}

void func02()
{
    throw "exception";
}

class MyException
{
public:
    MyException(char* str){
        error=new char[strlen(str)+1];
        strcpy(error,str);
    }
    void what()
    {
        cout << "未知异常" << endl;
    }

    ~MyException(){
        if(error!=nullptr){
            delete[] error;
        }
    }
public:
    char* error;
};

void func03()
{
    throw MyException("我刚写的异常");
}

void test01()
{
    try
    {
        func01();
    }
    catch (int e)
    {
        cout << "捕获异常" << endl;
    }

    try
    {
        func02();
    }
    catch (char const *e) // char const *e
    {
        cout << "捕获异常" << endl;
    }

    try
    {
        func03();
    }
    catch (MyException e)
    {
        e.what();
    }
}

int main()
{
    test01();
    return 0;
}