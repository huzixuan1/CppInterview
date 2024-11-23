// 观察者模式
// 它是一个行为型设计模式，它定义了一种一对多的依赖关系。使得一个对象（被观察者或主题）
//  状态发生变化时，所有以来的对象（观察者）都会收到通知并自动更新。
// 观察者模式是一种行为型设计模式，定义了一种一对多的依赖关系。
//  主题对象在状态改变时，会自动通知所有注册的观察者，并触发更新操作

// 优点：
//      1.实现了对象之间的松耦合，观察者和被观察者可以独立变化
//      2.支持动态添加观察者，灵活扩展
// 缺点：
//      1.如果观察者较多，通知的开销较大
//      2.如果有循环依赖或依赖较长，可能导致通知不一致


#include <iostream>
#include <vector>
#include <string>
#include <memory>

// 观察者接口
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(int age) = 0;
};

// 被观察者类
class Person {
private:
    int age;
    std::vector<std::shared_ptr<Observer>> observers;

public:
    Person(int initialAge) : age(initialAge) {}

    // 注册观察者
    void addObserver(const std::shared_ptr<Observer>& observer) {
        observers.push_back(observer);
    }

    // 设置年龄并通知观察者
    void setAge(int newAge) {
        age = newAge;
        notifyObservers();
    }

    int getAge() const {
        return age;
    }

    // 通知所有观察者
    void notifyObservers() {
        for (const auto& observer : observers) {
            observer->update(age);
        }
    }
};

// 观察者
class AgeObserver : public Observer {
private:
    std::string observerName;

public:
    AgeObserver(const std::string& name) : observerName(name) {}

    // 具体的更新逻辑
    void update(int age) override {
        std::cout << observerName << " has been notified. New age is: " << age << std::endl;
    }
};


int main() {
    // 创建一个被观察者
    Person person(25);

    // 创建两个观察者
    std::shared_ptr<Observer> observer1 = std::make_shared<AgeObserver>("Observer 1");
    std::shared_ptr<Observer> observer2 = std::make_shared<AgeObserver>("Observer 2");

    // 注册观察者
    person.addObserver(observer1);
    person.addObserver(observer2);

    // 改变年龄，观察者将会收到通知
    std::cout << "Changing age to 30..." << std::endl;
    person.setAge(30);

    return 0;
}

////////////////////////////////////
// 取消订阅 && get_can_vote
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm> // for std::remove_if

// 观察者接口
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(int age) = 0;
};

// 被观察者类
class Person {
private:
    int age;
    std::vector<std::shared_ptr<Observer>> observers;

public:
    Person(int initialAge) : age(initialAge) {}

    // 注册观察者
    void addObserver(const std::shared_ptr<Observer>& observer) {
        observers.push_back(observer);
    }

    // 取消注册观察者
    void removeObserver(const std::shared_ptr<Observer>& observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // 设置年龄并通知观察者
    void setAge(int newAge) {
        age = newAge;
        notifyObservers();
    }

    int getAge() const {
        return age;
    }

    // 通知所有观察者
    void notifyObservers() {
        for (const auto& observer : observers) {
            observer->update(age);
        }
    }
};

// 观察者类
class AgeObserver : public Observer {
private:
    std::string observerName;

public:
    AgeObserver(const std::string& name) : observerName(name) {}

    // 具体的更新逻辑
    void update(int age) override {
        std::cout << observerName << " has been notified. New age is: " << age << std::endl;
        if (age > 16) {
            std::cout << observerName << " is eligible to vote!" << std::endl;
        } else {
            std::cout << observerName << " is not eligible to vote." << std::endl;
        }
    }
};

int main() {
    // 创建一个被观察者
    Person person(25);

    // 创建一个观察者
    std::shared_ptr<Observer> observer1 = std::make_shared<AgeObserver>("Observer 1");

    // 注册观察者
    person.addObserver(observer1);

    // 改变年龄，观察者将会收到通知
    std::cout << "Changing age to 20..." << std::endl;
    person.setAge(20);

    // 取消订阅
    std::cout << "Removing Observer 1..." << std::endl;
    person.removeObserver(observer1);

    // 再次改变年龄，观察者将不会收到通知
    std::cout << "Changing age to 30..." << std::endl;
    person.setAge(30);

    return 0;
}


//////////////////
// 线程安全的订阅 && 取消订阅
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <mutex>

// 观察者接口
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(int age) = 0;
};

// 被观察者类
class Person {
private:
    int age;
    std::vector<std::shared_ptr<Observer>> observers;
    std::mutex mtx; // 互斥锁

public:
    Person(int initialAge) : age(initialAge) {}

    // 注册观察者
    void addObserver(const std::shared_ptr<Observer>& observer) {
        std::lock_guard<std::mutex> lock(mtx); // 保护访问
        observers.push_back(observer);
    }

    // 取消注册观察者
    void removeObserver(const std::shared_ptr<Observer>& observer) {
        std::lock_guard<std::mutex> lock(mtx); // 保护访问
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // 设置年龄并通知观察者
    void setAge(int newAge) {
        age = newAge;
        notifyObservers();
    }

    int getAge() const {
        return age;
    }

    // 通知所有观察者
    void notifyObservers() {
        std::lock_guard<std::mutex> lock(mtx); // 保护访问
        for (const auto& observer : observers) {
            observer->update(age);
        }
    }
};

// 观察者类
class AgeObserver : public Observer {
private:
    std::string observerName;

public:
    AgeObserver(const std::string& name) : observerName(name) {}

    // 具体的更新逻辑
    void update(int age) override {
        std::cout << observerName << " has been notified. New age is: " << age << std::endl;
        if (age > 16) {
            std::cout << observerName << " is eligible to vote!" << std::endl;
        } else {
            std::cout << observerName << " is not eligible to vote." << std::endl;
        }
    }
};

int main() {
    // 创建一个被观察者
    Person person(25);

    // 创建一个观察者
    std::shared_ptr<Observer> observer1 = std::make_shared<AgeObserver>("Observer 1");

    // 注册观察者
    person.addObserver(observer1);

    // 改变年龄，观察者将会收到通知
    std::cout << "Changing age to 20..." << std::endl;
    person.setAge(20);

    // 取消订阅
    std::cout << "Removing Observer 1..." << std::endl;
    person.removeObserver(observer1);

    // 再次改变年龄，观察者将不会收到通知
    std::cout << "Changing age to 30..." << std::endl;
    person.setAge(30);

    return 0;
}


// C++17中的线程安全
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <mutex>

// 观察者接口
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(int age) = 0;
};

// 被观察者类
class Person {
private:
    int age;
    std::vector<std::shared_ptr<Observer>> observers;
    std::mutex mtx; // 互斥锁

public:
    Person(int initialAge) : age(initialAge) {}

    // 注册观察者
    void addObserver(const std::shared_ptr<Observer>& observer) {
        std::scoped_lock lock(mtx); // 使用 scoped_lock 功能和std::lock(lock1,lock2);类似 std::scoped_lock(lock1,lock2)
        observers.push_back(observer);
    }

    // 取消注册观察者
    void removeObserver(const std::shared_ptr<Observer>& observer) {
        std::scoped_lock lock(mtx); // 使用 scoped_lock
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // 设置年龄并通知观察者
    void setAge(int newAge) {
        age = newAge;
        notifyObservers();
    }

    int getAge() const {
        return age;
    }

    // 通知所有观察者
    void notifyObservers() {
        std::scoped_lock lock(mtx); // 使用 scoped_lock
        for (const auto& observer : observers) {
            observer->update(age);
        }
    }
};

// 观察者类
class AgeObserver : public Observer {
private:
    std::string observerName;

public:
    AgeObserver(const std::string& name) : observerName(name) {}

    // 具体的更新逻辑
    void update(int age) override {
        std::cout << observerName << " has been notified. New age is: " << age << std::endl;
        if (age > 16) {
            std::cout << observerName << " is eligible to vote!" << std::endl;
        } else {
            std::cout << observerName << " is not eligible to vote." << std::endl;
        }
    }
};

int main() {
    // 创建一个被观察者
    Person person(25);

    // 创建一个观察者
    std::shared_ptr<Observer> observer1 = std::make_shared<AgeObserver>("Observer 1");

    // 注册观察者
    person.addObserver(observer1);

    // 改变年龄，观察者将会收到通知
    std::cout << "Changing age to 20..." << std::endl;
    person.setAge(20);

    // 取消订阅
    std::cout << "Removing Observer 1..." << std::endl;
    person.removeObserver(observer1);

    // 再次改变年龄，观察者将不会收到通知
    std::cout << "Changing age to 30..." << std::endl;
    person.setAge(30);

    return 0;
}

// 解决可重入性的问题
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <mutex>

// 观察者接口
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(int age) = 0;
};

// 被观察者类
class Person {
private:
    int age;
    std::vector<std::shared_ptr<Observer>> observers;
    std::recursive_mutex mtx; // 使用递归互斥锁

public:
    Person(int initialAge) : age(initialAge) {}

    // 注册观察者
    void addObserver(const std::shared_ptr<Observer>& observer) {
        std::scoped_lock lock(mtx); // 使用 scoped_lock
        observers.push_back(observer);
    }

    // 取消注册观察者
    void removeObserver(const std::shared_ptr<Observer>& observer) {
        std::scoped_lock lock(mtx); // 使用 scoped_lock
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // 设置年龄并通知观察者
    void setAge(int newAge) {
        age = newAge;
        notifyObservers();
    }

    int getAge() const {
        return age;
    }

    // 通知所有观察者
    void notifyObservers() {
        std::scoped_lock lock(mtx); // 使用 scoped_lock
        for (const auto& observer : observers) {
            observer->update(age); // 如果 update 方法内部又调用了 notifyObservers，可以安全执行
        }
    }
};

// 观察者类
class AgeObserver : public Observer {
private:
    std::string observerName;

public:
    AgeObserver(const std::string& name) : observerName(name) {}

    // 具体的更新逻辑
    void update(int age) override {
        std::cout << observerName << " has been notified. New age is: " << age << std::endl;
        if (age > 16) {
            std::cout << observerName << " is eligible to vote!" << std::endl;
        } else {
            std::cout << observerName << " is not eligible to vote." << std::endl;
        }

        // 可以安全地再次通知观察者
        // 例如，模拟一个条件，调用 notifyObservers
        if (age == 20) {
            std::cout << observerName << " triggers another notification for age 21." << std::endl;
            // 如果需要再次通知
            // person.notifyObservers(); // 这会导致递归调用
        }
    }
};

int main() {
    // 创建一个被观察者
    Person person(25);

    // 创建一个观察者
    std::shared_ptr<Observer> observer1 = std::make_shared<AgeObserver>("Observer 1");

    // 注册观察者
    person.addObserver(observer1);

    // 改变年龄，观察者将会收到通知
    std::cout << "Changing age to 20..." << std::endl;
    person.setAge(20);

    // 取消订阅
    std::cout << "Removing Observer 1..." << std::endl;
    person.removeObserver(observer1);

    // 再次改变年龄，观察者将不会收到通知
    std::cout << "Changing age to 30..." << std::endl;
    person.setAge(30);

    return 0;
}

