---
title: 原子操作和无锁编程
date: 2022-01-13 14:30:00
categories:
- Engine
- Unreal
- Thread
---
https://www.cnblogs.com/kekec/p/13750892.html
# 原子操作的Interlocked函数
> `InterlockedAdd`
> `InterlockedCompareExchange(-Pointer)`
> `InterlockedDecrement(-Increment)`
> `InterlockedExchange(-Pointer)`


```C++
// 自增操作
int32 nl = 100;
// nl = nl + 1; rl = nl;
int32 rl = FPlatformAtomics::InterlockedIncrement(&nl); // nl=101 rl = 101

// 自减操作
int32 n2 = 110;
// n2 = n2 - 1; r2 = n2;
int32 r2 = FPlatformAtomics::InterlockedDecrement(&n2); // n2=109 r2 = 109
```

# FCriticalSection(用户模式下的临界区段)
当有线程进入临界区段时，其他线程必须等待。基于原子操作的`Interlocked`函数实现。
优点：效率高（不需要昂贵的用户状态切换到内核态的上下文切换）
缺点：不能用于进程间同步，只能用于进程内各线程间同步

|平台|实现类|
|---|---|
|Windows|typedef FWindowsCriticalSection FCriticalSection;|
|Max Unix Android IOS |typedef FPThreadsCriticalSection FCriticalSection;|
|HoloLens | typedef FHoloLensCriticalSection FCriticalSection;|

# FSystemWideCriticalSection(系统范围内的临界区段)
当有线程进入临界区段时，其他线程必须等待。基于内核对象`Mutex`（互斥体）实现。
优点：可用于系统范围内进程间同步，也可以用于进程内各线程间同步
缺点：效率低（有昂贵的用户态切换到内核态的上下文切换）

|平台|实现类|
|---|---|
|Windows|typedef FWindowsSystemWideCriticalSection FSystemWideCriticalSection;|
|Mac|typedef FMacSystemWideCriticalSection FSystemWideCriticalSection;|
|Unix|typedef FUnixSystemWideCriticalSection FSystemWideCriticalSection;|
|Android IOS HoloLens|// 未实现 \n typedef FSystemWideCriticalSectionNotImplemented FSystemWideCriticalSection;|
