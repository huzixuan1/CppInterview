// 普通类型元素 引用 指针
// 普通元素 异常对象catch处理完了之后就析构
// 引用的话 不用调用拷贝构造，异常对象catch处理完了之后就析构
// 指针

// C++中标准库中的异常是有限的，所以需要自己定义一些异常类
// 编写自己的异常类
// 自定义的异常类需要继承标准异常类，因为C++中可以抛出任何异常类型
// 所以我们的异常类不继承标准异常类的话，会导致程序混乱，尤其是多人
// 协同开发的时候
// 1、当继承标准异常类时，应该重载父类的what函数和虚构函数
// 2、因为栈展开的过程中，要复制异常类型，那么要根据你在添加的成员
//      考虑是否提供自己的复制构造函数

#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Person
{
public:
    Person(){
        mAge=0;
    }

    void setAge(int age){
        if(age<0 || age>100){
            throw out_of_range("年龄应该在0-100之间");
        }
        this->mAge=age;
    }
public:
    int mAge;
};

void test01(){
    Person p;
    try{
        p.setAge(1000);
    }catch(out_of_range e){
        cout<<e.what()<<endl;
    }
}


class MyOutOfRange:public exception{
public:
    MyOutOfRange(char *error){
        pError=new char[strlen(error)+1];
        strcpy(pError,error);
    }
    virtual const char *what() const{
        return pError;
    }
    ~MyOutOfRange(){
        if(pError!=nullptr){
            delete[] pError;
        }
    }
public:
    char *pError;
};

void func02(){
    throw MyOutOfRange("我自己的out_of_range");
}

void test02(){
    try{
        func02();
    }catch(exception e){
        cout<<e.what()<<endl;
    }
}

int main()
{
    test01();
    return 0;
}
