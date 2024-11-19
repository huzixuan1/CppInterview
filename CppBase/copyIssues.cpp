/**在C++中，当你复制一个对象时，可能会遇到深拷贝和浅拷贝的概念。它们的主要区别在于复制对象时如何处理动态内存（如指针）的复制。

1. 浅拷贝 (Shallow Copy)
    浅拷贝指的是对对象的逐成员复制，但是它复制的是对象的指针，而不是指针指向的内存。
    当使用浅拷贝时，源对象和目标对象的指针成员指向同一块内存（如果有动态分配的内存）。
特点：
    浅拷贝只复制对象中的值和指针地址。
    如果对象中有指针类型的成员，浅拷贝只复制指针的值（即内存地址），而不复制指针指向的实际数据。
    结果是源对象和目标对象的指针成员指向相同的内存块。
问题：
    由于源对象和目标对象共享相同的内存块，删除一个对象时，会同时删除相同的内存块，导致 双重释放 或 内存访问错误。
    浅拷贝适用于不涉及动态内存管理的简单对象，但对于包含指针或动态内存的对象需要小心使用。
*/

#include <iostream>
using namespace std;

class ShallowCopyExample {
private:
    int* data;
public:
    ShallowCopyExample(int value) {
        data = new int(value); // 动态分配内存
        cout << "Constructor called" << endl;
    }

    ~ShallowCopyExample() {
        delete data; // 释放动态分配的内存
        cout << "Destructor called" << endl;
    }

    // 浅拷贝构造函数
    ShallowCopyExample(const ShallowCopyExample& other) {
        data = other.data; // 只是复制指针，不会复制实际的数据
        cout << "Shallow Copy Constructor called" << endl;
    }

    void print() const {
        cout << *data << endl;
    }
};

// int main() {
//     ShallowCopyExample obj1(42);
//     obj1.print();

//     ShallowCopyExample obj2 = obj1; // 浅拷贝构造函数调用
//     obj2.print();

//     return 0;
// }

// zixhu@MacBook cpp % ./a.out 
// Constructor called
// 42
// Shallow Copy Constructor called
// 42
// Destructor called
// a.out(71768,0x20052cf40) malloc: Double free of object 0x144605e70
// a.out(71768,0x20052cf40) malloc: *** set a breakpoint in malloc_error_break to debug


/**
 2. 深拷贝 (Deep Copy)
    深拷贝指的是对对象的逐成员复制，同时对指针成员指向的内存进行独立复制。
    即，不仅复制对象的值，还会复制动态分配的内存，使得源对象和目标对象拥有各自独立的内存区域。

特点：
    深拷贝会为指针成员分配新的内存空间，并将源对象中指针指向的内容复制到新内存。
    这样，源对象和目标对象的指针成员指向不同的内存，互不干扰。
    适用于包含动态内存（如指针成员）的对象。
*/

#include <iostream>
using namespace std;

class DeepCopyExample {
private:
    int* data;
public:
    DeepCopyExample(int value) {
        data = new int(value); // 动态分配内存
        cout << "Constructor called" << endl;
    }

    ~DeepCopyExample() {
        delete data; // 释放动态分配的内存
        cout << "Destructor called" << endl;
    }

    // 深拷贝构造函数
    DeepCopyExample(const DeepCopyExample& other) {
        data = new int(*other.data); // 为数据分配新的内存
        cout << "Deep Copy Constructor called" << endl;
    }

    void print() const {
        cout << *data << endl;
    }
};

int main() {
    DeepCopyExample obj1(42);
    obj1.print();

    DeepCopyExample obj2 = obj1; // 深拷贝构造函数调用
    obj2.print();

    return 0;
}

/*
如何避免双重释放问题？
    深拷贝： 每个对象在拷贝时独立分配新的内存，并复制原对象的数据内容。这确保了每个对象都有自己独立的动态内存。

    使用智能指针： 智能指针（如 std::shared_ptr 或 std::unique_ptr）会自动管理内存的生命周期，避免手动释放内存的问题：
        std::shared_ptr：多个对象可以共享同一块内存，当最后一个对象销毁时，内存才会释放。
        std::unique_ptr：确保每块动态内存只属于一个对象，禁止拷贝。
*/

// 示例：使用智能指针解决双重释放问题
#include <iostream>
#include <memory> // 智能指针

class SmartExample {
private:
    std::shared_ptr<int> data; // 使用共享指针管理内存
public:
    SmartExample(int value) : data(std::make_shared<int>(value)) {
        std::cout << "Constructor called" << std::endl;
    }

    void print() const {
        std::cout << *data << std::endl;
    }
};

int main() {
    SmartExample obj1(42);
    SmartExample obj2 = obj1; // 共享指针自动管理内存，无需担心双重释放

    obj1.print();
    obj2.print();

    return 0;
}



