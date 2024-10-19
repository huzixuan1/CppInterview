/**
 * stack api 
 */

#include <stack>
#include <iostream>
using namespace std;



void test01()
{

    std::stack<int> sta;
    for(int i = 0;i<=10;i++)
    {
        sta.push(i);
    }
    
    std::cout<<"get elem:"<<sta.top()<<std::endl;

    std::cout<<"stack size:"<<sta.size()<<std::endl;

    sta.emplace(13);
    while(!sta.empty())
    {
        std::cout<<sta.top()<<" ";
        sta.pop();
    }
    std::cout<<std::endl;

    std::stack<int> sta2;
    for(int i =10;i<=20;i++)
    {
        sta2.push(i);
    }

    sta.swap(sta2);

    while(!sta.empty())
    {
        std::cout<<sta.top()<<" ";
        sta.pop();
    }
    std::cout<<std::endl;

    while(!sta2.empty())
    {
        std::cout<<sta2.top()<<" ";
        sta2.pop();
    }
    std::cout<<std::endl;


}


int main()
{
    test01();
    

    return 0;
}
