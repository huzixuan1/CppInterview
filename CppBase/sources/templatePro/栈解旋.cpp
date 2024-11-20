#include <iostream>
using namespace std;

class Person{
    public:
        Person(){
            cout<<"对象构建"<<endl;
        }
        ~Person(){
            cout<<"对象析构"<<endl;
        }
};

int divide(int x,int y)
{
    Person p1,p2;   // 当y抛异常的时候，局部变量p1 p2就会被自动的析构掉
    if(y==0){
        throw y;    // 抛异常
    }

    return x/y;
}

void test(){
    try{
        divide(10,0);
    }catch(int e){
        cout<<"异常捕获！"<<endl;
    }
}

int main(){
    test();

    return 0;
}