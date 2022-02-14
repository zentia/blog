---
title: iOSProfile
date: 2022-01-02 15:38:00
password: zplan
categories:
- Engine
- Unreal
- ZPlan
---
本篇文章是私密文章，不会开放到外网。
以下测试均已iPhone8测试标准
通过我们大量测试，Game线程 Draw线程 RHI(GPU)三者只要有一个高于10ms，过一段时间必然发烫降频。所以，目标是在所有机型，这三个线程的耗时需要控制到10ms以内。
# SkyLight
## 开启
![EnableSkyLight](/images/Unreal/ZPlan/EnableSkyLight.PNG)
### 阴影
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth3.png)
此时开启天光的阴影整体耗时为402.12us
### 角色场景
![ActorScene](/images/Unreal/ZPlan/ActorScene.png)
角色和场景绘制耗时4.15ms(此时是LOD2的)
PostAO占了1.98ms
总共6.6ms
### 后处理
![LUT](/images/Unreal/ZPlan/SkyLightLUTS.png)
耗时319.65us
![Tonemap](/images/Unreal/ZPlan/Tonemap.png)
Tonemap 674.33us
FXAA 538.36us
UpScale (300.97us)
总共1.814.14ms
总体耗时：0.4+6.6+1.8=8.8ms

## 关闭
![NoSkyLight](/images/Unreal/ZPlan/NoSkyLight.PNG)
### 阴影 (r.ShadowQuality)
![NoSkyLightShadowDepth](/images/Unreal/ZPlan/NoSkyLightShadowDepth.png)
227us
### 角色场景
![NoSkyLightRender](/images/Unreal/ZPlan/NoSkyLightRender.png)
6.37ms
### 后处理
![NoSkyLightPostProcess](/images/Unreal/ZPlan/NoSkyLightPostProcess.png)
3.8ms
图中框住的部分，可以通过关闭HDR去掉，但是去掉的效果就是会很油腻，很黑。
也就是说场景分了3部分渲染，第一部分是Shadow第二部分是正常绘制，第三部分是后处理总共大概
6.37+1.778+0.227=8.375ms左右，但是手机上显示16ms左右，还有其它几毫秒的偏差，暂时不知道哪里造成的。（也可能不是同一帧截的，暂时忽略）
结论：在iPhone8 上开启天光和关闭天光只有0.4ms的差距，可以开启，但是Android之前测试的情况，会有一些莫名的uniformbuffer导致性能下降，等定位到问题，android在开。
# LOD
LOD 这里只测试强制LOD，因为不一定能截取到同一画面。导致数据差异性会大一些。
这里的LOD只是角色的LOD场景的LOD并没有测试。
可以看出会有1.3ms的左右的提升，内存也会有所降低，DrawCall降低了一半以上。但是性能提升效果其实并不明显，但是比天光要好一些。
## ForceLOD1
![LOD1](/images/Unreal/ZPlan/LOD1.PNG)
## ForceLOD2
![LOD2](/images/Unreal/ZPlan/LOD2.PNG)

# Shadow Quality
这里主要比较开启和关闭就可以了，主要是为了达到android效果，开太多也没有必要。
这里也可以看出，开启shadow和关闭shadow大概也是1.3ms左右的效率。
## Quality 0
![Shadow0](/images/Unreal/ZPlan/Shadow0.PNG)
![Shadow1](/images/Unreal/ZPlan/Shadow1.PNG)
## Quality 1
# AntiAntiAliasing (r.DefaultFeature.AntiAliasing)
iOS是必须开的，不开会白屏，目前不知道什么原因。
# Model RMA (Roughtess, Metallic, Ambient)
其实没啥意义，现在瓶颈主要是在GPU，而贴图卡顿主要是带宽，如果带宽不足的话，其实不管会卡GPU，而且会卡Draw，SkyLight就是这样子的，有一个2000多的uniformBuffer。
# ScreenPercentage （r.screenpercentage)
这里可以看出，每个梯度都有一点多毫秒的下降。
## 100
![ScreenPercentage100](/images/Unreal/ZPlan/Shadow1.PNG)
## 75
![ScreenPercentage75](/images/Unreal/ZPlan/ScreenPercentage75.PNG)
## 50
![ScreenPercentage50](/images/Unreal/ZPlan/ScreenPercentage50.PNG)
# Bloom (r.bloomquality 0)
这里也是测试0 和 1 就可以
可以看出，在开启Bloom和关闭Bloom之前的相差差不多也是1ms多点。
## Open
![Bloom1](/images/Unreal/ZPlan/Bloom1.PNG)
## Close
![Bloom0](/images/Unreal/ZPlan/Bloom0.PNG)
# SSR
开启和关闭大概有3ms的差距，所以一般还是关闭比较好，怀疑这个甚至比PPR还要耗(抽空测试一下)
## Open
![SSR](/images/Unreal/ZPlan/SSR.PNG)
## Close
![NoSSR](/images/Unreal/ZPlan/NoSSR.PNG)
# HDR(Tonemapper)
不支持，开了就卡死，材质找不到提示。暂时没时间去细察。PC上效果如下所示：
![LDR](/images/Unreal/ZPlan/LDR.png)
![LDRProfiling](/images/Unreal/ZPlan/LDR-Profiling.png)
可以看出，当关了HDR之后是没有tonemapper。
手机效果可以用`r.Mobile.TonemapperFilm 0`来看看，不过效果是有的，但是抓帧里面还有tonemapper。
# PlanarReflection
![PlanarReflection](/images/Unreal/ZPlan/planarReflection.png)
比较不解的是，已经关掉了平面反射，但是 iOS还在跑，需要实验删掉看看情况是否好转。

# UI
想不到UI还可以占2ms。可怕
![UI](/images/Unreal/ZPlan/UI.png)

# ShadowDepth
不管如何设置，大小都是2048 x 2048，有点奇怪。
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth.png)
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth1.png)
这里可以看出顶点数越多，画阴影的时间也会越多。
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth2.png)
这里又比较奇怪了，顶点数也比较多，但是耗时却比较少。怀疑跟屏占比有关系。

关闭天光的阴影真题耗时为227.41us
![NoSkyLightShadowDepth](/images/Unreal/ZPlan/NoSkyLightShadowDepth.png)

# MobileBasePass_PostAO(环境光遮蔽)
`r.Mobile.PixelProjectedReflectionQuality`
![PostAO](/images/Unreal/ZPlan/postAO.png)
这里控制开启和关闭
## Open
![OpenPostAO](/images/Unreal/ZPlan/OpenPostAO.png)
## Close (sg.PostProcessQuality 0)

# Fog
```C++
static TAutoConsoleVariable<int32> CVarFog(
	TEXT("r.Fog"),
	1,
	TEXT(" 0: disabled\n")
	TEXT(" 1: enabled (default)"),
	ECVF_RenderThreadSafe | ECVF_Scalability);
```

# 场景材质烘培
ios下测试，烘培要比不烘培性能要高一些，但是效果会发生改变，原因是重新生成了贴图和材质。

# 分享记录
人到的差不多了，那开始我今天的分享
今天，我主要分享的是zplan音游的性能优化，也是对我去年下半年的一个工作总结
那，我去年做的工作的一部分主要是跟性能优化相关的，借此机会分享一下，也是对自己的一个工作总结
我们主要围绕这三个问题来进行此次内容的分享
分别是，什么问题会影响性能，然后我们怎么分析（就是通过一些常见工具来快速定位瓶颈），还有一些常见的问题。
这次分享的话，我不会讲的太深，一方面的是我也不了解，另一方面我担心时间可能会来不及，因为主要跟渲染相关的，往深里面讲的话可能会讲到IMR，TBR，TBDR等比较深和偏硬件的东西，不然容易讲不清，我是这样想的。所以，这次先简单说一下，如果有机会
在找时间往深里挖掘一下。
然后这次主要是以实践为主，就是我们通过一些方法和手段快速定位到性能瓶颈，并解决掉他。然后掌握其中的原理。
也会熟悉一下引擎内置的工具，帮助我们找到快速这些问题。
然后，先说一下，为什么要性能优化？
这个是优化的主要三个目的
让手机不发烫、不掉帧（也是核心目的）
上面那个目的达到之后，在此基础，然后我们希望可以运行更好的美术效果。
并且，开发起来更容易。
我先说一下第三点，为什么开发起来更容易。
其实说白了，就是我们通过性能优化，总结出来一些规则，比如说场景材质该怎么使用，使用多少个？最大面数是多少等等，这样美术同学在开发的时候，就可以从设计层面到制作层面心里会有一个衡量做法的标尺，容易知道自己制作的边界在哪里。这样的话也会减少返工。
那，如何可以运行更好的美术效果呢？

## 机型适配
其实说白了，就是市面上有成千上百中机型，而我们如何去适配这么多机器呢，其实UE4已经帮我做了很多了，比如区分底层做好了Metal或者OpenGLES的RHI接口，我们只需要去适配什么机型会跑什么画质。
具体来说这个是怎么定呢？
盲人摸象，真的。
上面给了我们机型的标准，说我们一定要可以在某些最低配的机型上跑起来，比如iphone8和高通625，那么对于这种最低端的手机，肯定只需要跑最低配就可以了，然后我们这边是分了三个画质，低中高，对于iphone8和高通625，那肯定是低画质了，而且也只能跑低画质，因为高中跑不起来呀，跑了也卡，然后还可能会出一堆莫名奇妙的bug。那剩下的中画质和高画质怎么去确定呢？
我们先看ios


一般来讲，我们的目标帧率就是每秒30帧，但是音游比较吃操作，所以需要60帧，之前策划反馈过，如果30帧的手感会差很多，但是60帧就可以，其实我们最高只是50帧，但是策划以为是60帧。
然后的话，我们具体分析一帧的时候，我们其实看的是时间，就是毫秒，有一个16ms定律，就是说你的游戏，每帧耗时不超过16ms，因为，1s60帧就是16毫秒嘛，其实是没问题的，但是由于我们是包了一层metaverse（就是我们的元宇宙平台）和qq平台，在实际测试中来看，我们是不能超过10ms的，由此可见，元宇宙可能也只能做一些小游戏了吧，但是至少比H5的游戏规模会大一些？

## Avoid thinking
- Optimization as later or final step in a project.
- Performance as an exclusive responsibility of programmers
- The engine can optimize things for you.

不管有什么内容被加入了引擎，都要有人为之负责，有些人会犯一个常见的错误，不光是独立开发者，有时候3A开发者也会觉得。引擎能主动为你做优化。没错，有时候是这样，但引擎为你做优化的情况非常罕见，而在大多数情况下，你至少需要自己弄清楚面临的问题。从而在引擎中进行正确的设置。而从获得最佳性能表现。所以，别再把所有的Actor、建筑、植被都扔进一个地图里。然后指望着引擎优化你的开放世界游戏。这不管用，好的做法是

## Good practices
- The best optimizations you can make often begin with the work you do at the very beginning of your project.
- Profile soon and profile often.
- Avoid "The Janitor" syndrome:
  - Performance should be the responsibility of the entire team.
  - Make sure every team member is aware of the basic profiling process.
  - This includes artists and sound engineers.

正如我之前说的，最好的优化，要从项目的开头开始，你做的越早越好，要立即且频繁的做性能分析，这是团队中每个人的责任。要避免“两耳不闻窗外事”，只要你需要处理游戏中的特定资源。就要做好优化相关的东西，至少在Epic内部，大家是这么做的。作为项目主管，如果你要领导项目工作，那么项目主管的责任。就是，确保每个人都积极参与，都有基本的性能分析意识。

## Am I GPU or CPU bound(约束)?

我们继续，开始正篇
通常，当你开始做游戏性能分析时，你会寻找哪里除了问题，好吧，是什么东西拖累了性能表现呢？你应该问自己的第一个问题是，我是被GPU还是CPU拖累了？这是显卡的问题吗？还是CPU的问题？

### Identify bottlenecks

Use stat unit,not just stat fps
- Largest number shows you the likely bottleneck.

你一上来要做的事情，应该是使用stat unit而不是stat FPS命令，因为stat unit的硬件渲染某一帧所需的实际时间，也就是那些绿色的数字，
“GPU”线程 指GPU渲染时间
Frame时间，并不是其它时间的总和。它们都是不同的线程 是并行运行的 每个线程都是按顺序的 它们需要上一个线程的内容和结果 在我们简单介绍这些线程之前，我要和你们分享一些知识
理想情况下，当你做性能分析时，当你寻找游戏存在的瓶颈时，游戏的运行环境，越接近目标硬件和目标平台，你获得的数据就越准确，如果可以的话，要避免在编辑器里进行性能分析
但在移动平台，采用前向渲染，所以情况完全不同，一定要使用合适的打包版本，并且在目标硬件上运行

## Analysis of a frame

当游戏按图中的执行时，“游戏”线程计算的所有数据，都会被存储起来。并被“绘制”线程使用。它会算出所有需要的数据，这些内容不会显示在屏幕上
在这一步完成后，“GPU”线程会在屏幕上实际渲染出最终的像素。如果这些线程 这些数据 如果它们计算每个线程的这些帧 都需要耗费时间 那下一个步骤当然 就要等待
那么这些线程具体都是负责什么工作呢？
“CPU”线程“游戏”线程 正如名字所示 负责整个游戏的模拟 所有的东西  从游戏逻辑 所有Actor的位置 场景中所有物体的位置 动画 实际动画帧 物理效果 AI 一切与场景最终效果有关的计算 在进行处理时 都会在“游戏”线程中计算 
这步完成后 整个游戏世界就被计算完毕了 引擎就会知道什么东西该干什么了 
下一步“Draw”线程 它会过滤掉 它会剔除掉所有 不在摄像机范围内的物体（是根据AABB包围盒剔除的，所以如果不实时更新包围盒或者把物理组件删掉会有一些问题）却还需要引擎去渲染的对象
我们有很多办法来做到这点 我们之后在深入探讨这个话题 但基本上来说 所有不在摄像机范围的东西，都会被我们过滤掉 然后我们创建一个列表 包含所有的对象、着色器、材质、纹理 所有需要发送给GPU的数据 然后GPU会处理这些数据包括顶点、着色器、纹理
等各种数据 然后在屏幕上绘制最终的像素

## Profiling (CPU)
记录引擎所有操作，甚至是绘制调用的情况，他会生成一个记录文件，然后分析，

## Game Thread
基本上与游戏逻辑有关 与构建世界有关的一切 都要由游戏线程来计算 都要在你的CPU上进行计算 通常“游戏”线程中的性能问题
实际上 大部分蓝图的Tick都可以不执行的，因为他可能在检测某种状态，这种逻辑其实用事件更合适，因为大部分游戏脚本都是基于事件的 如果你的世界中 存在太多的Tick的话，那就会严重拖累游戏的流畅度，比如没有合并的时候物理骨骼。
dumpticks 可以列出正在更新的所有Actor 以及它们的总数 
我的意思不是让你完全不用Tick 不 我不是这个意思
假如你一定要在Tick中实现游戏逻辑
我们只想告诉你一件事
那就是在tick函数中
添加复杂逻辑之前
要谨慎评估
请考虑是否真的需要这么做
去每帧更新逻辑
有很多代替方法
你可以采用基于事件的方法
因为有些游戏逻辑未必会像你假设的那样
频繁更新
或者 如果真的需要以固定频率更新
你可以使用计时器
设置降低蓝图Tick函数的调用频率
大家都会调用GetAllActorByClass
如果你真的要调用这个函数 请在基于事件的逻辑中调用它
比如在游戏启动时调用它
或者编写某种基于事件的逻辑
在只有需要访问数据的时候才调用这个函数
然后把数据全部都缓存在数组中
所以请记住这些好习惯
此外 如果你需要用到For循环
尤其是在涉及多重循环时
记得要及时中断循环
这样等你找到需要的对象后
就不用运行其余的循环了
SpawnActor-- 在序列化上是比较耗费的
所以如果你在频繁的生成actor的时候就要小心了
你可以把场景中的actor保存在缓存池中
还有 你要注意
构造脚本会增加每个actor的生成耗时
我之前说过 我们的目的
并非阻止你在Tick函数中实现复杂的逻辑
如果你真的要在Tick函数中
实现复杂算法和运算
请考虑使用原生代码 或者说C++ 明白吗？
你不需要把所有逻辑都放到C++，只需要把复杂计算放进去就可以了
然后把他反射可以让TS调用就可以了

实际上，你所调用的Draw Call越多，你来回绘制所耗费的时间 就会呈线性增加 情况会越来越糟糕 
刚开始学UE4的时候，你可能会看到某些文章，说的是UI其实可以不用太关心Draw Call，其实这个结论是片面的，这个待会我再说
Draw Call耗时的影响 有时候比多边形数量的影响还要严重

一个好的入手点就是使用stat scenerendering
减少Draw Calls

减少场景复杂 模型复杂度 模型顶点复杂度
但LOD也可用于减少绘制调用
在常见的流程中 某个网格体上
往往会有多种材质