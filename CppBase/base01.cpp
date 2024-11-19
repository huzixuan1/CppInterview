
面试中的高频考点
以下是面试中相关问题的常见考察点，以及如何展示你的能力：

1. 构造函数和析构函数的作用
    可能的问题：
        为什么需要构造函数？
        什么时候需要拷贝构造函数和析构函数？
        如何避免内存泄漏？
    回答要点：
        构造函数用于对象初始化，析构函数用于资源释放。
        在需要深拷贝时，必须显式定义拷贝构造函数。
        在使用动态分配内存时，确保析构函数释放资源，防止内存泄漏。
    
    2. 拷贝构造函数 vs 移动构造函数
    可能的问题：
        拷贝构造函数和移动构造函数的区别？
        什么时候会调用移动构造函数？
        为什么要实现移动构造函数？
    回答要点：
        拷贝构造函数创建副本，移动构造函数转移资源所有权，避免昂贵的深拷贝。
        移动构造函数在处理临时对象（右值）时被调用，例如 std::move。
        移动构造函数提高性能，特别是在管理动态资源时。
    
    3. 委托构造函数和显式构造函数
    可能的问题：
        为什么要用委托构造函数？
        显式构造函数的作用是什么？
    回答要点：
        委托构造函数减少代码重复，特别是多个构造函数逻辑相似时。
        显式构造函数防止隐式类型转换引发的错误。
    
    4. 默认化与删除的构造函数
    可能的问题：
        为什么要使用 = default 和 = delete？
        什么场景下会删除构造函数？
    回答要点：
        = default 明确要求编译器生成默认实现。
        = delete 明确禁止特定行为，例如防止不合理的类型转换或拷贝。


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




// 实战

/**
 * 实战cpp中的构造函数
 */

#include <utility>
#include <iostream>

using namespace std;

class DynamicArray
{
private:
    int *data;   // 动态分配的内存
    size_t size; // 数组大小

public:
    // 1.默认构造函数
    DynamicArray() : data(nullptr), size(0)
    {
        std::cout << "Default Constructor called" << std::endl;
    }

    // 2.有参构造函数
    explicit DynamicArray(size_t n) : size(n)
    {
        data = new int(n);
        for (size_t i = 0; i < n; i++)
        {
            data[i] = 0; // 初始化为0
        }
        std::cout << "Parameterized Constructor called for size = " << size << std::endl;
    }

    // 3.拷贝构造函数
    DynamicArray(const DynamicArray &other) : size(other.size)
    {
        data = new int(size);
        for (size_t i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
        std::cout << "Copy Constructor called" << std::endl;
    }

    // 4.移动拷贝构造函数
    DynamicArray(DynamicArray &&other) noexcept : data(other.data), size(other.size)
    {
        other.data = nullptr; // 转移所有权
        other.size = 0;
        std::cout << "Move Constructor called" << std::endl;
    }

    // 5.委托构造函数
    DynamicArray(int defaultValue, size_t n) : DynamicArray(n) // 调用有参数构造函数
    {
        for (size_t i = 0; i < n; i++)
        {
            data[i] = defaultValue;
        }
        std::cout << "Delegating Constructor called with defaultValue = " << defaultValue << std::endl;
    }

    // 6.默认化构造函数
    // DynamicArray &operator=(const DynamicArray &) = default; //  使用编译器生成的赋值运算
    DynamicArray &operator=(const DynamicArray &other)
    {
        if (this == &other)
        {
            return *this; // 防止自赋值
        }

        // 释放当前对象的动态内存，防止内存泄露
        delete[] data;

        // 分配新的内存并拷贝数据
        size = other.size;
        data = new int(size);
        for (size_t i = 0; i < size; ++i)
        {
            data[i] = other.data[i];
        }

        std::cout << "Copy assignment operator called" << std::endl;
        return *this;
    }

    // 7.删除构造函数
    DynamicArray &operator=(DynamicArray &&) = delete; // 禁止移动复制运算符

    // 8.析构函数
    ~DynamicArray()
    {
        delete[] data;
        std::cout << "Destructor called" << std::endl;
    }

    // 打印数组内容
    void print() const
    {
        for (size_t i = 0; i < size; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

int main()
{
    // 1.默认构造函数
    DynamicArray arr1;

    // 2.有参构造函数
    DynamicArray arr2(5);
    arr2.print();

    // 3.拷贝构造函数
    DynamicArray arr3 = arr2;
    arr3.print();

    // 4.移动构造函数
    DynamicArray arr4 = std::move(arr3);
    arr4.print();

    // 5.委托构造函数
    DynamicArray arr5(42, 5);
    arr5.print();

    // 6.默认化构造函数（赋值运算符，未显示调用，隐式测试）
    arr1 = arr2; // 使用默认化的赋值运算符
    arr1.print();

    // 7.禁止移动赋值运算符（会导致编译错误）
    // arr1 = std::move(arr4);  // c错误：移动赋值运算符被删除

    return 0;
}

