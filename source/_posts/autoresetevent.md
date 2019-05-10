---
title: AutoResetEvent
mathjax: true
date: 2019-05-09 20:06:11
tags:
    - CSharp
categories: CSharp
---
AutoResetEvent允许线程通过信号互相通信，通常，此通信涉及线程需要独占访问的资源。
# AutoResetEvent与ManualResetEvnet的区别
他们的用法声明都很类似，Set方法将信号置为发送状态，Reset方法将信号设置为不发送状态，WaitOne等待信号的发送。其实，从名字就可以看出一个手动，一个自动，这个手动和自动实际指的是在Reset方法的处理上，如下面的例子：
```CSharp
public AutoResetEvent autoEvent = new AutoResetEvent(true);
public ManualResetEvent manualEvent = new ManualResetEvent(true);
```
默认信号都处于发送状态
autoEvent.WaitOne();
manualEvent.WaitOne();
如果某个线程调用上面方法，则当信号处于发送状态时，该线程会得到信号，得以继续执行。差别就在调用后，autoEvent.WaitOne()每次只允许一个线程进入，当某个线程得到信号（也就是说有其他线程调用了autoEvnet.Set()方法后），autoEvent会自动又将信号设置为不发送状态，则其他调用WaitOne的线程只有继续等待，也就是说，autoEvent一次只唤醒一个线程。而manualEvent则可以唤醒多个线程，因为当某个线程调用了set方法后，其他调用waitone的线程获得信号得以继续执行，而manualevent不会自动将信号置为不发送，也就是说，除非手动调用了manualEvent.Reset()方法，否则manualEvent将一直保持有信号状态，manualEvent也就可以同时唤醒多个线程继续执行。如果上面的程序换成ManualResetEvent的话，就需要在waitone后面做下reset。