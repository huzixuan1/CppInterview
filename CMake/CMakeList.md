### CMakeList教程
https://www.runoob.com/cmake/cmake-tutorial.html

1. 指定 CMake 的最低版本
cmake_minimum_required(VERSION 3.10)
2. 定义项目名称和语言
project(MyProject VERSION 1.0 LANGUAGES CXX)
3. 指定源文件
set(SOURCES
    src/main.cpp
    src/util.cpp
)
4. 添加可执行文件
add_executable(MyApp ${SOURCES})
5. 可选：指定编译选项
target_compile_options(MyApp PRIVATE -Wall)
6. 可选：链接其他库
target_link_libraries(MyApp PRIVATE some_library)
