#include <iostream>
using namespace std;

template <class T>
class MyArray
{
public:
    MyArray(int capacity) // 有参构造
    {
        this->mCapacity = capacity;
        this->mSize = 0;
        // 申请内存
        this->pAddr = new T[this->mCapacity];
    }

    MyArray(const MyArray<T> &arr) // 拷贝构造
    {
        this->mSize = arr.mSize;
        this->mCapacity = arr.mCapacity;

        // 申请内存空间
        this->pAddr = new T[this->mCapacity];
        // 进行数据的拷贝
        for (int i = 0; i < this->mSize; i++)
        {
            this->pAddr[i] = arr.pAddr[i];
        }
    }

    T &operator[](int index) // []运算符重载
    {
        return this->pAddr[index];
    }

    MyArray<T> operator=(const MyArray<T> &arr) // =运算符重载
    {
        if (this->pAddr != nullptr)
        {
            delete[] this->pAddr; // 如果不为空的话，则需要对内存进行释放
        }

        this->mSize = arr.mSize;
        this->mCapacity = arr.mCapacity;

        // 申请内存空间
        this->pAddr = new T[this->mCapacity];
        // 进行数据的拷贝
        for (int i = 0; i < this->mSize; i++)
        {
            this->pAddr[i] = arr.pAddr[i];
        }

        return *this;
    }

    void PushBack(T &data) // back数据追加
    {
        // 判断容器中是否有位置
        if (this->mSize >= this->mCapacity)
        {
            return;
        }
        // 调用拷贝构造 =号操作符
        // 1、对象元素必须能够被拷贝
        // 2、容器都是值语义，而非引用语义 (向容器中放元素都是放元素的拷贝份，放的都不是元素本身)
        // 3、如果元素的成员有指针，注意深拷贝和浅拷贝的问题
        this->pAddr[this->mSize] = data;
        // msize++
        this->mSize++;
    }

    void PushBack(T &&data) // 右值引用，在调用PushBack()的时候可以直接PushBack(10)
    { 
        // 判断容器中是否有位置
        if (this->mSize >= this->mCapacity)
        {
            return;
        }
        this->pAddr[this->mSize] = data;
        // msize++
        this->mSize++;
    }

    ~MyArray()
    {
        if (this->pAddr != nullptr)
        {
            delete[] this->pAddr;
        }
    }

public:
    // 数组的容量
    int mCapacity;
    // 当前数组有多少数据
    int mSize;
    // 保存数据的首地址
    T *pAddr;
};

int main()
{

    MyArray<int> marray(20);
    int a = 10, b = 20, c = 30, d = 40;
    marray.PushBack(a);
    marray.PushBack(b);
    marray.PushBack(c);
    marray.PushBack(d);

    marray.PushBack(100);
    marray.PushBack(200);
    marray.PushBack(500);

    for (int i = 0; i < marray.mSize; i++)
    {
        cout << marray[i] << " ";
    }
    cout << endl;

    return 0;
}