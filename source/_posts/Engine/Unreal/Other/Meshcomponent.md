---
title: Unreal 网格组件
mathjax: true
date: 2021-07-02 20:22:38
categories: 
- Engine
- Unreal
tags:
    Unreal
---
本文是对https://medium.com/realities-io/creating-a-custom-mesh-component-in-ue4-part-0-intro-2c762c5f0cd6 的一篇译文和扩充

<!--more-->

# 介绍

本文讲述了如何使用UE4C++创建一个custom mesh component。如果你不知道啥是mesh component，或者你体会不到使用custom mesh component的好处（哈皮一下：你可以获取vertex shader 和
vertex buffer的控制权）。反正就是说你先看一下这个文章，然后你在决定看不看后续的文章。

# Mesh Components

![](1.jpeg)

[UMeshComponent](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Components/UMeshComponent/) 继承了上面哪些爹，所以自然也就有了哪些爹的功能。
UActorComponent可以附加给actor，USceneComponent可以做变换，UPrimitiveComponent可以渲染，而它自己包含了一些三角形之类的东西。
一个比较好的例子是[UStaticMeshComponent](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Components/UStaticMeshComponent/) 它可以渲染静态的资源。
这个例子中，渲染数据是静态的并且不会再运行中改变，但是mesh component也可以渲染动态的mesh data，不行你去瞅瞅[UProceduralMeshComponent](https://docs.unrealengine.com/4.26/en-US/API/Plugins/ProceduralMeshComponent/UProceduralMeshComponent/) 。
