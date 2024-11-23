/**
 * 设计模式
 */

// 1.单例模式
// 单例模式是一种创建型设计模式， 让你能够保证一个类只有一个实例， 并提供一个访问该实例的全局节点。
// 所有单例的实现都包含以下两个相同的步骤：
//      1.将默认构造函数设为私有， 防止其他对象使用单例类的 new运算符。
//      2.新建一个静态构建方法作为构造函数。 该函数会 “偷偷” 调用私有构造函数来创建对象， 并将其保存在一个静态成员变量中。 此后所有对于该函数的调用都将返回这一缓存对象。
//      如果你的代码能够访问单例类， 那它就能调用单例类的静态方法。 无论何时调用该方法， 它总是会返回相同的对象。

// 优点：
//  1.可以保证一个类只有一个实例
//  2.可以保证获得一个指向该实例的全局访问节点
//  3.仅在首次请求单例对象时对其进行初始化

// 缺点：
//  1.违反了单一指责原则，因为该设计模式解决了两个问题
//  2.单例模式的代码单元测试写起来比较困难
//  3.同时该模式在多线程的环境下需要进行特殊的处理，避免多个线程多次创单例对象

// 饿汉模式
class Singleton {
private:
    static Singleton instance; // 提前初始化
    Singleton() {}
public:
    static Singleton& GetInstance() {
        return instance;
    }
};

Singleton Singleton::instance;


// 懒汉模式
class Singleton {
public:
    static Singleton& GetInstance() {
        static Singleton instance;
        return instance;
    }
private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};




// 线程安全的懒汉式单例设计模式
#include <mutex>
#include <iostream>

class Singleon{
    private:
        // 静态指针，指向唯一实例
        static Singleon* instance;
        static std::mutex mtx;  // 保证线程安全

        // 私有构造函数，防止外部创建实例
        Singleon()
        {
            std::cout<<"Singleton instance created."<<std::endl;
        }

        // 禁止拷贝构造函数和赋值运算符
        Singleon(const Singleon& other) = delete;
        Singleon& operator=(const Singleon& other) = delete;

    public:
        // 提供全局访问节点
        static Singleon* GetInstance()
        {
            if(instance == nullptr)     // 第一次检查
            {
                std::lock_guard<std::mutex> lock(mtx);  // 锁住线程
                if(instance == nullptr)     // 第二次检查
                {
                    instance = new Singleon();
                }
            }
            return instance;
        }

        void ShowMessage()
        {
            std::cout<<"Hello from Singleton!"<<std::endl;
        }
};

// 静态成员变量初始化
Singleon* Singleon::instance = nullptr;
std::mutex Singleon::mtx;

int main()
{

    Singleon* s1 = Singleon::GetInstance();
    Singleon* s2 = Singleon::GetInstance();

    s1->ShowMessage();

    // 验证两个是否相等
    if(s1 == s2){
        std::cout<<"s1 and s2 are the same instance."<<std::endl;
    }

    return 0;
}



// 单例模式在单元测试中的主要挑战
无法重置实例：
    单例的生命周期通常与程序相同，一旦实例创建后，整个运行期间都会存在。在单元测试中，可能需要在不同的测试用例之间重新初始化单例的状态，但单例的特性不允许轻易销毁或重置。

隐藏的全局依赖：
    单例模式通常引入某种全局状态，这种状态使得测试用例之间可能相互干扰（例如共享的数据或行为），违背了单元测试的独立性原则。

难以模拟（Mock）依赖：
    如果单例内部依赖某些外部服务（例如数据库连接或文件系统），在测试中模拟这些依赖会变得困难。

```cpp
// 仅仅在测试用例的时候才可以使用
class Singleton {
private:
    static Singleton* instance;
    Singleton() {}
public:
    static Singleton* GetInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    static void ResetInstance() { // 测试用方法
        delete instance;
        instance = nullptr;
    }

    void SetValue(int val) { value = val; }
    int GetValue() const { return value; }

private:
    int value = 0;
};

Singleton* Singleton::instance = nullptr;

// 测试代码
#include <cassert>

void TestSingleton() {
    Singleton* s1 = Singleton::GetInstance();
    s1->SetValue(42);
    assert(s1->GetValue() == 42);

    // 重置实例后应该是新的对象
    Singleton::ResetInstance();
    Singleton* s2 = Singleton::GetInstance();
    assert(s2->GetValue() == 0); // 初始值应该重置
}

```
