// 练习题 1: 基于 TCP 的简单聊天服务器
// 目标：
// 实现一个支持多个客户端连接的简单聊天服务器，要求使用 select 实现 I/O 复用，服务器可以广播消息给所有客户端。

// 要求：

// 服务器端：使用 select 监听多个客户端连接。
// 客户端：每个客户端可以发送消息给服务器，服务器将消息广播给其他所有客户端。
// 支持多个客户端并发连接。
// 练习题 2: 使用 epoll 的多客户端并发服务器
// 目标：
// 实现一个使用 epoll 的高效并发服务器，支持多个客户端的同时连接，并能正确处理客户端的消息。

// 要求：

// 服务器使用 epoll 进行 I/O 复用，提高性能。
// 服务器能够处理多个客户端连接，并根据客户端的输入做出响应。
// 客户端可以发送简单的指令，如 "ping"，服务器返回 "pong"。

// 练习题 4: 使用 poll 的服务器程序
// 目标：
// 用 poll 实现一个多客户端的服务器，服务器需要响应不同客户端的请求。

// 要求：

// 使用 poll 监听多个客户端连接。
// 服务器可以处理简单的请求，并根据客户端输入发送不同的响应。
// 实现适当的错误处理与异常连接管理。
// 练习题 5: 使用消息队列实现进程间通信
// 目标：
// 使用 POSIX 消息队列或 System V 消息队列实现两个进程之间的通信。

// 要求：

// 进程 A 发送消息到消息队列，进程 B 从消息队列读取消息。
// 实现简单的消息传递，确保多条消息能够有序传递。
// 练习题 6: 使用 socketpair 和多进程并发处理
// 目标：
// 使用 socketpair 在父进程和子进程之间进行通信。

// 要求：

// 父进程通过 fork 创建多个子进程，每个子进程处理不同的任务。
// 使用 socketpair 进行父子进程之间的通信。
// 实现多个并发任务的处理。



// 练习题 3: 进程间通信 (IPC) 的管道通信
// 目标：使用管道 (pipe) 实现父子进程之间的通信。
// 要求：
//     父进程创建一个子进程，子进程通过 pipe 从父进程接收消息并进行处理，处理结果再通过 pipe 发送回父进程。
//     父进程发送多条消息，并接收处理后的结果。

// #include <cstring>  // for strlen
// #include <sys/wait.h>       // for wait
// #include <unistd.h> // for pipe fork
// #include <iostream>

// using namespace std;

// int parentToChild()
// {
//     // 参数声明
//     int pipe1[2];   // 父进程到子进程的管道
//     int pipe2[2];   // 子进程到父子进程的管道

//     if(pipe(pipe1) == -1 || pipe(pipe2) == -1)
//     {
//         std::cout<<"pipe creation failed."<<std::endl;
//         return 1;
//     }

//     pid_t pid = fork();

//     if (pid < 0) {
//         std::cerr << "Fork failed." << std::endl;
//         return 1;
//     }

//     if(pid > 0)     // 父进程
//     {
//         close(pipe1[0]);    // 关闭pipe1的读
//         close(pipe2[1]);    // 关闭pipe2的写

//         const char* messages[] = {"Message1","Message2","Message3"};
//         char buffer[100];
//         for(const char* msg:messages)
//         {
//             // 父进程发送消息
//             write(pipe1[1],msg,strlen(msg)+1);      // 包括终止符
//             std::cout<<"Parent:sent message to child:"<<msg<<std::endl;

//             // 父进程接收子进程处理的结果
//             read(pipe2[0],buffer,sizeof(buffer));
//             std::cout<<"Parent:received processed message:"<<buffer<<std::endl;
//         }   // for

//         close(pipe1[1]);    // 关闭pipe1的写
//         close(pipe2[0]);    // 关闭pipe2的读

//         wait(nullptr);      // 等待子进程结束

//     }else{      // 子进程
//         close(pipe1[1]);    // 关闭pipe1的写端
//         close(pipe2[0]);    // 关闭pipe2的读端

//         char buffer[100];
//         while(read(pipe1[0],buffer,sizeof(buffer))>0)
//         {
//             std::cout<<"child:received message from parent:"<<buffer<<std::endl;
//             // 处理消息:将其转换成大写
//             for(int i = 0; buffer[i]!='\0';i++)
//             {
//                 buffer[i] = toupper(buffer[i]);
//             }   // for

//             // 将处理结果发送给父进程
//             write(pipe2[1],buffer,strlen(buffer)+1);

//         }   // while

//         close(pipe1[0]);
//         close(pipe2[1]);

//     }

//     return 0;

// }


// int main()
// {

//     parentToChild();

//     return 0;
// }


// 练习题 4: 使用 poll 的服务器程序
// 目标：
// 用 poll 实现一个多客户端的服务器，服务器需要响应不同客户端的请求。

// #include <iostream>
// #include <cstring>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <poll.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <vector>

// #define PORT 8080
// #define MAX_CLIENTS 10
// #define BUFFER_SIZE 1024

// int main()
// {
//     // 变量声明
//     int server_fd,new_socket,valread;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
//     char buffer[BUFFER_SIZE] = {0};

//     // 创建监听socket
//     if((server_fd = socket(AF_INET,SOCK_STREAM,0)) ==0)
//     {
//         std::cout<<"socket failed!"<<std::endl;
//         exit(EXIT_FAILURE);
//     }

//     // 允许复用地址和端口
//     int opt = 1;
//     if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEADDR,&opt, sizeof(opt)))
//     {
//         std::cout<<"setsockopt"<<std::endl;
//         close(server_fd);
//         exit(EXIT_FAILURE);
//     }

//     // 设置服务器地址和端口
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);

//     // 绑定socket到服务器地址
//     if(bind(server_fd,(struct sockaddr *)&address,sizeof(address))<0)
//     {
//         std::cout<<"bind failed."<<std::endl;
//         close(server_fd);
//         exit(EXIT_FAILURE);
//     }

//     // 开始监听
//     if(listen(server_fd,3)<0)
//     {
//         std::cout<<"listen"<<std::endl;
//         close(server_fd);
//         exit(EXIT_FAILURE);
//     }


//     // poll的操作
//     std::vector<pollfd> fds;

//     // 将监听socket添加到poll监视列表中
//     pollfd server_poll_fd = {server_fd,POLLIN,0};
//     fds.push_back(server_poll_fd);
//     std::cout<<"Server is listening on port:"<<PORT<<std::endl;

//     while(true)
//     {
//         // 使用poll监视所有socket
//         int poll_count = poll(fds.data(),fds.size(),-1);
//         if(poll_count < 0)
//         {
//             std::cout<<"poll error"<<std::endl;
//             break;
//         }
//         // 遍历所有的文件描述符
//         for(size_t i =0;i<fds.size();i++)
//         {
//             if(fds[i].revents & POLLIN)
//             {
//                 if((new_socket = accept(server_fd,(struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
//                 {
//                     std::cout<<"accept."<<std::endl;
//                     continue;
//                 }
//                 // 新客户端链接加入poll监视列表
//                 pollfd chlient_poll_fd = {new_socket,POLLIN,0};
//                 fds.push_back(chlient_poll_fd);
//                 std::cout<<"New client connected"<<std::endl;
//             }else{
//                 // 处理客户端请求
//                 memset(buffer,0,BUFFER_SIZE);
//                 valread = read(fds[i].fd,buffer,BUFFER_SIZE);
//                 if(valread <= 0){
//                     // 客户端断开连接
//                     std::cout<<"client disconnected"<<std::endl;
//                     close(fds[i].fd);
//                     fds.erase(fds.begin()+1);
//                     --i;
//                 }else{
//                     // 输出客户端消息
//                     std::cout<<"Received: "<<buffer<<std::endl;
//                     // 回复客户端
//                     send(fds[i].fd,buffer,strlen(buffer),0);
//                 }
//             }
//         }   // for
//     }   // while

//     // 关闭服务器
//     close(server_fd);

//     return 0;
// }

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
