/**
  MyVector:
    实现简易的vector
    支持push_back,insert,front,
    =,!=等重载运算符
    以及迭代器
*/

#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

namespace MySTL {

template<typename T>
class MyVector {
public:
    using value_type = T;
    class Iterator;

    using iterator = class Iterator;

private:
    value_type* _data;
    size_t _size;
    size_t _capacity;

public:
    MyVector() : _data(nullptr), _size(0), _capacity(0) {}

    ~MyVector() {
        delete[] _data;
        _size = 0;
        _capacity = 0;
    }

    MyVector(const MyVector& vec) {
        _size = vec._size;
        _capacity = vec._capacity;
        _data = new value_type[_capacity];
        for (int i = 0; i < _size; i++) {
            _data[i] = vec._data[i];
        }
    }

    MyVector& operator=(const MyVector& vec) {
        if (this == &vec) return *this;
        value_type* temp = new value_type[vec._capacity];
        for (int i = 0; i < vec._size; i++) {
            temp[i] = vec._data[i];
        }

        delete[] _data;
        _data = temp;
        _size = vec._size;
        _capacity = vec._capacity;

        return *this;
    }

    value_type& operator[](size_t index) const {
        return _data[index];
    }

    bool operator==(const MyVector& vec) const {
        if (_size != vec._size) return false;
        for (int i = 0; i < _size; i++) {
            if (_data[i] != vec._data[i])
                return false;
        }
        return true;
    }

    void push_back(value_type val) {
        if (_capacity == 0) {
            _capacity = 1;
            _data = new value_type[1];
        } else if (_size + 1 > _capacity) {
            _capacity *= 2;
            value_type* temp = new value_type[_capacity];
            for (int i = 0; i < _size; i++) {
                temp[i] = _data[i];
            }
            delete[] _data;
            _data = temp;
        }
        _data[_size] = val;
        ++_size;
    }

    void insert(iterator it, value_type val) {
        int index = it - begin(); // Use the begin iterator to find index
        if (_capacity == 0) {
            _capacity = 1;
            _data = new value_type[1];
            _data[0] = val;
        } else if (_size + 1 > _capacity) {
            _capacity *= 2;
            value_type* temp = new value_type[_capacity];
            for (int i = 0; i < index; i++) {
                temp[i] = _data[i];
            }
            temp[index] = val;
            for (int i = index; i < _size; i++) {
                temp[i + 1] = _data[i];
            }
            delete[] _data;
            _data = temp;
        } else {
            for (int i = _size - 1; i >= index; i--) {
                _data[i + 1] = _data[i];
            }
            _data[index] = val;
        }
        ++_size;
    }

    void pop_back() noexcept {
        if (_size > 0) {
            --_size;
        }
    }

    void erase(iterator it) {
        size_t index = it - begin(); // Use the begin iterator to find index
        for (int i = index; i < _size - 1; i++) {
            _data[i] = _data[i + 1];
        }
        --_size;
    }

    value_type front() const {
        return _data[0];
    }

    value_type back() const {
        return _data[_size - 1];
    }

    iterator begin() {
        return Iterator(_data);
    }

    iterator end() {
        return Iterator(_data + _size);
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return _capacity;
    }

    bool empty() const {
        return _size == 0;
    }

    class Iterator {
    private:
        value_type* ptr;
    public:
        // Constructor
        Iterator(value_type* p) : ptr(p) {}

        // Overloading operators
        value_type& operator*() {
            return *ptr;
        }

        Iterator& operator++() {
            ++ptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++ptr;
            return temp;
        }

        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }

        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }

        // Pointer arithmetic
        int operator-(const Iterator& other) const {
            return ptr - other.ptr;
        }
    };
};

} // namespace MySTL

int main() {
    MySTL::MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (auto val : vec) {
        std::cout << val << " ";
    }
    std::cout << endl;

    for(auto it = vec.begin();it!=vec.end();it++)
    {
        std::cout<<*it<<" ";
    }
    std::cout<<endl;

    return 0;
}
