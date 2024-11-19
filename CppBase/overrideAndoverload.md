| **特性**           | **虚函数**                 | **重写**                      | **重载**                  |
|--------------------|----------------------------|-------------------------------|---------------------------|
| **定义位置**       | 在基类中使用 `virtual` 声明 | 在派生类中，重写基类的虚函数  | 在同一个类中             |
| **参数列表**       | 与重写函数的参数列表一致    | 必须与基类虚函数的参数列表一致 | 参数列表必须不同         |
| **绑定时间**       | 运行时绑定（动态绑定）      | 运行时绑定（动态绑定）         | 编译时绑定（静态绑定）    |
| **关键字**         | 使用 `virtual` 声明         | 推荐使用 `override` 标注       | 无需关键字               |
| **是否涉及继承**   | 是                         | 是                            | 否                        |
