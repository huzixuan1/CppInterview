#include <unordered_map>
#include <iostream>
using namespace std;

template<class T,class U>
void printmap(const std::unordered_map<T,U>& map)
{
    for(auto it = map.begin();it!=map.end();it++)
    {
        std::cout<<it->first<<":"<<it->second<<std::endl;
    }
}

void test01()
{

    std::unordered_map<int,std::string> map1;
    map1.emplace(1,"taotao");
    map1.emplace(2,"ehll");
    printmap(map1);

    std::cout<<map1.at(2)<<std::endl;

    auto it = map1.find(2);
    std::cout<<"it is:"<<it->first<<":"<<it->second<<std::endl;
    


}


int main()
{
    test01();

    return 0;
}
