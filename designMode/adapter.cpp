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
