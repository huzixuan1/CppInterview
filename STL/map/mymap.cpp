#include <iostream>
#include <utility>  // for std::pair
#include <stack>    // for iterator traversal

template <typename Key, typename T>
class SimpleMap {
private:
    struct Node {
        std::pair<Key, T> data;
        Node* left;
        Node* right;
        Node(const Key& key, const T& value) : data(key, value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Insert helper function
    Node* insert(Node* node, const Key& key, const T& value) {
        if (!node) return new Node(key, value);
        if (key < node->data.first)
            node->left = insert(node->left, key, value);
        else if (key > node->data.first)
            node->right = insert(node->right, key, value);
        return node;
    }

    // Find node with specific key
    Node* find(Node* node, const Key& key) const {
        if (!node) return nullptr;
        if (key < node->data.first)
            return find(node->left, key);
        else if (key > node->data.first)
            return find(node->right, key);
        return node;
    }

public:
    SimpleMap() : root(nullptr) {}

    // Insert a key-value pair
    void insert(const Key& key, const T& value) {
        root = insert(root, key, value);
    }

    // Find the value associated with the given key
    T* find(const Key& key) {
        Node* result = find(root, key);
        if (result) return &(result->data.second);
        return nullptr;
    }

    // In-order traversal iterator
    class iterator {
    private:
        std::stack<Node*> nodeStack;
        Node* current;

        void pushLeft(Node* node) {
            while (node) {
                nodeStack.push(node);
                node = node->left;
            }
        }

    public:
        iterator(Node* root) : current(nullptr) {
            pushLeft(root);
            ++(*this);  // Initialize the iterator to the first element
        }

        iterator& operator++() {
            if (nodeStack.empty()) {
                current = nullptr;
                return *this;
            }
            current = nodeStack.top();
            nodeStack.pop();
            pushLeft(current->right);
            return *this;
        }

        std::pair<Key, T>& operator*() {
            return current->data;
        }

        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };

    iterator begin() {
        return iterator(root);
    }

    iterator end() {
        return iterator(nullptr);
    }
};

int main() {
    SimpleMap<int, std::string> map;
    map.insert(10, "ten");
    map.insert(20, "twenty");
    map.insert(5, "five");

    // Iterate over the map
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    // Find an element
    if (auto* value = map.find(20)) {
        std::cout << "Found 20: " << *value << std::endl;
    } else {
        std::cout << "20 not found" << std::endl;
    }

    return 0;
}

