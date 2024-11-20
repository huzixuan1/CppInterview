// 用户自定义的参数类型

// 学生类(Student)，有学号以及3门课的成绩，通过比较学生对象的
// 3门课的总分，实现求总分比较高的学生对象

// 模板函数和Student类的结合

#include <iostream>
#include <string>
using namespace std;

class Student
{
private:
    long Id;
    int score1,score2,score3;

public:
    Student(long i,int sc1,int sc2,int sc3)
    {
        Id=i;
        score1=sc1;
        score2=sc2;
        score3=sc3;
    }

    // 对score1 score2 score3相加进行然后比较 >符号重载
    bool operator>(const Student &s)
    {
        if(score1+score2+score3>s.score1+s.score2+s.score3){
            return true;
        }else{
            return false;
        }
    }

    // 输出必须是友元函数
    friend ostream &operator<<(ostream &myout,const Student &s);


};

// 自定义的类进行输出重载
// getmax(s1,s2),比较后会输出最大的，但是不能使用默认的<<输出
// 对象，所以需要使用输出符号的重载
ostream &operator<<(ostream &myout,const Student &s)
{
    cout<<"学号:"<<s.Id<<endl;
    cout<<"三门课的成绩:"<<s.score1<<"\t"<<s.score2<<"\t"<<s.score3<<endl;
    return myout;
}

template<typename T>
T getmax(T a,T b)
{
  if(a>b){          // s1>s2  s1.operator>(s2)  // 需要>符号重载
    return a;
  }else{
    return b;
  }
}

int main(){
    
    cout<<getmax(6,7)<<endl;
    cout<<getmax('c','i')<<endl;
    cout<<getmax(6.5,1.2)<<endl;
    cout<<getmax("hello","world")<<endl;


    Student s1(111,60,70,80);
    Student s2(222,68,98,88);
    cout<<getmax(s1,s2)<<endl;
    
    return 0;
}