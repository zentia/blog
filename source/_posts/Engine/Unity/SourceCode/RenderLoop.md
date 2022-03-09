---
title: RenderLoop
categories:
- Engine
- Unity
- SourceCode
---
```
有幸接触到引擎代码，现在记录一下整个RenderLoop的流程。
以下通过视图去分析，进而查找如何修复预览窗口下不显示的问题。

```C++
// Render the camera manually.
CUSTOM void Render()
{
    self->StandaloneRender(Camera::kRenderFlagSetRenderTarget, NULL, "");
}
```

预览会掉这个接口

```C++
void Camera::StandaloneCustomRender(RenderFlag renderFlags, Shader* replacementShader, const core::string& replacementTag, PerformRenderFunction* renderFunctionObj)
{

}
```

场景视图会掉

```C++
Camera::RenderEditorCamera(EditorDrawingMode drawMode, const DrawGridParamters* gridParam)
{

}
```

游戏视图会掉

```C++
// RepaintControllerShared.cpp
RepaintController::RenderGameViewCameras(RenderTexture* target, const int displayId, const Rectf& screenRect, const Vector2f& mousePosition, bool gizmos)
{

}
```

场景视图和游戏视图入口最终会流向

```C++
void RenderManager::RenderCameras(const int displayId, PostPerCameraFunctor postPerCameraCull, bool isPreview, UInt32 kSpecialTag)
{
    if (RenderCamerasWithScriptableRenderLoop(displayId))
    {
        ...
    }
    ...
}
```

SRP模式下都会走renderCamerasWithScriptableRenderLoop了，所以预览窗口直接掉Camera.Render是无效的。
然后看RenderCamerasWithScriptableRenderLoop内部

```C++
bool RenderManager::RenderCamerasWithScriptableRenderLoop(const int displayId)
{
    ...
    ScriptableRenderContext::ExtractAndExecuteRenderPipeline(visibleCameras);
    ...
}
```

进而

```C++
void ScriptableRenderContext::ExtractAndExecuteRenderPipeline(const dynamic_array<Camera*>& cameras)
{
    ...
    ScriptingInvocation invocation(GetCoreScriptingClasses().doRenderLoop_Internal);
    ...
    invocation.Invoke();
    ...
}
```

最后

```C#
// RenderPipelineManager.cs
[RequiredByNativeCode]
private static bool DoRenderLoop_Internal(IRenderPipelineAsset pipe, Camera[] camera, IntPtr loopPtr)
{

}
```

然后在流向项目中的

```C#
// HDRenderPipeline.cs
namespace Yarp
{
    [ExecuteInEditMode]
    public class HDRenderPipeline : IRenderPipeline
    {
        public void Render(ScriptableRenderContext context, Camera[] cameras)
        {
            if (Update(cameras))
            {
                RenderInternal(context, cameras);
            }
        }
    }
}
```

至此以上我们整条渲染流水线已经走通了，虽然不像UE那样把Game和Draw区分开，不过也能跑起来了。
那现在的问题是如何在预览中渲染出画面呢？
很简单，让预览也走这套流程。
我们在跟踪代码的时候发现

```C++
// PreviewWindow.cpp
void RenderPreviewWindows()
{
#if UNITY_EDITOR || UNITY_STANDALONE
    Mutex::AutoLock lock(g_PreviewWindowLock);

    for (auto it = previewWindows_.begin(); it != previewWindows_.end(); ++it)
    {
        it->second->Render();
    }
#endif
}
```

这里面也会走到我们的预期的效果。
然后我们继续跟踪，发现线索都在RuntimePreviewWindowBindings.gen.cpp中，我们知道这个是绑定文件，所以我们自然而然想到它的绑定文件。
在RuntimePreviewWindow.bindings文件中，
然后我们就在C#端的预览窗口使用RuntimePreviewWindow这个接口，但是在使用之前，我们咨询了原作者的思路，在小心求证中。但不妨我们对代码进行研究。
在追述的过程中，我们在Player.cpp中发现了大量的REGISTER_PLAYERLOOP_CALL宏
下面是这个宏的定义

```C++
// PlayerLoopCallbacks.h
#define REGISTER_PLAYERLOOP_CALL(type, name, body, ...) struct type##name##struct {static void Forward (__VA_ARGS__) { body; } }; gPlayerLoopCallbacks.type.name = type##name##struct::Forward;
```
这个宏首先定义了一个结构体，结构体中只有一个静态的Forward函数，然后把函数指针传给了gPlayerLoopCallbacks.type.name中了。
好了，我们举个例子：
```C++
// Player.cpp 1555
REGISTER_PLAYERLOOP_CALL(PostLateUpdate, FinishFrameRendering,
{
    if (::NeedToPerformRendering())
    {
        // Perform rendering
        if (!::IsBatchMode())
        {
            PlayerRender(false);
        }
    }
});
```
我们翻译一下就是
```C++
struct PostLateUpdateFinishFrameRenderingstruct
{
    static void Forward()
    {
        if (::NeedToPerformRendering())
        {
            // Perform rendering
            if (!::IsBatchMode())
            {
                PlayerRender(false);
            }
        }
    }
};
gPlayerLoopCallbacks.PostLateUpdate.FinishFrameRendering = PostLateUpdateFinishFrameRenderingstruct::Forward;
```
这样看就很easy了。好了，这只是个插曲，但是如何调用呢？
我们找到PlayerLoop.cpp
然后找到
```C++
void PlayerLoop()
{
    ...
#if ENABLE_PROFILER
    PlayerLoopWithProfiler();
    return;
#endif
    ...
    CALL_UPDATE_MODULAR(Initialization, PlayerUpdateTime);
    ...
    CALL_UPDATE_MODULAR(PreLateUpdate, UpdateNetworkManager);
    ...
    CALL_UPDATE_MODULAR(PostLateUpdate, FinishFrameRendering);
}
```
游戏引擎最经典的loop了，也是整个游戏的心脏。我们可以看到CALL_UPDATE_MODULAR()等一系列函数，这块就是一个执行次序，像是美国（虽然现在不行了）但是它还是有秩序的控制着整个世界。
然后我们看一下PlayerLoopWithProfiler做了些什么。
```C++
// PlayerLoop.cpp
void PlayerLoopWithProfiler()
{
    ...
    { PROFILER_AUTO(gPostLateUpdate24, NULL); CALL_UPDATE_MODULAR(PostLateUpdate, FInishFrameRendering); }
    ...
}
```
好了，它里面就是包裹了一层性能的逻辑，我们暂时不关心。
至此，我们已经知道了渲染的主体工作流程了，我们尝试把宏打开看看会怎样把。
编译好了
下一步我们回到工程。
C# 这一侧PreviewWindow和Camera是分离的，然后在新建Camera中的AwakeFromLoad函数中，会自动添加到RenderManager中
代码如下：

```C++
// Camera.cpp 351
void Camera::AwakeFromLoad(AwakeFromLoadMode awakeMode)
{
    ...
    GetRenderManager().AddCamera(this);
    ...
}
```
好吧，这块放弃了，由于是给UNITY_STANDALONE使用的，有一些其它依赖项，是使用起来变得麻烦，最后在 Camera.Render支持Preview而修复。