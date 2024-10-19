#include <set>
#include <iostream>
#include <algorithm>

using namespace std;

template<class T>
void printset(const std::set<T>& set)
{
    for(auto it = set.begin();it!=set.end();it++)
    {
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}

template<class T>
void printsetM(const multiset<T>& mutliset)
{
    for(auto it = mutliset.begin();it!=mutliset.end();it++){
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}

// the test for set
void test01()
{
    std::set<int> set1;
    // std::multiset<int> mset1;
    set1.emplace(1);
    printset(set1);

    set1.emplace_hint(set1.begin(),2);
    printset(set1);

    auto it1= set1.find(2);
    set1.emplace_hint(it1,3);
    printset(set1);

    auto it2= set1.find(1);
    set1.emplace_hint(it2,5);
    printset(set1);

    std::cout<<set1.count(1)<<std::endl;

    // 找到第一个不小于n的数
    auto res = set1.lower_bound(2);
    std::cout<<"lower_bound:"<<*res<<std::endl;

    // 找到第一个不大于n的数
    auto res1 = set1.upper_bound(5);
    std::cout<<"upper_bound:"<<*res1<<std::endl;


}


// the test for multiset

void test02()
{

    std::multiset<int> set1;
    set1.emplace(1);

    printsetM(set1);

    set1.emplace_hint(set1.begin(),2);
    printsetM(set1);

    auto it1= set1.find(2);
    set1.emplace_hint(it1,3);
    printsetM(set1);

    auto it2= set1.find(1);
    set1.emplace_hint(it2,5);
    printsetM(set1);

    set1.emplace(1);
    std::cout<<set1.count(1)<<std::endl;

    // 找到第一个不小于n的数
    auto res = set1.lower_bound(2);
    std::cout<<"lower_bound:"<<*res<<std::endl;

    // 找到第一个不大于n的数
    auto res1 = set1.upper_bound(5);
    std::cout<<"upper_bound:"<<*res1<<std::endl;


}



int main()
{
    // test01();

    test02();

    return 0;
}
