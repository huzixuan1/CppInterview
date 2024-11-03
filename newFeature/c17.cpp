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



#include <iostream>
#include <tuple>
using namespace std;



std::tuple<int,double>
func()
{
    return std::tuple(1,2.3);
}

int main()
{

    auto[i,d] = func();     // 结构化绑定
    std::cout<<i<<std::endl;
    std::cout<<d<<std::endl;

    std::pair<int,std::string> p{1,"hello"};
    auto[i1,s] = p;      // 对对象进行绑定
    std::cout<<i1<<std::endl;
    std::cout<<s<<std::endl;

    // auto& [i2,s2] = p;      // 对对象进行绑定(引用版)
    // std::cout<<i1<<std::endl;
    // std::cout<<s2<<std::endl;

    // 注意结构化绑定不能应用于constexpr
    // constexpr auto[x, y] = std::pair(1, 2.3f); // compile error, C++20可以

    // 结构化绑定还可以绑定结构体，数组




    return 0;
}


// 使用if,switch进行变量初始化
#include <iostream>

// if条件变量的初始化
int main() {
    int x = 10;

    // 使用 if 语句进行初始化
    if (int y = x > 5 ? 1 : 0; y) {
        std::cout << "x is greater than 5" << std::endl;
    } else {
        std::cout << "x is not greater than 5" << std::endl;
    }

    return 0;
}



int main() {
    int x = 2;

    // 使用 switch 语句进行初始化
    switch (int y = x) {
        case 1:
            std::cout << "x is 1" << std::endl;
            break;
        case 2:
            std::cout << "x is 2" << std::endl;
            break;
        default:
            std::cout << "x is not 1 or 2" << std::endl;
            break;
    }

    return 0;
}

// 内联变量

// 内联函数，一般使用准则：适合代码体积小，且频繁会被使用的
// 例如getter和setter就比较适合使用inline函数

// lamda表达式整理：
    // []：不捕获任何变量。
    // [x]：按值捕获变量 x。
    // [&x]：按引用捕获变量 x。
    // [=]：按值捕获所有外部变量。
    // [&]：按引用捕获所有外部变量。
    // [x, &y]：按值捕获 x，按引用捕获 y。

// 语法：
// auto lambda = [](int a, int b){return a+b;};
// auto lambda = [x](){std::cout<<x<<std::endl;};

// 在lambda表达式用*this捕获对象副本
// 正常情况下，lambda表达式中访问类的对象成员变量需要捕获this，但是这里捕获的是this指针，
// 指向的是对象的引用，正常情况下可能没问题，但是如果多线程情况下，函数的作用域超过了对象的作用域，
// 对象已经被析构了，还访问了成员变量，就会有问题。

struct A {
    int a;
    void func() {
        auto f = [this] {
            cout << a << endl;
        };
        f();
    }  
};
int main() {
    A a;
    a.func();
    return 0;
}


// 所以C++17增加了新特性，捕获*this，不持有this指针，
// 而是持有对象的拷贝，这样生命周期就与对象的生命周期不相关啦。
struct A {
    int a;
    void func() {
        auto f = [*this] { // 这里
            cout << a << endl;
        };
        f();
    }  
};
int main() {
    A a;
    a.func();
    return 0;
}


// __has_include预处理表达式
// 可以判断是否有某个头文件，代码可能会在不同编译器下工作，
// 不同编译器的可用头文件有可能不同，所以可以使用此来判断：

#if defined __has_include
#if __has_include(<charconv>)
#define has_charconv 1
#include <charconv>
#endif
#endif

std::optional<int> ConvertToInt(const std::string& str) {
    int value{};
#ifdef has_charconv
    const auto last = str.data() + str.size();
    const auto res = std::from_chars(str.data(), last, value);
    if (res.ec == std::errc{} && res.ptr == last) return value;
#else
    // alternative implementation...
    其它方式实现
#endif
    return std::nullopt;
}


// filesystem

// 示例 1: 检查文件或目录是否存在

#include <iostream>
#include <filesystem>

int main() {
    std::filesystem::path path("example.txt");

    if (std::filesystem::exists(path)) {
        std::cout << "文件或目录存在。" << std::endl;
    } else {
        std::cout << "文件或目录不存在。" << std::endl;
    }

    return 0;
}


// 示例 2: 遍历目录

int main() {
    std::filesystem::path dir("my_directory");

    if (std::filesystem::is_directory(dir)) {
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            std::cout << entry.path() << std::endl;
        }
    }

    return 0;
}


// 示例 3: 创建目录

int main() {
    std::filesystem::path dir("new_directory");

    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directory(dir);
        std::cout << "目录已创建: " << dir << std::endl;
    } else {
        std::cout << "目录已存在: " << dir << std::endl;
    }

    return 0;
}


// 示例 4: 复制文件

int main() {
    std::filesystem::path source("source.txt");
    std::filesystem::path destination("destination.txt");

    std::filesystem::copy(source, destination);
    std::cout << "文件已复制: " << source << " 到 " << destination << std::endl;

    return 0;
}


// 示例 5: 删除文件

int main() {
    std::filesystem::path file("to_delete.txt");

    if (std::filesystem::exists(file)) {
        std::filesystem::remove(file);
        std::cout << "文件已删除: " << file << std::endl;
    } else {
        std::cout << "文件不存在: " << file << std::endl;
    }

    return 0;
}


// std::string_view
// 通常我们传递一个string时会触发对象的拷贝操作，大字符串的拷贝赋值操作会触发堆内存分配，很影响运行效率，
// 有了string_view就可以避免拷贝操作，平时传递过程中传递string_view即可。

void func(std::string_view stv) { cout << stv << endl; }

int main(void) {
    std::string str = "Hello World";
    std::cout << str << std::endl;

    std::string_view stv(str.c_str(), str.size());
    cout << stv << endl;
    func(stv);
    return 0;
}


// as_const
// C++17使用as_const可以将左值转成const类型

std::string str = "str";
const std::string& constStr = std::as_const(str);

// std::any
// C++17引入了any可以存储任何类型的单个值，见代码：

int main() { // c++17可编译
    std::any a = 1;
    cout << a.type().name() << " " << std::any_cast<int>(a) << endl;
    a = 2.2f;
    cout << a.type().name() << " " << std::any_cast<float>(a) << endl;
    if (a.has_value()) {
        cout << a.type().name();
    }
    a.reset();
    if (a.has_value()) {
        cout << a.type().name();
    }
    a = std::string("a");
    cout << a.type().name() << " " << std::any_cast<std::string>(a) << endl;
    return 0;
}


