/**
 * 1、传递临时对象作为线程参数：
 *  1.1、要避免的陷阱(解释1)
 *  1.2、要避免的陷阱(解释2)
 *  1.3、总结
 * 2、临时对象作为线程参数继续讲解
 *  2.1、线程id概念
 *  2.2、临时对象构造时机抓捕
 * 3、传递类对象、智能指针作为线程参数
 * 4、用成员函数指针做线程函数
*/  

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <thread>
using namespace std;

// void myprint(const int &i,char *pmybuf){
void myprint(const int i,const string &pmybuf){

    // 分析认为:i并不是myvar的引用，实际是值传递
    // 即便主线程detach了,子线程中的i值也是安全的
    cout<<i<<endl;    

    // 指针在detach子线程的时候,绝对有问题  
    // cout<<pmybuf<<endl;
    cout<<pmybuf.c_str()<<endl;

}


int main(){

    // 一：传递临时对象作为线程参数
    // 1.1、要避免的陷阱(解释1)
    // 1.2、要避免的陷阱(解释2)
    // 事实1:只要用临时的构造函数A类最为对象(string类)作为参数传递给线程，
    // 那么就一定能够在主线程执行完毕之前把参数传递给子线程,从而确保即便detach(),子线程也是安全执行的
    // 1.3、总结：（因对detach()）
    //          a)若传递int这种类型的简单类型,建议都是值传递,不要使用引用,防止节外生枝
    //          b)如果传递类对象,避免隐式类型转换,全部都在创建线程这一行就构建出临时对象来,然后在函数参数里,用引用来接
    //                  否则系统会重复构造
    // 1.4、终极结论:
    //          c)建议不使用detach(),只使用join(),这样就不存在局部变量失效导致线程对内存的非法引用问题


    // 2、临时对象作为线程参数继续讲解
    // 2.1、线程id:
    //          id就是一个数字,不管是主线程还是子线程,实际上都对应着一个数字,而且每个线程对应的数字都不一样
    //          线程id可以使用C++标准函数来获取,std::this_thread::get_id()来获取
    // 2.2、临时对象的捕获时机:
    //              

    int myvar=1;
    int &myvary=myvar;
    char mybuf[]="this is a test!";
    // 但是mybuf到底是在什么时候转换为string的
    // 事实上,存在mybuf都被回收了(main函数执行完了),系统才用mybuf去转string的可能性
    // thread myobj(myprint,myvar,mybuf);  
    // 在传参数之前,先写了一个临时对象,临时对象是一个将亡值，如果将其赋值给其他值后，它就会立即释放
    // 在创建线程的同时构造临时对象的方法传递参数是可行的
    thread myobj(myprint,myvar,string(mybuf));  // 这里直接将mybuf转换为string对象,这是一个可以保证在线程中用肯定有效的
    // myobj.join();
    myobj.detach();



    // 3、传递类对象、智能指针作为线程参数
    // std::ref 函数

    cout<<"main主线程执行结束"<<endl;


    return 0;
}









