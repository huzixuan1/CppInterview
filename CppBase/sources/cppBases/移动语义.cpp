#include <cstring>
#include <iostream>
using namespace std;

/**
 *  C++移动语义：
 *      如果一个对象有堆区资源，需要编写拷贝构造函数和赋值函数实现深拷贝。
 *  深拷贝把对象中的堆资源复制了一份，如果源对象(被拷贝的对象)是临时的对象，
 *  拷贝完就没有用了，这样会造成没有意义的资源申请和释放操作。如果能够直接使用源对象拥有的资源，
 *  这样就可以大大的节省资源的申请和释放的时间。C++11中的移动语义就能够做到这一点。
 * 
 * 实现移动语义:
 *      <实现移动语义的花需要添加两个函数>
 * 1、移动构造函数
 * 2、移动赋值语句
 * 
 * 移动构造函数的语法：
 *      类名(类名&& 源对象){……}    // 类名&& 源对象：类的右值引用
 * 移动赋值语句的语法:
 *      类名& operator(类名 &&源对象){……}  // 类名&:返回左值引用  类名&& 源对象：类的右值引用
 *  
 * 
 * 
*/

class AA{
    public:
        int *m_data=nullptr;    // 数据成员，指向堆区资源的指针
        AA()=default;           // 启动默认构造函数

        void alloc(){   
            m_data=new int;     // 给数据成员m_data分配内存
            memset(m_data,0,sizeof(int));   // 初始化已分配的内存
        }

        AA(const AA& a){    // 拷贝构造函数
            cout<<"调用了拷贝构造函数 \n";  // 显示被调用日志
            if(m_data==nullptr) alloc();    // 如果没有分配内存，就分配
            memcpy(m_data,a.m_data,sizeof(int));    // 把数据从源对象中拷贝过来
            
        }

         AA(AA&& a){    // 移动构造函数
            cout<<"调用了移动构造函数 \n";  // 显示被调用日志
            if(m_data!=nullptr) delete m_data;    // 如果已经分配内存，就释放
            m_data=a.m_data;    // 把资源从源对象中转移过来
            a.m_data=nullptr;   // 把源对象中的指针置空            
        }

        AA& operator=(const AA& a){ // 赋值函数
            cout<<"调用了赋值函数\n";  // 显示自己被调用的日志
            if(this==&a) return *this;  // 避免自我赋值
            if(m_data==nullptr) alloc();    // 如果没有分配内存，就分配
            memcpy(m_data,a.m_data,sizeof(int));    // 把数据从源对象中拷贝过来
            return *this;
        }

        AA& operator=(AA&& a){ // 移动赋值函数
            cout<<"调用了移动赋值函数\n";  // 显示自己被调用的日志
            if(this==&a) return *this;  // 避免自我赋值
            if(m_data!=nullptr) delete m_data;    // 如果已经分配内存，就释放
            m_data=a.m_data;    // 把资源从源对象中转移过来
            a.m_data=nullptr;   // 把源对象中的指针置空
            return *this;
        }

    ~AA(){  // 析构函数
        if(m_data!=nullptr){
            delete m_data;
            m_data=nullptr;
        }
    }


};



int main(){
    AA a1;  // 创建对象a1
    a1.alloc(); // 分配堆区资源
    *a1.m_data=3;   // 给堆区内存赋值
    cout<<"a1.m_data="<<*a1.m_data<<endl;

    AA a2=move(a1);   // 将调用拷贝构造函数 
    cout<<"a2.m_data="<<*a2.m_data<<endl;

    AA a3;
    a3=move(a1);  // 将调用赋值函数
    cout<<"a3.m_data="<<*a3.m_data<<endl;

    cout<<"----------"<<endl;

    auto f=[]{  // 返回AA类对象的lambda函数
        AA aa;
        aa.alloc();
        *aa.m_data=8;
        return aa;
    };

    AA a4=f();  // lambda函数返回临时对象，是右值，将调用移动构造函数
    cout<<"a4.m_data="<<*a4.m_data<<endl;

    AA a6;
    a6=f();   // lambda函数返回临时对象，是右值，将调用移动构造函数
    cout<<"a6.m_data="<<*a6.m_data<<endl;


    cout<<"_________"<<endl;
    


    return 0;
}