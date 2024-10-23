
// 练习题 3: 进程间通信 (IPC) 的管道通信
// 目标：使用管道 (pipe) 实现父子进程之间的通信。
// 要求：
//     父进程创建一个子进程，子进程通过 pipe 从父进程接收消息并进行处理，处理结果再通过 pipe 发送回父进程。
//     父进程发送多条消息，并接收处理后的结果。

#include <cstring>  // for strlen
#include <sys/wait.h>       // for wait
#include <unistd.h> // for pipe fork
#include <iostream>

using namespace std;

int parentToChild()
{
    // 参数声明
    int pipe1[2];   // 父进程到子进程的管道
    int pipe2[2];   // 子进程到父子进程的管道

    if(pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        std::cout<<"pipe creation failed."<<std::endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed." << std::endl;
        return 1;
    }

    if(pid > 0)     // 父进程
    {
        close(pipe1[0]);    // 关闭pipe1的读
        close(pipe2[1]);    // 关闭pipe2的写

        const char* messages[] = {"Message1","Message2","Message3"};
        char buffer[100];
        for(const char* msg:messages)
        {
            // 父进程发送消息
            write(pipe1[1],msg,strlen(msg)+1);      // 包括终止符
            std::cout<<"Parent:sent message to child:"<<msg<<std::endl;

            // 父进程接收子进程处理的结果
            read(pipe2[0],buffer,sizeof(buffer));
            std::cout<<"Parent:received processed message:"<<buffer<<std::endl;
        }   // for

        close(pipe1[1]);    // 关闭pipe1的写
        close(pipe2[0]);    // 关闭pipe2的读

        wait(nullptr);      // 等待子进程结束

    }else{      // 子进程
        close(pipe1[1]);    // 关闭pipe1的写端
        close(pipe2[0]);    // 关闭pipe2的读端

        char buffer[100];
        while(read(pipe1[0],buffer,sizeof(buffer))>0)
        {
            std::cout<<"child:received message from parent:"<<buffer<<std::endl;
            // 处理消息:将其转换成大写
            for(int i = 0; buffer[i]!='\0';i++)
            {
                buffer[i] = toupper(buffer[i]);
            }   // for

            // 将处理结果发送给父进程
            write(pipe2[1],buffer,strlen(buffer)+1);

        }   // while

        close(pipe1[0]);
        close(pipe2[1]);

    }

    return 0;

}


int main()
{

    parentToChild();

    return 0;
}


// 练习题 4: 使用 poll 的服务器程序
// 目标：
// 用 poll 实现一个多客户端的服务器，服务器需要响应不同客户端的请求。

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main()
{
    // 变量声明
    int server_fd,new_socket,valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 创建监听socket
    if((server_fd = socket(AF_INET,SOCK_STREAM,0)) ==0)
    {
        std::cout<<"socket failed!"<<std::endl;
        exit(EXIT_FAILURE);
    }

    // 允许复用地址和端口
    int opt = 1;
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEADDR,&opt, sizeof(opt)))
    {
        std::cout<<"setsockopt"<<std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定socket到服务器地址
    if(bind(server_fd,(struct sockaddr *)&address,sizeof(address))<0)
    {
        std::cout<<"bind failed."<<std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 开始监听
    if(listen(server_fd,3)<0)
    {
        std::cout<<"listen"<<std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }


    // poll的操作
    std::vector<pollfd> fds;

    // 将监听socket添加到poll监视列表中
    pollfd server_poll_fd = {server_fd,POLLIN,0};
    fds.push_back(server_poll_fd);
    std::cout<<"Server is listening on port:"<<PORT<<std::endl;

    while(true)
    {
        // 使用poll监视所有socket
        int poll_count = poll(fds.data(),fds.size(),-1);
        if(poll_count < 0)
        {
            std::cout<<"poll error"<<std::endl;
            break;
        }
        // 遍历所有的文件描述符
        for(size_t i =0;i<fds.size();i++)
        {
            if(fds[i].revents & POLLIN)
            {
                if((new_socket = accept(server_fd,(struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
                {
                    std::cout<<"accept."<<std::endl;
                    continue;
                }
                // 新客户端链接加入poll监视列表
                pollfd chlient_poll_fd = {new_socket,POLLIN,0};
                fds.push_back(chlient_poll_fd);
                std::cout<<"New client connected"<<std::endl;
            }else{
                // 处理客户端请求
                memset(buffer,0,BUFFER_SIZE);
                valread = read(fds[i].fd,buffer,BUFFER_SIZE);
                if(valread <= 0){
                    // 客户端断开连接
                    std::cout<<"client disconnected"<<std::endl;
                    close(fds[i].fd);
                    fds.erase(fds.begin()+1);
                    --i;
                }else{
                    // 输出客户端消息
                    std::cout<<"Received: "<<buffer<<std::endl;
                    // 回复客户端
                    send(fds[i].fd,buffer,strlen(buffer),0);
                }
            }
        }   // for
    }   // while

    // 关闭服务器
    close(server_fd);

    return 0;
}

// 客户端
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define SERVER_IP "127.0.0.1"   // 设置服务器ip
#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{

    int sock = 0,valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {};

    // 创建socket
    if(sock == socket(AF_INET,SOCK_STREAM,0)<0){
        std::cout<<"Socket creation error."<<std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 转换IPV4地址
    if(inet_pton(AF_INET,SERVER_IP,&serv_addr.sin_addr)<=0){
        std::cout<<"Invalid address/address not supported"<<std::endl;
        return -1;
    }

    // 连接服务器
    if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
        std::cout<<"connection failed"<<std::endl;
        return -1;
    }

    std::cout<<"Connection to the server at "<<SERVER_IP<<":"<<PORT<<std::endl;

    while(true)
    {
        std::string message;
        std::cout<<"Enter message to send to the server"<<std::endl;
        std::getline(std::cin,message);

        if(message == "exit"){
            std::cout<<"Exiting client..."<<std::endl;
            break;
        }


        // 发送消息到服务器
        send(sock,message.c_str(),message.length(),0);
        std::cout<<"Message send:"<<message<<std::endl;

        // 连接到服务器的响应
        memset(buffer, 0, BUFFER_SIZE);
        valread = read(sock, buffer, BUFFER_SIZE);
        std::cout << "Server response: " << buffer << std::endl;

    }

   close(sock);

    return 0;
}




// 练习题 1: 基于 TCP 的简单聊天服务器
// 目标：
// 实现一个支持多个客户端连接的简单聊天服务器，要求使用 select 实现 I/O 复用，服务器可以广播消息给所有客户端。

// 要求：

// 服务器端：使用 select 监听多个客户端连接。
// 客户端：每个客户端可以发送消息给服务器，服务器将消息广播给其他所有客户端。
// 支持多个客户端并发连接。


// 服务端
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <vector>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, max_sd, activity;
    int client_socket[MAX_CLIENTS];
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    fd_set readfds;

    // 初始化所有客户端 socket 为 0，表示未连接
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_socket[i] = 0;
    }

    // 创建服务器 socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定 socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    int addrlen = sizeof(address);

    while (true) {
        // 清除 socket 集合
        FD_ZERO(&readfds);

        // 将服务器 socket 加入集合
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // 添加客户端 socket 到集合
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_socket[i];
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        // 使用 select 监听多个 socket
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            std::cerr << "select error" << std::endl;
        }

        // 检查服务器 socket 是否有新连接
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            std::cout << "New connection, socket fd is " << new_socket << ", IP is " 
                      << inet_ntoa(address.sin_addr) << ", port " << ntohs(address.sin_port) << std::endl;

            // 将新 socket 添加到客户端数组
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    std::cout << "Adding to list of sockets as " << i << std::endl;
                    break;
                }
            }
        }

        // 检查客户端是否有数据发送
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_socket[i];

            if (FD_ISSET(sd, &readfds)) {   // 监视客户端的socket中是否有数据
                int valread = read(sd, buffer, BUFFER_SIZE);
                if (valread == 0) {
                    // 客户端断开连接
                    getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
                    std::cout << "Host disconnected, IP " << inet_ntoa(address.sin_addr) 
                              << ", port " << ntohs(address.sin_port) << std::endl;
                    close(sd);
                    client_socket[i] = 0;
                } else {
                    // 广播消息给其他客户端
                    buffer[valread] = '\0';
                    std::cout << "Received message: " << buffer << std::endl;
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (client_socket[j] != 0 && client_socket[j] != sd) {
                            send(client_socket[j], buffer, strlen(buffer), 0);
                        }
                    }
                }
            }
        }
    }

    return 0;
}


// 客户端
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#define PORT 8080
#define BUFFER_SIZE 1024

void receive_messages(int sock) {
    char buffer[BUFFER_SIZE];
    while (true) {
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread > 0) {
            buffer[valread] = '\0';
            std::cout << "Server: " << buffer << std::endl;
        }
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // 创建客户端 socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将 IP 地址转换为二进制格式
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return -1;
    }

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    // 开启线程接收来自服务器的消息
    std::thread receiver(receive_messages, sock);
    receiver.detach();

    // 循环发送消息
    while (true) {
        std::string message;
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        send(sock, message.c_str(), message.length(), 0);
    }

    close(sock);
    return 0;
}


// 练习题 2: 使用 epoll 的多客户端并发服务器
// 目标：
// 实现一个使用 epoll 的高效并发服务器，支持多个客户端的同时连接，并能正确处理客户端的消息。
// 要求：
    // 服务器使用 epoll 进行 I/O 复用，提高性能。
    // 服务器能够处理多个客户端连接，并根据客户端的输入做出响应。
    // 客户端可以发送简单的指令，如 "ping"，服务器返回 "pong"。


// 服务器端

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 256

int main() {
    int server_fd, client_fd, epoll_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    // 创建服务器套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定套接字
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 开始监听
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 创建 epoll 实例
    if ((epoll_fd = epoll_create1(0)) < 0) {
        perror("epoll_create1 failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 将服务器套接字添加到 epoll 实例中
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) < 0) {
        perror("epoll_ctl failed");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    struct epoll_event events[MAX_EVENTS];

    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // 有新的客户端连接
                client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
                if (client_fd < 0) {
                    perror("accept failed");
                    continue;
                }
                std::cout << "New connection: " << client_fd << std::endl;

                // 将新的客户端套接字添加到 epoll 实例中
                event.events = EPOLLIN;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0) {
                    perror("epoll_ctl failed");
                    close(client_fd);
                }
            } else {
                // 处理客户端消息
                char buffer[BUFFER_SIZE] = {0};
                int bytes_read = read(events[i].data.fd, buffer, BUFFER_SIZE);
                if (bytes_read <= 0) {
                    // 客户端关闭连接
                    std::cout << "Client disconnected: " << events[i].data.fd << std::endl;
                    close(events[i].data.fd);
                } else {
                    // 输出接收到的消息
                    std::cout << "Received: " << buffer << " from " << events[i].data.fd << std::endl;

                    // 根据消息返回响应
                    if (strncmp(buffer, "ping", 4) == 0) {
                        const char *response = "pong\n";
                        write(events[i].data.fd, response, strlen(response));
                    }
                }
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}


// 客户端
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 256

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;

    // 创建客户端套接字
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 连接到服务器
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to server. Type 'ping' to get a response.\n";

    while (true) {
        std::string message;
        std::cout << "Enter message (type 'exit' to quit): ";
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        // 发送消息
        send(sock_fd, message.c_str(), message.size(), 0);

        // 接收服务器的响应
        char buffer[BUFFER_SIZE] = {0};
        int bytes_received = recv(sock_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            std::cout << "Server response: " << buffer << std::endl;
        } else {
            std::cerr << "Error receiving response from server." << std::endl;
        }
    }

    close(sock_fd);
    return 0;
}



// 练习题 5: 使用消息队列实现进程间通信
// 目标：
// 使用 POSIX 消息队列或 System V 消息队列实现两个进程之间的通信。
// 要求：
    // 进程 A 发送消息到消息队列，进程 B 从消息队列读取消息。
    // 实现简单的消息传递，确保多条消息能够有序传递。

#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

const int MAX_TEXT = 512; // 消息最大长度

// 消息结构体
struct Message {
    long type;               // 消息类型
    char text[MAX_TEXT];     // 消息正文
};

// 发送消息的进程 A
void processA(int msgid) {
    Message msg;
    msg.type = 1;  // 消息类型设置为 1
    
    while (true) {
        std::string input;
        std::cout << "Process A: Enter message to send: ";
        std::getline(std::cin, input);
        
        strncpy(msg.text, input.c_str(), MAX_TEXT);  // 复制输入消息到消息结构体
        
        // 发送消息到消息队列
        if (msgsnd(msgid, &msg, sizeof(msg.text), 0) == -1) {
            std::cerr << "Failed to send message" << std::endl;
            exit(1);
        }
        
        if (input == "exit") break;  // 输入 "exit" 时退出循环
    }
}

// 接收消息的进程 B
void processB(int msgid) {
    Message msg;
    
    while (true) {
        // 从消息队列中读取消息
        if (msgrcv(msgid, &msg, sizeof(msg.text), 1, 0) == -1) {
            std::cerr << "Failed to receive message" << std::endl;
            exit(1);
        }
        
        std::cout << "Process B received: " << msg.text << std::endl;
        
        if (strcmp(msg.text, "exit") == 0) break;  // 接收到 "exit" 时退出
    }
}

int main() {
    key_t key = ftok("msgqueue", 65); // 生成唯一键值
    int msgid = msgget(key, 0666 | IPC_CREAT); // 创建消息队列
    
    if (msgid == -1) {
        std::cerr << "Failed to create message queue" << std::endl;
        return 1;
    }
    
    pid_t pid = fork();  // 创建子进程
    
    if (pid < 0) {
        std::cerr << "Failed to fork" << std::endl;
        return 1;
    }
    
    if (pid == 0) {
        // 子进程 B
        processB(msgid);
    } else {
        // 父进程 A
        processA(msgid);
        
        // 等待子进程结束
        wait(nullptr);
        
        // 删除消息队列
        msgctl(msgid, IPC_RMID, nullptr);
    }
    
    return 0;
}



// 练习题 6: 使用 socketpair 和多进程并发处理
// 目标：
// 使用 socketpair 在父进程和子进程之间进行通信。

// 要求：
    // 父进程通过 fork 创建多个子进程，每个子进程处理不同的任务。
    // 使用 socketpair 进行父子进程之间的通信。
    // 实现多个并发任务的处理。

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

const int NUM_CHILDREN = 3;  // 子进程数量

// 子进程函数
void childProcess(int socket_fd, int child_num) {
    close(socket_fd);  // 关闭父进程的 socket
    
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
        close(socket_fd[i * 2 + 1]);  // 关闭子进程端的 socket
        
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
