
/**
 * 完美转发
 */

#include <string>
#include <utility>
#include <iostream>
using namespace std;

namespace test01
{
    void print(int &i)
    {
        std::cout << "Lvalue: " << i << std::endl;
    }

    void print(int &&i)
    {
        std::cout << "Rvalue: " << i << std::endl;
    }

    template <typename T>
    void wrapper(T &&arg)
    {
        // 使用std::forward进行完美转发
        print(std::forward<T>(arg));
    }
}

namespace test02
{ // 构造函数使用完美转发
    class Myclass
    {
    public:
        std::string name;

        // 使用完美转发
        template <typename T>
        Myclass(T &&nameArg) : name(std::forward<T>(nameArg))
        {
            std::cout << "Constructor called with: " << name << std::endl;
        }
    };
}

namespace test03
{
    // 类成员函数使用完美转发
    class Myclass
    {
    public:
        void printName(const std::string &name) const
        {
            std::cout << "Name: " << name << std::endl;
        }

        // 使用完美转发
        template <typename T>
        void setName(T &&newName)
        {
            printName(std::forward<T>(newName));
        }
    };
}

namespace test04
{
    class Myclass
    {
    public:
        void print(int &i) const
        {
            std::cout << "Lvalue: " << i << std::endl;
        }

        void print(int &&i)
        {
            std::cout << "Rvalue: " << i << std::endl;
        }

        template <typename... Args>
        void perfaectForwardPrint(Args &&...args)
        {
            (print(std::forward<Args>(args)),...);
        }
    };
}

int main()
{
    // 测试test01作用域中的方法
    int a = 10;
    test01::wrapper(a);
    test01::wrapper(20);

    // 测试test02作用域中的方法
    std::string str = "Hello world";
    test02::Myclass obj1(str);
    test02::Myclass obj2("Temp string");

    // 测试test03作用域中的方法
    std::string name = "John Doe";
    test03::Myclass obj;
    obj.setName(name);
    obj.setName("Jane");

    // 测试test04作用域中的方法
    int x = 10;
    test04::Myclass obj3;
    obj3.perfaectForwardPrint(x, 30, 49);
    obj3.perfaectForwardPrint(20, 40, 59);

    return 0;
}
