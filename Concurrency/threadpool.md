## Threadpool

线程池的核心步骤可以概括为以下几个部分：

1. 初始化线程池
创建工作线程：
根据指定的线程数量，创建多个工作线程。这些线程在启动时进入等待状态，准备处理任务。
2. 任务队列管理
任务队列：
创建一个任务队列，用于存储待执行的任务。通常使用 std::queue 来实现。
互斥保护：
使用互斥锁（std::mutex）保护任务队列，确保线程安全。
3. 提交任务
任务提交接口：
提供一个接口（如 enqueue 方法）来允许用户提交任务。任务通常是一个可调用对象（如函数、lambda 表达式等）。
封装任务：
将任务封装成可执行的形式（例如，使用 std::function），并将其添加到任务队列中。
4. 线程执行任务
工作线程循环：
每个工作线程进入一个无限循环，持续检查任务队列。
等待任务：
使用条件变量（std::condition_variable）使工作线程在没有任务时进入等待状态，减少 CPU 占用。
获取和执行任务：
当任务到达时，从任务队列中取出任务并执行。
5. 停止线程池
停止标志：
设置一个停止标志，以通知工作线程停止运行。
唤醒所有线程：
通知所有工作线程退出等待状态。
等待线程结束：
使用 join 等待所有工作线程完成执行。
6. 清理资源
资源释放：
释放所有线程和任务队列占用的资源。

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>

class ThreadPool {
public:
    // 构造函数，初始化线程池并启动指定数量的工作线程
    ThreadPool(size_t numThreads) : stopFlag(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this] {
                            return this->stopFlag || !this->tasks.empty();
                        });
                        
                        if (this->stopFlag && this->tasks.empty()) {
                            return;
                        }

                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    
                    task();  // 执行任务
                }
            });
        }
    }

    // 向线程池提交任务
    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using returnType = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<returnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<returnType> res = task->get_future();
        
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if (stopFlag) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }

            tasks.emplace([task]() { (*task)(); });     // (*this)()是说调用task函数
        }

        condition.notify_one();
        return res;
    }

    // 析构函数，停止线程池
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stopFlag = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers) {
            worker.join();  // 等待所有工作线程退出
        }
    }

private:
    std::vector<std::thread> workers;                // 工作线程
    std::queue<std::function<void()>> tasks;         // 任务队列

    std::mutex queueMutex;                           // 队列的互斥锁
    std::condition_variable condition;               // 条件变量，用于通知工作线程
    std::atomic<bool> stopFlag;                      // 是否停止线程池的标志
};

int main() {
    ThreadPool pool(4);  // 创建包含 4 个工作线程的线程池

    // 向线程池提交多个任务
    auto result1 = pool.enqueue([](int a, int b) { return a + b; }, 3, 7);
    auto result2 = pool.enqueue([](int a, int b) { return a * b; }, 5, 6);




    std::cout << "3 + 7 = " << result1.get() << std::endl;  // 输出：3 + 7 = 10
    std::cout << "5 * 6 = " << result2.get() << std::endl;  // 输出：5 * 6 = 30

    return 0;
}

