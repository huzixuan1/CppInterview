// 类模板派生类类模板
// 类模板的类内实现

#include <iostream>
using namespace std;

template <class T>      // 类模板的类内实现
class Animal
{
public:
    void Jiao(){
        cout<<mAge<<"岁动物在叫"<<endl;
    }

public:
    T mAge;
};

template<class T>
class Cat:public Animal<T>{

};

int main(){

    Cat<int> cat;
    return 0;
}