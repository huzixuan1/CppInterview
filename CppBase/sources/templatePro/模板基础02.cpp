// 函数模板案例

// 求两个数(整数、双精度、字符、字符串等)的最大值，采用函数模板实现

#include <iostream>
#include <string>
using namespace std;

template<typename T>
T getmax(T a,T b)
{
  if(a>b){
    return a;
  }else{
    return b;
  }
}

int main(){
    
    cout<<getmax(6,7)<<endl;        // 示例化为模板函数int getmax(int,int)
    cout<<getmax('c','i')<<endl;
    cout<<getmax(6.5,1.2)<<endl;
    cout<<getmax("hello","world")<<endl;

    return 0;
}