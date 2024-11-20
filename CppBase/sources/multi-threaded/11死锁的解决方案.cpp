// 死锁的解决方案：只要保证这两个互斥量的上锁顺序一致就不会出现问题！
/**
 * std::lock()函数模板：用来处理多个互斥量的时候
    能力：一次锁住两个或两个以上的互斥量（至少两个，多了不限，一个不行）；
    它不存在这种因为在多个线程中，因为所得顺序问题导致死锁的风险问题。
    std::lock()：如果互斥量中有一个没锁柱，它就在那等着，等所有互斥量锁住，他才能往下走。
    那么两个互斥量都锁住，要么两个互斥量都没锁住。如果只锁了一个，另外一个没锁成功，则它立即把已经锁住的解锁。
    修改之前的成员函数：
 * 
*/

void inMsgRecvQueue()
{
  for (int i = 0; i < 100000; ++i)
  {
    cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;

    std::lock(my_mutex1, my_mutex2);        // 用来处理多个互斥量的时候
    msgRecvQueue.push_back(i);

    my_mutex2.unlock();
    my_mutex1.unlock();     // 可以看到还是需要手动的进行解锁
  }
  return;
}


/**
 * 4 std::lock_guard的std::adopt_lock参数
    借助这个参数，就可以让lock_guard在构造函数里不再lock()，
    搭配std::lock()就可以实现不需要自己加lock()和unlock()了。
 *
*/

void inMsgRecvQueue()
{
  for (int i = 0; i < 100000; ++i)
  {
    cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;

    std::lock(my_mutex1, my_mutex2);
    std::lock_guard<std::mutex> sbguard1(my_mutex1, std::adopt_lock); // 不会lock()了,相当于解锁了
    std::lock_guard<std::mutex> sbguard2(my_mutex2, std::adopt_lock);

    msgRecvQueue.push_back(i);
  }
    //    std::adopt_lock是个结构体对象，起一个标记作用——表示这个互斥量已经lock()，
    //   不需要在std::lock_guard<std::mutex>里对对象进行再次lock()了。
    // 总结：std::lock()：一次锁多个互斥量；谨慎使用（建议一个一个锁），工程中慎用。

  return;
}


