# ALLOC_TEMP_AUTO
```C++
#define ALLOC_TEMP_AUTO(PTR_, COUNT_) \
    ALLOC_TEMP_ALIGNED_AUTO(PTR_, COUNT_, GetTypeAlignmentFromPointer(PTR_))
```
业务层使用的内存分配器，这是一个栈分配器，阅后即焚。
# UNITY_MALLOC UNITY_FREE
这是一个堆分配器，自己管理申请和释放
```C++
#define UNITY_MALLOC(label, size)                   malloc_internal(size, kDefaultMemoryAlignment, label, kAllocateOptionNone, __FILE_STRIPPED__, __LINE__)
// 这是一个带对齐方式的堆内存分配器
#define UNITY_MALLOC_ALIGNED(label, size, align)    malloc_internal(size, align, label, kAllocateOptionNone, __FILE_STRIPPED__, __FILE__)
#define UNITY_FREE(label, ptr)                      free_alloc_internal(ptr, label)
```
标签的定义在`AllocatorLabelNames.h`
每个标签有一个内存分配
# DualThreadAllocator
这个是最常用的内存分配器，这个分配器分配的内存是可以跨线程访问的。里面封装了两个分配器。

一个是主线程分配器`m_MainAllocator`，另外一个是线程分配器`m_ThreadAllocator`。这两个分配器的类型都是`DynamicHeapAllocator`，也就是说`DualThreadAllocator`其实是包装器，通过不同线程调用`DynamicHeapAllocator`。

# `DynamicHeapAllocator`
动态堆分配器，这个内部是一个封装了一个`BucketAllocator`无锁的块分配器。这个分配器分配大小和页表对齐，减少跨页访问的消耗。这里面的操作都是原子的。

这个算法和MONO的Boehm GC算法很像，BOEHM低于2K的是小块，大于的是大块，这里是按照4M做了区分。

4M的分配器是`VirtualAllocator`，这个类继承`LowLevelVirtualAllocator`。

这里的分配算法走的是BaseLib库，库兼容两个平台一个是posix，另外一个是Windows，都是直接摸到了操作系统函数接口，这一层的设计也是

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