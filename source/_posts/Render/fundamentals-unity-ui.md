---
title: Unity UI优化（一）-Unity UI的基本原理
mathjax: true
date: 2019-04-09 01:32:27
tags:
categories:
---
# 术语
画布（Canvas）是以原生代码编写的Unity组件，它给Unity的渲染系统按层划分的几何系统，可以在其内部或其上层绘制其他几何形状。
画布负责将其内部的几何形状合并到批处理、生成合适的渲染指令并发送到Unity图形系统。这些操作都是由原生C++代码完成，这杯成为重新批处理（rebatch）或批处理构建（batch build）。当一个画布被标记为含有需要重新批处理的几何形状时，称这个画布为脏（dirty）画布。
由CanvasRenderer组件向画布提供几何形状。
子画布（Sub-canvas）是嵌套在其他画布组件内部的画布组件。子画布能够将其孩子节点与其画布隔离开，一个被标记为脏的子节点不会迫使其父画布重新构建几何内容，反之亦然。有几种特殊情况会使上述情形失效，比如，改变父画布导致子画布改变尺寸。
Graphic类是由Unity UI系统的C#库提供的基类，所有的画布系统提供可绘制几何内容的UI系统C#类都继承它。大多数内置的UI系统绘制类都是通过MaskableGraphic子类实现的，这个子类实现了IMaskable接口，可以被遮罩。Drawable类的主要子类是Image和Text，它们能提供与其名称相对应的内容。

Layout组件控制RectTransform的尺寸和位置，它通常用于创建具有复杂布局并且内部组件需要相对尺寸或者相对位置的UI。Layout组件只依赖RectTransform并且只影响与其关联的RectTransform的属性。他们不依赖Graphic类，并且可以独立于UI系统的Graphic类使用。
Graphic和Layout组件都依赖