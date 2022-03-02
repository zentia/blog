---
title: Unity5.6 源码分析
categories:
- Engine
- Unity
- SourceCode
---
模板类`PPtr`里面会记录一个实例ID，如果是debug模式还会记录一个`DEBUGPTR`就是本身地址
这个东西就是包裹了一个实例的对象。记录引用的。
https://cloud.tencent.com/developer/article/1005786
会同时在C#的managed heap和C++ native heap中分配内存。
