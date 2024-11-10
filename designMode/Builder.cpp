构造器模式（Builder Pattern）的核心目的是将一个复杂对象的构建过程与其表示分离，使得同样的构建过程可以创建出不同的表示。
通过构造器模式，我们可以分阶段、逐步地构建一个复杂对象的各个部分，而不需要一次性构造出完整的对象。


1. 构造器模式的顺序
构造器模式的顺序通常涉及以下步骤：

客户端请求构建过程：客户端通过调用 Director 类来控制产品的构建流程。客户端并不直接创建产品对象，而是依赖于指导者（Director）来完成构建任务。

指导者 (Director) 开始构建：指导者（Director）控制具体的建造步骤，确保产品的构建顺序是正确的。它并不关心具体细节，只是负责协调每个构建部分的顺序。指导者会依次调用建造者的方法来构建产品的各个部分。

建造者（Builder）逐步构建产品：具体的建造者（ConcreteBuilder）实现了抽象建造者接口，负责构建产品的具体部分。每个具体建造者负责不同的产品表示，可以在不同的建造者中构建不同配置的产品。

产品完成：当所有部件都被构建完成后，指导者通过 Builder 获取最终的产品。此时，产品已经具备了所需的所有部分，可以交给客户端使用。

2. 构造器模式的组件
构造器模式通常包含以下几个组件：

1. 产品（Product）
产品类是最终构建的对象，通常包含多个不同的部分。
产品的构建过程是通过逐步添加不同的组件来完成的。每个部分可能是独立的，但它们共同组成了一个完整的对象。
示例：在构造一个复杂的计算机时，Product 可以是 Computer，它可能包括 CPU、RAM、硬盘、显示器 等部件。
2. 抽象建造者（Builder）
抽象建造者是定义了构建产品各个部件的接口，客户端通过它来控制产品的构建过程。
建造者接口通常包括方法来构建产品的各个部分，如 BuildCPU()、BuildRAM()、BuildStorage() 等。
这个接口并不负责产品的创建，而是为具体建造者提供统一的接口规范。
3. 具体建造者（ConcreteBuilder）
具体建造者实现了 Builder 接口，逐步构建产品的具体部分，并维护一个产品对象。
每个具体建造者负责创建一种特定类型的产品，可能有不同的部件、配置或结构。
具体建造者可以通过 getResult() 返回最终构建的产品对象。
4. 指导者（Director）
指导者类负责控制构建的流程。它不直接创建产品的部件，而是负责调用具体建造者的方法来逐步构建一个完整的产品。
指导者确保构建过程的顺序和结构正确，并且客户端只需与指导者打交道。
通过指导者，客户端可以灵活地选择不同的建造者，从而构建不同的产品。
5. 客户端（Client）
客户端代码负责创建一个具体建造者实例，并通过指导者来开始构建过程。
客户端不需要知道产品的具体构建细节，只需要关注最终的产品。
3. 构造器模式的顺序示例
我们以一个构建计算机的例子来展示构造器模式的顺序和组件：

客户端：客户端通过 Director 开始构建产品，选择具体的建造者。

指导者（Director）：指导者管理产品构建的顺序，调用建造者的各个方法来逐步构建产品。

具体建造者（ConcreteBuilder）：具体建造者实现了 Builder 接口，负责构建计算机的各个部分，如 CPU、RAM、硬盘等。

产品（Product）：最终由具体建造者构建完成的计算机对象，包括 CPU、RAM、硬盘等


#include <iostream>
#include <string>
using namespace std;

// 产品类：Computer
class Computer {
public:
    string CPU;
    string RAM;
    string storage;

    void Show() {
        cout << "Computer Configuration:" << endl;
        cout << "CPU: " << CPU << endl;
        cout << "RAM: " << RAM << endl;
        cout << "Storage: " << storage << endl;
    }
};

// 抽象建造者：Builder
class Builder {
public:
    virtual void BuildCPU() = 0;
    virtual void BuildRAM() = 0;
    virtual void BuildStorage() = 0;
    virtual Computer* GetResult() = 0;
};

// 具体建造者：GamingComputerBuilder
class GamingComputerBuilder : public Builder {
private:
    Computer* computer;
public:
    GamingComputerBuilder() { computer = new Computer(); }

    void BuildCPU() override {
        computer->CPU = "Intel i9";
    }

    void BuildRAM() override {
        computer->RAM = "32GB";
    }

    void BuildStorage() override {
        computer->storage = "1TB SSD";
    }

    Computer* GetResult() override {
        return computer;
    }
};

// 具体建造者：OfficeComputerBuilder
class OfficeComputerBuilder : public Builder {
private:
    Computer* computer;
public:
    OfficeComputerBuilder() { computer = new Computer(); }

    void BuildCPU() override {
        computer->CPU = "Intel i5";
    }

    void BuildRAM() override {
        computer->RAM = "16GB";
    }

    void BuildStorage() override {
        computer->storage = "512GB SSD";
    }

    Computer* GetResult() override {
        return computer;
    }
};

// 指导者：Director
class Director {
private:
    Builder* builder;
public:
    Director(Builder* builder) : builder(builder) {}

    void Construct() {
        builder->BuildCPU();
        builder->BuildRAM();
        builder->BuildStorage();
    }
};

int main() {
    // 客户端代码
    Builder* gamingBuilder = new GamingComputerBuilder();
    Director* director = new Director(gamingBuilder);
    director->Construct();
    Computer* gamingPC = gamingBuilder->GetResult();
    gamingPC->Show();

    cout << "------------------------" << endl;

    Builder* officeBuilder = new OfficeComputerBuilder();
    director = new Director(officeBuilder);
    director->Construct();
    Computer* officePC = officeBuilder->GetResult();
    officePC->Show();

    // 清理资源
    delete gamingPC;
    delete officePC;
    delete gamingBuilder;
    delete officeBuilder;
    delete director;

    return 0;
}
