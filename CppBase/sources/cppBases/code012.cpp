#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;

class CSingleton
{
public:
    ~CSingleton() {}

    static CSingleton *getInstance(string value);

    string getValue()
    {
        return mValue;
    }

private:
    CSingleton(const string value) : mValue(value)
    {
    }

    string mValue;
};

CSingleton *CSingleton::getInstance(string value)
{
    static CSingleton mObj(value);
    return &mObj;
}

void threadFirst()
{
    CSingleton *pSingleton = CSingleton::getInstance("first");
    cout << pSingleton->getValue() << "\n";
}

void threadSecond()
{
    CSingleton *pSingleton = CSingleton::getInstance("second");
    cout << pSingleton->getValue() << "\n";
}

void threadThird()
{
    CSingleton *pSingleton = CSingleton::getInstance("third");
    cout << pSingleton->getValue() << "\n";
}

int main(int argc, char *argv[])
{
    thread t1(threadFirst);
    thread t2(threadSecond);
    thread t3(threadThird);
    t1.join();
    t2.join();
    t3.join();

    return 0;
}

// 代码实例（线程安全）
template <typename T>
class Singleton
{
public:
    static T &getInstance()
    {
        if (!value_)
        {
            value_ = new T();
        }
        return *value_;
    }

private:
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (Singleton::value_)
                delete Singleton::value_;
        }
    };
    static CGarbo Garbo;
    Singleton();
    ~Singleton();
    static T *value_;
};
template <typename T>
T *Singleton<T>::value_ = NULL;






// 代码实例（线程不安全）
template <typename T>
class Singleton
{
public:
    static T &getInstance()
    {
        static T instance;
        return instance;
    }

private:
    Singleton(){};
    Singleton(const Singleton &);
    Singleton &operator=(const Singleton &);
};
