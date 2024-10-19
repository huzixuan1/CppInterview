#include <iostream>
#include <stdexcept>

template <typename T>
class SimpleList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    SimpleList() : head(nullptr), tail(nullptr) {}

    ~SimpleList() {
        while (head) {
            pop_front();
        }
    }

    void push_front(T value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
    }

    void push_back(T value) {
        Node* newNode = new Node(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void pop_front() {
        if (!head) throw std::runtime_error("List is empty");
        Node* temp = head;
        head = head->next;
        delete temp;
        if (!head) tail = nullptr; // List became empty
    }

    void pop_back() {
        if (!tail) throw std::runtime_error("List is empty");
        if (head == tail) { // Only one element
            delete tail;
            head = tail = nullptr;
        } else {
            Node* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
    }

    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* node) : current(node) {}

        T& operator*() { return current->data; }
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    // For debugging purposes
    void print() const {
        Node* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    SimpleList<int> myList;
    
    myList.push_back(1);
    myList.push_back(2);
    myList.push_front(0);
    
    myList.print(); // Output: 0 1 2 

    myList.pop_front();
    myList.print(); // Output: 1 2 

    myList.pop_back();
    myList.print(); // Output: 1 

    for (auto it = myList.begin(); it != myList.end(); ++it) {
        std::cout << *it << " "; // Output: 1
    }
    std::cout << std::endl;

    return 0;
}

