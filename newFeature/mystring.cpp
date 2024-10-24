#include <iostream>
#include <cstring>  // 用于strlen和strcpy

class MyString {
private:
    char* data;  // 用于存储字符串内容
    size_t length;  // 字符串长度

public:
    // 默认构造函数
    MyString() : data(nullptr), length(0) {}

    // 带C字符串的构造函数
    MyString(const char* str) {
        if (str) {
            length = std::strlen(str);
            data = new char[length + 1];  // 分配足够的空间存储字符串
            std::strcpy(data, str);  // 复制字符串
        } else {
            length = 0;
            data = nullptr;
        }
    }

    // 拷贝构造函数
    MyString(const MyString& other) {
        length = other.length;
        if (other.data) {
            data = new char[length + 1];  // 为新对象分配内存
            std::strcpy(data, other.data);  // 复制字符串内容
        } else {
            data = nullptr;
        }
    }

    // 移动构造函数
    MyString(MyString&& other) noexcept : data(other.data), length(other.length) {
        other.data = nullptr;
        other.length = 0;
    }

    // 拷贝赋值操作符
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            // 先释放当前对象的资源
            delete[] data;

            // 复制新对象的内容
            length = other.length;
            if (other.data) {
                data = new char[length + 1];
                std::strcpy(data, other.data);
            } else {
                data = nullptr;
            }
        }
        return *this;
    }

    // 移动赋值操作符
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data;

            data = other.data;
            length = other.length;

            other.data = nullptr;
            other.length = 0;
        }
        return *this;
    }

    // 重载 [] 操作符用于访问字符
    char& operator[](size_t index) {
        return data[index];
    }

    const char& operator[](size_t index) const {
        return data[index];
    }

    // 获取字符串长度
    size_t size() const {
        return length;
    }

    // 获取C风格的字符串
    const char* c_str() const {
        return data;
    }

    // 析构函数
    ~MyString() {
        delete[] data;  // 释放动态分配的内存
    }
};

// 测试函数
int main() {
    MyString str1("Hello, World!");
    MyString str2 = str1;  // 拷贝构造
    MyString str3;
    str3 = std::move(str1);  // 移动赋值

    std::cout << "str2: " << str2.c_str() << std::endl;
    std::cout << "str3: " << str3.c_str() << std::endl;

    std::cout<<str2[3]<<std::endl;
    std::cout<<str2.size()<<std::endl;

    return 0;
}
