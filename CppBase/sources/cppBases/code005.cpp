#include <iostream>
using namespace std;


class Person{
    public:
        static void test(){
            cout<<"hello world"<<endl;
        }
};

int main(){

    Person::test();

    Person *p=new Person();
    p->test();

    return 0;
}