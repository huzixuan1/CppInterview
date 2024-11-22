# `std::mutex` 和 `std::shared_mutex` 的区别

| 特性       | `std::mutex`                                | `std::shared_mutex`                           |
|------------|--------------------------------------------|----------------------------------------------|
| **锁类型**  | 独占锁（mutex），一次只能有一个线程持有锁 | 共享锁（shared lock）和独占锁（unique lock） |
| **读操作**  | 不允许多个线程并发读取；读和写都需要独占锁 | 允许多个线程并发读取（共享锁），无阻塞    |
| **写操作**  | 只有一个线程可以写，且会阻塞其他线程     | 只有一个线程可以写（独占锁），会阻塞读操作和其他写操作 |
| **适用场景**| 对共享资源进行独占访问的情况             | 频繁读数据，偶尔写数据的场景               |
| **性能**    | 当读操作较少时，性能较好，因为没有区分读写  | 适用于读操作占主导地位的场景，可以允许多个线程同时读，提高并发性 |
| **锁的类型**| 只能加独占锁                              | 可以加共享锁（多个线程同时读）和独占锁（一个线程写） |


#### 总结
什么时候使用 std::mutex？
如果你的程序中，线程操作共享资源的方式主要是写操作，且没有太多读操作，使用 std::mutex 就足够了。
std::mutex 适用于 读写交替 的场景，或者当只有少量线程进行写操作时。
性能要求不高，只关心操作的正确性时，std::mutex 是一种简单、有效的选择。
什么时候使用 std::shared_mutex？
当程序中有 大量读操作，且少量写操作时，std::shared_mutex 比 std::mutex 更适合，因为它允许多个线程同时读取数据，提高并发性。
例如，在一些高并发的 读取密集型 程序中，使用 std::shared_mutex 可以减少锁竞争，提升性能。

总结：
std::mutex 是最基本的锁机制，适用于需要完全独占访问共享资源的场景，不允许多个线程同时读写。
std::shared_mutex 则提供了 共享锁 和 独占锁，使得多个线程能够并发读取共享资源，只在写操作时才进行独占访问，适合读多写少的场景，能够提高并发性能。


#### std::atomic

std::atomic 用于提供原子操作，允许线程安全地执行单一变量的操作，如读取、写入、加法、减法等。通过原子操作，多个线程可以并发地对某些数据进行操作，而不需要显式的锁。
std::atomic 适用于处理基本数据类型（如整数、布尔值、指针等）的操作，确保操作的原子性。

```cpp

#include <atomic>

std::atomic<int> counter(0);

void increment() {
    counter.fetch_add(1, std::memory_order_relaxed);  // 原子增加
}

void decrement() {
    counter.fetch_sub(1, std::memory_order_relaxed);  // 原子减少
}


```

#### condition_variable

**std::condition_variable
std::condition_variable 用于线程之间的同步与通信。它允许一个线程在某个条件满足之前阻塞执行，直到另一个线程通知它。
std::condition_variable 通常与 std::mutex 配合使用，用来管理线程的等待和通知。**

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void print_id(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    while (!ready) {
        cv.wait(lock);  // 等待条件变量
    }
    // 当条件满足时，继续执行
    std::cout << "Thread " << id << " is ready!" << std::endl;
}

void go() {
    std::unique_lock<std::mutex> lock(mtx);
    ready = true;  // 设置条件变量
    cv.notify_all();  // 通知所有等待线程
}

int main() {
    std::thread threads[10];
    
    // 创建10个线程
    for (int i = 0; i < 10; ++i)
        threads[i] = std::thread(print_id, i);

    std::cout << "Preparing threads..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟一些准备工作
    
    go();  // 激活线程

    // 等待线程完成
    for (auto& th : threads) th.join();

    return 0;
}
```


#### std::future 和 std::promise

std::future 和 std::promise 用于线程间的异步通信，允许一个线程将计算的结果传递给另一个线程。
std::promise 用于设置值，而 std::future 用于获取这个值。

```cpp
#include <iostream>
#include <thread>
#include <future>

void calculate(std::promise<int> prom) {
    // 计算结果并通过 promise 设置
    prom.set_value(42);
}

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();  // 获取与 promise 相关联的 future

    std::thread t(calculate, std::move(prom));  // 异步计算
    std::cout << "The answer is: " << fut.get() << std::endl;  // 获取计算结果

    t.join();
    return 0;
}

```

#### std::barrier（C++20 引入）

std::barrier 是 C++20 中引入的一种同步机制，
它用于多个线程在达到某个“屏障”之前都会阻塞，直到所有线程都达到“屏障”，然后它们一起继续执行.

```cpp
#include <iostream>
#include <thread>
#include <barrier>

void task(std::barrier<> &bar) {
    std::cout << "Task before barrier\n";
    bar.arrive_and_wait();  // 等待所有线程到达屏障
    std::cout << "Task after barrier\n";
}

int main() {
    std::barrier<> bar(3);  // 3个线程一起等待
    std::thread t1(task, std::ref(bar));
    std::thread t2(task, std::ref(bar));
    std::thread t3(task, std::ref(bar));

    t1.join();
    t2.join();
    t3.join();

    return 0;
}

```
