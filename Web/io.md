## I/O复用

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


