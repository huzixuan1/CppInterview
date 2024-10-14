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
    v3.assign(v1.begin(),v1.end()-2);
    printvector(v3);

    // n个elem方式赋值
    vector<int> v4;
    v4.assign(5,2);
    printvector(v4);
}

void api03()
{
    std::vector<int> v1{1,2,3,4,5};
    printvector(v1);

    if(v1.empty())
    {
        std::cout<<"v1 is empty"<<std::endl;
    }else{
        std::cout<<"v1 is not empty"<<std::endl;
        std::cout<<"v1 capacity is:"<<v1.capacity()<<std::endl;
        std::cout<<"v1 size is:"<<v1.size()<<std::endl;
    }

    // 重新指定大小
    v1.resize(15);
    printvector(v1);
    std::cout<<"resize is:"<<v1.size()<<std::endl;
    v1.resize(20,9);
    printvector(v1);
    std::cout<<"resize is:"<<v1.size()<<std::endl;

    v1.resize(5);//长于现在的部分全部被delete
    printvector(v1);
    std::cout<<"resize is:"<<v1.size()<<std::endl;


}

void api04()
{
   std::vector<int> v1;
   v1.push_back(10);
   v1.push_back(20);
   v1.push_back(30);
   v1.push_back(40);
   v1.push_back(50);

   printvector(v1);

   v1.pop_back();
   printvector(v1);

   // 头插
   v1.insert(v1.begin(),100);
   v1.insert(v1.begin(),3,5);
   printvector(v1); 

   // 删除
   v1.erase(v1.begin());
   printvector(v1);

   // 最后一步
   // 清空
   v1.erase(v1.begin(),v1.end());
   printvector(v1);

}

void api05()
{
    std::vector<int> v1;
    for(int i=0;i<=10;i++)
    {
        v1.push_back(i);
    }

    // 利用[]来获取值
    for(int i = 0;i<v1.size();i++)
    {
        std::cout<<v1[i]<<" ";
    }
    std::cout<<std::endl;

    // 利用at来获取数据
    for(int i = 0;i<v1.size();i++)
    {
        std::cout<<v1.at(i)<<" ";
    }

    std::cout<<std::endl;

    std::cout<<"the first value is:"<<v1.front()<<std::endl;
    std::cout<<"the last value is:"<<v1.back()<<std::endl;
}

void api06()
{
    // 互换容器
    std::vector<int> v1{1,2,3,4,5};
    std::vector<int> v2{6,7,8,9,0};

    v1.swap(v2);
    printvector(v1);
    printvector(v2);
}


void api07()
{

    std::vector<int> v1{1,2,3,4};
    printvector(v1);

    v1.reserve(10);
    printvector(v1);

    std::cout<<"size is:"<<v1.capacity()<<std::endl;

    v1.shrink_to_fit();
    printvector(v1);
    std::cout<<"size is:"<<v1.capacity()<<std::endl;
}

void api08()
{
    std::vector<int> v1{1,2,3};
    v1.emplace_back(9);
    printvector(v1);

    v1.emplace(v1.begin()+2,10);
    printvector(v1);

}

int main()
{
    // api01();

    // api02();

    // api03();

    // api04();

    // api05();

    // api06();

    // api07();

    api08();



    return 0;
}
