/**
 * 1、运算符重载的概念
 *      将系统预定义的运算符,用于用户定义的数据类型，
 *      这就是运算符重载。运算符重载的实质是函数重载
 *
 * 2、运算符重载的规则
 *      重载运算符函数的一般格式为:
 *          返回类型 operator @(参数列表)
 *          {
 *              重载函数体
 *              }
 *
 * 3、运算符重载为友元函数
 *    在相应的类中声明为该类的友元函数,申明的一般形式为:
 *      friend 返回类型operator @(参数表);
 *    而函数的具体定义如下:
 *      返回类型operator @(参数表)
 *      {
 *          // 函数体
 *          }
 *
 * 4、运算符重载为成员函数
 *      4.1如果是重载双目运算符,就只要设置一个参数作为右侧运算量，而左侧运算量就是该对象本身
 *      4.2如果是重载单目运算符，就不必另外设置参数，运算符的操作量就是对象本身
 * 
 *
 * 5、几种常用运算符的重载(>>、<<、++、--、=等等)
 *      算数运算符:+ - * / %
 *      关系运算符:== != < > <= >=
 *      逻辑运算符:&& || !
 *      赋值运算符:= += -= *= /= %= &= |= ^= <<= >>=
 *      位运算符:| & ~ ^ << >>
 *      单目运算符:+ -* /
 *      自增、自减运算符:++ --
 *      动态内存操作运算符:new delete delete[]
 *      其他运算符:() []
 *
 * 6、C++中不能进行重载的运算符
 *      成员访问运算符 .
 *      成员指针运算符 *
 *      域操作运算符 ::
 *      条件运算符 ?:
 *      空间计算运算符 sizeof
 *
 * 7、定义运算符需要注意的事项
 *      不允许定义新的运算符
 *      不能改变该运算符操作数(个数)的个数
 *      不能改变该运算符的优先级别和结合性
 *      应该符合实际需要,重载的功能应该与运算符原有的功能相似,避免没有目的的
 *      使用重载运算符
 */

/**
 * 引例
 *      定于了一个复数类，为了实现复数的加法，可以定义一个
 * 成员函数Add,调用它可以实现两个复数对象相加，并返回一个新的复数对象
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

    // Complex c3=c1.Add(c2);
    // 由于在main中调用Add后返回的是一个Complex对象，
    // 所以这里Add成员函数的返回类型为Complex
    Complex Add(const Complex &c)
    {
        Complex temp;
        temp.real = this->real + c.real;
        temp.image = this->image + c.image;
        return temp;
    }



    // 使用运算符重载实现加法运算
    // 运算符重载为友元函数
    friend Complex operator+(const Complex &c1, const Complex &c2);

    // 使用运算符重载实现==运算
    // 运算符重载为友元函数
    friend bool operator==(const Complex &c1,const Complex &c2);



    

    // 运算符重载为类成员函数的方式实现
    Complex operator+(const Complex &c2)    // 当前对象相当于c1，所以只要另一个形参即可
    {
        Complex temp;
        temp.real = this->real + c2.real;
        temp.image = this->image + c2.image;
        return temp;
    }

    bool operator==(const Complex &c2)
{
    if(this->real==c2.real && this->image==c2.image){
        return true;
    }else{
        return false;
    }
}


};  // class结束

Complex operator+(const Complex &c1, const Complex &c2)
{
    Complex temp;
    temp.real = c1.real + c2.real;
    temp.image = c1.image + c2.image;
    return temp;
}

bool operator==(const Complex &c1,const Complex &c2)
{
    if(c1.real==c2.real && c1.image==c2.image){
        return true;
    }else{
        return false;
    }
}

int main()
{
    Complex c1(1, 1), c2(4, 5);
    c1.show();
    c2.show();

    // 做一个加法运算(使用对象的方式实现)
    // Complex c3 = c1.Add(c2);
    // cout<<"类对象的方式实现加法运算:";
    // c3.show();

    // 使用重载运算符的方式进行
    // Complex c3 = c1 + c2;       // 隐式调用
    Complex c3=operator+(c1,c2);  // 显式调用

    cout<<"重载运算符的方式实现加法运算:";
    c3.show();

    // 使用==重载运算符
    if(c1==c2)
    {
    cout<<"c1 c2两个对象相等……"<<endl;
    }else{
        cout<<"c1 c2两个对象不相等……"<<endl;
    }


    cout<<"-----------"<<endl;
    // 运算符重载为类成员函数的调用
    // 显式调用
    // Complex c4=c1.operator+(c2);
    // 隐式调用
    Complex c4=c1+c2;
    c4.show();

    // 类成员函数使用==重载运算符
    if(c1==c2)
    {
    cout<<"c1 c2两个对象相等……"<<endl;
    }else{
        cout<<"c1 c2两个对象不相等……"<<endl;
    }

    return 0;
}


////////////////////

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


////////////////////////

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


/////////////////////


// 几种常用运算符的重载

// 赋值操作运算符 =
/**
 * 赋值运算符'='是双目运算符
 * 如果不定义自己的赋值运算符函数，那么编译器会自动生成一个默认的赋值运算符函数
 * 赋值运算符函数必须是类的成员函数，不允许重载为友元函数
 * 赋值运算符函数不能被派生类继承
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

    

    // 实现赋值运算符操作
    Complex operator=(const Complex &c)
    {
        this->real=c.real;
        this->image=c.image;
        cout<<"我们自己定义的赋值运算符"<<endl;
        return *this;
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

   Complex c5;
   c5=c2=c1;   // 调用赋值运算符函数
//    c5.operator=(c2);
   cout<<c5;
   cout<<c2;


    return 0;
}
