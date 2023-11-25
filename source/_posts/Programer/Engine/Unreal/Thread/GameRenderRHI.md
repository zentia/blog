---
title: Game、Render、RHI多线程架构
date: 2022-01-13 15:36:00
categories:
- Engine
- Unreal
- Thread
tags:
    Unreal
---
# 游戏线程(GameThread)
GameThread是引擎运行的心脏，承载游戏逻辑、运行流程的工作，也是其它线程的数据发起者。在`FEngineLoop::Tick`函数执行每帧逻辑的更新。
在引擎启动时会把GameThread的线程id存储到全局变量`GGameThreadId`中，且稍后会设置到TaskGraph系统中。
```C++
int32 FEngineLoop::PreInitPreStartupScreen(const TCHAR* CmdLine)
{
    // ... ...
    
    // 创建线程自己的TLS数据FPerThreadFreeBlockLists  注：Binned2、Binned3内存分配器需要
    FMemory::SetupTLSCachesOnCurrentThread();
    
    // remember thread id of the main thread
    GGameThreadId = FPlatformTLS::GetCurrentThreadId();// 游戏线程id
    GIsGameThreadIdInitialized = true; // 游戏线程id是否被初始化

    FPlatformProcess::SetThreadAffinityMask(FPlatformAffinity::GetMainGameMask()); // 设置当前线程的cpu核的相关性  注：防止在多个cpu核心上跳来跳去，引发性能问题
    FPlatformProcess::SetupGameThread(); // 设置游戏线程数据(但很多平台都是空的实现体)
    
    // ... ...
    
    FTaskGraphInterface::Startup(FPlatformMisc::NumberOfCores()); // TaskGraph初始化，并根据当前机器cpu的核数来创建工作线程
    FTaskGraphInterface::Get().AttachToThread(ENamedThreads::GameThread); // 附加到TaskGraph的GameThread命名插槽中. 这样游戏线程便和TaskGraph联动了起来.
    

    if (GUseThreadedRendering)  // 如果使用渲染线程
    {
        if (GRHISupportsRHIThread) // 当前平台如果支持RHI线程
        {
            const bool DefaultUseRHIThread = true;
            GUseRHIThread_InternalUseOnly = DefaultUseRHIThread;
            if (FParse::Param(FCommandLine::Get(), TEXT("rhithread")))
            {
                GUseRHIThread_InternalUseOnly = true; // 创建独立的RHIThread，放加入到TaskGraph中，RHI会跑在TaskGraph的RHIThread上
            }
            else if (FParse::Param(FCommandLine::Get(), TEXT("norhithread")))
            {
                GUseRHIThread_InternalUseOnly = false;
            }
        }
            
        SCOPED_BOOT_TIMING("StartRenderingThread");
        StartRenderingThread();  // 创建并启动渲染线程
    }
    
    // ... ...
}
```
游戏线程和`TaskGraph`系统的`ENamedThreads::GameThread`其实是一回事，都是同一个线程！
经过上面的初始化和设置后，其它地方就可以通过TaskGraph系统并行地处理任务了，页可以访问全局变量，以便判断游戏线程是否初始化完，当前线程是否游戏线程：
```C++
bool IsInGameThread()
{
    return GIsGameThreadIdInitialized && FPlatformTLS::GetCurrentThreadId() == GGameThreadId;
}
```
# 渲染线程（RenderThread）
RenderThread在TaskGraph系统中有一个任务队列，其他线程（主要是GameThread）通过宏`ENQUEUE_RENDER_COMMAND`向该队列中填充任务。
RenderThread则不断从这个队列中取出任务来执行，从而生成与平台无关的Command List（渲染指令列表）。注：整个过程是异步的。
RenderThread是其他线程（主要是GameThread）的奴隶，只是简单地作为工作线程不断执行它们赋予的工作。

`RenderingThread.h`声明了全部对外的接口，部分如下：
```C++
// Engine\Source\Runtime\RenderCore\Public\RenderingThread.h

// 是否启用了独立的渲染线程, 如果为false, 则所有渲染命令会被立即执行, 而不是放入渲染命令队列.
extern RENDERCORE_API bool GIsThreadedRendering;

// 渲染线程是否应该被创建. 通常被命令行参数或ToggleRenderingThread控制台参数设置.
extern RENDERCORE_API bool GUseThreadedRendering;

// 是否开启RHI线程
extern RENDERCORE_API void SetRHIThreadEnabled(bool bEnableDedicatedThread, bool bEnableRHIOnTaskThreads);

(......)

// 开启渲染线程.
extern RENDERCORE_API void StartRenderingThread();

// 停止渲染线程.
extern RENDERCORE_API void StopRenderingThread();

// 检查渲染线程是否健康(是否Crash), 如果crash, 则会用UE_Log输出日志.
extern RENDERCORE_API void CheckRenderingThreadHealth();

// 检查渲染线程是否健康(是否Crash)
extern RENDERCORE_API bool IsRenderingThreadHealthy();

// 增加一个必须在下一个场景绘制前或flush渲染命令前完成的任务.
extern RENDERCORE_API void AddFrameRenderPrerequisite(const FGraphEventRef& TaskToAdd);

// 手机帧渲染前序任务, 保证所有渲染命令被入队.
extern RENDERCORE_API void AdvanceFrameRenderPrerequisite();

// 等待所有渲染线程的渲染命令被执行完毕. 会卡住游戏线程, 只能被游戏线程调用.
extern RENDERCORE_API void FlushRenderingCommands(bool bFlushDeferredDeletes = false);

extern RENDERCORE_API void FlushPendingDeleteRHIResources_GameThread();
extern RENDERCORE_API void FlushPendingDeleteRHIResources_RenderThread();

extern RENDERCORE_API void TickRenderingTickables();

extern RENDERCORE_API void StartRenderCommandFenceBundler();
extern RENDERCORE_API void StopRenderCommandFenceBundler();

(......)
```

`RenderingThread.h`还有一个非常重要的宏`ENQUEUE_RENDER_COMMAND`，它的作用是向渲染线程入队渲染指令。下面是它的声明和实现：
```C++
// 向渲染线程入队渲染指令，Type指明了渲染操作的名字。
#define ENQUEUE_RENDER_COMMAND(Type) \
    struct Type##Name \
    { \
        static const char* CStr() { return #Type; } \
        static const TCHAR* TStr() { return TEXT(#Type); } \
    }; \
    EnqueueUniqueRenderCommand<Type##Name>
```
上面最后一句使用了`EnqueueUniqueRenderCommand`命令，继续追踪之：
```C++
/* UnrealEngine/Engine/Source/Runtime/RenderCore/Public/RenderingThread.h */

/** The parent class of commands stored in the rendering command queue. */
class RENDERCORE_API FRenderCommand
{
public:
    // All render commands run on the render thread
    static ENamedThreads::Type GetDesiredThread() // 所有渲染指令都必须在渲染线程执行。
    {
        check(!GIsThreadedRendering || ENamedThreads::GetRenderThread() != ENamedThreads::GameThread);
        return ENamedThreads::GetRenderThread(); // 开启渲染多线程时，返回渲染线程。不开启渲染多线程时，返回GameThread
    }

    static ESubsequentsMode::Type GetSubsequentsMode()
    {
        // Don't support tasks having dependencies on us, reduces task graph overhead tracking and dealing with subsequents
        return ESubsequentsMode::FireAndForget;
    }
};
```
为了更好理解入队渲染命令操作，举2个具体的例子：
例1：在GameThread执行LoadMap切地图，在卸载掉Old World之后，会在TrimMemory()函数中使用宏**ENQUEUE_RENDER_COMMAND**向RenderThread的TaskGraph队列中头一个FlushCommon任务。