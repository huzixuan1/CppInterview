
#include <iostream>
#include <memory>
#include <stdexcept>

template <typename T, typename Allocator = std::allocator<T>>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* topNode;
    Allocator alloc;

public:
    Stack() : topNode(nullptr) {}

    ~Stack() {
        while (!empty()) {
            pop();
        }
    }

    void push(const T& value) {
        Node* newNode = alloc.allocate(1);
        alloc.construct(newNode, value);
        newNode->next = topNode;
        topNode = newNode;
    }

    void pop() {
        if (empty()) throw std::runtime_error("Stack is empty");
        Node* temp = topNode;
        topNode = topNode->next;
        alloc.destroy(temp);
        alloc.deallocate(temp, 1);
    }

    T& top() {
        if (empty()) throw std::runtime_error("Stack is empty");
        return topNode->data;
    }

    bool empty() const {
        return topNode == nullptr;
    }

    // For debugging purposes
    void print() const {
        Node* current = topNode;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    Stack<int> myStack;

    myStack.push(1);
    myStack.push(2);
    myStack.push(3);

    myStack.print(); // Output: 3 2 1 

    std::cout << "Top element: " << myStack.top() << std::endl; // Output: 3

    myStack.pop();
    myStack.print(); // Output: 2 1 

    return 0;
}
