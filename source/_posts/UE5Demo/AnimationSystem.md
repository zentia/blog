---
title: Unreal Animation System
date: 2022-01-02 14:20:00
tags:
    - Unreal
---

# Animation System
## Skeleton(骨架)
骨骼资源是一切骨骼动画的基础，通过分层结构，一般描述一个生物及非生物的骨架形状。
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