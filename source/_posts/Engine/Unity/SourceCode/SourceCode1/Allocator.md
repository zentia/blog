---
title: 内存分配
categories:
- Engine
- Unity
- SourceCode
---
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