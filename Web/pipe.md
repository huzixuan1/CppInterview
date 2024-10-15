## 进程间通信--管道

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

