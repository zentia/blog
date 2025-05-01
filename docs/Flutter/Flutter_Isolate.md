---
title: Flutter的Isolate机制
date: 2022-11-12 18:30:00
tags: Flutter
categories:
    - Flutter
---
Flutter是使用Dart语言为基础的，所以它的线程机制使用的也是Dart的线程机制。Dart是单线程，Dart为我们提供了isolate，isolate跟线程差不多，它可以理解为Dart中的线程。isolate与线程的区别就是线程与线程之间的共享内存的，而isolate和isolate之间的是不共享的，所以叫isolate（隔离）。
简单说，可以把他理解为Dart中的线程。但它不同于线程，更恰当的说应该是微线程，或者说线程。它与线程最大的区别就是不能共享内存，因此也不存在锁竞争问题，两个Isolate完全是两条独立的执行线，且每个Isolate都有自己的事件循环，它们之间只能通过发送消息通信，所以它的资源开销低于线程。

# 创建isolate及isolate通信
1. 获取当前main isolate的receivePort及sendPort
2. 使用isolate.spawn创建新的isolate，需要传入新的isolate需要完成的任务名称及创建者（main isolate）的sendPort（用于将新的isolate的sendPort传递给创建者）
3. 在任务方法中，获取新的isolate的ReceivePort及SendPort