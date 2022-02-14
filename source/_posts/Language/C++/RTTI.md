---
title: C++对象模型之RTTI的实现原理
date: 2022-01-26 11:28:00
categories:
- Language
- C++
---
https://blog.csdn.net/ljianhui/article/details/46487951

RTTI是Runtime Type Identification的缩写，意思是运行时类型识别。C++引入这个机制是为了让程序在运行时能根据基类的指针或引用来获得该指针或引用所指向的对象的实际类型。但是现在RTTI的类型识别已经不限于此了，它还能通过typeid操作符识别出所有的基本类型（int，指针等）的变量对应的类型。

C++通过以下的两个运算符提供RTTI：
1. typeid运算符，该运算符将基类的指针或引用安全地转换为派生类地指针或引用。
2. dynamic_cast运算符，该运算符将基类的指针或引用安全地转换为派生类类型的指针或引用。

下面分别详细的说明这两个操作的实现方式。

# typeid运算符
typeid运算符，后接一个类型名或一个表达式，该运算符返回一个类型为std::tpeinf的对象的const引用。type_info是std中的一个类，它用于记录与类型相关的信息。类type_info的定义大概如下：

# type_info
type_info类在头文件`<typeinfo>`中定义，代表了一个C++类型的相关信息。一般由typeid操作符返回，不能自己构造。
`type_info`是实现相关的，不同编译工具链的实现可能不一致。
