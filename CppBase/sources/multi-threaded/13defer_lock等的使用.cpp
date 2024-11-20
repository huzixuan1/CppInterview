// 用这个defer_lock的前提是 你不能自己先lock，否则会报异常。
// defer_lock的意思是并没有给mutex加锁：初始化了一个没有加锁的mutex。

// 一起验证defer_lock的效果，修改成员函数如下：


void inMsgRecvQueue()
{
  for (int i = 0; i < 100000; ++i)
  {
    std::unique_lock<std::mutex> sbguard1(my_mutex1,std::defer_lock); // 没有加锁的my_mutex1
    sbguard1.lock(); // 不用自己unlock()

    cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;

    msgRecvQueue.push_back(i);
  }
  return;
}

// 和lock()搭配使用，避免一整个函数执行完才unlock，即可以自行决定什么时候就可以unlock().
// 保证了一个目的——用lock锁住的代码段越少，执行越快，整个程序运行效率越高。
// 有人也把锁头锁住的代码多少称为锁的粒度。粒度过粗过细都不行！

//   尝试给互斥量加锁，如果拿不到锁，则返回false，如果拿到了锁，返回true，这个函数是不阻塞的。
//   用法和try_to_lock参数类似。
//   注意：try_lock要和defer_lock配合使用。

// 1.4 release()
//  返回它所管理的mutex对象指针，并释放所有权，也就是说，这个unique_lock和mutex不再有关系。
//  严格区分unlock()和release()！
//  如果原来mutex对象处于加锁状态，你有责任接管过来并负责解锁。


void inMsgRecvQueue()
{
  for (int i = 0; i < 100000; ++i)
  {
    std::unique_lock<std::mutex> sbguard1(my_mutex1);
    std::mutex *ptx = sbguard1.release(); // 关系解除
    cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
    msgRecvQueue.push_back(i);
    ptx->unlock(); // 接管
    // 运行后正常。接管过来后，release()返回的是原始mutex的指针。
  }
  return;
}



// unique_lock所有权的传递
// unique_lock与mutex配合使用。
// std::unique_lock<std::mutex> sbguard1(my_mutex1);
//  所有权概念——sbguard1拥有my_mutex1的所有权。
//  sbguard1可以把自己对mutex(my_mutex1)的所有权转移给其他的unique_lock对象。
//  所以，unique_lock对象这个mutex的所有权是可以转移 ，不可复制的。（这类似于智能指针unique_ptr）
//  转移：

std::unique_lock<std::mutex> sbguard2(std::move(sbguard1));  // 移动语义
 // 现在sbguard1指向空，sbguard2指向了my_mutex1





