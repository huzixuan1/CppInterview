#include<unordered_set>
#include<iostream>
#include <algorithm>
using namespace std;


template<class T>
void printset(const std::unordered_set<T>& set)
{

    for(auto it = set.begin();it!=set.end();it++) std::cout<<*it<<" ";
    std::cout<<std::endl;

}

void test01()
{
    std::unordered_set<int> set1;
    set1.emplace(1);
    set1.emplace(0);
    set1.emplace(10);
    set1.emplace(2);
    printset(set1);

    set1.emplace_hint(set1.begin(),100);
    printset(set1);

    std::cout<<set1.count(1)<<std::endl;

    std::cout<<set1.bucket_count()<<std::endl;
    std::cout<<set1.max_bucket_count()<<std::endl;  
    std::cout<<set1.bucket(1)<<std::endl; 
    std::cout<<set1.bucket(10)<<std::endl; 

    set1.max_load_factor(1.5);  // 设置负载因子
    std::cout<<set1.load_factor()<<std::endl;   // 返回每个桶的平均元素数量。它的值是当前元素数量与桶数量的比值

    // 修改桶的数量
    set1.reserve(10);
    std::cout<<set1.size()<<std::endl;

    // 修改桶的数量
    set1.rehash(15);
    std::cout<<set1.size()<<std::endl;

    // hash_function
    auto hashFunc = set1.hash_function();
    std::cout << "Hash of 2: " << hashFunc(2) << std::endl;


}

// unordered_mutiset如上


int main()
{

    test01();

    return 0;
}
