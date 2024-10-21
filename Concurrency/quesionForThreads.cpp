
//////////////////////////////////////////////////////////////////////////
// 题目 1:编写多个线程
// 任务: 编写一个程序，启动多个线程，每个线程输出 "Hello from thread X"（其中 X 是线程编号）。
// 确保主线程在所有子线程完成后结束。


#include <thread>
#include <vector>
#include <iostream>
using namespace std;

void threadFunction(int threadId)
{
    std::cout<<"the threadId is:"<<threadId<<std::endl;
}

int main()
{

    int threadNumber = 10;
    std::vector<std::thread> threads;
    for(int i = 0;i<threadNumber;i++)
    {
        threads.emplace_back(threadFunction,i);
    }


    for(auto& it:threads)
    {
        if(it.joinable())
        {
            it.join();
        }
    }

    std::cout<<"all threads completed and exiting main thread."<<std::endl;

    return 0;
}


/////////////////////////////////////

// 题目 2: 线程同步与互斥
// 任务: 创建多个线程，它们将同时对一个共享计数器进行递增操作。使用 std::mutex 来保护该计数器，确保最终结果正确。


#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
using namespace std;

int counter = 0;
std::mutex counterMutex;

void incrementCounter(int threadId,int increments)
{
    for(int i = 0;i<increments;i++)
    {
        std::lock_guard<std::mutex> lock(counterMutex);
        counter++;
        std::cout<<"Thread "<<threadId<<" increament counter to "<<counter<<std::endl;
    }
}

int main()
{

    int numThreads = 5;
    int incrementsPerThread = 10;
    std::vector<std::thread> threads;
    for(int i = 0 ;i<numThreads;i++)
    {
        threads.emplace_back(incrementCounter,i,incrementsPerThread);
    }

    for(auto& t:threads)
    {
        if(t.joinable())
        {
            t.join();
            std::cout<<"all threads completed and exiting main thread"<<std::endl;
        }
    }

    return 0;
}


// 题目 3: 条件变量
// 任务: 实现一个生产者-消费者模型。使用 std::condition_variable，让生产者生成数据，
// 消费者读取数据。你可以模拟一个缓冲区，生产者生成数据放入缓冲区，消费者从缓冲区中读取数据，确保线程安全。

#include <queue>
#include <chrono>
#include <random>
#include <thread>
#include <iostream>
#include <condition_variable>

using namespace std;

class ProducerConsumer
{
    private:
        size_t maxSize;
        std::queue<int> queue;
        std::mutex mutex;
        std::condition_variable cv;

        int produce()
        {
            static std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<int> dist(1,100);
            return dist(rng);   // 产生1到100的随机数
        }

    public:
        ProducerConsumer(size_t maxSize):maxSize(maxSize){}

        // producer
        void producer()
        {
            while(true)
            {
                std::unique_lock<std::mutex> lock(mutex);
                cv.wait(lock,[this]{return queue.size()<maxSize;});     // 等待直到缓冲区中有空间
                int data = produce();   // 随机生成一个数据
                queue.push(data);
                std::cout<<"produced: "<<data<<std::endl;

                lock.unlock();
                cv.notify_all();     // notify consumer
                std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 模拟休眠
            }
        }

        // consumer
        void consumer()
        {
            while(true)
            {
                std::unique_lock<std::mutex> lock(mutex);
                cv.wait(lock,[this]{return !queue.empty();});   // 等待直到缓冲区有数据

                int data = queue.front();
                queue.pop();
                std::cout<<"consumer: "<<data<<std::endl;

                lock.unlock();
                cv.notify_all();    // 通知生产者
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
        }
};

int main()
{
    ProducerConsumer pc(5);
    
    std::thread producerThread(&ProducerConsumer::producer,&pc);
    std::thread consumerThread(&ProducerConsumer::consumer,&pc);

    producerThread.join();
    consumerThread.join();

    return 0;
}



// 题目 4: 线程池
// 任务: 实现一个简易线程池，包含固定数量的工作线程，线程可以从任务队列中取任务执行。需要支持线程的自动调度和任务提交。


#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <future>
#include <iostream>
#include <functional>
#include <condition_variable>

using namespace std;

class ThreadPool
{
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable condition;
        std::atomic<bool> stopFlag;

    public:
        ThreadPool(size_t numThreads):stopFlag(false)
        {
            for(size_t i = 0;i<numThreads;i++)
            {
                workers.emplace_back([this]{
                    while(true)
                    {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->queueMutex);
                            this->condition.wait(lock,[this]{
                                return this->stopFlag || !this->tasks.empty();
                            });
                            if(this->stopFlag && this->tasks.empty())
                            {
                                return;
                            }
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }
                        task();
                    }   // while
                });
            }   // for
        }


        template<class F,class... Args>
        auto enqueue(F&& f,Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
        {
            using returnType = typename std::result_of<F(Args...)>::type;
            auto task = std::make_shared<std::packaged_task<returnType()>>(
                std::bind(std::forward<F>(f),std::forward<Args>(args)...)
            );

            std::future<returnType> res = task->get_future();
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if(stopFlag)
                {
                    throw std::runtime_error("enqueue on stopped ThreadPool.");
                }
                tasks.emplace([task]() {(*task)();});
            }
            condition.notify_one();
            return res;

        }


        ~ThreadPool()
        {
            {
            std::unique_lock<std::mutex> lock(queueMutex);
            stopFlag = true;
            }

            condition.notify_all();
            for(std::thread &worker : workers)
            {
                worker.join();
            }
        }



};


int main()
{
    ThreadPool pool(4);

    auto result1 = pool.enqueue([](int a,int b){return a+b;},3,7);
    auto result2 = pool.enqueue([](int a,int b){return a-b;},10,1);

    std::cout << "result1: " << result1.get() << std::endl;  
    std::cout << "result2:" << result2.get() << std::endl;  

    return 0;
}


// 题目 5: 异步任务
// 任务: 使用 std::future 和 std::async 实现一个简单的任务调度器，提交多个异步任务，主线程等待任务结果。任务之间可以有依赖关系

// std::async基础知识

#include <iostream>
#include <future>
#include <thread>



int add(int a, int b) {
    return a + b;
}

int main() {
    // 使用 std::async 启动异步任务
    std::future<int> result = std::async(std::launch::async, add, 5, 10);

    // 等待并获取结果
    std::cout << "Result: " << result.get() << std::endl; // 输出 Result: 15

    return 0;
}

int add(int a, int b) {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // 模拟长时间运行的任务
    return a + b;
}

int main() {
    // 使用 std::async 启动一个延迟执行的任务
    std::future<int> result = std::async(std::launch::deferred, add, 5, 10);

    // 这里可以做其他工作
    std::cout << "Doing other work while waiting for the result..." << std::endl;

    // 当调用 get() 时，任务开始执行
    std::cout << "Result: " << result.get() << std::endl; // 输出 Result: 15

    return 0;
}


#include <iostream>
#include <future>
#include <thread>

using namespace std;

class TaskScheduler
{
    public:

        template<class F,class... Args>
        auto submit(F&& func,Args&&... args) -> std::future<decltype(func(args...))>
        {
            return std::async(std::launch::async,std::forward<F>(func),std::forward<Args>(args)...);
        }

        template<class F,class DepFuture,class... Args>
        auto submit_with_dependency(DepFuture& dep_future,F&& func,Args&&... args) -> std::future<decltype(func(dep_future.get(),args...))>
        {
            return std::async(std::launch::async,[&,func,args...](){
                dep_future.wait();
                return func(dep_future.get(),args...);
            });
        }
};

int main()
{

    TaskScheduler scheduler;

    auto task1 = scheduler.submit([](int a,int b){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout<<"Task1 finished:"<<a+b<<std::endl;
        return a+b;
    },2,9);

    auto task2 = scheduler.submit_with_dependency(task1, [](int result_from_task1) {
        std::cout<<"result_from_task1:"<<result_from_task1<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 模拟任务执行时间
        std::cout << "Task 2 finished: " << result_from_task1 * 2 << std::endl;
        return result_from_task1 * 2;
    });

    auto task3 = scheduler.submit_with_dependency(task2, [](int result_from_task2) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 模拟任务执行时间
        std::cout << "Task 3 finished: " << result_from_task2 + 100 << std::endl;
        return result_from_task2 + 100;
    });

    task3.wait();
    std::cout<<"All tasks finished!"<<std::endl;


    return 0;
}


// 题目 6: 死锁预防
// 任务: 模拟两个线程需要同时获取两个资源的情况。故意设计一个可能引发死锁的场景，并尝试通过锁的顺序或使用 std::lock 来避免死锁。
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

std::mutex resource1;
std::mutex resource2;

void threadA()
{

    std::cout<<"Thread A: Trying to lock resource1."<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout<<"Thread A: Trying to lock resource2."<<std::endl;

    std::lock(resource1,resource2);
    std::lock_guard<std::mutex> lock1(resource1,std::adopt_lock);
    std::lock_guard<std::mutex> lock2(resource2,std::adopt_lock);

    std::cout<<"Thread A: Acquired both resource."<<std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout<<"Thread A: Releasing resources."<<std::endl;

}

void threadB()
{

    std::cout<<"Thread B: Trying to lock resource2."<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout<<"Thread B: Trying to lock resource1."<<std::endl;

    std::lock(resource1,resource2);
    std::lock_guard<std::mutex> lock1(resource1,std::adopt_lock);
    std::lock_guard<std::mutex> lock2(resource2,std::adopt_lock);

    std::cout<<"Thread B: Acquired both resource."<<std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout<<"Thread B: Releasing resources."<<std::endl;

}

int main()
{

    std::thread t1(threadA);
    std::thread t2(threadB);

    t1.join();
    t2.join();

    return 0;
}



// 题目 7: 高级任务 - 并发排序
// 任务: 实现一个并发的快速排序算法。将数组分区的任务分配给不同的线程执行，
// 确保并发性能提升的同时，结果仍然是正确的排序数组。
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
using namespace std;

void quickSort(std::vector<int>& arr,int left,int right)
{
    if(left>=right) return;
    int pivot = arr[right];     // 选择最左边为基准
    int i = left;

    for(int j = left;j<right;j++)
    {
        if(arr[j]<pivot)
        {
            std::swap(arr[i],arr[j]);
            i++;
        }
    }

    std::swap(arr[i],arr[right]);   // 将基准放在正确的位置

    // 使用线程完成并行处理子数组
    std::thread leftThread(quickSort,std::ref(arr),left,i-1);
    std::thread rightThread(quickSort,std::ref(arr),i+1,right);

    // 等待线程完成
    leftThread.join();
    rightThread.join();

}

int main()
{

    std::vector<int> arr = {34, 7, 23, 32, 5, 62, 32, 6, 99};
    std::cout<<"Original arr:";
    for(int& num:arr)
    {   
        std::cout<<num<<" ";
    }

    std::cout<<std::endl;

    quickSort(arr,0,arr.size()-1);
    std::cout<<"Sorted arr:";
    for(int num:arr)
    {
        std::cout<<num<<" ";
    }
    std::cout<<std::endl;


    return 0;
}









