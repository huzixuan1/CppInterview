#include <memory>  // std::allocator
#include <iostream>

template <typename T, typename Allocator = std::allocator<T>>
class SimpleVector {
public:
    // 定义类型别名
    typedef T value_type;
    typedef Allocator allocator_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::const_reference const_reference;
    typedef size_t size_type;

    // 迭代器类定义
    class Iterator {
    private:
        pointer ptr_;  // 指向数组元素的指针
    public:
        // 构造函数
        Iterator(pointer ptr) : ptr_(ptr) {}

        // 解引用运算符（*it）
        reference operator*() {
            return *ptr_;
        }

        // 前置递增运算符（++it）
        Iterator& operator++() {
            ++ptr_;
            return *this;
        }

        // 后置递增运算符（it++）
        Iterator operator++(int) {
            Iterator temp = *this;
            ++ptr_;
            return temp;
        }

        // 相等运算符
        bool operator==(const Iterator& other) const {
            return ptr_ == other.ptr_;
        }

        // 不相等运算符
        bool operator!=(const Iterator& other) const {
            return ptr_ != other.ptr_;
        }
    };

private:
    pointer data_;         // 数据指针
    size_type size_;       // 当前大小
    size_type capacity_;   // 当前容量
    allocator_type alloc_; // 分配器

public:
    // 构造函数
    SimpleVector(size_type capacity = 10)
        : size_(0), capacity_(capacity), alloc_() {
        data_ = alloc_.allocate(capacity_);
    }

    // 析构函数
    ~SimpleVector() {
        // 销毁元素并释放内存
        for (size_type i = 0; i < size_; ++i) {
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_, capacity_);
    }

    // 返回向量大小
    size_type size() const {
        return size_;
    }

    // 返回向量容量
    size_type capacity() const {
        return capacity_;
    }

    // 访问元素的操作符重载
    reference operator[](size_type index) {
        return data_[index];
    }

    const_reference operator[](size_type index) const {
        return data_[index];
    }

    // 向向量中添加元素
    void push_back(const T& value) {
        if (size_ == capacity_) {
            reserve(2 * capacity_);  // 扩容
        }
        alloc_.construct(data_ + size_, value);  // 在新位置构造元素
        ++size_;
    }

    // 预留空间
    void reserve(size_type new_capacity) {
        if (new_capacity > capacity_) {
            pointer new_data = alloc_.allocate(new_capacity);  // 分配新空间

            // 将旧数据复制到新空间
            for (size_type i = 0; i < size_; ++i) {
                alloc_.construct(new_data + i, data_[i]);
                alloc_.destroy(data_ + i);
            }

            // 释放旧数据
            alloc_.deallocate(data_, capacity_);
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    // 迭代器支持
    Iterator begin() {
        return Iterator(data_);
    }

    Iterator end() {
        return Iterator(data_ + size_);
    }
};

int main() {
    SimpleVector<int> vec;

    // 向向量中添加元素
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    // 使用迭代器遍历向量
    for (SimpleVector<int>::Iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";  // 解引用迭代器，访问元素
    }
    std::cout << std::endl;

    return 0;
}

