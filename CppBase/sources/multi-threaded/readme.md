# 锁机制对比表

| 锁类型               | 是否支持延迟加锁 | 是否支持显式解锁 | 是否支持多互斥量加锁 | 是否支持条件变量 | 适用标准 | 适用场景                      |
|----------------------|------------------|------------------|---------------------|------------------|----------|-------------------------------|
| `std::lock_guard`    | 否               | 否               | 否                  | 否               | C++11     | 简单单锁场景                  |
| `std::unique_lock`   | 是               | 是               | 否                  | 是               | C++11     | 复杂的单锁场景，需灵活控制锁  |
| `std::scoped_lock`   | 否               | 否               | 是                  | 否               | C++17     | 多锁场景，现代简洁用法        |
| `std::timed_mutex`   | 是（超时）       | 是               | 否                  | 否               | C++11     | 超时控制的单锁场景            |
| `std::shared_mutex`  | 是（共享锁）     | 是               | 是（读锁与写锁）    | 否               | C++17     | 读多写少，高并发场景          |
| `std::recursive_mutex` | 否             | 否               | 否                  | 否               | C++11     | 单线程递归加锁场景            |


#### 1.std::lock_guard
特点
    最简单的 RAII 锁，主要用于保护一个互斥量。
    在构造时自动加锁，在析构时自动解锁。
    不支持延迟加锁、解锁和条件变量等复杂操作。

```cpp
std::mutex mtx;

void func() {
    std::lock_guard<std::mutex> lock(mtx);  // 加锁
    // 临界区代码
}  // 自动解锁

```


#### 2.std::unique_lock
特点
    提供更灵活的锁管理：
    支持延迟加锁。
    支持显式解锁和重新加锁。
    可与条件变量一起使用。
    比 std::lock_guard 更强大，但稍微有点性能开销。

```cpp

std::mutex mtx;

void func() {
    // 延迟加锁
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock); 
    // 显式加锁
    lock.lock();
    // 临界区代码
    lock.unlock();  // 显式解锁
}

```

#### 3.std::scoped_lock
特点
    C++17 引入的现代锁，支持同时锁定多个互斥量。
    自动处理死锁问题。
    无需显式指定互斥量类型（例如无需 <std::mutex>）。
    不支持延迟加锁和显式解锁，专注于简单场景
    
```cpp
std::mutex mtx1, mtx2;

void func() {
    std::scoped_lock lock(mtx1, mtx2);  // 同时加锁 mtx1 和 mtx2
    // 临界区代码
}  // 自动解锁

```

#### 4.std::timed_mutex 和 std::recursive_timed_mutex
特点
    支持超时加锁，可以使用带有超时的 try_lock_for 和 try_lock_until。
    std::timed_mutex 是普通互斥量的增强版。
    std::recursive_timed_mutex 允许同一线程多次加锁，解锁次数需匹配。
    
```cpp
#include <mutex>
#include <chrono>

std::timed_mutex mtx;

void func() {
    if (mtx.try_lock_for(std::chrono::milliseconds(100))) {  // 尝试加锁
        // 临界区代码
        mtx.unlock();
    } else {
        // 超时处理
    }
}

```

#### 5.std::shared_mutex 和 std::shared_lock
特点
    C++17 引入的读写锁机制：
    共享锁：允许多个线程同时读取（读操作不会相互阻塞）。
    独占锁：写操作需要互斥（阻塞其他读写操作）。
    提供更高的并发性，适合读多写少的场景。

```cpp
#include <shared_mutex>

std::shared_mutex rw_mutex;

void reader() {
    std::shared_lock<std::shared_mutex> lock(rw_mutex);  // 共享锁
    // 读取数据
}

void writer() {
    std::unique_lock<std::shared_mutex> lock(rw_mutex);  // 独占锁
    // 写入数据
}

```

#### 6.std::adopt_lock 和 std::defer_lock
特点
    这些是锁构造选项，控制加锁的方式：
    std::adopt_lock：采用已有的锁状态（假设互斥量已经被锁定）。
    std::defer_lock：延迟加锁，稍后通过 lock() 手动加锁。

    接管锁，不需要再对接管的锁进行加锁了。同时还可以对锁进行释放。

```cpp
std::mutex mtx;

void func() {
    mtx.lock();  // 手动加锁
    std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);  // 接管锁
    // 临界区代码
}  // 自动解锁

```


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

#### std::recursive_mutex
递归锁（也称为递归互斥锁，std::recursive_mutex）是一个特殊的锁，它允许同一个线程多次锁定同一个资源而不会导致死锁。换句话说，当一个线程已经持有某个锁时，它可以在不释放锁的情况下再次请求该锁，而不会被阻塞。

递归锁的特点：
同一线程可以多次锁定：与普通的 std::mutex 不同，std::recursive_mutex 允许同一线程在已持有该锁的情况下，继续获得该锁。每次锁定都会增加一个锁计数，当锁计数归零时，锁才会被释放。
适用于递归调用场景：递归锁非常适用于那些可能会因为递归调用而多次进入同一临界区的场景，避免了死锁的发生。
递归锁的使用场景：
递归函数：如果一个函数在调用自己时需要访问共享资源，而这个共享资源已经被当前线程锁定，那么使用递归锁可以避免死锁。
复杂的多层调用：在复杂的多层函数调用中，可能会重复锁定相同的资源。在这种情况下，如果使用普通的互斥锁，可能会因为多次尝试锁定同一个资源而死锁。递归锁可以解决这个问题。

```cpp
#include <iostream>
#include <mutex>
#include <thread>

std::recursive_mutex mtx;  // 递归锁
int counter = 0;

void recursive_lock_function(int depth) {
    if (depth <= 0) return;
    std::lock_guard<std::recursive_mutex> lock(mtx);  // 锁定递归锁
    counter++;  // 操作共享资源
    std::cout << "Depth: " << depth << ", Counter: " << counter << std::endl;
    recursive_lock_function(depth - 1);  // 递归调用
}

int main() {
    std::thread t1(recursive_lock_function, 5);
    t1.join();
    return 0;
}

```

#### Spinlock
自旋锁（Spinlock）
类型：std::atomic_flag 或 std::spinlock（C++20 引入）
用途：如果锁已经被其他线程占用，当前线程会不断检查锁是否释放，而不会被挂起。
特点：
    在锁未被释放时，线程会不停地自旋（循环检查锁状态），这比传统的阻塞方式（如 std::mutex）要轻量，但如果锁长期不释放，可能会导致 CPU 浪费。
    适合短时间占用锁的场景。
    使用场景：用于锁的争用时间非常短的场景，例如频繁操作小的共享资源。


```cpp
std::atomic_flag lock = ATOMIC_FLAG_INIT;

// 自旋锁
void spinlock_example() {
    while (lock.test_and_set(std::memory_order_acquire)) {
        // 等待锁释放
    }
    // 访问共享资源
    lock.clear(std::memory_order_release);
}

```

#### 读写锁（Read-Write Lock）
类型：std::shared_timed_mutex 或 std::shared_mutex
用途：允许多个线程同时读取共享资源，但只有一个线程可以写入该资源。
特点：
    适用于读多写少的场景，避免了每次写操作都需要独占锁的情况，提高了读操作的并发性。
    使用场景：高并发读取的场景，例如缓存读取。

```cpp
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>

std::shared_mutex rw_lock;  // 读写锁
int shared_data = 0;        // 共享资源

// 读操作，多个线程可以并发读取
void read_data(int id) {
    std::shared_lock<std::shared_mutex> lock(rw_lock);  // 获取共享锁
    std::cout << "Thread " << id << " is reading shared_data: " << shared_data << std::endl;
}

// 写操作，只有一个线程可以写入
void write_data(int id, int value) {
    std::unique_lock<std::shared_mutex> lock(rw_lock);  // 获取独占锁
    shared_data = value;
    std::cout << "Thread " << id << " is writing shared_data: " << shared_data << std::endl;
}

int main() {
    std::vector<std::thread> threads;

    // 启动多个读线程
    for (int i = 0; i < 5; ++i) {
        threads.push_back(std::thread(read_data, i));
    }

    // 启动一个写线程
    threads.push_back(std::thread(write_data, 100, 42));

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    return 0;
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



