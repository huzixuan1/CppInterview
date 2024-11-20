#include <iostream>
using namespace std;


// 这个函数只能抛出int float char三种类型的异常，抛出其他异常就会报错
void func() throw(int,float,char){
    throw "abc";
}


// 不能抛出任何异常
void func02() throw(){
    throw -1;
}

// 可以抛出任何异常
void func03(){
     
}


int main(){

    try{
        func();
    }catch(char* str){
        cout<<str<<endl;
    }catch(int e){
        cout<<"异常"<<endl;
    }catch(...){    // 捕获所有异常
        cout<<"未知类型异常"<<endl;
    }

    return 0;
}