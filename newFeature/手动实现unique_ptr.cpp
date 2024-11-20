/**
 * 手动实现unique_ptr && shared_ptr
 */

// unique_ptr实现

#include <iostream>
using namespace std;

template <class T>
class UniquePtr
{
private:
    T *ptr;

public:
    // 构造函数
    explicit UniquePtr(T *p = nullptr) : ptr(p)
    {
    }

    // 禁止拷贝函数和拷贝赋值（因为unique_ptr是独占的
    UniquePtr(const UniquePtr &other) = delete;
    UniquePtr &operator=(const UniquePtr &other) = delete;

    // 允许移动构造和移动赋值
    UniquePtr(UniquePtr &&other) noexcept
    {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    UniquePtr &operator=(UniquePtr &&other) noexcept
    {
        if (this != &other)
        {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // 重载*和->运算符
    T &operator*() const
    {
        return *ptr;
    }

    T *operator->() const
    {
        return ptr;
    }

    T* get() const
    {
        return ptr;
    }

    void reset(T* p = nullptr)
    {
        delete ptr; // 删除当前对象
        ptr = p;    // 绑定新资源
    }

    // 显示布尔转换操作符
    explicit operator bool() const
    {
        return ptr != nullptr;
    }

    // 析构函数
    ~UniquePtr()
    {
        delete ptr;
        std::cout << "ptr has been release" << std::endl;
    }
};

// if you want to use c++14,you should use namespace,like as below:
namespace mystd{
template <typename T, typename... Args>
UniquePtr<T> make_unique(Args &&...args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
}

class Test
{
public:
    void sayHello()
    {
        std::cout << "hello from test!" << std::endl;
    }
};

int main()
{

    UniquePtr<Test> p1(new Test()); // 实现Unique_ptr
    p1->sayHello();

    UniquePtr<Test> p2 = std::move(p1); // 转移所有权
    if (!p1)
    {
        std::cout << "p1 is now empty!" << std::endl;
    }

    p2->sayHello(); // p2调用对象

    // 测试移动构造函数
    UniquePtr<Test> p3(new Test());
    UniquePtr<Test> p4(std::move(p3));

    if (!p3) // 测试bool
    {
        std::cout << "p3 is now empty!" << std::endl;
    }

    // 测试移动赋值
    UniquePtr<Test> p5;
    p5 = std::move(p4);
    if (!p4)
    {
        std::cout << "p4 is now empty" << std::endl;
    }

    // 测试make_unique
    UniquePtr<Test> p6 = mystd::make_unique<Test>();
    if(p6)
    {
        std::cout<<"p6 now is alive!"<<std::endl;
    }else{
        std::cout<<"p6 now has been relased"<<std::endl;
    }

    // 测试get
    Test *raw = p6.get();
    raw->sayHello();

    // 测试reset()
    p6.reset();     // 重置为nullptr
    if(!p6)
    {
        std::cout<<"p6 is now empty after reset!"<<std::endl;
    }

    p6.reset(new Test());   // 重新设置一个新的对象
    p6->sayHello();
    


    return 0;
}
