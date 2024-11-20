#include <iostream>
using namespace std;


template<typename T>
int add(T a,T b){
    return a+b;
}


int main(){
    int a=10;
    int b=19;
    cout<<add(a,b)<<endl;

    return 0;
}