---
title: Actor模型原理
date: 2018-03-23 10:36:48
tags:
---

# 1.Actor模型

在使用Java进行并发编程时需要特别的关注锁和内存原子性等一系列线程问题，而Actor模型内部的状态由它自己维护即它内部数据只能由它自己修改（通过消息传递来进行状态修改），所以使用Actors模型进行并发编程可以很好避免这些问题，Actor由状态(state),行为(Behavior)和邮箱(mailBox)三部分组成
1.状态(state):Actor中的状态指的是Actor对象的变量信息，状态由Actor自己管理，避免了并发环境下的锁和内存原子性等问题
2.行为(Behavior):行为指的是Actor中计算逻辑，通过Actor接收到消息来改变Actor的状态
3.邮箱(mailBox):邮箱是Actor和Actor之间的通信桥梁，邮箱内部通过FIFO消息队列来存储发送发Actor消息，接收方Actor从邮箱队列中获取消息

Actor的基础就是消息传递

# 2.使用Actor模型的好处

1.事件驱动--Actor之间的通信是异步的，即使Actor在发送消息后也无需阻塞或者等待就能够处理其他事件
2.强隔离性--Actor中的方法不能由外部部署直接调用，所有的一切都通过消息传递进行的，从而避免了Actor之间的数据共享，想要观察到另一个Actor的状态变化只能通过消息传递进行询问
3.位置透明--无论Actor地址实在本地还是在远程机上对于代码来说都是一样的
4.轻量性--Actor是非常轻量的计算单机，单个Actor仅占400多字节，只需少量内存就能达到高并发

# 3.Actor模型原理

以下通过学生于教师之间的邮件通信来理解akka中的Actor模型

## 学生-教师的消息传递

首先只考虑学生单向发送消息给教师（学生--》教师），如下图：
<img src="http://oxcvfpext.bkt.clouddn.com/actor-1.png">
图解：
1.学生创建一个ActorSystem
2.通过ActorSystem创建ActorRef，将QuoteRequest消息发送到ActorRef(教师代理)
3.ActorRef(教师代理)消息传到Dispatcher中
4.Dispather依次的将消息发送到TeacherActor邮箱中
5.Dispather将邮箱推送到一条线程中
6.邮箱取出一条消息并委派给TeacherActor的Receiver方法
