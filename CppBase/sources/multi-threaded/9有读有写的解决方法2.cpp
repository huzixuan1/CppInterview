// 为了防止大家忘记unlock()，引入了一个叫
// std::lock_guard()的类模板：你忘记unlock()不要紧，我替你unlock()。


// std::lock_guard类模板：直接取代lock()和unlock()
//  用了lock_guard()之后，就不能再使用lock()和unlock()。
//  修改了一个成员函数作为示例：

bool outMsgLULPro(int& command)
{
  std::lock_guard<std::mutex> sbguard(my_mutex); // 
  //my_mutex.lock();
  if (!msgRecvQueue.empty())
  {
    // 取数据 & 去除这个数据
    int commond = msgRecvQueue.front();
    msgRecvQueue.pop_front();
    //my_mutex.unlock();
    return true;
  }
  //my_mutex.unlock();
  return false;
}

/**
 * 
 *    实际lock_guard的构造函数里执行了mutex::lock()，析构函数里执行了mutex::unlock()。
    但是相比lock()和unlock()，这个lock_guard还是不够灵活，因为他必须一个在所在函数作用域结束的时候才会析构。
    也可以借助大括号，提前结束（析构）lock_guard()。
 * 
 * /