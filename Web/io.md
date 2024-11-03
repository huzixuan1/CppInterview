## I/O复用

select：

select 是最早出现的 I/O 多路复用机制之一，适用于多个文件描述符上的 I/O 事件监听。它使用线性扫描方式遍历所有的文件描述符，并阻塞等待事件发生。但是 select 的缺点是每次调用时都需要将所有的文件描述符集合从用户空间拷贝到内核空间，导致效率较低。

poll：

poll 是对 select 的改进版本，也可以用于监听多个文件描述符上的 I/O 事件。与 select 不同的是，poll 在内核中维护了一个链表来存放被监视的文件描述符，避免了每次调用时都需要拷贝整个描述符集合。但是随着监视的文件描述符数量增加，效率会逐渐下降。

epoll：

epoll 是在 Linux 上引入的高级I/O多路复用机制，相对于 select 和 poll 具有更高的效率和扩展性。epoll 使用基于事件驱动方式工作，并通过回调函数通知应用程序哪些文件描述符就绪。它能够处理大量并发连接而不受限于单个进程/线程或文件描述符数量。此外，epoll 还提供了三种工作模式：EPOLLIN（读事件就绪）、EPOLLOUT（写事件就绪）和 EPOLLET（边缘触发），使其更加灵活。

“事件驱动”是指一种编程模型，在这种模型中，程序通过事件通知机制来处理 I/O 操作，而不是主动查询文件描述符的状态。这种机制在 epoll 中得到了很好的应用。

epoll 的工作原理
注册事件：

使用 epoll 时，程序首先创建一个 epoll 实例，并将需要监控的文件描述符和相关事件（如可读、可写等）注册到这个实例中。
事件等待：

程序调用 epoll_wait 函数，这个调用会阻塞，直到有注册的文件描述符发生事件。此时，程序不需要在此期间忙碌地进行轮询，而是可以被挂起，直到内核通知有事件发生。

事件通知：

一旦有事件发生，内核会通过 epoll_wait 返回相应的事件信息。这些事件信息包含了哪些文件描述符已经准备好进行 I/O 操作。
处理事件：

程序获取到事件后，可以针对准备好的文件描述符进行相应的读写操作。
优势：
高效性：

因为 epoll 只在事件发生时通知应用程序，避免了像 poll 和 select 那样的线性扫描。这使得 epoll 可以高效地处理大量并发连接。

边缘触发和水平触发：

epoll 支持两种触发模式：
水平触发（Level Triggered）：只要文件描述符的状态满足条件（例如可读），就会一直通知。
边缘触发（Edge Triggered）：只有当状态从不可用变为可用时才会通知。这种模式可以减少系统调用的次数，提高效率，但需要应用程序管理好状态。
总结
事件驱动的模型使得 epoll 能够在高并发场景下更有效地管理 I/O 操作，减少不必要的资源浪费。通过只在有事件发生时进行处理，epoll 提高了程序的整体性能。



#### select

```c
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    fd_set readfds;
    struct timeval timeout;
    int fd = 0; // 标准输入
    char buffer[128];

    // 设置非阻塞模式
    fcntl(fd, F_SETFL, O_NONBLOCK);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        timeout.tv_sec = 5; // 超时5秒
        timeout.tv_usec = 0;

        int result = select(fd + 1, &readfds, NULL, NULL, &timeout);
        if (result == -1) {
            perror("select error");
            return 1;
        } else if (result == 0) {
            printf("Timeout, no data\n");
        } else {
            if (FD_ISSET(fd, &readfds)) {
                int n = read(fd, buffer, sizeof(buffer));
                if (n > 0) {
                    buffer[n] = '\0';
                    printf("Received: %s", buffer);
                }
            }
        }
    }

    return 0;
}


#### poll

```c

#include <stdio.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    struct pollfd fds[1];
    char buffer[128];
    int fd = 0; // 标准输入

    // 设置非阻塞模式
    fcntl(fd, F_SETFL, O_NONBLOCK);

    fds[0].fd = fd;
    fds[0].events = POLLIN;

    while (1) {
        int result = poll(fds, 1, 5000); // 超时5秒
        if (result == -1) {
            perror("poll error");
            return 1;
        } else if (result == 0) {
            printf("Timeout, no data\n");
        } else {
            if (fds[0].revents & POLLIN) {
                int n = read(fd, buffer, sizeof(buffer));
                if (n > 0) {
                    buffer[n] = '\0';
                    printf("Received: %s", buffer);
                }
            }
        }
    }

    return 0;
}


#### epoll 

```c

#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 error");
        return 1;
    }

    struct epoll_event event;
    struct epoll_event events[10];
    int fd = 0; // 标准输入
    char buffer[128];

    // 设置非阻塞模式
    fcntl(fd, F_SETFL, O_NONBLOCK);

    event.events = EPOLLIN;
    event.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);

    while (1) {
        int nfds = epoll_wait(epoll_fd, events, 10, 5000); // 超时5秒
        if (nfds == -1) {
            perror("epoll_wait error");
            return 1;
        } else if (nfds == 0) {
            printf("Timeout, no data\n");
        } else {
            for (int i = 0; i < nfds; i++) {
                if (events[i].events & EPOLLIN) {
                    int n = read(events[i].data.fd, buffer, sizeof(buffer));
                    if (n > 0) {
                        buffer[n] = '\0';
                        printf("Received: %s", buffer);
                    }
                }
            }
        }
    }

    close(epoll_fd);
    return 0;
}


