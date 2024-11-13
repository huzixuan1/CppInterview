## 各种排序算法的比较

| 排序算法      | 最好情况     | 平均情况    | 最坏情况    | 空间复杂度 | 稳定性   | 适用场景        |
|---------------|--------------|-------------|-------------|------------|----------|-----------------|
| 冒泡排序      | O(n)         | O(n²)       | O(n²)       | O(1)       | 稳定     | 小数据集或接近有序数据 |
| 选择排序      | O(n²)        | O(n²)       | O(n²)       | O(1)       | 不稳定   | 小数据集        |
| 插入排序      | O(n)         | O(n²)       | O(n²)       | O(1)       | 稳定     | 小数据集或接近有序数据 |
| 归并排序      | O(n log n)   | O(n log n)  | O(n log n)  | O(n)       | 稳定     | 大数据集，稳定性要求高 |
| 快速排序      | O(n log n)   | O(n log n)  | O(n²)       | O(log n)   | 不稳定   | 一般数据集，快速且不要求稳定性 |
| 堆排序        | O(n log n)   | O(n log n)  | O(n log n)  | O(1)       | 不稳定   | 需要稳定 O(n log n) 的场景 |
| 希尔排序      | O(n log n)   | O(n^(3/2))  | O(n²)       | O(1)       | 不稳定   | 数据较大的插入排序优化 |
| 计数排序      | O(n + k)     | O(n + k)    | O(n + k)    | O(k)       | 稳定     | 元素范围小的整数排序 |
| 基数排序      | O(nk)        | O(nk)       | O(nk)       | O(n + k)   | 稳定     | 数字或字符串排序 |
| 桶排序        | O(n + k)     | O(n + k)    | O(n²)       | O(n + k)   | 稳定     | 数据均匀分布且范围已知 |



## 快速排序
```cpp
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

int PartitionHoare(std::vector<int>& nums,int left,int right)
{
    int i = left;
    int j = right;
    int key = nums[left];   // 选择第一个元素当作基准

    while(i != j)
    {
        while(i<j && nums[j] >= key)
        {
            j--;
        }
        while(i<j && nums[i] <= key)
        {
            i++;
        }
        swap(nums[i],nums[j]);
    }
    swap(nums[left],nums[i]);
    return i;
}

void show(const std::vector<int>& nums)
{
    for(const int& x:nums)
    {
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;
}

void QuickSort(std::vector<int>& nums,int left,int right)
{
    if(left > right) return;
    int index = PartitionHoare(nums,left,right);
    QuickSort(nums,left,index-1);
    QuickSort(nums,index+1,right);
}


int main()
{
    std::vector<int> nums{2,3,1,6,4,8,0,10};
    std::cout<<"Before Sorting:"<<std::endl;
    show(nums);

    QuickSort(nums,0,nums.size()-1);

    std::cout<<"After Sorting:"<<std::endl;
    show(nums);

}

```

### sort的相关问题
https://blog.csdn.net/qq_21989927/article/details/109164878

https://liam.page/2018/09/18/std-sort-in-STL/

https://www.cnblogs.com/DSCL-ing/p/18354021




