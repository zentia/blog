---
title: Unity dynamic_array
categories:
- Engine
- Unity
---
```C++
// 添加一个元素
void push_back(const T& t)
{

}
// 预分配大小
void reserve(size_t inCapacity)
{

}
```
如果是栈内定义，需要使用`kMemTempAlloc`标明一下。不然会crash。