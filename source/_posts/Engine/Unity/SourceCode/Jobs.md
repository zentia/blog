---
title: Unity JobSystem
categores:
- Engine
- Unity
- SourceCode
---
Unity有两种多线程实现方式，一种是直接使用Thread，另一种是JobSystem。

JobFunc

JobInfo

JobGroup

```C++
class JobQueue
{
    static AtomicStack* g_GroupPool
    static AtomicStack* g_JobPool;
}
```
JobManager 

JobSystem 全局生产器，不用管

QueueJobTypes 记录所有需要使用Job的类型

# 如何使用
需要继承`IQueueJob`
```C++
class IQueueJob
{
    // Serialized properties
    Hash128         m_Hash;
}
```
实现接口`GetJobType()`、`Execute(JobInfo& info)`
`Execute`是通过`InvokeExecuteJob`调用
数据不知道是否可以序列化结构，先不使用。
还需要再`QueueJobTypes.cpp`中定义
`sNumConcurrentJobsPerTypeMin`表示并行最小数量
`sNumConcurrentJobsPerTypeLow`

