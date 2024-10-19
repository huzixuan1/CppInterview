#include <queue>
#include <iostream>
using namespace std;


int main() {

    std::queue<int> myqu;
    myqu.push(1);
    myqu.push(2);
    myqu.push(3);

    myqu.emplace(10);


    while(!myqu.empty())
    {
        std::cout<<myqu.front()<<" ";
        myqu.pop();
    }
    std::cout<<std::endl;

    std::cout<<"the queue size is:"<<myqu.size()<<std::endl;


    std::queue<int> myqu2;
    myqu2.push(10);
    myqu2.push(20);
    myqu2.push(30);

    myqu.swap(myqu2);
    while(!myqu.empty())
    {
        std::cout<<myqu.front()<<" ";
        myqu.pop();
    }
    std::cout<<std::endl;   

    return 0;
}

