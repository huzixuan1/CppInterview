#include <iostream>
using namespace std;

inline int add(int a,int b){
    return a+b;
}

int add01(int a ,int b){
    return a+b;
}

int main(){

    cout<<add(1,5)<<endl;
    cout<<add01(5,2)<<endl;

    return 0;
}