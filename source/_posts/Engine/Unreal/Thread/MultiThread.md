---
title: MultiThread
date: 2022-01-13 17:46:00
categories:
- Engine
- Unreal
- Thread
---
在UE4里面，我们可以自己继承`FRunnable`接口创建单个线程，也可以直接创建`AsyncTask`来调用线程池里面空闲的线程，还可以通过`TaskGraph`系统来异步完成一些自定义任务。
# FRunnable
我们先从最基本的创建方式谈起，我们创建一个继承自`FRunable`的类，把这个类要执行的任务分发给其它线程去执行。`FRunable`就是一个很简单的类，里面只有5，6个函数接口，为了与真正的线程区分，我这里称`FRunable`为“线程执行体“。
```C++
//Runnable.h
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"

/**
 * Interface for "runnable" objects.
 *
 * A runnable object is an object that is "run" on an 任意 thread. The call usage pattern is
 * Init(), Run(), Exit(). The thread that is going to "run" this object always uses those calling
 * 语义. It does this on the thread that is created so that any thread specific uses (TLS, etc.)
 * are 有用的 in the contexts of those calls. A "runnable" does all initialization in Init().
 *
 * If initialization fails, the thread stops execution and returns an error code. If it succeeds,
 * Run() is called where the real threaded work is done. Upon completion, Exit() is called to allow
 * correct clean up.
 */
class CORE_API FRunnable
{
public:

	/**
	 * Initializes the runnable object.
	 *
	 * This method is called in the context of the thread object that aggregates this, not the
	 * thread that passes this runnable to a new thread.
	 *
	 * @return True if initialization was successful, false otherwise
	 * @see Run, Stop, Exit
	 */
	virtual bool Init()
	{
		return true;
	}

	/**
	 * Runs the runnable object.
	 *
	 * This is where all per object thread work is done. This is only called if the initialization was successful.
	 *
	 * @return The exit code of the runnable object
	 * @see Init, Stop, Exit
	 */
	virtual uint32 Run() = 0;

	/**
	 * Stops the runnable object.
	 *
	 * This is called if a thread is requested to terminate early.
	 * @see Init, Run, Exit
	 */
	virtual void Stop() { }

	/**
	 * Exits the runnable object.
	 *
	 * Called in the context of the aggregating thread to perform any cleanup.
	 * @see Init, Run, Stop
	 */
	virtual void Exit() { }

	/**
	 * Gets single thread interface pointer used for ticking this runnable when multi-threading is disabled.
	 * If the interface is not implemented, this runnable will not be ticked when FPlatformProcess::SupportsMultithreading() is false.
	 *
	 * @return Pointer to the single thread interface or nullptr if not implemented.
	 */
	virtual class FSingleThreadRunnable* GetSingleThreadInterface( )
	{
		return nullptr;
	}

	/** Virtual destructor */
	virtual ~FRunnable() { }
};

```
# 线程同步
UE4对操作系统提供的线程同步相关接口进行了一定的封装。
## Atomics
基本的接口可以在`FPlatformAtomics`找到，针对不同的平台，有不同的实现。
`FScopeLock`类实现的是范围锁定。
`FScopeLock`是处理范围级别锁定的实用工具类。在避免捕获异常并知道给定线程在资源上的锁数是非常有用的

同步线程访问下列数据`FScopeLock ScopeLock (synchobject)`访问数据，在多个线程共享，当ScopeLock超出范围时，其他线程可以访问数据。

举例说明：
1. 先定义一个成员变量


```C++
FCriticalSection Mutex;
```

2. 用大括号将需要同步在代码块，并将**`FScopeLock ScopeLock(Mutex);`**，这句话放在大括号的第一行：

```
void MyFunction()
{
    ...不需要同步的代码块
    {
        FScopeLock ScopeLock(Mutex);
        ...需要同步的代码块或数据块
    }
    ...不需要同步的代码块
}
```
# TaskGraph系统
说完了`FAsyncTask`系统，接下来我们再谈谈更复杂的TaskGraph系统（应该不会有比他更复杂的了）。Task Graph系统是UE4一套抽象的异步任务处理系统，可以创建多个多线程任务，指定各个任务之间的依赖关系，按照该关系来依次处理任务。具体的实现方式网上也有很多案例，这里先给出UE4Wiki的教程链接：
## 从Tick函数谈起
平时调试的时候，我们随便找个Tick断点以下都能看到类似下图这样的函数堆栈。如果你前面的章节都看懂的话，这个堆栈也能大概理解。World在执行Tick的时候，出发了FNamedTaskThread线程去执行任务（FTickFunctionTask），任务FTickFunctionTask具体的工作内容就是执行`ActorComponent`的Tick函数。其实，这个堆栈也说明了所有Actor与Component的Tick都是通过`TaskGraph`系统来执行的（在TG_PrePhysics阶段）。
## `TaskGraph`系统中的任务与线程
既然是Task系统，那么应该能猜到他和前面的AsyncTask系统相似，我们可以创建多个Task任务然后分配给不同的线程去执行。在`TaskGraph`系统里面，任务类也是我们自己创建的，如`FTickFunctionTask`、`FReturnGraphTask`等，里面需要声明`DoTask`函数来标识要执行的任务内容，`GetDesiredThread`函数来表示在哪个线程上面执行，大概的样子如下：
```C++
class FMyTestTask
{
public:
    FMyTestTask() // send in property defaults here
    {}
    static const TCHAR* GetTaskName()
    {
        return TEXT("FMyTestTask");
    }
    FORCEINLINE static TStatId GetStatId()
    {
        RETURN_QUICK_DECLARE_CYCLE_STAT(FMyTestTask, STATGROUP_TaskGraphTasks);
    }
    /** return the thread for this task **/
    static ENamedThreads::Type GetDesiredThread()
    {
        return ENamedThreads::AnyThread;
    }
    /*
    namespace ESubsequentsMode
    {
        enum Type
        {
            // 存在后续任务
            TrackSubsequents,
            // 没有后续任务
            FireAndForget
        };
    }
    */
    static ESubsequentsMode::Type GetSubsequentsMode()
    {
        return ESubsequentsMode::TrackSubsequents;
    }

    void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
    {

    }
}
```
而线程在该系统里面称为`FWorkerThread`，通过全局的单例类`FTaskGraphImplementation`来控制创建和分配任务的，默认情况下会开启5个基本线程，额外线程的数量则有下面的函数`NumberOfWorkerThreadsToSpawn`来决定，`FTaskGraphImplementation`的初始化在`FEngineLoop.PreInit`里面进行。然后如果平台本身不支持多线程，那么其他的工作也会在`GameThread`里面进行。
```C++
FTaskGraphImplementation(int32)
{
	bCreatedHiPriorityThreads = !!ENamedThreads::bHasHighPriorityThreads;
	bCreatedBackgroundPriorityThreads = !!ENamedThreads::bHasBackgroundThreads;

	int32 MaxTaskThreads = MAX_THREADS;
	int32 NumTaskThreads = FPlatformMisc::NumberOfWorkerThreadsToSpawn();

	// if we don't want any performance-based threads, then force the task graph to not create any worker threads, and run in game thread
	if (!FPlatformProcess::SupportsMultithreading())
	{
		// this is the logic that used to be spread over a couple of places, that will make the rest of this function disable a worker thread
		// @todo: it could probably be made simpler/clearer
		// this - 1 tells the below code there is no rendering thread
		MaxTaskThreads = 1;
		NumTaskThreads = 1;
		LastExternalThread = (ENamedThreads::Type)(ENamedThreads::ActualRenderingThread - 1);
		bCreatedHiPriorityThreads = false;
		bCreatedBackgroundPriorityThreads = false;
		ENamedThreads::bHasBackgroundThreads = 0;
		ENamedThreads::bHasHighPriorityThreads = 0;
	}
	else
	{
		LastExternalThread = ENamedThreads::ActualRenderingThread;
	}
		
	NumNamedThreads = LastExternalThread + 1;

	NumTaskThreadSets = 1 + bCreatedHiPriorityThreads + bCreatedBackgroundPriorityThreads;

	// if we don't have enough threads to allow all of the sets asked for, then we can't create what was asked for.
	check(NumTaskThreadSets == 1 || FMath::Min<int32>(NumTaskThreads * NumTaskThreadSets + NumNamedThreads, MAX_THREADS) == NumTaskThreads * NumTaskThreadSets + NumNamedThreads);
	NumThreads = FMath::Max<int32>(FMath::Min<int32>(NumTaskThreads * NumTaskThreadSets + NumNamedThreads, MAX_THREADS), NumNamedThreads + 1);
        .......
}
//GenericPlatformMisc.cpp
int32 FGenericPlatformMisc::NumberOfWorkerThreadsToSpawn()
{
	static int32 MaxGameThreads = 4;
	static int32 MaxThreads = 16;

	int32 NumberOfCores = FPlatformMisc::NumberOfCores();//物理核数，4核8线程的机器返回的是4
	int32 MaxWorkerThreadsWanted = (IsRunningGame() || IsRunningDedicatedServer() || IsRunningClientOnly()) ? MaxGameThreads :MaxThreads;
	// need to spawn at least one worker thread (see FTaskGraphImplementation)
	return FMath::Max(FMath::Min(NumberOfCores - 1, MaxWorkerThreadsWanted), 1);
}

```
前面理解的FWorkerThread虽然可以理解为工作线程，但其实他不是真正的线程。
`FWorkerThread`里面有两个重要成员，一个是`FRunnableThread* RunableThread`，也就是真正的线程。另一个是`FTaskThreadBase* TaskGraphWorker`，即继承自`FRunnable`的线程执行体。`FTaskThreadBase`有两个子类，`FTaskThreadAnyThread`和`FNamedTaskThread`，分别表示非指定名称的任意Task线程执行体和有名字的Task线程执行体。我们平时说的渲染线程、游戏线程就是有名称的Task线程，而那些我们创建后还没有使用到的线程就是非指定名称的任意线程。
在引擎初始化`FTaskGraphImplementation`的时候，我们就会默认构建24个`FWorkerThread`工作线程（这里支持最大的线程数量也就是24），其中里面有5个是默认带名字的线程，`StatThread`、`RHIThread`、`AudioThread`、`GameThread`、`ActualRenderingThread`，还有前面提到的N个非指定名称的任意线程，这个N由CPU核数决定。对于带有名字的线程，他不需要创建新的`Runnable`线程，因为他们会在其他的时机创建，如`StatThread`以及`RenderingThread`会在`FEngineLoop.PreInit`里创建。而那N个非指定名称的任意线程，则需要在一开始就手动创建`Runnable`线程，同时设置其优先级比前面线程的优先级要低。到这里，我们应该可以理解，有名字的线程专门要做他名字对应的事情，非指定名称的任意线程则可以用来处理其他工作，我们在`CreateTask`创建任务时会通过自己写好的函数决定当前任务应该在哪个线程执行。
现在我们可以先回答一下上一节的问题了，`FTickFunctionTask`到底是在哪个线程执行的？答案是游戏主线程，我们可以`FTickFunctionTask`的Desired线程是`Context.Thread`。

## TaskGraph系统中的任务与事件
虽然前面已经比较细致的描述了`TaskGraph`系统的框架，但是一个非常重要的特性我们还没讲到，就是任务依赖的实现原理。怎么理解任务依赖呢？简单来说，就是一个任务的执行可能依赖多个事件对象，这些事件对象都触发之后才会执行这个任务。而这个任务完成之后，又可能触发其他事件，其他事件再进一步触发其他任务。

这里再看一下Task任务的创建代码，分析一下先决依赖事件与后续等待事件都是如何产生的。

```C++
FGraphEventRef Join = TGraphTask<FVictoryTestTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady();
```

`CreateTask`的第一个参数就是该任务依赖事件数组（这里为NULL），如果传入一个事件数组的话，那么当前任务就会通过`SetupPrereqs`函数设置这些依赖事件，并且在所有依赖事件都出发后再将该任务放到任务队列里面分配给线程执行。

当执行CreateTask时，会通过`FGraphEvent::CreateEvent()`构建一个新的后续事件，再通过函数`ConstructAndDispatchWhenReady`返回。这样我们就可以在当前的位置执行
```C++
FTaskGraphInterface::Get().WaitUntilTaskCompletes(Join, ENamedThreads::GameThread_Local);
```
## 其他相关的技术细节
### `FThreadSafeCounter`
通过调用不同平台的原子操作来实现线程安全的技术
```C++
int32 Add(int32 Amount)
{
    return FPlatformAtomics::InterlockedAdd(&Counter, Amount);
}
```
### Task的构造方式
我们看到相比`AsyncTask`，`TaskGraph`的创建可谓是即新奇又复杂，首先要调用静态的`CreateTask`，然后又要通过返回值执行`ConstructAndDispatchWhenReady`。
### FScopeEvent
在上一节讲过，带有`Scope`关键字的基本都是同一个思想，在构造的时候初始化析构的时候执行某些特殊的操作。FScopeEvent作用实在当前作用域内等待触发，如果没有激活该事件，就会一直处理wait中。

## 总结
对于消耗大的，复杂的任务不建议使用`TaskGraph`，一是因为`TaskGraph`如果被分配到游戏线程，就会阻塞整个游戏线程的执行，二是即使你不在那几个有名字的线程上执行，也可能会影响到游戏的其他逻辑。比如物理计算相关的任务就是在非指定名称的线程上执行的（参考`PhysScene_PyhsX.cpp`中的`FPhySXTask`）。这种复杂的任务，建议你自己继承`Runnable`创建线程，或者使用`AsynTask`系统，`AsyncTask`使用的都是新建的线程池里面的线程，一般与游戏`Tick`是无关的。
而对于简单的任务，或者想要比较方便实现线程之间的依赖等待关系，直接扔给`TaskGraph`就可以了。
另外，不要在非`GameThread`线程内执行下面几个操作：
- 不要Spawn/Modify/delete UObject or AActors
- 不要使用定时器 TimerManager
- 不要使用任何绘制接口，例如`DrawDebugLine`

一开始我也不是很理解，所以就在其他线程执行了Spawn操作，然后蹦在了下面的地方。可以看到，`SpawnActor`的时候会执行物理数据的初始化，而这个操作是必须要在主线程里面执行的，我猜其他的位置肯定还有很多类似的宏。至于原因，我像就是我们最前面提到的”游戏不适合利用多线程优化”，游戏GamePlay中各个部分非常依赖顺序，多线程没办法很好的处理这些关系。再者，游戏逻辑如此复杂，你怎么做到避免“竞争条件”呢？到处加锁吗？我像那样的话，游戏代码就没法看了吧。
