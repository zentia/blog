---
title: UE4的内存分配
mathjax: true
date: 2021-07-02 10:00:23
categories: 
- Engine
- Unreal
---

起因，看spine，看到了string，想起了stringID，然后看里面直接裸memory，在想FMemory和memory有什么区别，以及适用情况。

FMemory内部是区分平台的，但是我没看到GameCore内部区分平台。
UE4的window平台直接用的是`GenericPlatformMemory`，IOS中间有一层包装，但是最后也是走的是`GenericPlatformMemory`，Android也是，所以其实也还好？

好吧，我们还是从头开始看吧。

<!--more-->

# FMemory

这个第一个接口是`FMemory::GCreateMalloc`这里面主要调的是`FMemory_GCreateMalloc_ThreadUnsafe`，在这里面我们逐步分析。

第一步先取统计接口`FPlatformMemory::GetStats()` ，但是这里有个非MAC的判定，不知道为啥。

然后就是取`GMalloc`这里是通过一些模式和命令行在加上架构去分析的，太偏底层的不太熟悉了，有熟悉的大佬可以在下方留言提醒。

在然后就是拉取Crash助手叫做`FPlatformMallocCrash`它有以下职责：

- 当内存分配低于16字节对齐的时候会触发crash。
- 分配器的线程只能是当前线程。

接着就是判断`GMalloc->IsInternallyThreadSafe()`里面针对不同的malloc做一些判定是否是线程安全的，这块不是太懂。

接着就是跑内存分析器，这里有个奇怪的宏是`IS_MONOLIGHTIC`是和mono相关的吗？先不管了。

下一步又判断了以下`GMalloc->IsInternallyThreadSafe()`这个感觉是针对profile的，就是说你如果非要非线程安全的，那我就帮你搞一个代理吧，叫`FMallocThreadSafeProxy`。
反正怎么说呢，我认为哈，所以的proxy都是认为，你把握不住的东西，我帮你搞一层代理，容易把你绕晕的地方你就不用管了，也不要瞎搞了。交给我吧。
然后在所有的内存操作之前都会做一个同步锁。

下一步又有一个校验器，里面又加了一把锁，外面通过宏切换开关，以我浅薄的认识，这两个可能是不会同时开启的，不然两把锁没有意义呀。
这个的作用注释上面写着是过滤无效的指针，原理就是存储了一下指针，做了校验。这个平时开发最好一直开启着。然后暴露出来。

下一步是检查内存泄露的，我猜测和上面的使用方式差不多。
不过里面逻辑还是挺多了，首先分配完内存之后，要先判断当前线程是否开启检查，是否开启捕获alloc，是否申请内存高于阈值，小内存你就不考虑了吗？
然后又是一把锁，果然很多线程呀。然后里面纪录了一件事情，就是记录内存申请的堆栈，这里感觉和AssetService很相似呀。

下一步就是叫啥有害内存分配代理`FMallocPosionProxy`又是代理，看到没，人与人之前的就这么不信任的吗？
这个主要职责就很简单了，因为我们内存基本都是从池子里面取出来的，可能还会残留一些之前的数据，如果你不清理的话又不赋值直接使用的话，可能会引用到一些奇奇怪怪的东西，
我们调试的时候看到空，就知道，哦，他们的忘记赋值了，一看到奇怪的内容，我擦，这是什么意思，怎么会这样呢，害人又害己。

下一步是打印了程序能用到多少内存。

下一步是搞一个看起来像是为了dump搞得，有点没看明白，直接贴代码`GMalloc = FMallocDoubleFreeFinder::OverrideIfEnabled(GMalloc)`

下一个就是分析统计的`GMalloc = FMallocFrameProfiler::OverrideIfEnabled(GMalloc)`

## 分配器

```c++
FMemory::Malloc(SIZE_T Count, uint32 Alignment)
```

使用其实比较简单的，就是根据不同的分配器去分配内存，[知乎的大佬](https://zhuanlan.zhihu.com/p/81481072) 举了一个`MallocTBB`的例子。

他大概是说分配超出了就回调`GetOutOfMemoryDelegate`，然后这里字节对齐是8字节的。超过就16字节，这里小伙伴可能会疑惑，你有个`FPlatformMallocCrash`上面不是说16字节的吗？是的，
但是其实这些是针对平台的，我看的那个实例其实是`FGenericPlatformMallocCrash`，聪明的小伙伴一看这个字面量就知道什么意思了，我这个是针对通用平台的逻辑，而他这个分配器应该是特殊平台的，
所以并不冲突。


好了，现在我们回到我们的问题，两个有什么区分，GameCore由于时间问题和一些其它原因，我就不便介绍了，基本可以按照自研项目那一套方式，底层采用操作系统接口，加一下统计分析逻辑，代码的兼容性和健全性肯定是没法和UE4的比了。


好吧，就这样了。时间原因我没法做太多的分析。

水一篇文章吧。