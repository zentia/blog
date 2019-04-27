---
title: 基于物理的着色：BRDF
date: 2018-06-09 11:34:41
tags:
---
在计算机图形学领域，着色（Shading）是指根据表面或者多边形相对光源和相机的角度和距离来计算它的颜色的过程。不同的用户可以使用不同的着色算法，CAD等追求相应速度的交互式图形领域可以使用简单快速的着色算法，卡通油画等艺术效果使用非真实感（Nonphotorealistic）着色算法，而追求真实感的CG电影或游戏则可以使用基于物理建模的着色算法。
本文关注基于物理着色的BRDF模型，希望能将BRDF的来龙去脉将清楚，并分析Cook-TorranceBRDF公式的推倒过程。
要模拟真实光照，我们先要弄清楚光照的现象。

## 光照现象

光由光子组成，光子即具有粒子的特性，又表现出波的特性。从波的角度看，光是电磁波的一种，不同频率（波长）的光波能量不同，频率越高（波长越短），能量越高，频率越低（波长越长），能量越低，其中波长在380nm-780nm范围内的光波能被认类的视网膜感知到，这个范围的光波成为可见光，不同频率的可见光被人感知为不同的颜色，频率越高的光偏蓝，频率较低的光则偏红。
<img src="http://oxcvfpext.bkt.clouddn.com/BRDF-1.jpg">
