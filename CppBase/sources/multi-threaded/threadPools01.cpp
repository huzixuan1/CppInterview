#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <condition_variable>

using namespace std;

// 线程安全队列的实现
template<typename T>
class thread_safe_queue {
private:
    mutable std::mutex mtx; // 互斥锁，保护队列访问
    std::queue<T> data_queue; // 实际存储数据的队列
    std::condition_variable data_cond; // 条件变量，用于通知

public:
    thread_safe_queue() = default;

    void push(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        data_queue.push(std::move(value));
        data_cond.notify_one(); // 通知一个等待线程
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (data_queue.empty()) {
            return false;
        }
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lock(mtx);
        data_cond.wait(lock, [this]{ return !data_queue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return data_queue.empty();
    }
};

// join_threads类，用于在析构时等待所有线程完成
class join_threads {
    std::vector<std::thread>& threads;
public:
    explicit join_threads(std::vector<std::thread>& threads_): threads(threads_) {}
    ~join_threads() {
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
};

// 线程池类的实现
class thread_pool {
private:
    std::atomic_bool done; // 标志线程池是否已结束
    thread_safe_queue<std::function<void()>> work_queue; // 任务队列
    std::vector<std::thread> threads; // 工作线程集合
    join_threads joiner; // 管理线程的加入

    void worker_thread() {
        while (!done) {
            std::function<void()> task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield(); // 暂时让出线程控制权
            }
        }
    }

public:
    thread_pool() : done(false), joiner(threads) {
        unsigned const thread_count = std::thread::hardware_concurrency(); // 获取并发线程数量
        try {
            for (unsigned i = 0; i < thread_count; ++i) {
                threads.emplace_back(&thread_pool::worker_thread, this);
            }
        } catch (...) {
            done = true;
            throw;
        }
    }

    ~thread_pool() {
        done = true;
    }

    template<typename FunctionType>
    void submit(FunctionType f) {
        work_queue.push(std::function<void()>(f));
    }
};

// 示例函数，供任务使用
void example_task(int n) {
    std::cout << "Task " << n << " is being processed by thread " << std::this_thread::get_id() << std::endl;
}

int main() {
    thread_pool pool;

    // 提交多个任务到线程池
    for (int i = 0; i < 10; ++i) {
        pool.submit([i]() { example_task(i); });
    }

    std::this_thread::sleep_for(std::chrono::seconds(2)); // 等待一段时间以确保所有任务执行完毕

    return 0;
}
