## 进程间通信--管道

pipe() 函数是用于进程间通信（IPC）的一个系统调用，它创建一个匿名管道（pipe），可以在父进程与子进程之间传输数据。通过管道，进程可以通过写入数据到管道的一端，并从另一端读取数据来实现通信。

pipe() 函数简介
函数定义：

int pipe(int pipefd[2]);
参数：
pipefd[2]：这是一个长度为2的整型数组。pipefd[0] 是管道的读取端，pipefd[1] 是管道的写入端。
返回值：
成功时返回 0。
失败时返回 -1 并设置 errno 以指示错误。
管道的工作原理
管道是一个内核缓冲区，它单向工作，即数据只能从写入端流向读取端。其基本原理是：

写入端：进程可以通过 pipefd[1] 向管道写入数据。
读取端：另一个进程可以通过 pipefd[0] 从管道读取数据。
匿名管道的局限性：通常只能在有亲缘关系的进程之间使用（如父进程与子进程）。
pipe() 的典型使用场景：父子进程通信
管道通常用于父进程和子进程之间的通信。父进程创建一个管道并 fork 出子进程，父子进程分别关闭管道的一端，使用另一端进行数据传递。


管道的特点
单向通信：管道是单向的，数据只能从写端流向读端。如果需要双向通信，通常需要创建两个管道。

只能用于亲缘进程间：匿名管道只能用于具有亲缘关系的进程之间，如父子进程或兄弟进程。若需要在非亲缘关系的进程之间通信，可以使用命名管道（FIFO）或其他更复杂的IPC机制。
匿名管道（PIPE）是一种特殊的文件，但虽然它是一种文件，却没有名字， 因此一般进程无法使用open()来获取他的描述符，它只能在一个进程中被创建出来， 然后通过继承的方式将他的文件描述符传递给子进程，这就是为什么匿名管道只能用于亲缘关系进程间通信的原因。

阻塞读取/写入：默认情况下，管道的 read() 和 write() 调用是阻塞的。即：

如果读端没有数据可读，read() 会阻塞，直到写端有数据写入。
如果写端写入数据时读端没有准备好接收数据，并且内核缓冲区已满，write() 也会阻塞。
使用管道实现进程间通信的步骤
创建管道：使用 pipe() 创建管道，得到读端和写端的文件描述符。
创建进程：使用 fork() 创建子进程，父进程和子进程共享文件描述符。
关闭不必要的端：子进程关闭写端，父进程关闭读端，以保证单向通信。
传递数据：父进程写数据到写端，子进程从读端读取数据。
关闭管道：传递完数据后，双方都要关闭管道的对应端口。
总结
pipe() 是一个简单而常用的进程间通信（IPC）机制，适用于父子进程或兄弟进程之间的单向数据传输。
它通过在内核中维护一个缓冲区，使数据从一个进程传递到另一个进程。
典型用途：父子进程之间的数据传输，例如在创建子进程后，父进程向子进程传递初始化数据，或者子进程返回处理结果给父进程。


```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2]; // pipefd[0] 是读端，pipefd[1] 是写端
    pid_t pid;
    char buffer[1024];

    // 创建管道
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // 创建子进程
    pid = fork();
    
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // 子进程，关闭写端，使用读端读取父进程传递的数据
        close(pipefd[1]); // 关闭写端

        // 从管道读取数据
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child process received: %s\n", buffer);

        close(pipefd[0]); // 关闭读端
    } else {
        // 父进程，关闭读端，使用写端向子进程传递数据
        close(pipefd[0]); // 关闭读端

        // 向管道写入数据
        char *message = "Hello from parent process!";
        write(pipefd[1], message, strlen(message) + 1); // +1 包含字符串结束符

        close(pipefd[1]); // 关闭写端
    }

    return 0;
}



### socket 进程间通信

···cpp
/**
 * introduce:
 *  每个子进程都有一对 socketpair，也就是说，
 *  创建每个子进程时，会为它分配两个文件描述符。
 *  这两个文件描述符分别对应于同一个 socketpair 的两个端口：
 *      socket_fd[i * 2] 是父进程用来与子进程通信的一个端口
        socket_fd[i * 2 + 1] 是子进程用来与父进程通信的另一个端口
 * */ 


#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

const int NUM_CHILDREN = 3;  // 子进程数量

// 子进程函数(子进程和父进程方向的通信)
void childProcess(int socket_fd, int child_num) {
    
    // 子进程的任务
    std::string message = "Hello from child " + std::to_string(child_num) + "!";
    if (write(socket_fd, message.c_str(), message.size() + 1) == -1) {
        std::cerr << "Child " << child_num << " failed to send message." << std::endl;
        exit(1);
    }

    // 读取父进程的消息
    char buffer[128];
    int n = read(socket_fd, buffer, sizeof(buffer));
    if (n > 0) {
        std::cout << "Child " << child_num << " received from parent: " << buffer << std::endl;
    }
    
    close(socket_fd);  // 关闭子进程的 socket
    exit(0);
}

// 父进程函数
void parentProcess(int socket_fd[], int num_children) {
    char buffer[128];
    
    for (int i = 0; i < num_children; ++i) {
        
        // 读取子进程的消息
        int n = read(socket_fd[i * 2], buffer, sizeof(buffer));
        if (n > 0) {
            std::cout << "Parent received from child " << i + 1 << ": " << buffer << std::endl;
        }
        
        // 向子进程发送消息
        std::string reply = "Message from parent to child " + std::to_string(i + 1);
        if (write(socket_fd[i * 2], reply.c_str(), reply.size() + 1) == -1) {
            std::cerr << "Parent failed to send message to child " << i + 1 << "." << std::endl;
        }
        
        close(socket_fd[i * 2]);  // 关闭父进程端的 socket
    }

    // 等待所有子进程结束
    for (int i = 0; i < num_children; ++i) {
        wait(nullptr);
    }
}

int main() {
    int socket_fd[NUM_CHILDREN * 2];  // 每个子进程有一对 socketpair

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        // 创建 socketpair
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, &socket_fd[i * 2]) == -1) {
            std::cerr << "Failed to create socket pair" << std::endl;
            return 1;
        }

        pid_t pid = fork();  // 创建子进程

        if (pid == -1) {
            std::cerr << "Failed to fork" << std::endl;
            return 1;
        } else if (pid == 0) {
            // 子进程逻辑
            childProcess(socket_fd[i * 2 + 1], i + 1);  // 传递子进程的 socket
        }
    }

    // 父进程逻辑
    parentProcess(socket_fd, NUM_CHILDREN);
    
    return 0;
}
