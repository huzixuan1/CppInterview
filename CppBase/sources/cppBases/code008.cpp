#include <iostream>
using namespace std;

int main(){

    int a=10;
    int &b=a;
    int *c=&a;
    cout<<b<<endl;
    cout<<c<<endl;
    cout<<*c<<endl;
    return 0;
}