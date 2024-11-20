#include <iostream>
using namespace std;

// 函数模板小案例：
// 实现一个排序算法

// 对char类型和Int类型数组进行排序

// 打印函数
template <typename T>
void PrintArray(T *arr, int len)
{
    for (int i = 0; i < len; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// 排序
template<typename T>
void MySort(T *arr, int len)
{
    for (int i = 0; i < len; i++)
    {
        for (int j = i + 1; j < len; j++)
        {
            if (arr[i] > arr[j])
            {
                T temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main()
{

    // 数组
    int arr[] = {2, 6, 1, 8, 9, 2};
    // 数组长度
    int len = sizeof(arr) / sizeof(int);

    // 排序之前
    // cout << "排序之前:" << PrintArray(arr, len);
     PrintArray(arr, len);
    // 排序
    MySort(arr, len);
    // 排序之后
    // cout << "排序之后:" << PrintArray(arr, len);
     PrintArray(arr, len);

    // 字符数组
    char chArr[] = {'a', 'h', 'c', 'b'};
    len = sizeof(chArr) / sizeof(char);
    PrintArray(chArr, len);
    MySort(chArr, len);
    PrintArray(chArr, len);

    return 0;
}