---
title: CSM
date: 2022-01-07 20:53:00
tags:
    - Unreal
    - Renderer
categories:
- Engine
- Unreal
- Renderer
---
本篇主要是给大家介绍关于Unreal CSM实时阴影的技术实现。
# CSM简介
CSM的基本概念很容易理解，相机平截头体的不同区域需要具有不同分辨率的阴影贴图，距离眼睛最近的物体比较远的物体需要更高的分辨率。事实上，当眼睛移动非常接近几何时，最接近眼睛的像素可能需要如此大的分辨率，甚至4096x4096的阴影贴图也不够。
CSM的基本思想是将截头锥体分割成多个截头圆锥体。为每个子树呈现阴影贴图；像素着色器然后从最接近匹配所需分辨率的地图采样，如下图所示：
![1](1.png)
在图1中，品质显示（从左到右）从最高到最低，表示阴影图的一系列网格，具有截头锥体（反向锥体为红色），显示了像素覆盖率如何收到不同分辨率阴影贴图的影响。阴影是质量最高的（白色像素），当比例为1：1的比例时，将光线中的像素映射到阴影贴图中纹素。当多个像素映射到相同的阴影纹素时，透视混叠以大的、块状的纹理贴图（左图）的形式发生。阴影贴图太大时，将被采样。如下图所示：
![2](2.png)
图2显示了图1中每个阴影贴图中最高质量部分的切口。具有最紧密放置的像素（在顶点处）的阴影贴图最接近摄像机。在技术上，这些时相同大小的地图，白色和灰色用于举例说明级联阴影贴图的成功，白色时理想的，因为它显示了良好的覆盖率-视空间像素和阴影贴图纹素的1：1比例。
CSM需要每帧执行以下步骤：
1. 将截锥体分为子块。
2. 计算每个子视锥体的正投影。
3. 为每个子视锥体渲染一个阴影贴图。
4. 渲染场景

a 绑定阴影贴图和渲染。
b 顶点着色器执行以下操作：
计算每个灯光的纹理坐标（除非在像素着色器中计算所需的纹理坐标）。转换灯光顶点，依次类推。
c 像素着色器执行以下操作：
确定正确的阴影贴图。
必要时转换纹理坐标。
处理灯光。
下面介绍如何分割视锥体，分割截锥体时创建子视锥体的行为，用于分割平截头体的一种技术是计算Z方向从零百分之一百的间隔。
每个间隔然后表示作为Z轴的百分比的近平面和远平面。
![3](3.png)
在实际操作中，重新计算每帧的平截头体分裂会导致阴影边缘闪烁。普遍接受的做法是使用每个场景的静态级联间隔集。在这种情况下，沿Z轴的间隔用于描述在分割截头锥体时发生的子节点，确定给定场景的正确大小间隔取决于几个因素。
相对于场景几何，相机方向影响级联间隔选择。例如，非常靠近地面的相机，例如足球游戏中的地面摄像机，具有与天空中的照相机不同的静态级联间隔。
![4](4.png)
图4，显示一些不同的摄像机及其各自的分区。当场景的Z范围非常大时，需要更多的分割平面。例如，当眼睛非常靠近地平面，但是遥远的物体仍然可见时，可能需要多个级联。分割平截头体，是得眼睛附近更多的分裂（透视混叠正在改变最快）也是有价值的。当大多数几何结构被集成到视锥体的小部分（例如俯视图或飞行模拟器）中时，需要更少的级联。
# CMS使用技巧
可调参数：
`r.ShadowQuality`
`r.Shadow.UnbuiltPreviewInGame`
`r.Shadow.UnbuiltNumWholeSceneDynamicShadowCascades`
`r.Shadow.DistanceScale` CSM近平面远平面距离缩放。
`r.Shadow.CSM.TransitionScale` CSM分级间过渡距离缩放。
`r.Shadow.CSM.MaxCascades`
`r.Shadow.MaxResolution`
`r.Shadow.MaxCSMResolution`
`r.Shadow.RadiusThreshold`
`r.CapsuleShadows`
# Unreal原理解析
主要代码文件`ShadowSetup.cpp`, `DirectionLightComponent.cpp`

# GenerateMeshDistanceFields

在ProjectSetting->Render->GenerateMeshDistanceFields 勾选重启
`DistanceFieldAtlas.cpp`
```c++
static TAutoConsoleVariable<int32> CVarDistField(
    TEXT("r.GenerateMeshDistanceFields"),
    0,
    TEXT("Whether to build distance fields to static meshes, needed for distance field AO, which is used to implement Movable SkyLight shadows.\n")
    
);
```