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
