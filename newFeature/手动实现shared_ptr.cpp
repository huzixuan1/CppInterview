/**
 * 手动实现unique_ptr && shared_ptr
 */

// 2.手动实现shared_ptr

#include <iostream>
using namespace std;

template<class T>
class Shared_ptr
{
    private:
        T* ptr;
        int* ref_count;
    
    public:
        // 构造函数
        explicit Shared_ptr(T* p = nullptr):ptr(p),ref_count(new int(1))
        {
            if(!p)
            {
                *ref_count = 0;     // 修改ref_count为0
            }
        }

        // 拷贝构造函数
        Shared_ptr(const Shared_ptr& other):ptr(other.ptr),ref_count(other.ref_count)
        {
            (*ref_count)++; // 计数器自增
        }

        //赋值拷贝运算符
        Shared_ptr &operator=(const Shared_ptr& other)
        {
            if(this != other)
            {   /**
                表示它不再持有原来的资源。如果引用计数变为 0，
                说明没有其他 SharedPtr 对象再指向该资源，那么就需要释放资源 */
                // 先减少当前对象的引用计数
                if(--(*ref_count) == 0)
                {
                    delete ptr;
                    delete ref_count;
                }

                // 复制新的指针和引用计数器
                ptr = other.ptr;
                ref_count = other.ref_count;
                (*ref_count)++;
            }

            return *this;
        }

        // 移动构造函数
        Shared_ptr(Shared_ptr&& other) noexcept:ptr(other.ptr),ref_count(other.ref_count)
        {
            other.ptr = nullptr;
            other.ref_count = nullptr;  // 将原对象置为nullptr
        }

        // 移动赋值
        Shared_ptr& operator=(Shared_ptr&& other) noexcept
        {
            if(this!=&other)
            {
                // 先释放当前的资源（如果当前的ref_count是最后一个count）
                if(--(*ref_count) == 0)
                {
                    delete ptr;
                    delete ref_count;
                }
            }
            // 转移所有权
            ptr = other.ptr;
            ref_count = other.ref_count;

            other.ptr = nullptr;
            other.ref_count = nullptr;

            return *this;
        }

        // 重载*
        T& operator*() const
        {
            return *ptr;
        }

        T* operator->() const
        {
            return ptr;
        }

        explicit operator bool() const
        {
            return ptr == nullptr;
        }

        // 获取计数器
        int use_count() const
        {
            return *ref_count;
        }

        // 返回原始指针
        T* get() const
        {
            return ptr;
        }

        // 重置shared_ptr，释放当前资源并可选的绑定新资源
        void reset(T* new_ptr = nullptr)
        {
            if(--(*ref_count) == 0)
            {
                delete ptr;
                delete ref_count;
            }
            
            // 重新绑定
            ptr = new_ptr;
            ref_count = new int(1);
        }

        // 析构
        ~Shared_ptr()
        {
            if(--(*ref_count) == 0)
            {
                delete ptr;
                delete ref_count;
            }
        }
};

namespace mystd
{
    template<typename T,typename... Args>
    Shared_ptr<T> make_shared(Args... args)
    {
        return Shared_ptr<T>(new T(std::forward<Args>(args)...));
    }
};

class Test
{
    public:
        void sayHell()
        {
            std::cout<<"Hello from test!"<<std::endl;
        }
};

int main()
{

    // 测试构造函数
    Shared_ptr<Test> p1(new Test());
    p1->sayHell();
    std::cout<<"p1 use count: "<<p1.use_count()<<std::endl;

    {
        Shared_ptr<Test> p2 = p1;   // 测试拷贝构造
        std::cout<<"p1 use count: "<<p1.use_count()<<std::endl;
        std::cout<<"p2 use count: "<<p2.use_count()<<std::endl;
        p2->sayHell();
    }
    // 离开作用域就会自动减少1
    std::cout<<"p1 use count after p2 out of scope:"<<p1.use_count()<<std::endl;

    std::cout<<"======"<<std::endl;
    // 测试裸指针
    Test* raw = p1.get();
    raw->sayHell();
    std::cout<<"after getting raw point use count:"<<p1.use_count()<<std::endl;

    // 测试reset(nullptr)
    p1.reset(new Test());   // 重置为一个新的对象
    p1->sayHell();
    std::cout<<"p1 use count after reset:"<<p1.use_count()<<std::endl;

    // 测试make_shared
    Shared_ptr<Test> p2 = mystd::make_shared<Test>();
    std::cout<<"p2 use count:"<<p2.use_count()<<std::endl;

    // 测试移动拷贝
    Shared_ptr<Test> p4 = mystd::make_shared<Test>();
    Shared_ptr<Test> p5 = std::move(p4);
    std::cout<<"after moving p5 count:"<<p5.use_count()<<std::endl;

    // 测试移动赋值
    Shared_ptr<Test> p6;
    p6 = std::move(p1);
    std::cout<<"after moving p6 count:"<<p6.use_count()<<std::endl;

    return 0;
}

