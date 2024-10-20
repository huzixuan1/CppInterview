#include <iostream>
#include <vector>
#include <list>
#include <functional>  // for std::hash

template <typename T>
class SimpleUnorderedSet {
private:
    static const size_t DEFAULT_BUCKET_COUNT = 10; // 默认桶数量
    std::vector<std::list<T>> buckets; // 使用链表来处理冲突
    size_t current_size; // 当前元素数量

    // 哈希函数
    size_t hash(const T& value) const {
        return std::hash<T>()(value) % buckets.size();
    }

public:
    SimpleUnorderedSet() : buckets(DEFAULT_BUCKET_COUNT), current_size(0) {}

    // 插入元素
    void insert(const T& value) {
        size_t index = hash(value);
        // 检查元素是否已存在
        for (const auto& elem : buckets[index]) {
            if (elem == value) return; // 如果已存在，不插入
        }
        buckets[index].push_back(value);
        current_size++;
    }

    // 查找元素
    bool contains(const T& value) const {
        size_t index = hash(value);
        for (const auto& elem : buckets[index]) {
            if (elem == value) return true;
        }
        return false;
    }

    // 删除元素
    void erase(const T& value) {
        size_t index = hash(value);
        auto& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (*it == value) {
                bucket.erase(it);
                current_size--;
                return;
            }
        }
    }

    // 迭代器
    class iterator {
    private:
        SimpleUnorderedSet& set;
        size_t bucket_index; // 当前桶的索引
        typename std::list<T>::iterator list_it; // 当前桶中的元素迭代器

        // 移动到下一个有效位置
        void moveToNext() {
            while (bucket_index < set.buckets.size() && (list_it == set.buckets[bucket_index].end())) {
                bucket_index++;
                if (bucket_index < set.buckets.size()) {
                    list_it = set.buckets[bucket_index].begin();
                }
            }
        }

    public:
        iterator(SimpleUnorderedSet& set, size_t index, typename std::list<T>::iterator it)
            : set(set), bucket_index(index), list_it(it) {
            moveToNext(); // 初始化时移动到第一个有效位置
        }

        // 解引用运算符
        T& operator*() {
            return *list_it;
        }

        // 前缀递增
        iterator& operator++() {
            ++list_it;
            moveToNext(); // 移动到下一个有效位置
            return *this;
        }

        // 不等于运算符
        bool operator!=(const iterator& other) const {
            return bucket_index != other.bucket_index || list_it != other.list_it;
        }
    };

    iterator begin() {
        return iterator(*this, 0, buckets[0].begin());
    }

    iterator end() {
        return iterator(*this, buckets.size(), typename std::list<T>::iterator());
    }

    size_t size() const {
        return current_size;
    }
};

// 测试代码
int main() {
    SimpleUnorderedSet<int> mySet;
    mySet.insert(1);
    mySet.insert(2);
    mySet.insert(3);

    // 打印元素
    std::cout << "Elements in the set: ";
    for (const auto& elem : mySet) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 查找元素
    std::cout << "Contains 2: " << mySet.contains(2) << std::endl;
    std::cout << "Contains 5: " << mySet.contains(5) << std::endl;

    // 删除元素
    mySet.erase(2);
    std::cout << "After erasing 2, elements in the set: ";
    for (const auto& elem : mySet) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}

