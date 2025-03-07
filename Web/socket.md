## socket

https://blog.csdn.net/everysmile/article/details/139637570

1.socket和tcp/ip协议族的关系
  数据链路层，网络层，传输层协议都是在内核中实现的。因此操作系统需要实现一组系统调用，使得应用程序能够访问到这些协议提供的服务。实现这组系统调用的API主要有两套socket和XTI.XTI基本不再使用。
  由socket API有两点功能：
  
    1.将应用程序数据从用户缓冲区中复制到TCP/UDP内核发送缓冲区，以交付内核来发送数据。或者是从内核TCP/UDP接收缓冲区复制数据到用户缓冲区，以读取数据；
    
    2.应用程序可以通过它来修改内核中各层协议的某些头部信息或者其他数据结构，从而精细地控制底层通信的行为。比如可以i通过setsockopt函数来设置IP数据报在网络上的存活时间。


============
要熟悉网络编程，确实完成一些实际项目是非常有效的方法。通过实践，你可以掌握socket的使用、协议的理解、数据传输的控制等方面的知识。以下是两个项目建议，可以帮助你提高网络编程的技能：

项目1：简单的聊天程序
项目描述：创建一个基于TCP的聊天程序，允许多个客户端连接到服务器并进行实时聊天。服务器应能接收来自任一客户端的消息并将其广播到所有连接的客户端。

主要功能：

服务器：

监听特定端口并接受客户端连接。
接收来自任一客户端的消息并将其广播给所有其他连接的客户端。
处理客户端断开连接的情况。
客户端：

连接到服务器。
能够输入消息并发送到服务器。
显示从服务器接收到的消息。
技术要点：

使用TCP socket进行通信。
使用select()或poll()来处理多个客户端连接。
处理文本输入输出的方式（如使用线程或非阻塞IO）。
项目2：文件传输工具
项目描述：创建一个基于UDP或TCP的文件传输工具，允许用户将文件从一个计算机传输到另一台计算机。

主要功能：

服务器：

接收文件并将其保存到指定位置。
显示接收进度（如已接收字节数）。
客户端：

选择要传输的文件并指定目标服务器的IP地址和端口。
将文件分块发送到服务器。
显示传输进度。
技术要点：

使用TCP或UDP socket进行文件传输。
处理文件的读取和写入。
设计简单的协议来管理文件传输的开始、进度和结束。
处理可能出现的错误情况，如传输中断和重传。
额外建议：
阅读文档：在实现过程中，查阅相关的网络编程文档和资料（如man手册、RFC文档）以加深理解。
调试和测试：在项目实现后，进行充分的测试，确保程序在高并发或网络不稳定的情况下仍能正常工作。
扩展功能：在完成基本功能后，可以尝试添加更多功能，如用户身份验证、加密传输、图形用户界面等，进一步提升项目的复杂性和学习深度。
通过完成这些项目，你可以提高对网络编程的理解和实际操作能力，从而更熟悉网络编程的各个方面。

### socket基本用法
```cpp
// 服务器
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>

int main() {
    // 创建套接字
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        return 1;
    }

    // 设置服务器地址
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // 任意地址
    server_addr.sin_port = htons(8080);        // 设置端口

    // 绑定套接字到地址
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Bind failed!" << std::endl;
        return 1;
    }

    // 开始监听连接
    if (listen(server_fd, 5) == -1) {
        std::cerr << "Listen failed!" << std::endl;
        return 1;
    }

    std::cout << "Waiting for connection..." << std::endl;

    // 等待客户端连接
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        std::cerr << "Accept failed!" << std::endl;
        return 1;
    }

    std::cout << "Client connected!" << std::endl;

    // 接收客户端数据
    char buffer[1024];
    int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Received message: " << buffer << std::endl;
    }

    // 关闭套接字
    close(client_fd);
    close(server_fd);
    return 0;
}
```

```cpp
// 客户端
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>

int main() {
    // 创建套接字
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        return 1;
    }

    // 设置服务器地址
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 本地地址

    // 连接到服务器
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Connection failed!" << std::endl;
        return 1;
    }

    std::cout << "Connected to server!" << std::endl;

    // 发送数据
    const char* message = "Hello, server!";
    send(client_fd, message, strlen(message), 0);

    // 关闭套接字
    close(client_fd);
    return 0;
}
```


