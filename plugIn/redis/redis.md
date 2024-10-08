### redis教程
https://redis.com.cn/tutorial.html

https://www.runoob.com/redis/redis-tutorial.html


1.常见的数据结构

2.Redis中的Reactor模型

3.Reactor模型跟libevent有什么联系吗？


Reactor 线程模型详解
Reactor 线程模型是一种基于事件驱动的高效 I/O 处理模型，广泛应用于高性能网络服务器和事件驱动的应用程序。Reactor 模型通过将 I/O 操作和业务逻辑分离，以高效地处理并发连接。下面详细解释 Reactor 线程模型的概念、机制、实现方式及其高效性。

一、Reactor 模型的基本概念
Reactor 模型是一种事件驱动的设计模式，用于处理并发 I/O 操作。其核心思想是将 I/O 事件的监听和实际的 I/O 操作分离开来，由事件循环（Event Loop）负责监听 I/O 事件，当事件发生时，将事件分发给相应的事件处理器（Event Handler）进行处理。

Reactor 模型的主要组件包括：

事件多路分离器（Demultiplexer）：负责监听多个 I/O 事件，并将就绪事件通知给事件循环。常用的事件多路分离器有 select、poll、epoll 等。
事件循环（Event Loop）：不断地从事件多路分离器获取就绪事件，并分发给相应的事件处理器。
事件处理器（Event Handler）：处理具体的 I/O 事件，如读、写、连接等。
事件源（Event Source）：产生 I/O 事件的对象，如套接字（Socket）。
二、Reactor 模型的工作流程
Reactor 模型的工作流程可以分为以下几个步骤：

事件注册：应用程序将感兴趣的 I/O 事件（如读、写事件）注册到事件多路分离器中。
事件等待：事件循环调用事件多路分离器的等待函数，阻塞等待 I/O 事件的发生。
事件分发：一旦有 I/O 事件发生，事件循环将事件分发给相应的事件处理器。
事件处理：事件处理器处理具体的 I/O 事件，如读取数据、处理业务逻辑、发送响应等。
三、Reactor 模型的实现
Reactor 模型可以有多种实现方式，常见的有以下几种：

单线程 Reactor 模型：

所有的 I/O 事件和业务逻辑都在一个线程中处理。
适用于 I/O 密集型任务，业务处理时间较短的场景。
简单但在业务处理时间较长时性能有限。
多线程 Reactor 模型：

I/O 事件和业务逻辑由不同的线程处理。
事件循环线程负责监听 I/O 事件，并将事件分发给工作线程处理业务逻辑。
适用于 I/O 和业务处理均较密集的场景，能够充分利用多核 CPU 的优势。
主从 Reactor 模型（Master-Slave Reactor）：

主线程（Master）负责监听 I/O 事件，并将事件分发给从线程（Slave）处理。
从线程池（Slave Pool）中的多个线程负责处理具体的 I/O 事件和业务逻辑。
适用于高并发、大量连接的场景，进一步提升性能和可扩展性。
四、Reactor 模型的高效性
Reactor 模型的高效性主要体现在以下几个方面：

事件驱动机制：

通过事件驱动机制，Reactor 模型只在有事件发生时进行处理，避免了无事件情况下的轮询开销。
事件循环高效地等待和分发事件，减少了 CPU 空转时间。
非阻塞 I/O：

使用非阻塞 I/O 操作，防止单个 I/O 操作阻塞整个事件循环，提高并发处理能力。
配合事件驱动机制，进一步提升了 I/O 处理效率。
线程池：

多线程和主从 Reactor 模型通过线程池处理业务逻辑，充分利用多核 CPU 的并行处理能力。
线程池避免了频繁创建和销毁线程的开销，提升了系统的稳定性和性能。
解耦 I/O 和业务逻辑：

Reactor 模型将 I/O 事件监听和业务逻辑处理解耦，简化了编程模型，提高了代码的可维护性。
事件循环只负责事件的监听和分发，业务逻辑处理由独立的事件处理器完成，分工明确。
五、Reactor 模型的应用场景
Reactor 模型广泛应用于各种高性能、高并发的网络服务器和事件驱动的应用程序中，例如：

Web 服务器：如 Nginx、Apache 等，处理大量 HTTP 请求。
数据库服务器：如 Redis、MySQL 等，处理大量客户端连接。
即时通讯服务器：如 WhatsApp、微信等，处理大量即时消息。
事件驱动框架：如 Node.js，基于事件驱动模型处理异步 I/O 操作。
示例代码
以下是一个简化的单线程 Reactor 模型示例代码，演示了如何使用 epoll 实现 Reactor 模型的基本工作流程：
    ```cpp

#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
 
#define MAX_EVENTS 10
 
void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}
 
void handle_read(int fd) {
    char buf[512];
    ssize_t count;
    while ((count = read(fd, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, count);
    }
    if (count == -1 && errno != EAGAIN) {
        perror("read");
        close(fd);
    } else if (count == 0) {
        close(fd);
    }
}
 
int main() {
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
 
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET; // 监视读事件，使用边缘触发模式
    event.data.fd = STDIN_FILENO;
 
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }
 
    struct epoll_event events[MAX_EVENTS];
 
    while (1) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
 
        for (int n = 0; n < nfds; ++n) {
            if (events[n].events & EPOLLIN) {
                handle_read(events[n].data.fd);
            }
        }
    }
 
    close(epfd);
    return 0;
}
    ```
总结
Reactor 模型通过事件驱动和非阻塞 I/O 机制，高效地处理大量并发连接。其高效性体现在事件驱动机制、非阻塞 I/O、线程池以及 I/O 和业务逻辑的解耦上。理解 Reactor 模型的工作原理和高效性，有助于在高性能网络编程中充分发挥其优势。
