| 智能指针类型       | 特点                             | 使用场景                                           |
|--------------------|----------------------------------|--------------------------------------------------|
| `std::unique_ptr`  | 独占所有权，轻量级，不允许复制。  | 对象的唯一所有者，资源的独占管理。                 |
| `std::shared_ptr`  | 共享所有权，引用计数管理资源的生命周期。 | 多个对象共享同一资源，需要动态分配资源并保证资源被自动释放。 |
| `std::weak_ptr`    | 不参与引用计数，提供对资源的弱引用。  | 防止循环引用，或需要观察但不拥有资源的场景（如缓存、观察者模式）。 |




#include <memory>
#include <iostream>
using namespace std;

/**
std::unique_ptr 是一种独占所有权的智能指针，表示某个资源只能被一个指针拥有。它是轻量级的智能指针，通常用于对象的独占管理。

主要功能和特点：
    禁止复制操作（拷贝构造和拷贝赋值被删除）。
    支持转移所有权（通过 std::move）。
    自动释放内存（调用资源的析构函数）。
常用函数：
    std::make_unique<T>(args...)：创建一个 std::unique_ptr。
    get()：返回裸指针，但不转移所有权。
    release()：释放所有权，返回裸指针，用户需要手动管理释放的内存,避免内存泄漏。
    reset(ptr)：将 std::unique_ptr 重置为管理新的资源，释放旧资源。
*/

class Resource
{
    private:
        int value_;
    public:
        Resource(int value):value_(value)
        {
            std::cout<<"Resource acquired,value = "<<value_<<std::endl;
        }
        ~Resource()
        {
            std::cout<<"Resource released"<<std::endl;
        }

        void show()
        {
            std::cout<<"test for Resource smartPoint"<<std::endl;
        }

        void doSometing()
        {
            std::cout<<"Doing someting……"<<std::endl;
        }

        void showValue()
        {
            std::cout<<"value:"<<value_<<std::endl;
        }
};

void  useRawPointer(Resource* rawPtr)
{
    // 接收裸指针并执行操作
    rawPtr->doSometing();
}

int main()
{

    // 基本使用方式

    std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>(10);

    // 转移所有权
    std::unique_ptr<Resource> ptr2 = std::move(ptr1);
    if(!ptr1){
        std::cout<<"ptr1 is now null"<<std::endl;
    }

    ptr2->show();

    if(ptr1)
    {
        ptr1->show();
    }else{
        std::cout<<"ptr1 is nullptr,cannt call show()"<<std::endl;
    }
    std::cout<<"------------get()----------"<<std::endl;
    // 自动释放内存，出作用域

    // get方式
    std::unique_ptr<Resource> ptr3 = std::make_unique<Resource>(20);
    Resource* rawPtr = ptr3.get();
    useRawPointer(rawPtr);

    std::cout<<"------------release()----------"<<std::endl;
    // release释放所有权
    auto ptr4 = std::make_unique<Resource>(30);
    Resource* raw = ptr4.release();
    if(!ptr4)
    {
        std::cout<<"smart ptr4 no longer owns the resourece"<<std::endl;
    }

    // 需要使用裸指针进行资源管理
    raw->doSometing();
    // 需要手动释放资源
    delete raw;

    std::cout<<"------------reset()----------"<<std::endl;
    // reset的使用方式
    std::unique_ptr<Resource> ptr5 = std::make_unique<Resource>(40);
    // 原资源操作
    ptr5->showValue();
    // 使用reset()重置为新的资源
    ptr5.reset(new Resource(50));
    // 新资源的操作
    ptr5->showValue();
    // 显示的释放资源
    ptr5.reset(nullptr);
    if(!ptr5)
    {
        std::cout<<"Resource has been released"<<std::endl;
    }
    return 0;
}


//////shared_ptr//////
#include <memory>
#include <iostream>
using namespace std;

/**
std::shared_ptr 是一种共享所有权的智能指针，
多个 shared_ptr 可以共同管理同一资源。当最后一个 std::shared_ptr 离开作用域时，资源会被释放。
主要功能和特点：
    引用计数机制：每次拷贝会增加引用计数，每次销毁会减少引用计数。
    自动释放资源：当引用计数为零时释放资源。
    线程安全的引用计数。
常用函数：
    std::make_shared<T>(args...)：创建一个 std::shared_ptr。
    use_count()：返回当前资源的引用计数。
    get()：返回裸指针，不影响引用计数。
    reset(ptr)：重置为新资源，引用计数归零时释放旧资源。
*/

class Resource
{
    public:
        Resource()
        {
            std::cout<<"Resource acquired"<<std::endl;
        }

        ~Resource()
        {
            std::cout<<"Resource release"<<std::endl;
        }

        void print()
        {
            std::cout<<"Resource is in use"<<std::endl;
        }
};

// Resource acquired
// Initial reference count:1
// Reference count after shaing:2
// Reference count after ptr2 goes out of scope:1
// Resource is in use
// Reference count after remain:1
// Resource acquired
// Resource release
// Reference count after reset:1
// Resource release

int main()
{

    // 1.使用std::make_shared<T>(args...)创建std::shared_ptr
    std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>();
    std::cout<<"Initial reference count:"<<ptr1.use_count()<<std::endl;

    //2.use_count()返回当前引用计数
    {
        std::shared_ptr<Resource> ptr2 = ptr1;  // 共享资源
        std::cout<<"Reference count after shaing:"<<ptr1.use_count()<<std::endl;
    }   // ptr2离开作用域，引用计数减1 （shared_ptr的生命周期和作用域也是相关的）

    std::cout<<"Reference count after ptr2 goes out of scope:"<<ptr1.use_count()<<std::endl;

    // 3.get()返回裸指针，不影响引用计数
    Resource* rawPtr = ptr1.get();
    rawPtr->print();    // 使用裸指针访问资源
    std::cout<<"Reference count after remain:"<<ptr1.use_count()<<std::endl;

    // 4.reset(ptr)重置为新的资源
    ptr1.reset(new Resource());     // 释放旧资源并管理新资源
    std::cout<<"Reference count after reset:"<<ptr1.use_count()<<std::endl;

    return 0;
}

// 手动实现UniquePtr

#include <iostream>

template<typename T>
class UniquePtr {
private:
    T* ptr;  // 用于管理原始指针

public:
    // 构造函数
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}

    // 禁止拷贝构造和拷贝赋值
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // 允许移动构造和移动赋值
    UniquePtr(UniquePtr&& other) noexcept {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // 重载 * 和 -> 操作符
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    // 显式布尔转换操作符
    explicit operator bool() const {
        return ptr != nullptr;
    }

    // 析构函数
    ~UniquePtr() {
        delete ptr;
    }
};

class Test {
public:
    void sayHello() {
        std::cout << "Hello from Test!" << std::endl;
    }
};


int main() {
    UniquePtr<Test> p1(new Test());  // 创建 UniquePtr
    p1->sayHello();

    UniquePtr<Test> p2 = std::move(p1);  // 转移所有权
    if (!p1) {
        std::cout << "p1 is now empty!" << std::endl;
    }
    p2->sayHello();  // p2 拥有对象

    return 0;
}


// 手动实现SharedPtr

#include <iostream>

template<typename T>
class SharedPtr {
private:
    T* ptr;  // 管理的对象指针
    int* ref_count;  // 引用计数

public:
    // 构造函数
    explicit SharedPtr(T* p = nullptr) : ptr(p), ref_count(new int(1)) {
        if (!p) {
            *ref_count = 0;
        }
    }

    // 拷贝构造函数
    SharedPtr(const SharedPtr& other) : ptr(other.ptr), ref_count(other.ref_count) {
        (*ref_count)++;
    }

    // 拷贝赋值操作符
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            // 先减少当前对象的引用计数
            if (--(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }

            // 复制新的指针和引用计数
            ptr = other.ptr;
            ref_count = other.ref_count;
            (*ref_count)++;
        }
        return *this;
    }

    // 重载 * 和 -> 操作符
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    // 获取引用计数
    int use_count() const {
        return *ref_count;
    }

    // 析构函数
    ~SharedPtr() {
        if (--(*ref_count) == 0) {
            delete ptr;
            delete ref_count;
        }
    }
};

class Test {
public:
    void sayHello() {
        std::cout << "Hello from Test!" << std::endl;
    }
};


int main() {
    SharedPtr<Test> sp1(new Test());
    std::cout << "sp1 use count: " << sp1.use_count() << std::endl;

    {
        SharedPtr<Test> sp2 = sp1;  // 拷贝 sp1
        std::cout << "sp1 use count: " << sp1.use_count() << std::endl;
        std::cout << "sp2 use count: " << sp2.use_count() << std::endl;

        sp2->sayHello();  // 使用 sp2 调用方法
    }

    std::cout << "sp1 use count after sp2 out of scope: " << sp1.use_count() << std::endl;

    return 0;
}

