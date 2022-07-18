---
title: Unity 内存分配
categories:
- Engine
- Unity
- SourceCode
---
# 临时的分配器 Temporay Allocations
内存的生命周期如果低于一帧的话必须使用临时分配器。临时分配器很快并且会使用上一个活跃的分配器。不会造成丝毫浪费。反正就是你得用临时分配器，不然会有碎片，性能也不友好。
下面是一个例子每个线程使用临时的堆分配器和先进后出的内存管理方式
```C++
// use kMemTempAlloc allocator label to get the fast stackbased temporary allocator
allocator
{
    dynamic_array<UInt8> myTempArray (kMemTempAlloc);
}

// or
{
    // Allocate a floatArray of 12 floats automatically deallocates when leaving the {} scope
    float* floatArray;
    ALLOC_TEMP(floatArray, float, 12);
}
```
为了使内存块可以正常的在jobs读写访问，

# MemoryManager 内存管理器
```C++

```