#include <iostream>
using namespace std;


// 函数指针
/**
 * 函数指针：函数的入口地址
 * 定义的函数指针表示可以指向一类函数
 * 可以直接使用函数指针对函数进行调用
 * 
*/
int add(int a,int b){
    return a+b;
}

int main(){

    int (*fp)(int,int);
    fp=add;
    cout<<fp(1,3)<<endl;

    return 0;
}