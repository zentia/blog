---
title: 骨架网格体动画系统
sticky: 3000
date: 2022-01-02 14:20:00
categories:
- Engine
- Unreal
- Animation
tags:
- Engine
- Unreal
- Animation
---

# 基本资源介绍
![](unreal-animation.svg)

# 骨架（Skeleton）

- 骨骼层级信息
- 参考姿势信息
- 骨骼名称
- 插槽（Socket）信息
- 曲线（Curve）信息
- 动画通知（Animation Notify）信息
- 插槽数据（插槽名称、所属骨骼、Transform等）
- 虚拟骨骼信息
- 骨骼名称Index映射表
- 其它骨骼设置信息：包括位移重定向（ Translation Retarget）设置，LOD设置等信息

# 骨骼模型(Skeletal Mesh)

- 模型的顶、线、面信息
- 顶点的骨骼蒙皮权重
- 模型的材质信息
- 模型LOD信息
- 所属骨架
- Morph Target信息
- Physics Asset设置信息
- 布料系统相关设置

# 动画序列（Animation Sequence）

- 动画关键帧信息
- 包含运动的骨骼信息
- 每帧骨骼的Transform信息：包含了骨骼的旋转、唯一和缩放
- 动画通知信息：记录了触发通知的类型和时间
- 动画曲线信息：记录了随时间轴变化的曲线信息

# 参考
- [1] [UE4 动画系统 源码及原理剖析](https://blog.csdn.net/qq_23030843/article/details/109103433)
- [2] [骨架网格体动画系统](https://docs.unrealengine.com/5.3/zh-CN/skeletal-mesh-animation-system-in-unreal-engine/)

{% pdf skinned-mesh-and-character-animation-with-directx9.pdf %}