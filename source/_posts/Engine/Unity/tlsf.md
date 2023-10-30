---
title: TLSF 动态内存分配算法
categories:
- Engine
- Unity
---
```C++
// 这里是入口函数
// align对齐方式，16字节
// size分配大小
void* tlsf_memalign(tlsf_pool tlsf, size_t align, size_t size)
{
    pool_t* pool = tlsf_cast(pool_t*, tlsf);//强制类型转换
    // ALIGN_SIZE 是8字节对齐
    const size_t adjust = adjust_request_size(size, ALIGN_SIZE);
}
```
