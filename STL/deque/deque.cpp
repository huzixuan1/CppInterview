#include <deque>
#include <iostream>
using namespace std;

template<class T>
void printdeq(const std::deque<T>& deq)
{
    for(auto it = deq.begin();it!=deq.end();it++)
    {
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}

template<class T>
void printdeqwithR(const std::deque<T>& deq)
{
    for(auto it = deq.rbegin();it!=deq.rend();it++)
    {
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}

void test01()
{

    std::deque<int> deq1{1,2,3,4,5};
    auto size_ = deq1.size();
    std::cout<<"deq1 size is:"<<size_<<std::endl;

    printdeq(deq1);
    printdeqwithR(deq1);

    auto res = deq1.at(4);
    std::cout<<"res is:"<<res<<std::endl;   // 5

    deq1.insert(deq1.begin()+1,15);
    printdeq(deq1);

    deq1.emplace(deq1.begin()+3,16);    // 效率高
    printdeq(deq1);

    auto res1 = deq1.back();
    std::cout<<"res1 is:"<<res1<<std::endl;

    std::deque<int> deq2;
    deq2.assign(deq1.begin(),deq1.end()-2);
    printdeq(deq2);
    std::cout<<deq2[0]<<std::endl;
    std::cout<<deq2.front()<<std::endl;

    if(!deq2.empty()) std::cout<<"deq1 size is:"<<deq2.size()<<std::endl;
    deq2.push_back(34);
    deq2.push_front(0);
    std::cout<<"get front:"<<deq2.front()<<" and get back:"<<deq2.back()<<std::endl;
    printdeq(deq2);

    deq2.pop_front();
    deq2.pop_back();
    printdeq(deq2);

    if(deq1 == deq2)
    {
        std::cout<<"true"<<std::endl;
    }else{
        std::cout<<"false"<<std::endl;
    }

    deq1.swap(deq2);
    printdeq(deq2);

    deq2.resize(10);
    printdeq(deq2);

    deq2.shrink_to_fit();
    printdeq(deq2);

    deq2.erase(deq2.begin());
    printdeq(deq2);

    // deq2.clear();
    std::sort(deq2.begin(),deq2.end());
    printdeq(deq2);

}



int main()
{
    test01();

    return 0;
}
