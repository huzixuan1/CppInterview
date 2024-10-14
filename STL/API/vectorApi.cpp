/**
 * this code for vector api using
 */

#include<iostream>
#include<vector>
using namespace std;

template<class T>
void printvector(const std::vector<T>& vec)
{
    auto it = vec.begin();
    for(;it!=vec.end();it++)
    {
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;

}


/*
** 下面的这个SFINAE的实现方式
template<class T>
typename std::enable_if<!std::is_integral<T>::value,void>::type
printvector(const std::vector<T>& vec)
{
    std::cout<<"this is command type"<<std::endl;
    auto it = vec.begin();
    for(;it!=vec.end();it++)
    {
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}

template<class T>
typename std::enable_if<std::is_integral<T>::value,void>::type
printvector(const std::vector<T>& vec)
{
    std::cout<<"this is int type"<<std::endl;
    auto it = vec.begin();
    for(;it!=vec.end();it++)
    {
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}

 */

void api01()
{
    // construct 01
    vector<int> v1;
    for(int i = 0;i<=10;i++)
    {
        v1.push_back(i);
    }

    printvector(v1);


    // construct 02
    vector<int> v2(v1.begin(),v1.end());
    printvector(v2);

    // construct 03
    vector<int> v3{2,3,6};
    printvector(v3);

    // construct 04
    vector<int> v4(v3);
    printvector(v4);

    // construct 05
    vector<int> v5(10,100);
    printvector(v5);

    // construct 06
    vector<double> v6{1.0,2.0};
    printvector(v6);

}

void api02()
{
    vector<int> v1;
    for(int i =0;i<10;i++)
    {
        v1.push_back(i);
    }
    printvector(v1);

    // operator =
    vector<int> v2;
    v2 = v1;
    printvector(v2);

    // assign
    vector<int> v3;
    v3.assign(v1.begin(),v1.end()-1);
    printvector(v3);

    // n个elem方式赋值
    vector<int> v4;
    v4.assign(5,2);
    printvector(v4);
}


int main()
{
    api01();

    api02();

    return 0;
}
