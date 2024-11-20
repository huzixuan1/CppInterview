// 类模板案例

// 设计一个类模板，有数据成员T data[size],有求最大值
// 的方法getMax()和排序的方法sort(),模板参数T可以实例
// 化成int char double float string等

#include <iostream>
using namespace std;

template <typename T, int size>
class Data
{
    T data[size];

public:
    Data()
    {
        cout << "input" << size << "datas:" << endl;
        for (int i = 0; i < size; i++)
        {
            cin >> data[i];
        }
    }

    void show()
    {
        cout << size << "datas:" << endl;
        for (int i = 0; i < size; i++)
        {
            cout << data[i] << "\t";
        }
        cout << endl;
    }

    // 返回最大值
    T getMax(); // 放在类外面实现

    // 对数据进行排除(直接插入排序)
    void insertSort();
};

template <typename T, int size>
T Data<T, size>::getMax()
{
    T max = data[0];
    for (int i = 1; i < size; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
        }
    }
    return max;
}

template <typename T, int size>
void Data<T, size>::insertSort()
{
    T temp;
    for (int i = 1; i <= size - 1; i++)
    { // 当前插入data[i+1]到有序序列data[0……i-1]
        temp = data[i];
        for (int j = i - 1; j >= 0; j--)
        {
            if (data[j] > temp)
            {
                data[j + 1] = data[j];
            }
            else
            {
                break;
            }
            data[j + 1] = temp;
        }
    }
}

int main()
{
    Data<int, 5> d; // 实例化模板类型对象
    d.show();

    cout << d.getMax() << endl; // 调用getMax()返回最大值

    cout << "after sort:" << endl; // 排序
    d.insertSort();
    d.show();
    return 0;
}