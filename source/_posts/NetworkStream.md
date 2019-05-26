---
title: NetworkStream
mathjax: true
date: 2019-03-30 16:55:27
tags:
	- CSharp
categories: CSharp
---
作用：如果服务器和客户端之间基于TCP连接的，他们之间能够依靠一个稳定的字节流进行相互传输信息。
命名空间：System.Net.Sockets
注意事项：
1. NetworkSteam只能用在具有TCP/IP协议之中，如果用在UDP中编译不抱错，会有异常
2. NetworkStream是面向连接的
3. 在网络中利用流的形式传递信息
4. 必须借助Socket（也称为流失Socket），或使用一些返回的返回值
5. 用法和普通流方式几乎一模一样，但具有特殊性

TCP/IP（Transmission Control Protocol/Internet Protocol）（传输控制协议/因特网互联协议）

# IPAddress
命名空间:System.Net
```CSharp
IPAddress ip = IPAddress.Parse("192.168.1.1");
```

