#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

template <typename T>
class SimpleSet {
private:
    std::vector<T> data;

public:
    // 插入元素
    void insert(const T& value) {
        if (std::find(data.begin(), data.end(), value) == data.end()) {
            data.push_back(value);
            std::sort(data.begin(), data.end()); // 保持有序
        }
    }

    // 查找元素
    bool contains(const T& value) const {
        return std::find(data.begin(), data.end(), value) != data.end();
    }

    // 迭代器类型
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // 获取迭代器
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }
};

int main() {
    SimpleSet<int> mySet;
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(7);
    mySet.insert(3); // 不会插入重复的元素

    // 遍历并输出元素
    for (const auto& value : mySet) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}

