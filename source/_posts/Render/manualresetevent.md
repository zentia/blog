---
title: ManualResetEvent
mathjax: true
date: 2019-05-09 19:51:55
tags:
    - CSharp
categories: CSharp
---
ManualResetEvent用于线程间通信，实现一种类似信号量的功能。
```CSharp
public ManualResetEvent(bool initalState);
```
initState为true表示有信号，为false表示无信号
WaitOne起到线程阻塞的作用
Set将ManualResetEvent对象的信号设为有信号状态，这个时候WaitOne如果正在阻塞中的话，将会立即终止阻塞，向下继续执行。而且这个状态一直不变的话，每次执行到WaitOne都将五任何阻塞。
Reset将ManualResetEvent对象的信号设置为无信号状态，当下次执行到WaitOne时，又将重新开始阻塞。