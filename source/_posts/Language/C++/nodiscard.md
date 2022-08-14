---
title: nodiscard
categories:
 - C++
tags:
 - C++
---
nodiscard是C++17引入的一种标记符，其语法一般为`[[nodiscard]]`或`[[nodiscard("string")]]`(C++20引入)，含义可以理解为“不应舍弃”。nodiscard一般用于标记函数的返回值或某个类，当使用某个弃值`表达式`而不是cast to void 来调用相关函数时，编译器会发出相关warning。