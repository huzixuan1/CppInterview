适配器模式（Adapter Pattern）是一种结构型设计模式，用于将一个类的接口转换成客户期望的另一个接口。通过这种模式，原本由于接口不兼容而不能一起工作的类可以协同工作。

适配器模式的主要场景：
现有类的接口不符合客户的需求：当你想使用某个已经存在的类，但它的接口与目标接口不兼容时，就可以通过适配器模式进行转换。
使用第三方库时的接口转换：如果一个类库提供的接口和你的项目不完全兼容，适配器可以帮助你在不修改原类的前提下进行集成。
适配器模式的两种实现方式：
类适配器（基于继承）：通过多重继承实现适配器，适配器继承目标接口和已有的接口，重写已有类的部分接口，实现适配功能。这种方式在C++中较为少见，因为多重继承的复杂性。
对象适配器（基于组合）：通过组合已有类对象的方式实现适配器，适配器持有已有类的实例，并通过委托的方式实现接口的适配。
适配器模式的基本组成：
目标接口（Target）：定义客户端期望的接口。客户端通过这个接口与适配器交互。
待适配类（Adaptee）：这是一个现有的类，它的接口不符合目标接口。
适配器（Adapter）：实现目标接口，并将请求委托给Adaptee对象进行处理



1. 适配器模式是什么？
回答重点：适配器模式是一种结构型设计模式，旨在通过引入一个适配器类来解决接口不兼容的问题。它通过将一个类的接口转换成客户端希望的另一个接口，
使得两个本来不能一起工作的类能够协同工作。简单来说，适配器模式就是将一个不兼容的接口转化为一个兼容的接口。
2. 适配器模式的应用场景有哪些？
回答重点：
当你需要将一个类的接口转换成客户端期望的接口时。
当你想让一些不兼容的接口能够协同工作时。
当你希望为现有的类增加功能，而不改变原始代码时。
当需要在不修改现有代码的情况下与多个外部系统或库进行集成时（例如，集成第三方API时，接口可能不一致）。
3. 适配器模式与桥接模式有什么区别？
回答重点：
适配器模式主要是为了将两个不兼容的接口对接在一起，通常是通过一个适配器将现有接口转换成目标接口。
桥接模式是用来解耦接口和实现，它允许接口和实现可以独立变化。桥接模式通常用于需要多维度扩展的场景，而适配器模式则是更为特定的转换接口的场景。
4. 适配器模式有哪几种类型？
回答重点：
类适配器：通过继承的方式，适配器类继承目标类并实现适配功能（多用于单继承的语言，如 Java）。
对象适配器：通过组合的方式，适配器类持有目标类的实例并通过该实例调用方法来实现适配（C++、Python 等语言常用）。
5. 请举一个适配器模式的实际应用场景？
回答重点：
数据库适配器：假设你在开发一个ORM框架，支持多个数据库，如 MySQL 和 PostgreSQL。你可以通过适配器模式，将不同数据库的查询接口适配为统一的接口，让客户端代码只关注数据库操作，而不关心具体使用的数据库类型。
电源插头适配器：通过适配器将两脚插头转换为三脚插头，在不同国家/地区的电源插座中使用。
6. 在适配器模式中，适配器类如何实现目标接口？
回答重点：
适配器类需要实现目标接口，通常通过组合的方式，将现有类的接口适配为目标接口。例如，适配器会持有源类的实例，然后在适配器的方法中调用源类的相应方法。
7. 适配器模式的优点和缺点有哪些？
回答重点：
优点：
可以让不同接口的类协同工作，提高系统的灵活性。
可以通过适配器对不兼容的类进行转换而无需修改现有代码。
适配器通常用于现有代码与新系统的集成，减少系统间的依赖。
缺点：
可能会增加系统的复杂度，过多的适配器可能导致代码臃肿。
适配器模式通常会牺牲一些性能，因为它增加了额外的层次（如方法调用的间接性）。
8. 如何使用适配器模式实现跨平台兼容？
回答重点：适配器模式可以在不同的平台间提供接口转换。例如，假设你有一个应用需要支持 Windows 和 macOS 平台的文件读写接口，但每个平台提供的接口不同。通过适配器模式，你可以为每个平台编写适配器类，使得客户端代码不需要关心底层平台的差异，只需要通过统一的接口进行文件操作。
9. 可以举一个使用适配器模式的代码实例吗？
回答重点：面试官可能要求你通过一个简单的代码示例来演示适配器模式，比如如何将一个两脚插头适配为三脚插头（电源插头示例）或者如何在数据库操作中使用适配器模式来统一接口（ORM 示例）。你可以展示如何创建目标接口、源接口、适配器类，并且展示如何通过适配器使客户端代码无需更改即可使用不同接口。
10. 适配器模式的缺点有哪些？如何解决？
回答重点：
缺点：
可能增加系统的复杂度：适配器会导致代码结构变得更加复杂，特别是在有多个适配器和目标接口时。
难以理解：对于不熟悉适配器模式的开发者来说，使用适配器可能会增加代码的理解难度。
解决方法：
适配器的设计要尽量简洁，避免过多的嵌套或复杂的适配逻辑。
适配器应当遵循单一职责原则，确保每个适配器只负责一种适配任务，避免出现过多职责的适配器类。
11. 适配器模式和外观模式有何异同？
回答重点：
相同点：
都是结构型设计模式，旨在简化接口，使得系统更加灵活。
不同点：
适配器模式是用来将不兼容的接口适配为目标接口，解决接口不兼容的问题。
外观模式是用来为一组接口提供一个统一的、更简单的接口，通常用于简化复杂系统的使用。外观模式关注的是简化系统内部多个类的使用，而适配器模式关注的是让不兼容的接口可以协同工作。
12. 适配器模式如何解决继承关系中的问题？
回答重点：
适配器模式通过组合而不是继承来解决接口不兼容的问题，避免了多重继承可能带来的复杂性和不灵活性。它让客户端通过适配器类访问已有类，而不需要改变已有类的代码或结构。
总结：
适配器模式是一个常见的设计模式，通常涉及接口转换和适配的问题。面试时，面试官往往会测试你对适配器模式的理解，要求你讲解适配器模式的原理、应用场景、优缺点，以及如何在实际项目中使用它。同时，通过代码示例展示如何实现适配器模式也是常见的面试问题。


#include <iostream>
using namespace std;

// Client:I can use three-pin plus directly
// Connected to a three-pin socket
// Client: I have a two-pin plug but I can't use it directly with a three-pin socket
// Client: Using an adapter to make it compatible:
// Adapter converts two-pin to three-pin
// Connected to a two-pin socket

// 目标接口，客户端期望的接口：国外的三角插座
class ThreePinPlug
{

    public:
        virtual ~ThreePinPlug() = default;
        virtual void ConnectToThreePinSocket() const
        {
            std::cout<<"Connected to a three-pin socket"<<std::endl;
        }

};

// 被适配的类:中国的两脚插头

class TwoPinPlug
{

    public:
        
        void ConnectTwoPinPlug() const
        {
            std::cout<<"Connected to a two-pin socket"<<std::endl;
        } 
};

class PlugAdapter:public ThreePinPlug
{
    private:
        const TwoPinPlug* two_pin_plug_;    // 持有被适配对象的实例
    
    public:
        PlugAdapter(const TwoPinPlug* two_pin_plug) : two_pin_plug_(two_pin_plug) {}

        // 适配器实现三脚插头的接口，内部调用两脚插头的接口
        void ConnectToThreePinSocket() const override
        {
            std::cout<<"Adapter converts two-pin to three-pin"<<std::endl;
            two_pin_plug_->ConnectTwoPinPlug();
        }
};

// 客户端的代码：只认识三脚插座
void ClientCode(const ThreePinPlug* plug)
{
    plug->ConnectToThreePinSocket();
}

int main()
{

    // 客户端可以正常使用三脚插头
    ThreePinPlug* three_pin_plug = new ThreePinPlug;
    std::cout<<"Client:I can use three-pin plus directly"<<std::endl;
    ClientCode(three_pin_plug);

    // 两脚插头不能直接插入三角插座
    TwoPinPlug* two_pin_plug = new TwoPinPlug;
    std::cout<<"Client: I have a two-pin plug but I can't use it directly with a three-pin socket"<<std::endl;

    // 使用适配器来适配两脚插头
    PlugAdapter* adapter = new PlugAdapter(two_pin_plug);
    std::cout << "Client: Using an adapter to make it compatible:\n";
    ClientCode(adapter);

    delete three_pin_plug;
    delete two_pin_plug;
    delete adapter;

    return 0;
}
