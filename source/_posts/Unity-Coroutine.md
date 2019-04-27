---
title: Unity ECS
date: 2018-04-16 19:47:16
tags:
	- Unity
---
# Coroutines

WaitForENdOffFrame 帧末尾执行
yield return null 表示暂缓一帧，在下一帧接着往下处理，于yield return 0 或 yield return 1一样的功能
yield return new WaitForSeconds
协程的真正用途是分布做一个比较耗时的事情，比如游戏里面的加载资源

# Unity面对的问题

- 面向对象的编程方式（曾经的圣典，已经跟不上时代，OOP最大的问题是数据和逻辑混在一起，而现在我们要数据驱动模型）
- 由Mono编译的非最优机器码
- GC机制
- 单线程开发

## 工作流程

- 创建一个GameObject对象；
- 在对象上添加组件：Renderer，Collider，Rigidbody physics；
- 创建MonoBehaviour脚本并将其添加到对象中，以便在运行时控制和更改这些组件的状态属性；

以上3个步骤执行，我们成为Unity的执行流程，作为Unity开发者来说，这个是最基本的流程。
但是这种做法有它自己的缺点和性能问题。比如数据和逻辑是紧密耦合的，这意味着代码重用的频率比较低，因为逻辑与特定数据相关联，无法单独分离出来。
例如下图所示的GameObject和Components实例中，GameObject依赖于Transform、Renderer、Rigidbody和Collder引用，这些脚本中的引用对象分散在堆内存中。

<div align=center>{% asset_img 1.jpg %}</div>

游戏对象、其行为及其组件之间的内存引用：

<div align=center>{% asset_img 2.png %}</div>

Unity GameObject场景可以让游戏在非常短的时间内完成原型构建并运行，这个也是Unity的特色可以让开发者快速入手，但它对于性能来说不太理想。我们在深层次的探讨这个问题，每个引用类型都包含可能不需要访问的许多额外数据，这些未使用的成员也占用了处理器缓存中的宝贵空间。比如我们继承的Mono就是一个典型的案例，如果只需要现有组件的极少功能接口函数或者变量，则可以将其余部分视为浪费空间，如下面的“浪费空间”图所示：

<div align=center>{% asset_img 3.jpg %}</div>

在上图中，粗体表示实际用于移动操作的成员，其余的就是浪费空间，若要移动GameObject，脚本需要从Transform组件访问位置和旋转数据成员。当硬件从内存中获取数据时，缓存航中会填充许多可能无用的数据，如果只是为所有应该移动的GameObjects设置一个只有位置和旋转成员的阵列，这将能够在很短的时间内执行，如何去掉无用的数据？ECS就是味解决此问题而设计的。

# ECS实体组件系统

Unity的新实体组件系统可帮助消除低效的对象引用，我们考虑只包含它所需数据的实体，而不考虑自带集合的GameObjects。
在下面的实体组件系统中，请注意Bullet实体没有附件Transform或Rigidbody组件，Bullet实体只是显示运行更新所需的原始数据，借助这个新系统，您可以将逻辑与各个对象类型完全分离。

<div align=center>{% asset_img 4.jpg %}</div>

这个系统具有很大的优势：它不仅可以提供缓存效率，缩短访问时间；它还支持在需要使用这种数据对齐方式的现代CPU中采用先进技术（自动矢量化/SIMD）这为游戏提供了所需的默认性能。如下图所示：
<div align=center>{% asset_img 5.jpg %}</div>

上图请注意缓冲行存储中的碎片和继承Mono系统生成的空间浪费，数据对比如下所示：

<div align=center>{% asset_img 6.jpg %}</div>

上图是将与单个移动操作相关的内存空间与实现系统目标的两个操作进行对比的结果。