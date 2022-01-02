---
title: GetTransientPackage
date: 2021-12-30 22:20:00
tags:
    - Unreal
---

https://docs.unrealengine.com/4.27/en-US/API/Runtime/CoreUObject/UObject/GetTransientPackage/

这是官方解释，返回临时的top-level package，绝对不会被保存。
感觉说了些什么，又感觉什么也没说。
有用的信息是top-level
为了解决这个问题，我们先来捋一下以下几个概念
# Object(对象)
所有继承自UObject类并创建的对象都称之为Object。排除通过new创建的对象，剩下几乎所有的对象都是Object对象。编辑器中拖动的Actor，蓝图，游戏中各种常见资源如贴图，地图，音频资源等。甚至包括C++源码中通过UClass声明的类，也是Object。不出意外的话，几乎99%的时间都是在和UObject打交道。假想一个FPS游戏中人物死亡的场景，从技术实现角度来讲，这个角色包括：
- 角色骨骼Skeleton
- 人物材质MaterialInstanceConstant
- 贴图Texture2D
- 等等

当角色死亡消失时，我们希望玩家看到的应该是整个角色完整的消失，并不希望将角色这个整体打散对待（例如，剩下一张头像贴图），也不希望把这项清理的工作交给编程人员，要求他们必须硬编码手动释放对象。这应该是一个自然递归的过程。UE4采用组合模式来解决此类问题。

# Outer(父对象)
所有的Object都拥有一个Outer，称之为父对象。每个Object都可以有（至多）一个Outer(父UObject)，且可以拥有任意数量的子Object，换句话说，每个子对象的类型必须是UObject或者必须派生自UObject，每个Object将自身的Outer存储在OuterPrivate信息中，子对象的信息则是构建Object调用AddObject函数，通过全局的FUObjectHashTables来存储的。除了自举