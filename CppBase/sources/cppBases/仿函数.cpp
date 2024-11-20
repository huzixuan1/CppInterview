#include <iostream>
using namespace std;


class Person{
    public:
        void operator() (const int a){
            cout<<"a="<<a<<endl;
        }

};

int main(){

    Person p;
    p(3);


    return 0;
}