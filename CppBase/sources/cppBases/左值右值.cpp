#include <iostream>
using namespace std;


/**
 * 左值引用:有名字、可以取到地址的
 * 右值引用:没有名字的、取不到地址的、临时变量、计算的表达式
 * 右值引用:
 *      语法:变量类型 &&变量名称=右值
 *      简单的理解右值引用就是给临时变量、计算的表达式、取不到地址的
 *      起一个别名，使得其可以变成一个普通的变量一样使用：可以对其进行运算、可以取地址
 *      同时右值引用可以延长临时变量的生命周期
 * 
 * 左值引用:
 * 
*/



int num(){
    return 10;  // 右值
}

int main(){

    int a=10;   // a左值 10右值
    int &&data=20;  // data就是左值  20是右值
    data++;
    int ans=a+10;       // a+10 右值 ans左值
    cout<<a<<endl;
    cout<<data<<" "<<&data<<endl;
    cout<<ans<<endl;


    /**
     *  右值引用的主要目的是实现移动语句
     *  左值引用只能绑定左值，右值引用只能绑定右值，
     * 但是常量左值引用却是一个奇葩，可以算是一个万能的引用类型了
    */

   int &&data1=90;  // 现在的data1就变成了左值，
   int &data2=data1;   // 所以下面的引用只能用左值引用&data2来进行，使用&&data2就会报错
//    int &&data2=data1;  // 无法将右值引用绑定到左值

   cout<<data2<<endl;

    // 常量左值引用（万能）
    cout<<"-------------"<<endl;

    int aa=1;
    const int &ra=aa;    // aa是非常量左值

    const int b=1;
    const int &rb=b;    // b是常量左值 rb是常量左值引用

    const int &rc=1;    // 1是右值  rc是常量左值引用

    cout<<ra<<" "<<rb<<" "<<rc<<endl;







    return 0;
}
