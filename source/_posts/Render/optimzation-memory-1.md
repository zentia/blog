---
title: optimzation_memory_1
date: 2018-07-05 11:25:56
tags:
	uwa
---
内存开销：

# 资源内存占用

在一个较为复杂的大中型项目中，资源的内存占用往往占据了总体内存的70%以上。因此，资源使用是否恰当直接决定了项目的内存占用情况。一般来说，一款游戏项目的资源主要可分为如下几种：纹理（Texture）、网格（Mesh）、动画片段（AnimationClip）、音频片段（AudioClip）、材质（Material）、着色器（Shader）、字体资源（Font）以及文本资源（Text Asset）等等。其中，纹理、网格、动画片段和音频片段则是最容易造成较大内存开销的资源。

## 一、纹理

纹理资源可以说是几乎所有游戏项目中占据最大内存开销的资源。一个6万面片的场景，网格资源最大才不过10MB，但一个2048x2048的纹理，可能直接就达到16MB。因此，项目中纹理资源的使用是否得当会极大地影响项目地内存占用。

### 纹理格式

纹理格式是研发团队最需要关注的纹理属性。因为它不仅影响着纹理的内存占用，同时还决定了纹理的加载效率。
在使用硬件支持的纹理格式时，你可能会遇到以下几个问题：

- 色阶问题
	由于ETX、PVRTC等格式均为有损压缩，因此，当纹理色查范围跨度较大时，均不可避免地造成不同程度地“阶梯”状的色阶问题。因此，很多研发团队使用RGBA32/ARGB32格式来实现更好的效果。但是，这种做法将造成很大的内存占用。比如，同样一张1024x1024的纹理，如果不开启Mipmap，并且PVRTC格式，则其内存占用为512KB，而如果转换为RGBA32位，则很可能占用达到4MB。

- ETC1不支持透明通道问题
	在Andorid平台上，对于使用OpenGL ES 2.0的设备，其纹理格式仅能支持ETC1格式，该格式有个较为严重的问题，即不支持Alpha透明通道，使得透明贴图无法直接通过ETC1格式来进行储存。对此，我们建议研发团队将透明贴图尽可能拆分成两张，即一张RGB24位纹理记录原始纹理的颜色部分和一张Alpha8纹理记录原始纹理的透明通道部分。然后，将这两张贴图分别转化为ETC1格式的纹理，并通过特定的Shader来进行渲染，从而达到支持透明贴图的效果。这种方法极大程度伤毕竟RGBA透明贴图的渲染效果，同时降低纹理的内存占用。

### 纹理尺寸

一般来说，纹理尺寸越大，则内存占用越大。所以，尽可能降低纹理尺寸，如果512x512的纹理对于显示效果已经够用，那么就不要使用1024x1024的纹理，因为后者的内存占用时前者的4倍。

### Mipmap功能

Mipmap旨在有效降低渲染带宽的压力，提升游戏的渲染效率。但是，开启Mipmap会将纹理内存提升1.33倍。对于具有较大纵深感的3D游戏来说，3D场景模型和角色我们一般是建议开启Mipmap功能的，而绝大多数UI均是渲染在屏幕最上层，开启Mipmap并不会提升渲染效率，反倒会增加无谓的内存占用。

### Read & Write

一般情况下，纹理资源的"Read & Write"功能在Unity引擎中默认是关闭的。开启该选项将会使纹理内存增大一倍。

## 二、网格

### Normal、Color和Tangent

Mesh资源的数据中经常会含有大量的Color数据、Normal数据和Tangent数据。这些数据的存在将大幅度增加Mesh资源的文件提及和内存占用。其中，Color数据和Normal数据主要为3DMax、Maya等建模软件导出时设置所生成，而Tangent一般为导入引擎的生成。

更为麻烦的是，如果项目对Mesh进行Draw Call Batching操作的话，那么将很有可能进一步增大总体内存的占用。比如，100个Mesh进行拼合，其中99个Mesh均没有Color、Tangent等属性，剩下一个则包含有Color、Normal和Tangent属性，那么Mesh拼合后，CombinedMesh中将为每个Mesh来添加上此三个顶点属性，进而造成很大的内存开销。

# 引擎模块自身内存占用

引擎自身中存在内存开销的部分纷繁复杂，可以说是由巨量“微小”内存所积累起来的，比如GameObject及其各种Component（量最大的Component应该算是Transform）了、ParticleSystem、MonoScript以及各种各样的模块Manager（SceneManager、CanvasManager、PersistentManager）等。

一般情况下，上面所指出的引擎各组成部分的内存开销均比较小，真正占据较大内存开销的这两处：WebStream和SerializedFile。其绝大部分的内存分配则是由AssetBundle加载资源所致。当使用new WWW或CreateFromMemory来加载AssetBundle时，Unity引擎会加载原始数据到内存中并对其进行解压，而WebStream的大小则是AssetBundle原始文件大小+解压后的数据大小+DecompressionBuffer(0.5MB)。同时，由于Unity5.3版本之前的AssetBundle文件为LZMA压缩，其压缩比类似于Zip(20%-50%)，所以对于1MB的原始AssetBundle文件，其加载后WebStream的大小则可能时5~6MB，因此，当项目中通过new WWW加载多个AssetBundle文件，且AssetBundle又无法即时释放时，WebStream的内存可能会很大。

对于SerilizedFile，则是当你使用LoadFromCacheOrDownload、CreateFromFile或new WWW本地AssetBundle文件时产生的序列化文件。

对于WebStream和SerializedFile，需要关注两点：

- 是否存在AssetBundle没有被清理干净的情况。可以通过Unity Profiler直接查看其具体的使用情况，并确定Take Sample时AssetBundle的存在是否合理；
- 对于占用WebStream较大的AssetBundle文件（如UI Atlas相关的AssetBundle文件等），建议使用LoadFromCacheOrDownLoad或CreateFromFile来替换，即将解压后的AssetBundle数据存储与本地Cache中进行使用。这种做法非常适合于内存特别吃紧的项目，即通过本地的磁盘控件来存储内存空间。

# 托管堆内存占用

对于目前绝大多数基于Unity引擎开发的项目而言，其托管堆是由Mono分配和管理的。“托管”的本意是Mono可以自动地改变堆地大小来适应你所需要的内存，并且适当地调用垃圾回收（Garbage Collection操作来释放已经不需要地内存，从而降低开发人员再代码内存管理方面的门槛。

目前Unity所使用的Mono版本存在一个严重的问题，即：Mono的堆内存一旦分配就不会返还给系统。这意味着Mono的堆内存是只升不降的。举个粒子，项目运行时，再场景A中开辟了60MB的托管堆内存，而到下一个场景B时，只需要使用20MB的托管堆内存，那么Mono中将会存在40MB空闲的堆内存，且不会返还给系统。

- 高频率地New Class/Container/Array等。不要再Update、FixUpdate或较高调用频率地函数中开辟堆内存。

项目中较为合理地内存分配：

- 纹理资源：50MB
- 网格资源：20MB
- 动画片段：15MB
- 音频片段：15MB
- Mono堆内存：40MB
- 其它：10MB

