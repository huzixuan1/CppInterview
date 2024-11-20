#include <iostream>
using namespace std;

// 仿函数

class Func{
    public:
        void operator()(const string& str) const{
            cout<<str<<endl;
        }

        void operator()(int a) const{
            cout<<a<<endl;
        }
};

int main(){

    Func func;
    func("hello C++");

    Func pri;
    pri(33);

    return 0;
}