1. C++ 中的类型转换运算符有哪些？它们之间的区别是什么？
  考点：考察对 static_cast、dynamic_cast、const_cast 和 reinterpret_cast 的理解。
  提示回答：解释每种类型转换的用法、限制和适用场景，最好能举例说明。


2. 什么时候会使用 dynamic_cast？它与 static_cast 的区别是什么？
  考点：了解多态类型的安全转换和编译时与运行时的区别。
  参考回答：dynamic_cast 用于多态类型的运行时安全转换，而 static_cast 是编译时转换。dynamic_cast 会在转换失败时返回 nullptr 或抛出异常，而 static_cast 不会检查转换的合法性。

3. const_cast 的作用是什么？什么时候会导致未定义行为？
  考点：移除或添加 const 的用法及风险。
  提示回答：
  用于在需要移除 const 性质的场景，例如向只接受非常量指针的函数传递 const 对象。
  修改真正的常量对象会导致未定义行为。

4. reinterpret_cast 有哪些常见的使用场景？为什么不推荐使用？
  考点：低级别类型转换的应用和风险。
  参考回答：reinterpret_cast 用于完全不相关类型的转换，例如将指针转换为整数类型。因为它不检查转换的安全性，容易导致未定义行为。

5. C 风格类型转换和 C++ 类型转换运算符的区别是什么？
  考点：对 C++ 风格类型转换的优势理解。
  提示回答：C++ 提供的类型转换运算符（如 static_cast）更安全，能明确表达转换的意图，而 C 风格转换的行为可能难以预测，降低了代码的可维护性。

6. 如何安全地将基类指针转换为派生类指针？写一段代码实现。
  考点：使用 dynamic_cast 检查多态类型转换。
```cpp
#include <iostream>
using namespace std;

class Base {
    virtual void func() {}
};

class Derived : public Base {
public:
    void show() { cout << "Derived class" << endl; }
};

int main() {
    Base* basePtr = new Derived; // 基类指针指向派生类对象
    Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);

    if (derivedPtr) {
        derivedPtr->show();
    } else {
        cout << "Conversion failed!" << endl;
    }

    delete basePtr;
    return 0;
}
```

7. const_cast 修改常量会发生什么？举例说明。
考点：对常量性修改的风险理解。
```cpp
#include <iostream>
using namespace std;

int main() {
    const int val = 42;
    int* modifiable = const_cast<int*>(&val);
    *modifiable = 100; // 未定义行为

    cout << "Original value: " << val << endl; // 结果无法保证
    return 0;
}
```
问题分析：
  为什么修改 const 对象可能会导致未定义行为？
  是否有合法的场景可以使用 const_cast？
  
8. 将一个 void* 指针转换为具体类型并访问其成员。
  考点：reinterpret_cast 的使用。
```cpp
#include <iostream>
using namespace std;

class Data {
public:
    int value;
    Data(int v) : value(v) {}
};

int main() {
    Data d(42);
    void* voidPtr = &d;

    // 转换回 Data 类型
    Data* dataPtr = reinterpret_cast<Data*>(voidPtr);
    cout << "Value: " << dataPtr->value << endl;

    return 0;
}
```

9. 解释下面代码中的错误并修正：
```cpp
#include <iostream>
using namespace std;

class Base {};
class Derived : public Base {};

int main() {
    Base baseObj;
    Derived* derivedPtr = static_cast<Derived*>(&baseObj); // 编译通过，但危险
    derivedPtr->show(); // 错误
    return 0;
}
```
问题分析：
  为什么 static_cast 编译通过却不安全？
  修正后应使用 dynamic_cast，并确保 Base 是多态类型。

10. 能否通过 static_cast 将 const int* 转为 int*？为什么？
考点：const_cast 和 static_cast 的作用范围。
提示回答：static_cast 不允许移除 const 限定符；移除 const 应使用 const_cast。
11. 解释为什么以下代码是未定义行为：
cpp
复制代码
#include <iostream>
using namespace std;

int main() {
    int x = 65;
    char* c = reinterpret_cast<char*>(&x);
    *c = 'B'; // 修改位模式

    cout << "New value of x: " << x << endl; // 未定义行为
    return 0;
}
问题分析：
为什么修改 x 的位模式可能导致未定义行为？
在什么情况下可以合法地使用 reinterpret_cast？
12. 如何在 C++ 中安全地将一个类对象的成员序列化为字节流？
考点：reinterpret_cast 在序列化中的应用。
提示回答：
可以将对象的地址通过 reinterpret_cast 转换为 char*，然后逐字节处理。
应确保类没有包含指针或其他不直接存储数据的成员。
实际场景分析
13. 设计一个函数，接受一个 void* 指针，并根据上下文信息将其安全地转换为特定类型。
考点：reinterpret_cast 和动态类型安全。
示例问题：
如何通过额外的上下文信息（如类型标识）确保转换的安全性？
14. 在什么情况下不建议使用类型转换？
考点：理解类型转换的局限性。
提示回答：
尽量避免不必要的类型转换，特别是使用 reinterpret_cast 和 const_cast。
类型设计和接口定义应该尽量减少转换的需求。
15. 什么时候会发生隐式类型转换？如何避免可能的错误？
考点：隐式转换的触发条件和潜在问题。
示例问题：
为什么以下代码可能引发问题？
cpp
复制代码
void func(int x);
func(3.14); // 隐式转换 double -> int

