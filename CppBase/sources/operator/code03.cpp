// 几种常用运算符的重载

// 自增运算符 ++

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

    friend ostream &operator<<(ostream &myout, const Complex &c);



    // 自增运算符 （单目运算）
    Complex operator++()    // 前置
    {
        this->real++;
        this->image++;
        return *this;
    }

    // 自增运算
    Complex operator++(int)     // 后置
    {
        Complex temp=*this;
        this->real++;
        this->image++;
        return temp;
    }

};


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



int main()
{

    Complex c1(1, 1), c2(4, 5);

    // Complex c4=++c1;
    // Complex c4=c1.operator++();

    Complex c5=c1++;
    cout<<c1;
    cout<<c5;


    return 0;
}