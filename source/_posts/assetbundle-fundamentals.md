---
title: Unity资源管理（三）-AssetBundle基本原理
mathjax: true
date: 2019-04-09 01:43:10
tags:
    - Unity
categories: Unity
---
# 概述
AssetBundle系统提供了将一个或多个文件存储到Unity能够进行索引和序列化的档案格式中方法，它是Unity用来在应用程序安装之后进行分发和更新非代码内容的首选工具。通过AssetBundle，开发者可以提交更小的程序安装包、最小化运行时内存压力以及根据终端用户设备选择性地加载优化内容。

# AssetBundle布局

简单来说，一个AssetBundle中包含两部分：数据头和数据段。
数据头中含有AssetBundle的相关信息，例如标志符（Identifier）、压缩类型（Compression Type）和配置文件（Manifest）。配置文件是一个以Object名称为 键的查找表，表中的每个条目头提供了一个用于标识Object在数据段中的位置的byte索引。在大多数平台上，这个查找表是用平衡查找树实现的，在Windows和OSX衍生平台（包括IOS）上的查找表是使用红黑树实现的。
因此，构建配置文件所需的时间随着AssetBundle中的Asset数量而增长的速度大于线性增长。

数据段中含有由序列化AsseBundle中的Asset而生成的原始数据。如果指定了压缩方案为LZMA，所有序列化的Asset会被压缩成一个字节数组中；如果指定了压缩方案位LZ4，不同的Asset的字节数据会被单独压缩；如果没有使用任何压缩，数据段会保留原始的字节流数据。

在Unity5.3之前的版本中，AssetBundle中的Object不能被单独压缩。因此，在5.3之前的版本的Unity中，如果要从已压缩的AssetBundle中读取Object，引擎必须压缩整个AssetBundle。通常情况下，Unity会缓存一份解压后的AssetBundle副本，以此来提高加载性能。

# 加载AssetBundle

AssetBundle可以通过5个不同的API来加载，这5个API会受下面的两种因素的影响而产生不同的行为；
1. AssetBundle使用了LZMA压缩方式或者LZ4压缩方式或者没有进行压缩
2. 进行加载AssetBundle的平台

这些API是：
- AssetBundle.LoadFromMemory（可选择异步模式）Unity不推荐使用这个API
- AssetBundle.LoadFromFile（可选择异步模式）
- AssetBundle.LoadFromStream（可选择异步模式）
- UnityWebRequest的DownloadHandlerAssetBundle
- WWW.LoadFromCacheOrDownload（Unity5.6以及更高版本）

通过这些API获取的AssetBundle引用可以随意混用，也就是说，通过UnityWebRequest加载的AssetBundle可以兼容通过AssetBundle.LoadFromFile或者AssetBundle.LoadFromMemoryAsync加载的AssetBundle。

AssetBundle.LoadFromMemoryAsync从托管代码的字节数组（C#中的byte[]）中加载AssetBundle。该方法总是将托管代码中的源数据赋值到新分配的连续的内存块中。如果AssetBundle使用了LZM压缩，在赋值过程中AssetBundle会被解压；如果使用了LZ4压缩或者没有压缩，AssetBundle会被逐字复制（be copied verbatim）。

这个API占用内存的峰值至少是他处理的AssetBundle大小的两倍：一份由此API创建在本机内存中的副本和一份传递给此API的位于托管字节数组的副本。通过此API从AssetBundle加载的Asset会在内存中复制3次；在托管代码中的字节数组、本机内存中的副本以及在GPU或系统内存中的Asset本体。