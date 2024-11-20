// 类模板语法

#include <iostream>
using namespace std;

template <class T>
class Person
{
public:
    Person(T id, T age)
    {
        this->mId = id;
        this->mAge = age;
    }

    void show()
    {
        cout << "ID:" << mId << " "
             << "Age:" << mAge << endl;
    }

public:
    T mId;
    T mAge;
};

int main()
{
    // 函数模板在调用的时候，可以自动推导
    // 类模板必须显式指定类型
    Person<int> p(10, 20);
    p.show();
    return 0;
}