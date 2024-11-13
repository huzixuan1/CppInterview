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

using namespace std;

void merge(std::vector<int> &nums, int left, int mid, int right)
{

    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> leftArr(n1);
    std::vector<int> rightArr(n2);

    for (int i = 0; i < n1; i++)
    {
        leftArr[i] = nums[left + i];
    }

    for (int i = 0; i < n2; i++)
    {
        rightArr[i] = nums[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            nums[k++] = leftArr[i++];
        }
        else
        {
            nums[k++] = rightArr[j++];
        }
    }

    while (i < n1)
        nums[k++] = leftArr[i++];
    while (j < n2)
        nums[k++] = rightArr[j++];
}

void mergeSort(std::vector<int> &nums, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(nums, left, mid);
        mergeSort(nums, mid + 1, right);
        merge(nums, left, mid, right);
    }
}

int main()
{

    std::vector<int> nums = {12, 11, 13, 5, 6, 7};
    int n = nums.size();
    mergeSort(nums, 0, n - 1);

    for (int num : nums)
    {
        std::cout << num << " ";
    }

    std::cout << std::endl;

    return 0;
}
```

### sort的相关问题
https://blog.csdn.net/qq_21989927/article/details/109164878

https://liam.page/2018/09/18/std-sort-in-STL/

https://www.cnblogs.com/DSCL-ing/p/18354021



### 归并排序

```cpp
#include <iostream>
#include <vector>
using namespace std;

// 合并两个已排序的子数组
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<int> leftArr(n1), rightArr(n2);
    
    for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];
    for (int i = 0; i < n2; i++) rightArr[i] = arr[mid + 1 + i];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }
    
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];
}

// 归并排序函数
void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);  // 递归排序左半部分
        mergeSort(arr, mid + 1, right);  // 递归排序右半部分
        merge(arr, left, mid, right);  // 合并已排序的子数组
    }
}

int main() {
    vector<int> arr = {12, 11, 13, 5, 6, 7};
    int n = arr.size();
    mergeSort(arr, 0, n - 1);
    for (int num : arr) {
        cout << num << " ";
    }
    return 0;
}

```

```cpp

#include <iostream>
#include <vector>
using namespace std;

// 调整堆，使得当前节点为父节点，左右子节点都满足堆的性质
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;  // 将当前节点作为最大元素
    int left = 2 * i + 1;  // 左子节点
    int right = 2 * i + 2;  // 右子节点

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);  // 递归调整堆
    }
}

// 堆排序函数
void heapSort(vector<int>& arr) {
    int n = arr.size();

    // 构建最大堆
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // 逐个从堆中取出最大元素，并调整堆
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);  // 将最大元素交换到数组末尾
        heapify(arr, i, 0);  // 调整堆
    }
}

int main() {
    vector<int> arr = {4, 10, 3, 5, 1};
    heapSort(arr);
    for (int num : arr) {
        cout << num << " ";
    }
    return 0;
}
```


std::sort 的底层实现通常是基于 快速排序（QuickSort）的，但它会根据数据的特性和规模动态切换到其他排序算法，形成 混合排序 的策略。具体来说：

快速排序 是主排序算法，适用于大多数情况。
插入排序 用于小规模或部分有序的数据。
堆排序 用于防止快速排序的最坏情况发生，或者在递归深度过深时作为备选算法。
这种多算法混合的策略，确保了 std::sort 在大多数情况下能够提供 O(n log n) 的性能，并且避免了快速排序最坏情况下的退化。

