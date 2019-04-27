---
title: Andorid NDK
date: 2018-07-21 18:52:00
tags:
    Android
---

### JNI

JNI的全称是Java Native Interface，Java原生接口。提供别的代码调用的一组函数。首先使用C++写出了一些函数，然后将这些函数在Java类中再声明一次（加上关键字native），这样Java类中的函数和C++中的函数就匹配到一起了，我们使用Java类中的函数，其实就是使用C++中的函数。这个在Java类中声明的函数就是一个JNI。

### NDK

NDK的全称是Native Development Kit，原生开发工具包。

### 交叉编译

交叉编译就是在一个平台上生成另一个平台上的可执行代码。