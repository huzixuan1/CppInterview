#include <iostream>
using namespace std;

/**
 * 类型转换：
 *  类型转换的含义是通过改变一个变量的类型为别的类型，从而
 *  改变改变量的表示方式，C中的类型转换Type b=(Type)a;
 *  C++提供了4种类型转换操作符来应对不同场合的应用
 *  static_cast:一般类型的转换
 *  dynamic_cast:通常在基类和派生类之间转换使用
 *  const_cast:主要针对const的转换
 *  reinterpret_cast:用于进行没有任何关联之间的转换，比如一个指针类型转换为整型
 * 
*/

class Building{};
class Animal{};
class Cat:public Animal{};


// static_cast
void test01()
{
    int a=78;       // static_cast进行基础类型的转换
    char c=static_cast<char>(a);
    cout<<a<<endl;
    cout<<c<<endl;

    // 基础类型的指针转换
    // int *p=nullptr;  
    // char *sp=static_cast<char*>(p);  // 报错，没办法转

    // 对象指针
    // Building *building=nullptr;
    // Animal *ani=static_cast<Animal*>(building); // 报错，没办法转

    // 转换有继承关系的对象指针
    // 父类指针转子类指针
    Animal *ani=nullptr;
    Cat *cat=static_cast<Cat*>(ani);

    // 子类转父类指针
    Cat *soncat=nullptr;
    Animal *anifater=static_cast<Animal*>(soncat);
    
    // 父类引用转子类引用
    Animal aniobj;
    Animal& aniref=aniobj;
    Cat& cat1=static_cast<Cat&>(aniref);

    // 子类引用转父类引用
    Cat catobj;
    Cat& catref=catobj;
    Animal& anifater2=static_cast<Animal&>(catref);

    // 总结:
    // static_cast 用于内置的数据类型，还有具有继承关系的指针或者引用

}

// dynamic_cast
// 转换具有继承关系的指针或者引用，在转换前进行对象类型检查
void test02()
{
    // 基础数据类型
    // int a=10;
    // char c=dynamic_cast<char>(0);   // 没有办法转换

    // 非继承关系的指针
    // Animal* ani=nullptr;
    // Building* building=dynamic_cast<Building*>(ani);    // 没有办法转换

    // 具有继承关系指针
    // Animal* ani=nullptr;
    // Cat* cat=dynamic_cast<Cat*>(ani);    //  没有办法转换

    // 原因：因为dynamic_cast具有类型检查
    // 即:子类指针可以转换为父类指针(从大到小)，类型安全
    // 父类指针转成子类指针(从小到大)，不安全(容易访问到非授权的内存地址)

    // 子类转换为父类
    // Cat* cat2=nullptr;
    // Animal* ani=dynamic_cast<Animal>(cat2);  // 可以转换

    // 父类也可以转子类，有虚函数就行
    
    // 结论:dynamic只能转换具有继承关系的指针或引用，并且只能由子类型转换为基类型

}


// const_cast类型转换 指针 引用或者对象引用
// 
void test03(){

    // 基础类型指针
    int a=10;
    const int& b=a;
    int &c=const_cast<int&>(b);
    c=20;
    cout<<"a="<<a<<endl;

    // 指针
    const int *p=nullptr;
    int *p2=const_cast<int*>(p);

    int *p3=nullptr;
    const int *p4=const_cast<const int*>(p3);

    // 增加或者去除变量的const
}


// reinterpret_cast 强制类型转换 无关的指针类型，包括函数指针都可以进行转换
// 函数指针
typedef void(*FUNC1)(int,int);
typedef int(*FUNC2)(int,char*);

void test04(){
    // 1、无关类型的指针都可以进行转换
    Building *building =nullptr;
    Animal *ani=reinterpret_cast<Animal*>(building);

    // 2、函数指针的转换
    FUNC1 func1;
    FUNC2 func2=reinterpret_cast<FUNC2>(func1);

    // 1、程序员必须清除的知道要转变变量，转换前是什么类型，
    // 转换后是什么类型,以及转换后会出现什么后果

    //2、一般情况下不建议类型转换，避免类型转换
}

int main(){

    test01();

    test03();

    return 0;
}