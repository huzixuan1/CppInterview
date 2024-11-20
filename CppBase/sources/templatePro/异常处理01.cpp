#include <iostream>
using namespace std;

// 异常基本语法

int divide(int x,int y)
{
    if(y==0){
        throw y;    // 抛异常
    }

    return x/y;
}

void test01(){
    try{    // 试着去捕获异常
        divide(10,0);
    }catch(int e){   // 异常是根据类型进行匹配的
        cout<<"除数为:"<<e<<endl;
    }
}
    
void CallDivide(int x,int y){
    divide(x,y);
}

void test02(){
    try{
        CallDivide(10,0);
    }catch(int e){
        cout<<"除数为::"<<e<<endl;
    }
}

// 如果异常抛到顶层还没有进行处理的话，这个时候
// 会导致程序挂掉
// C++中的异常机制是跨函数的
// C++中的异常是必须处理的

int main(){

    test01();
    test02();
    return 0;
}