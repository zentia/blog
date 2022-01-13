---
title: UE4多线程访问共享数据锁FScopeLock
date: 2022-01-13 17:46:00
comments: true
categories:
- Unreal
- Thread
---
`FScopeLock`类实现的是范围锁定。
`FScopeLock`是处理范围级别锁定的实用工具类。在避免捕获异常并知道给定线程在资源上的锁数是非常有用的

同步线程访问下列数据`FScopeLock ScopeLock (synchobject)`访问数据，在多个线程共享，当ScopeLock超出范围时，其他线程可以访问数据。

举例说明：
1. 先定义一个成员变量


```C++
FCriticalSection Mutex;
```

2. 用大括号将需要同步在代码块宝珠，并将**`FScopeLock ScopeLock(Mutex);`**，这句话放在大括号的第一行：

```
void MyFunction()
{
    ...不需要同步的代码块
    {
        FScopeLock ScopeLock(Mutex);
        ...需要同步的代码块或数据块
    }
    ...不需要同步的代码块
}
```