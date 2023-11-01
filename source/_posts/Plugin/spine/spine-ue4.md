---
title: Spine UE4
mathjax: true
date: 2021-07-02 09:26:08
tags:
- Plugin
- Spine
categories: 
- Plugin
- Spine
---
explicit 指定构造函数或转换函数（C++11起）为显式，即踏板不能用于隐式转换和复制初始化。

# 开篇

好吧，google搜spine基本都是如何导入UE4的一些操作，只能自己看代码了。
spine分为两个部分，一个是spine底层的通用C++层，一个是对UE4的包装。我这边也分为两部分讲解。
## 通用C++层

这里面结构其实都是这一套，动画，图集，附件，骨骼，颜色，约束，事件，IK反向动力学，蒙皮，很奇怪的是里面的代码明显是按照UE4去做的，比如直接用UE_LOG打日志，所以就比较搞不懂，为什么在github是分开的。

### Animation

`Vector<Timeline *> _timelines;` 一个animation是由多个timeline构成的。
`HashMap<PropertyId, bool> _timelineIds;`哪些属性需要修改。是从timeline上搜集来的

#### String SimpleString
看到一个有意思的东西，之前看别人说是FString里面是按照4字节存储的，但是如果我们只是存储非宽字节，可以自己写一套，而他这个就是很好的参考。
这里很简单的就是一个长度，一个缓存区，然后里面采用的FMemory做一些内存分配器。简单，高效。
而SimpleString不走缓冲区的。

### Timeline

是个基类
看着像是平时的那个Timeline的轨道的概念，不过这个里面包含的东西很少，访问者模式，外部传递参数，他去处理。主要处理的骨骼的变化和事件的触发。

`Vector<PropertyId> _propertyIds;`初步看是animation拿着这些属性去计算一些东西，总感觉是单线程，总感觉可以改成Anim System那种多线程的方式，不过想想，算了，不至于，稳点。
`Vector<float> _frames;`总的帧数，除以下面的的_frameEntries就是单条轨迹的帧数。
`size_t _frameEntries;`初步估计是轨道中轨迹的数量，（我拿着AGE去类比，然后创造一些概念，希望可以理解）

### CurveTimeline : Timeline

### RGBATimeline : CurveTimeline

### Atlas

`Vector<AtlasPage*> _pages;` 多个图集页，其实主要是贴图，我们这边基本也就一个了
`Vector<AtlasRegion*> _regions;` 区域，可以理解为图集中的sprite。

spine的内存操作的规定就是，一次性的都系统接口，比如atlas，非一次性的比如string走FMemory做缓存。暂时不清楚这样是好是坏，放着。
里面有一个LoadTexture，暂时我没看到哪里加载贴图，不过讲道理来说应该是这块逻辑，晚点调试试试。

### Skeleton

`SkeletonData *_data;` 骨骼数据

### SkeletonData

`Vector<BoneData*> _bones;` 骨头数据

### BoneData

`BoneData *_parent;` 

### Event

[事件](http://zh.esotericsoftware.com/spine-events)

### Constraint

[约束](http://zh.esotericsoftware.com/spine-constraints) ，基类是`Updatable`，我也不知道为什么要叫这个名字，大概是因为每帧都计算？骨骼变换难道不每帧计算？起个Constraint不香吗？搞不懂。

下面有三个子类分别是：`TransformContraint`[变换约束](http://zh.esotericsoftware.com/spine-transform-constraints) 、`PathConstraint`[路径约束](http://zh.esotericsoftware.com/spine-path-constraints) 、`IKContraint`[反向动力学约束](http://zh.esotericsoftware.com/spine-ik-constraints)

约束在这里的意思其实是取巧的一些做法，比如某些条件或者方法，在某些约束的情况做是没有问题，比如反向动力学，一个坐着的角色手扶桌面，慢慢起身，这时FK需要不段的调整手部的位置，以使其能保持按在桌面上。这将需要创建大量关键帧来实现。
IK从下至上驱动。设置好手部的位置后，角色起身时，手部保持不动，小臂和大臂的骨骼会自动旋转到合适角度。

### Attachment

[附件](http://zh.esotericsoftware.com/spine-attachments) ，基类是`Attachment`

下面有几个子类分别是：`PointAttachment`[点附件](http://zh.esotericsoftware.com/spine-points) 、`MeshAttachment`[网格附件](http://zh.esotericsoftware.com/spine-meshes) 、

## 渲染

渲染的组件主要就是了Spine对UE4的包装，我就叫它为胶水层，粘结这下层的spine数据和上层的游戏逻辑。

胶水层有个组件渲染组件是`USpineSkeletonRendererComponent`这个继承`UProceduralMeshComponent`虽然我没写完，但是我迟早有一天会写完的。
好了，我们知道`UProceduralMeshComponent`这个组件是可以动态渲染数据的，那么刚好和我们的需求相吻合，然后就是它啦。
