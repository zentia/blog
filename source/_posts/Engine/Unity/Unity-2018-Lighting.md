---
title: Unity2018照明流程
mathjax: true
date: 2019-03-30 22:58:30
tags:
	- Unity-2018
categories: Unity-2018
---
目前Unity提供了多种渲染管道，两种全局照明系统，四种照明模式，三种灯光模式，以及两种Shadowmask模式，为开发者在创建面向高配PC，主机，移动和XR设备项目的过程中提供了高度灵活性。

渲染管线决定对象如何在场景中呈现出来，分为以下三个阶段
第一阶段：剔除（Culling）。在此阶段列出需要被渲染的对象，优先呈现摄像机可见的范围，以及未被其它物体遮挡的对象。
第二阶段：渲染（rendering）。在此阶段，根据光照设置以及相关的灯光属性，将对象绘制到基于像素的缓冲区种。
第三阶段：后处理（post-processing）。一般在缓冲区上执行操作，比如应用ColorGrading、Bloom、Depth of Field等效果，将最终输出到每一帧。
