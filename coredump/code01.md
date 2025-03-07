# Linux 中 Core Dump 错误码总结

在 Linux 系统中，当程序崩溃时，操作系统会生成 core dump 文件，通常伴随着一个崩溃信号和对应的错误码。以下是 Linux 中常见的 core dump 错误码的总结。

## 1. 常见信号及其对应的错误码

| 信号名称       | 信号编号 | 错误码解释                                              |
|----------------|----------|----------------------------------------------------------|
| **`SIGSEGV`**   | 11       | **Segmentation Fault (段错误)**：程序访问了非法的内存地址，常见于空指针解引用、数组越界等。         |
| **`SIGABRT`**   | 6        | **Abort (程序终止)**：程序调用 `abort()` 或者遇到致命错误时主动中止。常用于程序内部检测失败时。   |
| **`SIGFPE`**    | 8        | **Floating Point Exception (浮点异常)**：程序在进行浮点运算时发生了错误，如除零、浮点溢出等。        |
| **`SIGILL`**    | 4        | **Illegal Instruction (非法指令)**：程序执行了无效的或者无法识别的指令，通常由于程序崩溃或损坏导致。 |
| **`SIGBUS`**    | 10       | **Bus Error (总线错误)**：程序尝试访问未对齐的内存或者访问不可用的物理内存。                     |
| **`SIGKILL`**   | 9        | **Kill (强制终止)**：由操作系统或用户发送的信号，强制终止进程，无法被捕获或忽略。                  |
| **`SIGTERM`**   | 15       | **Termination (终止)**：通常由操作系统或用户请求终止进程，进程可以捕获并进行清理操作。               |
| **`SIGQUIT`**   | 3        | **Quit (退出)**：程序通过接收到该信号并退出，通常会生成核心转储并终止执行。                         |
| **`SIGTSTP`**   | 20       | **Terminal Stop (终端停止)**：程序被发送到后台，停止执行。常由 `Ctrl+Z` 引发。                      |
| **`SIGCHLD`**   | 17       | **Child Status (子进程状态变化)**：子进程的状态发生变化时发送该信号，通常是子进程退出。              |
| **`SIGCONT`**   | 19       | **Continue (继续执行)**：继续执行暂停的进程，常见于后台进程的操作。                                |
| **`SIGINT`**    | 2        | **Interrupt (中断)**：通常由用户发送的 `Ctrl+C` 信号，用于中断程序的正常执行。                     |
| **`SIGSTOP`**   | 19       | **Stop (停止)**：强制暂停进程，通常由操作系统或用户触发。不可捕获或忽略。                          |

## 2. 其他可能导致 Core Dump 的信号

除了上述常见的信号外，还有一些较为特殊的信号，可能也会导致程序崩溃或生成 core dump 文件，常见的有：

- **`SIGXCPU`**（错误码为 24）：表示程序超出了允许的 CPU 时间限制，通常是在资源限制设置下发生。
- **`SIGXFSZ`**（错误码为 25）：表示程序超出了文件大小限制，常见于文件操作时。
- **`SIGSEGV`** 和 **`SIGBUS`** 通常是引起 core dump 的最常见信号，其他信号可能也会在某些情况下导致进程崩溃。

## 3. Core Dump 文件的内容和错误码的联系

- **Core Dump 文件**：当程序收到以上信号时，操作系统会在当前进程的地址空间生成一个核心转储（core dump）。这个文件包含了崩溃时进程的内存映像、寄存器状态、调用栈等信息，是分析程序崩溃原因的重要工具。
  
- **信号和错误码的对应关系**：每个信号的错误码可以帮助开发者快速定位问题。例如，`SIGSEGV` 错误码为 11，通常是由访问非法内存引起的；而 `SIGFPE` 错误码为 8，通常是由浮点错误引起的。

## 4. 设置 Core Dump 文件的生成与大小

在 Linux 中，开发者和系统管理员可以通过 `ulimit` 命令设置程序生成 core dump 文件的条件与大小限制。常见的设置包括：

- **`ulimit -c unlimited`**：允许生成不限大小的 core dump 文件。
- **`ulimit -c <size>`**：限制 core dump 文件的最大大小。

此外，可以通过修改 `/proc/sys/kernel/core_pattern` 来设置 core dump 文件的保存路径和命名方式。

## 5. 总结

Linux 中的 core dump 文件通常伴随着程序崩溃的信号，每个信号都有其对应的错误码。最常见的错误码包括：

- **11**：`SIGSEGV`（Segmentation Fault）
- **6**：`SIGABRT`（Abort）
- **8**：`SIGFPE`（Floating Point Exception）
- **4**：`SIGILL`（Illegal Instruction）
- **10**：`SIGBUS`（Bus Error）
- **9**：`SIGKILL`（Kill）

了解这些信号及其错误码，有助于开发者在调试时定位问题，进而解决程序崩溃的根本原因。

总结来说，core dump 是由信号触发的，而不是信号量。信号量并不涉及程序崩溃时的内存转储，它主要用于进程间的同步和资源控制。
信号量与 Core Dump 的关系
### 信号量（semaphores）和 信号（signals）是不同的概念：
信号量：主要用于 进程同步 和 互斥，确保多个进程之间的协调。它们不是用来处理程序错误或崩溃的。信号量是通过 sem_wait() 和 sem_post() 等系统调用来操作的。
信号：是操作系统用来通知进程发生某些事件的机制，通常是 异步的。进程通过接收信号来响应各种系统事件，例如程序错误、子进程状态变化、终端中断等。



飞书文档：
      https://kxfun8g8gux.feishu.cn/docx/IvledJsTooyxqdxCsAQckW3pnR8

valgrind:
    https://kxfun8g8gux.feishu.cn/wiki/YZwtwVgnuicDIykIk9Jc3x8Nntb



