---
title: LevelSequencer
mathjax: true
date: 2021-06-29 20:55:59
categories:
- Engine
- Unreal
---
https://www.bilibili.com/video/BV16y4y1S7yk?spm_id_from=333.999.0.0
https://www.bilibili.com/video/BV1YV411a7SV/?p=2&spm_id_from=pageDriver



1. LevelSequencer的`CutCameraTrack`的意思是用来切换相机的，且`MovieScene`会有一条单独的`CutSceneTrack`去管理。
2. LevelSequencer文件分类是由`TrackEditor`，`Track`，`Section`，`Channel`，`Template`等组成。
   1. 有一个`MovieScene3DTransform`但是这个文件彷佛是一个误导逻辑，实际的求值不会走。
   2. `(&GetComponentTransform, &SetComponentTransformAndVelocity);`这两个函数才是真正会走的地方。
   3. LevelSequencer是多线程的，调试经常容易卡死。


# LevelSequence（关卡序列）使用说明
以下文档都来自官方文档，但是我这边还是单独拿出来分析以下，足以证明，这块还是很有意思的。
## 基础操作
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/ReferenceEditor/

仔细看文档，基础操作，没啥说的，但是要注意，文档和引擎有些许差异，影响不大。

## 从摄像机导轨拍摄
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/CameraRigRail/


## 在Sequencer中使用帧标记
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/FrameMarkers/
一个书签的东西，运行时不会用，随便搞。

挺有意思的，但是感觉可以用其它方式代替。不过可以试试。
## 切换Sequencer中Actor的材质
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/MaterialAnimation/

没啥好说的，跟着做就好。
## 混合动画属性
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/CharacterAnimation/

## 使用模板序列
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/TemplateSequences/

单个轨道复用的，相当于做一个template sequencer可以给多个level sequencer使用。
制作的时候可以想清楚，如果有复用的情况下可以用这个。

没啥好说的，两个动画做blend，可以选如何插值。
## CineCameraActor（电影摄像机Actor）

https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/CineCameraActors/

这个镜头我还是非常喜欢的，如果项目中可以使用，我是极力推荐的。
- 启用观看跟踪（Enable Look at Tracking） -选取此选项可以让摄像机跟踪Actor。
- 提取调试观看跟踪位置（Draw Debug Look at Tracking Position）-选取此项可以让我们看到摄像机正在观看的位置。
- 要跟踪的Actor（Actor to Track）-将此项设置为ThirdPersonCharacter，因为我们希望在镜头中跟踪此角色。
- 相对偏移（Relative Offset）-将Z设置为60以将跟踪位置从默认跟踪位置稍微提高。
在CineCameraActor的细节（Details）面板中，将菲林版设置（Filmback Settings）设置为使用Super 16mm。菲林板设置（Filmback Settings）提供了不同的格式，虽然我没看懂是啥意思，但是实验了一下，感觉镜头拉的更近了。

## 淡入淡出场景
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/TracksFade/

使用的轨道是Fade Track
## 关卡可见性轨道（项目包装可能与此不同，谨慎使用）
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/TracksLevelVisibility/

使用的轨道是Level Visibility Track
简单来说其实就是卸载一个关卡，在加载另一个关卡的表现形式，但是项目的关卡定义方式可能会与此不同，不过即使不同，我们应该也会有类似的实现方式。所以原理可以参考。
做过长动画用这个感觉挺好的。

## 为材质参数集（Material Parameter Collections）设置动画
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/TracksMaterialParameter/

使用的轨道是Material Parameter Collection Track
设置材质的一些参数。

## 调整场景的播放速率
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/TracksPlayRate/

使用的轨道是Time Dilation，这有点坑，文档上面写着是Play Rate，用的时候要注意，这个之前磊子哥用的挺多的，在AGE中叫SetTimeScale。

## 创建镜头和副镜头
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/TracksShot/

## 关于蒙太奇
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/MatineeConversionTool/

上面这个文档没啥用，他的意思是说之前有蒙太奇的过场动画，说是一个轻量的，但是后来官方不维护了，就没用了。一堆废话，总之，代码还在，但是我们不用了。
跟着官方做，没有找到方法，但是找了一个可以把蒙太奇录入到subsequence里面，也是蛮方便的。录入的前提是需要在运行中录入的，非运行时录入应该也可以，但是这时候你如果可以把这个蒙太奇动起来就可以录入吧，我猜的。

https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/SequenceRecorder/

真正的是看这个文档，说的很清楚就是运行游戏，然后开始录制，不过这个不是录制视频，而是level sequencer，挺好的。感觉可以做新手引导之类的东西，或者找一个样本，然后二次加工。

## 使用音频轨迹
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/TracksAudio/

说是要用wwise，做demo感觉可以用这个，不过不管吧，毕竟没啥需求，将来加了wwise，可以在加一个wwise的音频轨道，也好加。

## 自动关键帧
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/UsingAutoKey/

没啥可说的，K帧最基本的东西。

## 保留或存储通过Sequencer进行的修改
https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/WhenFinished/

没啥可说的，播完之后选择如何设置绑定对象或者全局的状态。

## 媒体轨道

https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/UsingMediaTracks/

## 使用摄像机镜头切换

https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/TracksCameraCut/

这个东西，反正我挺喜欢的，不过按照AGE来说，我们可能只会用一个镜头，但是说实在的，一个镜头却是靠操控性会差很多。而且程序这边还需要传一个主相机。
其实level sequencer和age来比，或者age和其它编辑器来说，最大的优势其实是多种坐标系这个概念，让发生的过场动画，跟你的环境不是受太大的影响。然后引导你去这种编辑一些东西。
但是其实这种不太好。美术做的东西本来就很容易受环境影响，不同的环境表现力肯定是不相同的，所以我比较倾向程序尽量少传递这些比如镜头和角色等之类的东西，如果在性能可以hold的情况下。

## 使用动态变形创建关卡序列

https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/DynamicTransforms/

标题有点难受，其实就是指定坐标原点，相对坐标系那种思路。但是这个是个全局的概念，而不是age那种可以指向到某个轨道上去。

## 通过Sequencer混合动画蓝图

https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/BlendingAnimBPs/

讲道理来说，我们项目是不推荐使用自带蓝图的，在项目自己的蓝图中没有出来的时候，可以使用这种方式。

## 扩展

### 使用几何体缓存轨迹

https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/HowTo/GeometryCacheTrack/

有点秀操作的东西，但是我们应该用不上。

### 非线性动画和混合工具

https://docs.unrealengine.com/4.26/zh-CN/AnimatingObjects/Sequencer/AnimationBlendingTools/

反正官方说是测试功能，不建议使用，我们也就不要使用了吧。大概意思是说，在动画的过渡区域，普通的混合会出现由于骨骼的控制权的归属和权重控制问题导致的错乱。而这个主要是处理这个问题的。但是由于是测试功能，说明还是有些坑没有解决掉的。


MovieSceneSkeletalAnimationTemplate.cpp 575
求值器演算驱动器
FMovieSceneSkeletalAnimationSectionTemplate::Evaluate
MovieSceneSkeletalAnimationTemplate.cpp 271
// 驱动器去执行真正的逻辑
virtual void Actuate()

MovieSceneBlendingAccumulator.cpp
// 累加器进行步进收集执行
void FMovieSceneBlendingAccumulator::Apply()
MovieSceneAccumulatedBlendState.h 180
// 令牌数不足导致无法进行驱动
TArray<TInlineValue<FTokenEntry, 125>> TokensToBlend;
MovieSceneBlendingAccumulator.h //混合增加令牌
BlendToken

MovieSceneAccumulatedBlendState.cpp
void FMovieSceneAccumulatedBlendState::Consolidate()

void FMovieSceneObjectCache::UpdateBindings(const FGuid& InGuid, IMovieScenePlayer& Player)