// c++ new feature
// c++17：
//      1.折叠表达式

#include <vector>
#include <utility>
#include <iostream>
using namespace std;

template<typename ...Args>
// auto add(Args...args)
// {
//     return (args+...);      // 左折叠
// }

auto add(Args...args)
{
    return (...+args);  // 右折叠
}


// 逻辑运算
template<typename ... Args>
bool all_true(Args...args)
{
    return (args && ...);
}

template<typename... Args>
bool any_true(Args...args)
{
    return (args || ...);
}

// 打印多个参数
template<typename...Args>
void print(Args...args)
{
    (std::cout<<...<<args)<<std::endl;      // 打印方式需要研究一下
}

// 函数使用链
void process(int x);
template<typename... Args>
void call_all(Args... args)
{
    (process(args),...);        // 这里是递归的方式？
}

void process(int x)
{
    std::cout<<"Processing: "<<x<<std::endl;
}


// 初始化列表
template<typename... Args>
std::vector<int> create_vector(Args...args)
{
    return {args...};   // 用折叠表达式将可变参数展开为初始化列表
}

// 参数检查
template<typename... Args>
bool are_all_integers(Args...args)
{
    return (std::is_integral_v<Args> && ...);
}

// 递归替代
template<typename...Args>
void print_args(Args...args)
{
    (std::cout<<...<<args)<<std::endl;    // 这种依然是递归吗？
}


// 元编程中的使用
template<typename...Args>
struct AreAllIntegral
{
    static constexpr bool value = (std::is_integral_v<Args> && ...);
};

// 序列化
template<typename...Args>
std::string toCSV(Args...args)
{
    return (...+(std::to_string(args)+","));
}

// 参数完美转发
void printInfo(const std::string& name,int age)
{
    std::cout<<"Name: "<<name<<",Age: "<<age<<std::endl;
}

template<typename Func,typename...Args>
void callF(Func&& f,Args&&...args)
{
    f(std::forward<Args>(args)...);     // 完美转发
}

/**
 * 非折叠表达式的完美转发
 * 
 * 
 */
void toPrint(int arg)
{
    std::cout<<"ToProcess is:"<<arg<<std::endl;
}

template<typename T>
void forwardToProcess(T&& arg)
{
    toPrint(std::forward<T>(arg));
}

int main()
{
    std::cout<<add(1,24,8,2)<<std::endl;

    std::cout<<all_true(true,false,true)<<std::endl;
    std::cout<<any_true(true,false,true)<<std::endl;

    print(1,2,"hello",25.4);

    call_all(3,4,6);

    std::vector<int> vec = {1,2,3,4,5};
    for(auto v:vec) std::cout<<v<<" ";
    std::cout<<std::endl;

    std::cout << are_all_integers(1, 2, 3) << std::endl; // 输出 1 (true)
    std::cout << are_all_integers(1, 2.5, 3) << std::endl; // 输出 0 (false)

    print_args(1, 2.5, "Hello", 'C'); 

    std::cout<<AreAllIntegral<int,char,long>::value<<std::endl;
    std::cout<<AreAllIntegral<int,double,long>::value<<std::endl;

    // 序列化
    std::cout<<toCSV(1,2,3,4,5,6);
    std::cout<<std::endl;

    // 完美转发
    callF(printInfo,"taotao",25);

    // 非折叠表达式的完美转发
    int a = 100;
    forwardToProcess(a);    // 左值
    forwardToProcess(32);   //右值
    return 0;
}
