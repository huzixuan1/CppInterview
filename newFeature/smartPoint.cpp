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


////////weak_ptr/////////

#include <memory>
#include <iostream>
using namespace std;

/**
 * 
 * std::weak_ptr 是一种弱引用，不参与资源的引用计数管理，
 * 通常用于打破循环引用。当 std::shared_ptr 创建的对象需要共享给多个位置，
 * 而这些位置又可能形成循环依赖时，使用 std::weak_ptr 可以避免内存泄漏。

主要功能和特点：
    不增加资源的引用计数。
    提供 expired() 判断资源是否已经被释放。
    提供 lock() 方法获取对应的 std::shared_ptr，如果资源已释放，返回空的 shared_ptr。

常用函数：
    lock()：返回一个 std::shared_ptr，指向所管理的资源。
    expired()：判断资源是否已经被释放。
    reset()：释放 weak_ptr 对资源的弱引用。
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
            std::cout<<"Resource released"<<std::endl;
        }
};

int main()
{
    // lock()使用
    std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>();
    std::weak_ptr<Resource> weakPtr = ptr1; // 不增加计数

    if(auto sp = weakPtr.lock())
    {
        std::cout<<"Resource is still alive"<<std::endl;
    }else{
        std::cout<<"Resouce has been released"<<std::endl;
    }

    ptr1.reset();   // 释放资源

    if(auto sp = weakPtr.lock())
    {
        std::cout<<"Resource is still alive"<<std::endl;
    }else{
        std::cout<<"Resource has been released"<<std::endl;
    }

    std::cout<<"=====expired()===="<<std::endl;
    
    // expired()使用
    std::shared_ptr<Resource> ptr2 = std::make_shared<Resource>();
    std::weak_ptr<Resource> weakPtr2 = ptr2;    // 创建弱引用

    // 检查资源是否过期
    if(weakPtr2.expired())
    {
        std::cout<<"Resource has expired"<<std::endl;
    }else{
        std::cout<<"Resource is still alive"<<std::endl;
    }

    // 释放资源，再次检查
    ptr2.reset();
    if(weakPtr2.expired())
    {
        std::cout<<"Resource has expired"<<std::endl;
    }else{
        std::cout<<"Resource is still alive"<<std::endl;
    }

    std::cout<<"====解除循环引用==="<<std::endl;
}


///////打破循环引用///////////

    /*
    循环引用问题通常出现在 std::shared_ptr 之间的相互持有。比如，两个对象 A 和 B 都持有对方的 std::shared_ptr，
    这就会导致它们的引用计数永远不会为零，造成内存泄漏。
    要解决这个问题，可以使用 std::weak_ptr。我们可以将其中一个对象持有 std::weak_ptr 来打破循环引用。
    */

#include <memory>
#include <iostream>
using namespace std;

// 原本的循环引用
#include <iostream>
#include <memory>

class B; // 前向声明

class A {
public:
    std::shared_ptr<B> b;  // A 持有对 B 的 shared_ptr
    ~A() { std::cout << "A destroyed\n"; }
};

class B {
public:
    // std::shared_ptr<A> a;  // B 持有对 A 的 shared_ptr
    // 解除循环引用，B修改成weak_ptr就可以解决这个问题了
    std::weak_ptr<A> a;
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {
    // 创建 A 和 B 的共享指针
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();

    a->b = b;  // A 持有 B
    b->a = a;  // B 持有 A，形成循环引用

    // 此时是没有办法释放资源的

    return 0;
}

//////////////////////////

