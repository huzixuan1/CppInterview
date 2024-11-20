// 几种常用运算符的重载

// 1、输入输出运算符的重载<< >>
/**
 *   只能采用友元函数的形式，通常格式如下:
 *     ostream &operator<<(ostream &,const类对象引用)
 *     istream &operator>>(istream &,类对象引用)
 */

#include <iostream>
using namespace std;

class Complex
{
private:
    double real, image;

public:
    Complex(double r = 0, double i = 0)
    {
        real = r;
        image = i;
    }

    void show()
    {
        if (image > 0)
        {
            if (image == 1)
            {
                cout << real << "+"
                     << "i" << endl; // 处理虚部为1的情况 如:3+i
            }
            else
            {
                cout << real << "+" << image << "i" << endl; // 处理虚部为其他正数的情况 如:3+5i
            }
        }
        else if (image < 0)
        {
            if (image == -1)
            {
                cout << real << "-"
                     << "i" << endl; // 处理虚部为-1的情况 如:3-i
            }
            else
            {
                cout << real << image << "i" << endl; // 处理虚部为其他负数的情况 如:3-4i
            }
        }
        else
        { // image=0
            cout << real << endl;
        }
    }


    //
   friend ostream &operator<<(ostream &myout, const Complex &c);
   friend istream &operator>>(istream &myin,Complex &c);

};

// ostream &operator<<(ostream &,const类对象引用)
// istream &operator>>(istream &,类对象引用)

ostream &operator<<(ostream &myout, const Complex &c)
{
    if (c.image > 0)
    {
        if (c.image == 1)
        {
            cout << c.real << "+"
                 << "i" << endl; // 处理虚部为1的情况 如:3+i
        }
        else
        {
            cout << c.real << "+" << c.image << "i" << endl; // 处理虚部为其他正数的情况 如:3+5i
        }
    }
    else if (c.image < 0)
    {
        if (c.image == -1)
        {
            cout << c.real << "-"
                 << "i" << endl; // 处理虚部为-1的情况 如:3-i
        }
        else
        {
            cout << c.real << c.image << "i" << endl; // 处理虚部为其他负数的情况 如:3-4i
        }
    }
    else
    { // image=0
        cout << c.real << endl;
    }

    return myout;   // 这样就可以连续输出了
};


istream &operator>>(istream &myin,Complex &c)   // 因为cin是一个动态输入的，所以myin c都是可以被修改的，所以不能用const进行修饰
{
    cin>>c.real>>c.image;
    return myin;
}

int main()
{

    


    // cout.operator<<(3) << endl;      // 显式的调用
    // cout.operator<<('a') << endl;

    cout<<c1;
    cout<<"------"<<endl;
    cout<<c1<<c2<<endl;

    cout<<"-----------"<<endl;
    cin>>c1>>c2;
    cout<<c1<<c2<<endl;

    return 0;
}