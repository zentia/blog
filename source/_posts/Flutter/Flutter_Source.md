---
title: Flutter源码导读
---
Flutter的总体架构视角，总共分为3层：
- 负责嵌入宿主的Embedder
- 负责渲染、通信、逻辑（Dart）的核心模块：Engine
- 负责提供UI开发上层接口的Framework层（同时处理了手势等基础能力）

{% asset_img archdiagram.png %}

# Embedder架构（Android）

{% asset_img Flutter_Source2.png %}
{% asset_img Flutter_Source3.png %}
Engine继续拆分，可以分别得到Surface和DartVM的封装架构（对应Embedder层的FlutterView和FlutterEngine）
{% asset_img Flutter_Source4.png 1440 1024 %}
Dart持有与管理（负责逻辑，注意图中Mapping及其类的箭头画反了）
{% asset_img DartVM.png 1440 1024 %}
Framework的架构（负责提供上层开发者接口，根据Binding命名可以知道其主要职责）
{% asset_img Framework.png 1440 1024 %}
总结
{% asset_img Summary.png 1440 1024 %}

# 启动流程

Embedder层启动的核心要点（隐藏在各平台宿主的生命周期回调中）：

{% asset_img Embedder.png 1440 2000 %}

Engine本身没有启动能力，是Embbdder在启动过程中，通过`Java->C++`或者`OC->C++`调用触发的（同样，要注意每个启动流程对应的线程）

{% asset_img Engine.png 1440 1024 %}

Framework的启动通过Engine的`C++->Dart`调用触发的，主要逻辑就是`runApp`的逻辑：

{% asset_img FrameworkLoad.png 1440 1024 %}

# 渲染管道

渲染管道的全局视角

{% asset_img RenderPipeline.png 1440 1024 %}

下面分析每个阶段的逻辑（特别要注意无处不在的服用，手势和动画比较独立，在此不做展开讨论）

- 三棵树的完整创建流程：

{% asset_img ThreeTreeCreate.png 1440 1024 %}

{% asset_img ThreeTreeCreate1.png 1440 1024 %}

- Vsync: Framework每一帧的渲染都以监听Embedder的Vsync信号开始

{% asset_img Vsync.png 1440 1024 %}

- Continuation机制是渲染开始前的一个重要逻辑，负责保证逻辑的一致性、限流和解耦。

{% asset_img Continuation.png 1400 1100 %}

- 三棵树更新（注意脏节点排序，避免冗余的build）

{% asset_img ThreeTreeUpdate.png 1400 1100 %}

- Render Tree的更新（layout，重新计算每个RenderObject的大小和偏移）：

{% asset_img RenderTreeLayout.png 1400 1100 %}

- Layer Tree的更新（paint，注意Layout Tree时Flutter体系的第4颗树）

{% asset_img LayoutTreeUpdate.png 1400 1100 %}

- Layer Tree时Engine需要的数据，所以会有一个Dart到C++的映射，如下：

{% asset_img LayerTreeUpdateComposition.png 1400 1100 %}

- Composition时Engine层的操作，数据来源时Framework产生的Layer Tree

{% asset_img SceneUpdate.png 1400 1100 %}

- Raster是一个偏绘制层的操作，可以用火焰图观察：

{% asset_img ConsumeDataFrame.png 1400 1100 %}

# 布局深入

Flutter中主要有两种布局模型Box和Sliver，以下是Box布局模型：

{% asset_img BoxMode.png 1400 1100 %}

Sliver布局模型过于复杂，如果有机会，后面单独开讲

# Framework其他功能

{% asset_img FrameworkOtherFeature.png 1400 1100 %}

# Embedder其他功能

- 通过PlatformChannel支持逻辑复用

{% asset_img PlatformChannel.png 1400 1100 %}

- 通过PlatformView支持UI复用

{% asset_img PlatformView.png 1400 2200 %}

注意PlatformView的性能很大，基本不适合业务场景。

- Plugin一方面提供了生命周期感知能力，另一方面提供了工程上可复用的代码组织形式

{% asset_img Plugin.png 1400 1100 %}

# 总结

{% asset_img Summary1.png %}