


/**
 * 工厂模式
 */

// 工厂模式（Factory Pattern）是创建型设计模式之一，用于通过定义一个接口或抽象类来创建对象，而不直接实例化具体类。
// 工厂模式的核心思想是：将对象的创建与使用分离，以提高代码的灵活性和可扩展性

// 简单的说：工厂方法模式是一种创建型设计模式， 其在父类中提供一个创建对象的方法， 允许子类决定实例化对象的类型

// 优点：
    // 1.避免创建者和具体产品之间的紧密耦合
    // 2.单一职责原则。 你可以将产品创建代码放在程序的单一位置， 从而使得代码更容易维护
    // 3.开闭原则。 无需更改现有客户端代码， 你就可以在程序中引入新的产品类型

// 缺点：
    // 1.应用工厂方法模式需要引入许多新的子类， 代码可能会因此变得更复杂。 最好的情况是将该模式引入创建者类的现有层次结构中

// 简单工厂模式的实现
    // 简单工厂模式通过一个工厂类，根据传入的参数决定实例化哪一个具体类
    // 假设我们有一个基类 Shape 和两个派生类 Circle 和 Rectangle，使用简单工厂模式来创建对象

// 简单工厂模式
    //1.优点：简化了对象的创建，客户端只需提供参数即可，不需要关心具体类的构造细节
    //2.缺点：新增类型时，需要修改工厂类，违背了开闭原则

#include <memory>
#include <iostream>

// 抽象类
class Shape
{
    public:
        virtual void Draw() const = 0;  // 纯虚函数
        virtual ~Shape() = default;     // 虚构函数
};

// 派生类：圆形
class Circle:public Shape
{
    public:
        void Draw() const override
        {
            std::cout<<"Drawing a Circle."<<std::endl;
        }
};

// 派生类：矩形
class Ractangle:public Shape
{
    public:
        void Draw() const override
        {
            std::cout<<"Drawing a Ractangle."<<std::endl;
        }
};

// 简单工厂类
class ShapeFactor
{
    public:
        static std::unique_ptr<Shape> CreateShape(const std::string& type)
        {
            if(type == "Circle")
            {
                return std::make_unique<Circle>();
            }else if(type == "Rectangle"){
                return std::make_unique<Ractangle>();
            }else{
                return nullptr;
            }
        }
};

int main()
{

    // 使用工厂创建对象
    auto shape1 = ShapeFactor::CreateShape("Circle");
    auto shape2 = ShapeFactor::CreateShape("Rectangle");

    shape1->Draw();
    shape2->Draw();

    return 0;
}

// 工厂方法模式
// 工厂方法模式通过定义一个工厂接口，让子类决定实例化哪个具体类。这样，当新增类型时，只需增加新的工厂类，而无需修改已有代码
// 优点：
    // 1.遵循 开闭原则，新增类型只需新增对应的工厂类
    // 2.客户端无需修改，只需使用新的工厂即可

// 缺点：
    // 1.增加了类的数量，导致系统复杂性提高

#include <iostream>
#include <memory>

// 抽象类
class Shape {
public:
    virtual void Draw() const = 0;
    virtual ~Shape() = default;
};

// 派生类：圆形
class Circle : public Shape {
public:
    void Draw() const override {
        std::cout << "Drawing a Circle." << std::endl;
    }
};

// 派生类：矩形
class Rectangle : public Shape {
public:
    void Draw() const override {
        std::cout << "Drawing a Rectangle." << std::endl;
    }
};

// 工厂基类
class ShapeFactory
{
    public:
        virtual std::unique_ptr<Shape> CreateShape() const = 0; // 工厂方法
        virtual ~ShapeFactory() = default;
};

// 工厂子类：生成圆形
class CircleFactory:public ShapeFactory{
    public:
        std::unique_ptr<Shape> CreateShape() const override
        {
            return std::make_unique<Ractangle>();
        }
};


// 工厂子类：生成矩形
class RectangleFactory:public ShapeFactory
{
    public:
        std::unique_ptr<Shape> CreateShape() const override
        {
            return std::make_unique<Rectangle>();
        }
};

int main() {
    // 使用工厂创建对象
    std::unique_ptr<ShapeFactory> circleFactory = std::make_unique<CircleFactory>();
    std::unique_ptr<ShapeFactory> rectangleFactory = std::make_unique<RectangleFactory>();

    auto shape1 = circleFactory->CreateShape();
    auto shape2 = rectangleFactory->CreateShape();

    shape1->Draw();
    shape2->Draw();

    return 0;
}


// 抽象工厂模式
// 抽象工厂模式用于创建一组关联的对象。它功过定义多个工厂方法来生成不同
// 类型的对象

// 优点：1.遵循 开闭原则，可以轻松增加新的产品族（例如 Linux 工厂）。
//      2.方便创建相关联的一组对象。

// 缺点：增加了代码复杂度，需要定义更多类。

// 下面以两种类型的按钮和文本框来举例：分别是win,mac

// 产品接口：按钮
class Button
{
    public:
        virtual void Render() const = 0;
        virtual ~Button() = default;
};

// 产品接口：文本框
class TextBox
{
    public:
        virtual void Render() const = 0;
        virtual ~TextBox() = default;
};

// win产品系列
class WindowsButton:public Button
{
    void Render() const override
    {
        std::cout<<"Rendering windows Button."<<std::endl;
    }
};

class WindowsTextBox:public TextBox
{
    public:
        void Render() const override
        {
            std::cout<<"Rendering windows TextBox."<<std::endl;
        }
};

// mac系列产品
class MacButton:public Button
{
    public:
        void Render() const override
        {
            std::cout<<"Rendering Mac Button."<<std::endl;
        }
};

class MacTextBox:public TextBox
{
    public:
        void Render() const override
        {
            std::cout<<"Rendering Mac TextBox."<<std::endl;
        }
};

// 抽象工厂接口
class UIFactory
{
    public:
        virtual std::unique_ptr<Button> CreateButton() const = 0;
        virtual std::unique_ptr<TextBox> CreateTextBox() const = 0;
        virtual ~UIFactory() = default;
};

// win工厂
class WindowsFactory:public UIFactory
{
    public:
        std::unique_ptr<Button> CreateButton() const override
        {
            return std::make_unique<WindowsButton>();
        }

        std::unique_ptr<TextBox> CreateTextBox() const override
        {
            return std::make_unique<WindowsTextBox>();
        }
};


// mac工厂
class MacFactory:public UIFactory
{
    public:
        std::unique_ptr<Button> CreateButton() const override
        {
            return std::make_unique<MacButton>();
        }

        std::unique_ptr<TextBox> CreateTextBox() const override
        {
            return std::unique_ptr<MacTextBox>();
        }
};

int main()
{

    std::unique_ptr<UIFactory> factory = std::make_unique<WindowsFactory>();
    auto button = factory->CreateButton();
    auto testBox = factory->CreateTextBox();

    button->Render();
    button->Render();


    return 0;
}



| 模式               | 优点                               | 缺点                      | 使用场景                                                |
|--------------------|------------------------------------|---------------------------|--------------------------------------------------------|
| **简单工厂模式**    | 简单易用，隐藏了对象创建逻辑       | 不符合开闭原则，工厂类难以扩展 | 对象种类较少且变化不频繁时                             |
| **工厂方法模式**    | 符合开闭原则，可扩展性好           | 增加了类的数量            | 对象种类较多且需要经常扩展时                           |
| **抽象工厂模式**    | 创建一组相关联对象，符合开闭原则    | 系统复杂度较高            | 需要创建一组相关对象，且对其进行统一管理时（如 UI 组件的跨平台开发） |




    
