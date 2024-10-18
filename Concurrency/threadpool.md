## Threadpool

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

