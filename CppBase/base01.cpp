/**
 * 关于基础的类的一些代码复习
 * 构造函数的分类
 */

// 1.默认构造函数
// 作用：无参数或所有的参数都是默认值，用于对象的默认初始化
class MyClass
{
    public:
        MyClass(){}
};
MyClass obj;

// 2.有参构造函数
// 作用：通过参数初始化对象的成员变量，支持自定义初始化值

class MyClass{
    public:
        int x;
        MyClass(int value):x(value){}   // 有参构造函数
};
MyClass obj(10);    // 调用有参构造函数

// 3.拷贝构造函数
// 作用：通过同类的另一个对象（左值）来初始化当前对象，
//      通常用于深层拷贝或者浅层拷贝
// 特点：参数是一个const T&类型

class MyClass
{
    public:
        int x;
        MyClass(const MyClass& other):x(other.x){}  // 拷贝构造函数
};
MyClass obj1(10);
MyClass obj2 = obj1;    // 调用拷贝构造函数


// 4.移动构造函数（c++11）
// 作用：通过右值引用（T&&）转移资源的所有权到新对象，
//      避免不必要的拷贝，提高效率
// 特点：参数是一个右值引用（T&&）

class MyClass
{
    public:
        int* ptr;
        MyClass(MyClass&& other) noexcept:ptr(other.ptr)
        {
            other.ptr = nullptr;    // 转移所有权
        }   // 移动构造函数
};
MyClass obj1 = MyClass();   //调用移动构造函数


// 5.委托构造函数（c++11）
// 作用：一个构造函数调用同类的另一个构造函数避免重复代码

class MyClass
{
    public:
        int x;
        int y;
        MyClass(int a,int b):x(a),y(b){}    // 主构造函数
        MyClass():MyClass(0,0){}    // 委托构造函数
};
MyClass obj;    // 调用委托构造函数

// 6.显示构造函数（c++11）
// 作用：用explicit修饰的构造函数不能被隐式调用

class MyClass
{
    public:
        explicit MyClass(int value){}   // 显示调用
};
MyClass obj1 = 10;  // 错误：禁止隐式调用
MyClass obj2(10);   // 正确

// 7.默认化和删除构造函数（c++11引入）
// 作用：显示声明由编译器生成的默认构造函数
class MyClass
{
    public:
        MyClass() = default;    // 默认化构造函数
};

// 删除
// 作用：显示声明禁用某些构造函数
class MyClass
{
    public:
        MyClass(int value) = delete;    // 禁止有参构造函数
};
MyClass obj(10);    // 错误：构造函数被删除

// 8.析构函数
// 作用：释放资源，清理内存

class MyClass
{
    public:
        ~MyClass(){
            // 清理逻辑
        }
};



