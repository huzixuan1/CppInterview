#include <iostream>
using namespace std;

class Base{
    public:
        virtual void sleep(){
            cout<<"Base sleeping"<<endl;
        }

};

class A:public Base{
    public:
        void sleep(){
            cout<<"A sleeping"<<endl;
        }    
};



class B{
    public:
        void test(){
            cout<<"test nullptr是否可以访问"<<endl;
        }
};


class Animal{
    public:
        virtual void eat()=0;   // 定义一个纯虚函数
};

class Person:public Animal{
    public:
        void eat(){
            cout<<"吃饭"<<endl;
        }

        void sleep(){
            cout<<"人在睡觉"<<endl;
        }

};


int main(){

    Base *base=new A();
    base->sleep();

    // 验证p初始化为nulptr是否可以访问类成员变量
    B *p=nullptr;
    p->test();

    // 验证纯虚函数
    Animal *a=new Person();
    a->eat();

    Person *q=new Person();
    q->sleep();
    


    return 0;
}