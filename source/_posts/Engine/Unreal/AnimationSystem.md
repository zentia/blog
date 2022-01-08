---
title: Unreal Animation System
date: 2022-01-02 14:20:00
tags:
    - Unreal
---
https://blog.csdn.net/qq_23030843/article/details/109103433
# 动画基础概念
3D模型动画的基本原理是让模型中各顶点的位置随时间变化。主要种类有Morph(变形)动画，关节动画和骨骼蒙皮动画(Skinned Mesh)。
从动画数据的角度来看，三者一般都采用关键帧技术，即只给出关键帧的数据，其它帧的数据使用插值得到。但由于这三种技术的不同，关键帧的数据是不一样的。
1. Morph（变形）动画是直接指定动画每一帧的顶点位置，其动画关键存储的是Mesh所有顶点在关键帧对应时刻的位置。
2. 关节动画的模型不是一个整体的Mesh，而是分成很多部分(Mesh)，通过一个父子层次结构将这些分散的Mesh组织在一起，父Mesh带动其子Mesh的运动，各Mesh中顶点坐标定义在自己的坐标系中，这样各个Mesh是作为一个整体参与运动的。动画帧中设置各子Mesh相对于父Mesh的变化（主要是旋转，当然也可以包括移动和缩放），通过子到父，一级级的变换累加（当然从技术上，如果是矩阵操作是累乘）得到该Mesh在整个动画模型所在的坐标空间中的变换（从本文的视角来说就是世界坐标系了）从而确定每个Mesh在世界坐标系中的位置和方向，然后以Mesh为单位渲染即可。关节动画的问题是，各部分Mesh中的顶点固定在其Mesh坐标系中，这样在两个Mesh结合就可能出现裂缝。
3. 第三类就是骨骼蒙皮动画即SkinnedMesh了，骨骼蒙皮动画的出现解决了关节动画的裂缝问题。骨骼动画的基本原理概括为：在骨骼控制下，通过顶点混合动态计算蒙皮网格的顶点，而骨骼的运动相对于其父谷歌，并由动画关键帧数据驱动。一个骨骼动画通常包括为骨骼层级结构数据，网格（Mesh）数据，网格蒙皮数据（Skin Info）和骨骼的动画（关键帧）数据。

# 解算动作
先知道解算这块是可以开启多线程加快运算的，具体如何加快，看原文。
还有一个概念，AnimInstance是一个数据和运算集中性业务，外面把握不住，所以推出AnimInstanceProxy来配合使用。
再有一点是虽然主要讲的是骨骼蒙皮，即skeletonMesh，但是StaticMesh其实也包含进去了。只不过没讲，怎么说呢，其实都属于一块的，只不过没有动，所以就是一个简单的模型处理了。
# Unreal 动画基本数据结构解析
![1](/images/Unreal/Animation/1.png)
## Skeleton(骨架)
骨骼资源是一切骨骼动画的基础，通过分层结构，一般描述一个生物及非生物的骨架形状。
作为链接Mesh和动画的桥梁，其实其中所拥有的数据几乎没有什么东西。
1. BlendProfiles：混合描述文件。
2. AnimRetargetSources: 对于这个骨骼来说可以序列化的重定向资源
3. Socket: 每一根骨骼的点
4. VirutalBones: 在引擎中不增加蒙皮的虚拟骨骼，可以在动画蓝图中使用
5. RefLocalPoses_DEPRECATED: 和在local space 下的skeleton poses对应
6. Bone Tree: 由BoneNode组成，这里记录着的所有的骨架结构
7. SmartNames

### 渲染数据SkeletalMeshRenderData
对于Mesh来说，不用于渲染的数据，其实并没有什么用处。
```C++
/** Per-LOD render data. */
TIndirectArray<FSkeletalMeshLODRenderData> LODRenderData;
```
### 对骨骼的理解
我们想要理解骨骼，首先先看看静态模型吧，静态模型没有骨骼，我们在世界坐标系中放置静态模型师，只要指定模型自身坐标系在世界坐标系中的位置和朝向。
在骨骼动画中，不是把Mesh直接放到世界坐标系中，Mesh只是作为Skin使用的，是依附于骨骼的，真正决定模型在世界坐标系中的位置和朝向是骨骼。
在渲染静态模型时，由于模型的顶点都是定义在模型坐标系中的，所以各顶点只要经过模型坐标系到世界坐标系变换就可以进行渲染。
而对于骨骼动画，我们设置模型的位置和朝向，实际是在设置根骨骼的位置和朝向，然后根据骨骼层次结构中父子骨骼之间的变换关系计算出各个骨骼的位置和朝向，然后根据骨骼对Mesh中顶点的绑定计算出顶点在世界坐标系中的坐标，从而对顶点进行渲染。
要记住，在骨骼动画中，骨骼才是模型主体，Mesh不过是一层皮，一件衣服。
如何理解骨骼？请看第二个概念：骨骼可理解为一个坐标空间。
在一些文章中往往会提到关节和骨骼，那么关节是什么？骨骼又是什么？下图是一个手臂的骨骼层次的实例。
![2](/images/Unreal/Animation/2.png)
骨骼(Skeleton)只是一个形象的说法，实际上骨骼可理解为一个坐标空间，关节可理解为骨骼坐标空间的原点。关节的位置由它在父骨骼坐标空间中的位置描述。上图中有三块骨骼，分别是上臂、前臂和两个手指。Clavicle(锁骨)是一个关节，它是上臂的原点，同时肘关节(elbow joint)是前臂的原点，腕关节(wrist)是手指骨骼的原点。关节即决定了骨骼空间的位置，又是骨骼空间的旋转和缩放中心。为什么用一个4X4矩阵就可以表达一个骨骼，因为4X4矩阵中含有的平移分量决定了关节的位置，又是骨骼空间的旋转和缩放中心。
## Animation Sequence(动画序列)
Animation Sequence(动画序列)是一个可以在骨架网格物体上播放的独立动画资源，与Unity中的Animation相同。它们包含了一些关键帧，这些关键帧，这些关键帧可以及时地指出某个特定点处的一个骨骼位置、旋转度及缩放比例。通过在序列中播放这些关键帧，让骨架网格物体的骨骼之间进行混合，使得骨骼可以产生平滑的动画效果。
## Notifies(通知)
单词通知，需要新建自定义类，继承自AnimNotify，并实现其中的`Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)`（这是一个带有BlueprintImplementableEvent的方法）。
Curves(曲线)

## RootMotion(根骨骼)
某些动作资源带有偏移属性，区别是在动画播放后，人物的坐标位置是否真是移动（俗称假位移）。这些资源需要勾上RootMotion属性。
Animation Retargeting(相同骨架重定位)
Animation Retargeting(不同骨架重定位)
Animation Composite(合成动画)

Blendspaces(混合空间)
AimOffset(瞄准偏移)
## AnimMontage(蒙太奇)
AnimMontage(动画蒙太奇)一个多用途的工具，常用来控制动画播放，跳转和衔接。与其他资源不同，AnimMontage具有Slot属性，可以通过插槽的方式替换或融合基本动作。
Sync Groups(同步组)

SkeletalControls(骨架控制器)
## Slot(插槽)
当执行Montage
## Section(动画片段)
Section是一个动画片段，一个AnimMontage资源中可以包含多个Section。在Section区域右键即可新建一个Section。
Section之间可以有默认的连接顺序，在进行MontagePlay的时候默认会从第一个Section播放，如果未编辑Section连接，则会在第一个Section播放完结束。按照下图编辑可以实现自动连接Section，甚至形成循环（灰蓝色部分）。
Vertex Animation Tool(顶点动画工具)

AnimBlueprint(动画蓝图)
Animation State Machine(动画状态机)
## State Machines(状态机)
Transitions

Animation Blend(混合动画)
Blend by Variable(混合动作)
Use Cached Pose
Layeredblend Per Bone
Inverse Kinematics(反向动力学)

|Name                               |Desc   |
|   ---                             |   ---|
|USkeletalMesh                      |绑定到可以为使网格变形而设置的骨骼层级骨架的几何体。由两部分组成：构造网格表面的多边形和可用于为多边形设置动画的层级骨架。|
|FSkeletalMeshRenderData            |渲染数据，包括LOD渲染数据等。|
|FSkeletalMeshLODRenderData         |骨骼蒙皮LOD渲染数据，包括蒙皮权重、皮肤权重配置文件数据等。|
|USceneComponent                    |需要添加到FScene中的任意对象的基础类，如光照、网格体、雾等。|
|UPrimitiveComponent                |基元组件，是可视性和相关性的基本单位。可渲染或进行物理交互（碰撞检测）的任意资源的基础类。也可以作为可视性剔除的粒度和渲染属性规范（投射阴影等）。与所有UObjects一样，游戏线程拥有所有变量和状态，渲染线程不应直接对其进行访问。是一些可见几何体的父类，比如：ShapeComponents(Capsule, Sphere, Box)：用于碰撞检测但不渲染。StaticMeshComponent、SkeletalMeshComponent：包含渲染的预构建几何体，也可用于碰撞检测。|
|UMeshComponent                     |继承自UPrimitiveComponent，是所有具有可渲染的三角形集合实例的组件的抽象基类，如：UStaticMeshComponent、USkeletalMeshComponent。|
|UStaticMeshComponent               |继承自UMeshComponent，用于创建UStaticMesh实例，静态网格是由一组静态多边形组成的几何体。|
|USkinnedMeshComponent              |继承自UMeshComponent，支持骨骼蒙皮网格渲染，不支持动画。提供USkeletalMesh接口等。|
|USkeletalMeshComponent             |继承自USkinnedMeshComponent，创建带动画的SkeletalMesh资源的实例。支持动画。|
|FScene                             |UWorld的渲染器版本。对象仅在其被添加到FScene（注册组件时调用）后才会存在于渲染器中。渲染线程拥有FScene中的所有状态，游戏线程无法直接对其进行修改。主要操作是添加或删除图元（Primitive）和灯光。|
|FPrimitiveSceneProxy               |UPrimitiveComponent的渲染器版本，为渲染线程映射UPrimitiveComponent状态。存在于引擎模块中，用于划分为子类以支持不同类型的基元（骨架、刚体、BSP等）。实现某些非常重要的函数，如GetViewRelevance、DrawDynamicElements等。|
|FPrimitiveSceneInfo                |内部渲染器状态（FRendererModule实现专用），对应于UPrimitiveComponent和FPrimitiveSceneProxy。存在于渲染器模块中，因此引擎看不到它。单个UPrimitiveComponent的内部渲染器状态。|
|FDynamicSkelMeshObjectDataGPUSkin  |存储顶点蒙皮所需的更新矩阵。|
|FSkeletalMeshObjectCPUSkin         |CPU蒙皮网格渲染数据。|
|FSkeletalMeshObjectStatic          |静态网格渲染数据。|
|FSkeletalMeshObjectGPUSkin         |GPU蒙皮渲染数据。包含FDynamicSkelMeshObjectDataGPUSkin& DynamicData，它用于动态更新和渲染。|
|FGPUBaseSkinVertexFactory          |存储皮肤处理所需的骨基质，以及对GPU皮肤顶点工厂着色器代码需要用作输入的各种顶点缓冲区的引用。|