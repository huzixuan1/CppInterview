// c++20新增的关键字
    concept
    requires
    constinit
    consteval
    co_await
    co_return
    co_yield
    char8_t

1. concept
作用: 用于定义概念（concept），概念是一种约束，用于描述类型满足某些条件的要求。它可以用来在模板中更精确地约束类型。
示例:
    template<typename T>
    concept Integral = std::is_integral_v<T>;

    template<typename T>
    concept Integral = std::is_integral_v<T>;

    template<Integral T>
    void func(T value) {
        // ...
    }

    concept和SFINAE类似
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    void func(T value) {
        // ...
    }



2. requires

作用: 用于定义概念的要求，通常与模板参数结合使用，以确保类型满足特定条件。
示例:
    template<typename T>
    requires Integral<T>
    void func(T value) { /* ... */ }

3. constinit
作用: 用于指定变量在编译时初始化，并确保它们在运行时不被修改。主要用于全局和静态变量。
示例:
    constinit int x = 10; // x 必须在编译时初始化

4. consteval
作用: 用于指定一个函数在编译时求值。这样的函数只能在常量表达式中使用。
示例:
    consteval int square(int x) {
        return x * x;
    }

5. co_await
作用: 用于异步编程，表示一个协程（coroutine）可以等待某个异步操作的完成。它是 C++20 协程的一部分。
示例:
    co_await some_async_function();

6. co_return
作用: 用于在协程中返回一个值。它标记了协程的返回点。
示例:
    co_return 42; // 返回值


7. co_yield
作用: 用于在协程中生成一个值并暂停协程的执行。它允许协程返回多个值。
示例:
    co_yield value; // 生成值并暂停协程
8. char8_t
作用: 新增的一种字符类型，用于表示 UTF-8 字符。它提供了更好的类型安全性，特别是在处理字符串时。
示例:
    char8_t ch = u8'A'; // 使用 char8_t 类型

#include <iostream>
#include <concepts>
// g++ -std=c++20 test.cpp -o test

// 示例：使用概念
template<typename T>
concept Integral = std::is_integral_v<T>;

template<Integral T>
T add(T a, T b) {
    return a + b;
}

int main() {
    std::cout << add(5, 10) << std::endl; // 输出 15
    return 0;
}


// export and import新特性
// math_operations.ixx
export module math_operations; // 定义模块

export int add(int a, int b) { // 导出加法函数
    return a + b;
}

export int subtract(int a, int b) { // 导出减法函数
    return a - b;
}


// 编译命令：
// g++ -std=c++20 -fmodules-ts -c math_operations.ixx


// main.cpp
import math_operations; // 导入模块
#include <iostream>

int main() {
    int a = 10;
    int b = 5;

    std::cout << "Addition: " << add(a, b) << std::endl; // 使用加法函数
    std::cout << "Subtraction: " << subtract(a, b) << std::endl; // 使用减法函数

    return 0;
}

