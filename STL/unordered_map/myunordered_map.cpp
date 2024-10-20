#include <iostream>
#include <vector>
#include <list>
#include <utility> // for std::pair
#include <functional> // for std::hash

template <typename Key, typename Value>
class SimpleUnorderedMap {
private:
    static const size_t DEFAULT_BUCKET_COUNT = 10; // 默认桶数量
    std::vector<std::list<std::pair<Key, Value>>> buckets; // 使用链表来处理冲突
    size_t current_size; // 当前元素数量

    // 哈希函数
    size_t hash(const Key& key) const {
        return std::hash<Key>()(key) % buckets.size();
    }

public:
    SimpleUnorderedMap() : buckets(DEFAULT_BUCKET_COUNT), current_size(0) {}

    // 插入元素
    void insert(const Key& key, const Value& value) {
        size_t index = hash(key);
        for (auto& elem : buckets[index]) {
            if (elem.first == key) {
                elem.second = value; // 如果键已存在，则更新值
                return;
            }
        }
        buckets[index].emplace_back(key, value); // 插入新键值对
        current_size++;
    }

    // 查找元素
    bool contains(const Key& key) const {
        size_t index = hash(key);
        for (const auto& elem : buckets[index]) {
            if (elem.first == key) return true;
        }
        return false;
    }

    // 获取值
    Value get(const Key& key) const {
        size_t index = hash(key);
        for (const auto& elem : buckets[index]) {
            if (elem.first == key) return elem.second; // 返回对应的值
        }
        throw std::runtime_error("Key not found");
    }

    // 删除元素
    void erase(const Key& key) {
        size_t index = hash(key);
        auto& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) {
                bucket.erase(it);
                current_size--;
                return;
            }
        }
    }

    // 迭代器
    class iterator {
    private:
        SimpleUnorderedMap& map;
        size_t bucket_index; // 当前桶的索引
        typename std::list<std::pair<Key, Value>>::iterator list_it; // 当前桶中的元素迭代器

        // 移动到下一个有效位置
        void moveToNext() {
            while (bucket_index < map.buckets.size() && (list_it == map.buckets[bucket_index].end())) {
                bucket_index++;
                if (bucket_index < map.buckets.size()) {
                    list_it = map.buckets[bucket_index].begin();
                }
            }
        }

    public:
        iterator(SimpleUnorderedMap& map, size_t index, typename std::list<std::pair<Key, Value>>::iterator it)
            : map(map), bucket_index(index), list_it(it) {
            moveToNext(); // 初始化时移动到第一个有效位置
        }

        // 解引用运算符
        std::pair<Key, Value>& operator*() {
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
        return iterator(*this, buckets.size(), typename std::list<std::pair<Key, Value>>::iterator());
    }

    size_t size() const {
        return current_size;
    }
};

// 测试代码
int main() {
    SimpleUnorderedMap<int, std::string> myMap;
    myMap.insert(1, "one");
    myMap.insert(2, "two");
    myMap.insert(3, "three");

    // 打印元素
    std::cout << "Elements in the map: " << std::endl;
    for (const auto& elem : myMap) {
        std::cout << elem.first << ": " << elem.second << std::endl;
    }

    // 查找元素
    std::cout << "Contains key 2: " << myMap.contains(2) << std::endl;
    std::cout << "Value for key 2: " << myMap.get(2) << std::endl;

    // 删除元素
    myMap.erase(2);
    std::cout << "After erasing key 2, elements in the map: " << std::endl;
    for (const auto& elem : myMap) {
        std::cout << elem.first << ": " << elem.second << std::endl;
    }

    return 0;
}

