/**
  对code01.cpp进行优化之后的版本
*/

#include <iostream>

using namespace std;

namespace MySTL {

template<typename T>
class MyVector {
public:
    using value_type = T;
    class Iterator;  // 前向声明
    using iterator = Iterator;  // 类型别名

private:
    value_type* _data;
    size_t _size;
    size_t _capacity;

public:
    MyVector() : _data(nullptr), _size(0), _capacity(0) {}

    ~MyVector() {
        delete[] _data;
    }

    MyVector(const MyVector& vec) {
        _size = vec._size;
        _capacity = vec._capacity;
        _data = new value_type[_capacity];
        for (size_t i = 0; i < _size; i++) {
            _data[i] = vec._data[i];
        }
    }

    MyVector& operator=(const MyVector& vec) {
        if (this == &vec) return *this;
        value_type* temp = new value_type[vec._capacity];
        for (size_t i = 0; i < vec._size; i++) {
            temp[i] = vec._data[i];
        }
        delete[] _data;
        _data = temp;
        _size = vec._size;
        _capacity = vec._capacity;
        return *this;
    }

    // 移动构造函数
    MyVector(MyVector&& vec) noexcept : _data(vec._data), _size(vec._size), _capacity(vec._capacity) {
        vec._data = nullptr;
        vec._size = 0;
        vec._capacity = 0;
    }

    // 移动赋值运算符
    MyVector& operator=(MyVector&& vec) noexcept {
        if (this == &vec) return *this;
        delete[] _data;
        _data = vec._data;
        _size = vec._size;
        _capacity = vec._capacity;
        vec._data = nullptr;
        vec._size = 0;
        vec._capacity = 0;
        return *this;
    }

    value_type& operator[](size_t index) const {
        return _data[index];
    }

    void push_back(value_type val) {
        if (_capacity == 0) {
            _capacity = 1;
            _data = new value_type[1];
        } else if (_size >= _capacity) {
            _capacity *= 2;
            value_type* temp = new value_type[_capacity];
            for (size_t i = 0; i < _size; i++) {
                temp[i] = _data[i];
            }
            delete[] _data;
            _data = temp;
        }
        _data[_size++] = val;
    }

    iterator begin() {
        return Iterator(_data);
    }

    iterator end() {
        return Iterator(_data + _size);
    }

    // 其他方法...
    
    // 清空
    void clear() {
        delete[] _data;
        _data = nullptr;
        _size = 0;
        _capacity = 0;
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    class Iterator {
    private:
        value_type* ptr;
    public:
        Iterator(value_type* p) : ptr(p) {}
        
        value_type& operator*() {
            return *ptr;
        }

        Iterator& operator++() {
            ++ptr;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }

        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }
    };
};

} // namespace MySTL

int main() {
    MySTL::MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << endl;

    return 0;
}
