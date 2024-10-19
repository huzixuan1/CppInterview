/**
 * 
 * 模板类list是一个容器，list是由双向链表来实现的，每个节点存储1个元素。list支持前后两种移动方向。
优势： 任何位置执行插入和删除动作都非常快
list与vector的区别：
    list不支持随机存取;
    在list的任何位置执行插入和移除都非常快.插入和删除动作不影响指向其它元素的指针,引用,迭代器,
    不会造成失效;
    list不支持随机存取,不提供下标操作符和at()函数;
    list没有提供容量,空间重新分配等操作函数,每个元素都有自己的内存;
    list也提供了特殊成员函数,专门用于移动元素.

    // 迭代器失效？？
 */

#include<list>
#include<iostream>
using namespace std;

template<class T>
void printlist(const std::list<T>& list)
{
    auto it = list.begin();
    for(;it!=list.end();it++)
    {
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;

}


void test01()
{
    list<int> mylist;
    for(int i = 0;i<10;i++)
    {
        mylist.emplace_back(i);
    }

    printlist(mylist);

    list<int> mylist1(10);
    printlist(mylist1);

    list<int> mylist2(10,10);
    printlist(mylist2);

    list<int> mylist3{1,2,6,7};
    printlist(mylist3);

    list<int> mylist4(mylist);
    printlist(mylist4);

    // 双向链表是不支持+和-的，应该使用advance来进行
    // list<int> mylist5(mylist1.begin(),mylist1.end()-3);
    // printlist(mylist5);

    auto it = mylist4.end();
    std::advance(it,-3);
    list<int> mylist5(mylist4.begin(),it);
    printlist(mylist5);


}

void test02()
{

    std::list<int> list{1,2,3};
    std::cout<<"the first elem:"<<list.front()<<std::endl;
    std::cout<<"the last elem:"<<list.back()<<std::endl;
    
    std::cout<<"list size:"<<list.size()<<std::endl;
    if(!list.empty()) std::cout<<"list is not empty"<<std::endl;

    // list.assign(10,2);   // iterator
    // printlist(list);

    std::cout<<typeid(list.get_allocator()).name()<<std::endl;

    std::cout<<"the contain max size:"<<list.max_size()<<std::endl;

    list.clear();
    printlist(list);

}


void test03()
{
    list<int> list1{1,2,3};
    list<int> list2{4,5,6};
    list2.insert(list2.begin(),10);
    printlist(list2);

    list2.emplace(list2.begin(),90);
    printlist(list2);

    auto it = list2.begin();
    std::advance(it,3);
    list2.emplace(it,71);
    printlist(list2);

    list2.sort();
    printlist(list2);
    

}

void test04()
{
    list<int> list{1,2,3,4};
    list.erase(list.begin());
    printlist(list);

    list.push_back(5);
    list.emplace_back(6);
    list.push_front(1);
    printlist(list);

    list.pop_back();
    list.pop_front();
    printlist(list);

    list.resize(10);
    printlist(list);

}

void test05()
{

    list<int> list1{1,2,3};
    printlist(list1);

    list<int> list2{4,5,6};
    printlist(list2);

    list1.swap(list2);
    printlist(list1);
    printlist(list2);

    // merge
    list1.merge(list2);
    printlist(list1);
    printlist(list2);   // after merge list2 is empty



}

void test06()
{
    std::list<int> list1{1,2,3};
    std::list<int> list2{4,5,6,7,7,7,7,8,9};

    list2.unique();
    printlist(list2);

    auto it = list2.begin();
    for(;it!=list2.end();it++)
    {
        std::cout<<*it<<" ";
        if(*it == 5) *it = 10;
    }
    std::cout<<std::endl;

    printlist(list2);


    // auto it = list2.cbegin();
    // for(;it!=list2.end();it++)
    // {
    //     std::cout<<*it<<" ";
    //     if(*it == 10) *it = 11;
    // }
    // std::cout<<std::endl;

    // printlist(list2);



    std::list<int> list11 = {5, 10, 15};

    auto it1 = list11.rbegin();
    for (; it1 != list11.rend(); it1++) { // 使用 it1 代替 it
        std::cout << *it1 << " "; // 输出: 15 10 5
    }

    std::cout << std::endl;




}



int main()
{

    // test01();

    // test02();

    // test03();

    // test04();

    // test05();

    test06();

    return 0;
}
