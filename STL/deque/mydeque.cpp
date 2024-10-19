#include <iostream>
#include <stdexcept>

template<typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t size_;

public:
    Deque() : head(nullptr), tail(nullptr), size_(0) {}

    ~Deque() {
        while (!empty()) {
            pop_front();
        }
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size_++;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        size_++;
    }

    void pop_front() {
        if (empty()) throw std::runtime_error("Deque is empty");
        Node* temp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        delete temp;
        size_--;
        if (empty()) tail = nullptr; // 更新 tail
    }

    void pop_back() {
        if (empty()) throw std::runtime_error("Deque is empty");
        Node* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        delete temp;
        size_--;
        if (empty()) head = nullptr; // 更新 head
    }

    T& front() {
        if (empty()) throw std::runtime_error("Deque is empty");
        return head->data;
    }

    T& back() {
        if (empty()) throw std::runtime_error("Deque is empty");
        return tail->data;
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    // 迭代器实现
    class Iterator {
    private:
        Node* current;
    public:
        explicit Iterator(Node* node) : current(node) {}

        T& operator*() {
            return current->data;
        }

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        Iterator& operator--() {
            current = current->prev;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};

int main() {
    Deque<int> deq;

    deq.push_back(1);
    deq.push_back(2);
    deq.push_front(0);

    std::cout << "Front: " << deq.front() << std::endl; // 输出: 0
    std::cout << "Back: " << deq.back() << std::endl;   // 输出: 2

    deq.pop_front();
    deq.pop_back();

    for (auto it = deq.begin(); it != deq.end(); ++it) {
        std::cout << *it << " "; // 输出: 1
    }
    std::cout << std::endl;

    return 0;
}

