## Stack相关问题
1、为什么stack底层容器使用的是deque？
C++ 标准库中的 std::stack 默认情况下是使用 std::deque 作为其底层容器。让我们详细解析这个关系，并解释它们各自的特点和用途。

1. std::stack 和 std::deque 的关系
std::stack：容器适配器
定义：std::stack 是一种容器适配器，它提供了栈（后进先出，LIFO）接口。
功能：主要提供以下操作：
push(): 在栈顶插入元素。
pop(): 移除栈顶元素。
top(): 访问栈顶元素。
empty(): 检查栈是否为空。
size(): 获取栈中元素的数量。
默认底层容器：默认使用 std::deque，但也可以使用 std::vector 或 std::list 等其他支持必要操作的容器。
cpp
复制代码
#include <stack>
#include <deque>
#include <vector>
#include <iostream>

int main() {
    // 默认情况下，std::stack 使用 std::deque 作为底层容器
    std::stack<int> stack1;

    // 你也可以显式指定其他容器，如 std::vector
    std::stack<int, std::vector<int>> stack2;

    stack1.push(10);
    stack1.push(20);
    stack1.push(30);

    while (!stack1.empty()) {
        std::cout << stack1.top() << " "; // 输出: 30 20 10
        stack1.pop();
    }

    return 0;
}
std::deque：双端队列
定义：std::deque 是一种双端队列，支持在序列的两端高效地插入和删除元素。
特点：
动态大小：可以动态扩展和收缩。
双端操作：支持在头部和尾部插入、删除元素。
随机访问：支持常量时间的随机访问。
非连续存储：内部通常由多个固定大小的缓冲区组成，不像 std::vector 那样使用单一的连续内存块。
适用场景：适合需要频繁在两端插入或删除元素的场景，同时也需要随机访问元素。
2. 为什么 std::stack 默认使用 std::deque
效率：std::deque 提供在尾部插入和删除元素的常量时间复杂度，非常适合实现栈的 push 和 pop 操作。
灵活性：虽然 std::deque 支持双端操作，但 std::stack 只使用其中的一端（通常是尾部），确保了栈的后进先出（LIFO）特性。
随机访问：虽然 std::stack 不需要随机访问，但 std::deque 提供的随机访问能力保证了在需要时可以轻松切换底层容器（例如使用 std::vector），而不影响 std::stack 的接口和性能。
3. std::stack 使用 std::deque 的内部机制
当你使用 std::stack 时，实际上是在操作底层的 std::deque，但这些操作被封装在 std::stack 的接口中。以下是 std::stack 的一个简化实现示例：

cpp
复制代码
#include <deque>
#include <stdexcept>

namespace mystl {

template <class T, class Container = std::deque<T>>
class stack {
protected:
    Container c;  // 底层容器

public:
    // 常见类型定义
    typedef typename Container::value_type        value_type;
    typedef typename Container::size_type         size_type;
    typedef typename Container::reference          reference;
    typedef typename Container::const_reference    const_reference;

    // 构造函数
    stack() = default;
    explicit stack(const Container& cont) : c(cont) {}
    explicit stack(Container&& cont) : c(std::move(cont)) {}

    // 主要操作
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
    void push(const value_type& value) { c.push_back(value); }
    void push(value_type&& value) { c.push_back(std::move(value)); }
    void pop() { 
        if (c.empty()) {
            throw std::out_of_range("pop on empty stack");
        }
        c.pop_back(); 
    }
};

} // namespace mystl

int main() {
    mystl::stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    while (!s.empty()) {
        std::cout << s.top() << " "; // 输出: 3 2 1
        s.pop();
    }

    return 0;
}
4. 其他底层容器的选择
虽然 std::deque 是 std::stack 的默认底层容器，但你也可以选择其他容器，如 std::vector 或 std::list。不过，需要注意的是，所选容器必须支持以下操作：

对于 push 和 pop：需要在容器的一个端进行高效的插入和删除操作（通常是尾部）。
对于 top：需要能够高效地访问容器的最后一个元素。
以下是使用 std::vector 作为底层容器的 std::stack 示例：

cpp
复制代码
#include <vector>
#include <iostream>

int main() {
    // 使用 std::vector 作为底层容器
    std::stack<int, std::vector<int>> s;
    s.push(1);
    s.push(2);
    s.push(3);

    while (!s.empty()) {
        std::cout << s.top() << " "; // 输出: 3 2 1
        s.pop();
    }

    return 0;
}
5. 为什么不使用 std::list？
虽然 std::list 也是一个支持在任意位置插入和删除元素的容器，但它不支持随机访问（即 operator[] 或 at()），并且在缓存局部性和内存分配方面性能较差。因此，std::deque 和 std::vector 更适合作为 std::stack 的底层容器，因为它们提供更好的性能和缓存局部性。

6. 结论
std::stack 默认使用 std::deque，因为 std::deque 提供了高效的尾部插入和删除操作，非常适合实现栈的 push 和 pop。
双端队列的特性（支持两端的高效操作）并不会影响 std::stack，因为 std::stack 只使用了其中的一端（通常是尾部），确保了栈的后进先出（LIFO）特性。
可以自定义底层容器：虽然默认使用 std::deque，但你也可以选择其他满足条件的容器，如 std::vector，以适应不同的需求和性能优化。
