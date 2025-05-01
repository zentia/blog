---
title: 'UE Build System: Target and Module'
date: 2022-01-26 10:23:00
categories:
- Engine
- Unreal
- Build System
---

https://imzlp.com/posts/16643/

`*.Target.cs`与`*.Build.cs`是Unreal构建系统的实际控制者，UBT通过扫描这两个文件夹确定整个编译环境，它们也是本篇文章研究的重点。
它们的职责各不相同：
- `*.Target.cs`控制的生成的可执行程序的外部编译环境，就是所谓的Target。比如，生成的是扫描`Type`(Game/Client/Server/Editor/Program)，开不开启(`bForceEnableRTTI`)，CRT使用什么方式链接(bUseStaticCRT)等等。
- `*.Build.cs`控制的是Module编译过程，由它来控制所属Module的对其他Module的依赖、文件包含、链接、宏定义等等相关的操作，`*.Build.cs`告诉UE的构建系统，它是一个Module，并且编译的时候要做哪些事情。

以一言以弊之：与外部编译环境相关的都归`*.target.cs`管，与Module自身相关的都归`*.build.cs`管。