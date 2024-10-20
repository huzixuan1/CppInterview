#include <map>
#include <iostream>
using namespace std;

template<class T,class U>
void printmap(const std::map<T,U>& map)
{
    for(auto it = map.begin();it!=map.end();it++)
    {
        std::cout<<it->first<<":"<<it->second<<std::endl;;
    }

}

template<class T,class U>
void printmapM(const std::multimap<T,U>& map)
{
    for(auto it = map.begin();it!=map.end();it++)
    {
        std::cout<<it->first<<":"<<it->second<<std::endl;
    }
}

void test01()
{
    std::map<int,std::string> map1;
    map1.emplace(1,"zixhu");
    map1.emplace(2,"taotao");
    map1.insert({3,"tom"});
    printmap(map1);

    std::cout<<"map size:"<<map1.size()<<std::endl;

    map1.emplace_hint(map1.begin(),4,"hello");
    map1.try_emplace(4,"hello");
    map1.try_emplace(5,"zi");
    map1.try_emplace(6,"hello");
    printmap(map1);

    int keyTocheck = 2;
    if(map1.count(keyTocheck) > 0) std::cout<<"this value is :"<<map1[keyTocheck]<<std::endl;

    auto getRes = map1.find(2);
    std::cout<<"getRes:"<<getRes->first<<" "<<getRes->second<<std::endl;

    std::cout<<map1[3]<<std::endl;
    std::cout<<map1.at(4)<<std::endl;

    // swap,erase,merge


}

// multimap test
void test02()
{
    std::multimap<int,std::string> map1;
    map1.emplace(1,"taotao");
    map1.emplace(1,"zhangsan");
    map1.emplace(1,"taotao");
    map1.insert({2,"tom"});
    map1.insert({3,"hello"});
    map1.insert({4,"coffe"});
    printmapM(map1);

    std::cout<<"map1 size:"<<map1.size()<<std::endl;



    auto getRes = map1.find(2);
    std::cout<<"getRes:"<<getRes->first<<" "<<getRes->second<<std::endl;

    std::cout<<map1.count(1)<<std::endl;
    

    auto it = map1.lower_bound(2);

    if (it != map1.end()) {
        std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
    } else {
        std::cout << "No element found" << std::endl;
    }

    auto it1 = map1.upper_bound(2);
    if (it1 != map1.end()) {
        std::cout << "Key: " << it1->first << " Value: " << it1->second << std::endl;
    } else {
        std::cout << "No element found" << std::endl;
    }


    std::multimap<int,std::string> map2;
    map2.insert({1,"world"});

    map1.swap(map2);
    printmapM(map1);


}

int main()
{
    // test01();

    test02();

    return 0;
}
