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

