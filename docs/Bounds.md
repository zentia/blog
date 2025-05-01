---
title: UE4 Bounds详解
date: 2022-01-20 14:14:00
tags:
- Engine
- Unreal
- Other
categories:
- Engine
- Unreal
- Other
---
https://blog.csdn.net/Bob__yuan/article/details/119813418
任何的游戏引擎，最终呈现出在屏幕上的效果，都是经过很多变换和计算的，然后才转换成屏幕像素空间才能被用户看到，这个过程中，相机位置，FOV，物体位置，透明度，深度值等等信息，都会影响物体在最终屏幕上的位置和大小，如果每个物体的每个点都要计算，就很耗，所以每个物体先进行可见性剔除，对于完全不可见的物体，就可以在流程中很早的舍弃掉，减少很多计算。
Bounds就是用于进行可见性剔除的，对于Bounds都不在摄像机的视锥体范围内的物体，直接就不画了。
UE中的 Bounds都是AABB（Axis-Aligned Bounding Box）的，即轴平行包围盒，类型是`FBoxSphereBounds`。不管是实时计算的，还是固定（Fixed）的，都会因为旋转而变化，使得始终与世界的XYZ轴平行。
# `Bounds Scale` - Bounds缩放
# `Use Attach Parent Bounds`-使用父节点 Bounds
```C++
/** If true, this component uses its parents bounds when attached.
 *  This can be a significant optimization with many components attached together.
 */
UPROPERTY(EditAnyWhere, BlueprintReadWrite, AdvancedDisplay, Category=Rendering)
uint8 bUseAttachParentBound:1;
```
所在类: `USceneComponent`
用途：如果勾选了这个Component的Bounds不自己计算，而是直接使用Parent的，从注释也可以看到，这个选项如果选了，可以大幅提升了一个很多个Component在一个Parent下的效率。

# `Include Component Location Into Bounds`

在`USkeletalMeshComponent::CalcBounds` 中，会判断是否勾选，如果勾选了，则会根据`ComponentLocation`重新计算`Bounds`

# `Use Bounds from Master Pose component`
类：`USkinnedMeshComponent`
```C++
/**
 * When true, we will just using the bounds from our MasterPostComponent. This is useful for when we have a Mesh Parented
 * to the main SkelMesh (e.g. outline mesh or a full body overdraw effect that is toggled) that is always going to be the some
 * bounds as parent. We want to do no calculations in that case.
 */
UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = SkeletalMesh)
uint8 bUseBoundsFromMasterPoseComponent:1;
```
是否采用主姿势组件的Bounds
# `Skip Bounds Update When Interpolating`
插值的时候，是否跳过Bounds
# `Component Use Fixed Skel Bounds` - 使用固定Bounds
# `Consider All Bodies for Bounds` -考虑所有部位

Play之后，在命令行窗口`Show Bounds`即可显示所有的Bounds