#include <vector>
#include <iostream>
using namespace std;



int main(){

    vector<char> vec;
    vec.push_back('a');
    vec.push_back('b');
    vec.push_back('c');
    cout<<vec.at(1)<<endl;
    cout<<vec.front()<<endl;
    cout<<vec.back()<<endl;
    cout<<vec.data()<<endl;

    cout<<vec.capacity()<<endl;

    

    return 0;
}