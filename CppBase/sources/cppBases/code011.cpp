#include <iostream>
using namespace std;

class Animal
{
public:
    Animal();
    virtual void sleep()
    {
        cout << "站着睡觉" << endl;
    }

    virtual ~Animal();
};

class Person : public Animal
{
public:
    void sleep()
    {
        cout << "躺着睡觉" << endl;
    }
};

int main()
{

    Animal *p = new Person();
    p->sleep();
    delete p;
    p = NULL;

    return 0;
}