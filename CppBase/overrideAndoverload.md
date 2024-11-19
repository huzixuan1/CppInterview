| **特性**           | **虚函数**                 | **重写**                      | **重载**                  |
|--------------------|----------------------------|-------------------------------|---------------------------|
| **定义位置**       | 在基类中使用 `virtual` 声明 | 在派生类中，重写基类的虚函数  | 在同一个类中             |
| **参数列表**       | 与重写函数的参数列表一致    | 必须与基类虚函数的参数列表一致 | 参数列表必须不同         |
| **绑定时间**       | 运行时绑定（动态绑定）      | 运行时绑定（动态绑定）         | 编译时绑定（静态绑定）    |
| **关键字**         | 使用 `virtual` 声明         | 推荐使用 `override` 标注       | 无需关键字               |
| **是否涉及继承**   | 是                         | 是                            | 否                        |


```cpp
#include <iostream>
using namespace std;

class Base
{
    public:
        virtual void show()
        {
            std::cout<<"Base class"<<std::endl;
        }
};

class Derived:public Base
{
    public:
        void show() override
        {
            std::cout<<"Derived class"<<std::endl;
        }
};

int main()
{
    // 动态绑定
    // Base* basePtr;
    // Derived d;
    // basePtr = &d;

    Base* basePtr = new Derived();

    basePtr->show();

    return 0;
}

#include <iostream>
using namespace std;

class Calculator {
public:
    int add(int a, int b) { // 重载函数
        return a + b;
    }
    double add(double a, double b) { // 重载函数
        return a + b;
    }
    int add(int a, int b, int c) { // 重载函数
        return a + b + c;
    }
};

int main() {
    Calculator calc;
    cout << calc.add(2, 3) << endl;          // 调用 int add(int, int)
    cout << calc.add(2.5, 3.5) << endl;      // 调用 double add(double, double)
    cout << calc.add(1, 2, 3) << endl;       // 调用 int add(int, int, int)
    return 0;
}


// 钻石继承的问题
#include <iostream>
using namespace std;

class Base {
public:
    void show() {
        cout << "Base class show()" << endl;
    }
};

class Derived1 : public Base {
};

class Derived2 : public Base {
};

class Final : public Derived1, public Derived2 {
};

int main() {
    Final obj;

    // obj.show(); // 编译错误：调用 show() 时存在歧义
    obj.Derived1::show(); // 明确指定 Derived1 中的 Base
    obj.Derived2::show(); // 明确指定 Derived2 中的 Base

    return 0;
}


// 解决方案：
#include <iostream>
using namespace std;

class Base {
public:
    Base(int val) {
        cout << "Base initialized with " << val << endl;
    }
};

class Derived1 : virtual public Base {
public:
    Derived1() : Base(0) {} // 派生类不负责初始化 Base
};

class Derived2 : virtual public Base {
public:
    Derived2() : Base(0) {} // 派生类不负责初始化 Base
};

class Final : public Derived1, public Derived2 {
public:
    Final() : Base(42) {} // 最终派生类负责初始化 Base
};

int main() {
    Final obj;
    return 0;
}

```
