---
title: C++并发编程（二）：Mutex（互斥锁）
date: 2022-01-13 18:00:00
categories:
- C++
- Thread
---
https://segmentfault.com/a/1190000006614695
多个线程访问同一资源，为了保证数据的一致性，最简单的方式就是使用mutex（互斥锁）。

引用 cppreference的介绍：
> The mutex class is a synchronization primitive that can be used to protect shared data from being simulataneously 