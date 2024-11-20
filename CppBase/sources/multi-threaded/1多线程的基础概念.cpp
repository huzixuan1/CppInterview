

/**
 * 多线程编程001
 * 
 * 线程并不是创建的越多越好：
 *  线程之间的切换需要保存很多的中间状态,同时切换会耗费本该属于程序运行的时间
 *  
 * 总结：
 *  1、线程是用来执行代码的
 *  2、把线程这个东西理解为一条代码的执行通路，一个新线程代表一条新的通路
 *  3、一个进程自动包含一个主线程，主线程随着进程的创建而创建并启动运行，
 *      我们可以通过编码来创建多个其他的线程(非主线程)，但是创建的数量不建议超过
 *      200-300个，至于创建多少个合适，需要在实际的项目中不断地进行调整和优化，
 *  4、因为主线程是自动启动的，所以一个进程中至少有一个线程
 *  5、多线程程序可以同时干多个事情，所以它的运行效率高。但是到底有多高是一个
 *      不太容易的进行评估和量化的东西。需要在实际的项目开发中进行调整和体会
 *  心得：
 *      开发多线程：实力的体现，一个商用的必要需求
 *      线程开发有难度，不急不躁
*/


/**
 * 并发的实现方法：
 *  实现并发的手段：
 *  1、通过多个进程实现并发
 *  2、在单独的进程中创建多个线程来实现并发
 *
*/


/**
 * 多进程并发：
 *  进程之前的通信方式：
 *      (同一个电脑上)管道、文件、共享内存、消息对列
 *      (不同电脑上)套接字
*/


/**
 *  多线程并发:单个进程中，创建一个多线程
 *  线程：感觉像轻量级的进程，每个线程都有自己独立的运行路径，但是
 *      一个进程中的所有线程都是共享内存的(共享地址空间)
 *      全局变量、指针、引用都可以在线程之间进行传递，所以使用多线程开销远远小于多进程
 *    
 *  共享内存带来的一个问题：
 *      数据一致性问题        
 * 
*/

/**
 * 多线程和多进程并发虽然可以混合使用，
 * 但是建议优先考虑多线程的技术手段
 * 
*/


/**
 * 总结：
 *  和进程比，线程有如下优点：
 *      1、线程启动速度更快，更轻量级；
 *      2、系统资源开销更小，执行速度更快，比如内存共享这种通信方式比
 *          任何其他的通信方式都快
 *      但是在使用起来有一点过难度，要小心处理数据不一致的问题
*/

