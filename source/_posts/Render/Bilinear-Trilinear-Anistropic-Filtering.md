---
title: 纹理过滤模式中的Bilinear、Trilinear以及Anistropic Filtering
mathjax: true
date: 2019-09-05 21:02:39
tags:
categories:
---
# 为什么在纹理采样时需要texture filter（纹理过滤）
我们的纹理是要贴到三维图形表面的，而三维图形上的pixel中心和纹理上的texel中心点并不一致（pixel不一定对应texture的采样中心texel），大小也不一定一致。当纹理大于三维图形表面时，导致一个像素被映射到许多纹理像素上；当纹理小于三维图形表面时，许多个像素都映射到同一个纹理。
当这些情况发生时，贴图就会变得模糊或发生错位，马赛克。要解决此类问题，必须通过技术平滑texel和pixel之间的对应。这种技术就叫纹理滤波。
不同的过滤模式，计算复杂度不一样，会得到不同的效果。过滤模式由简单到复杂包括：Nearest Point Sampling（最近采样点），Bilinear（双线性过滤），Trilinear（三线性过滤）、Anisotropic Filtering（各项异性过滤）。
# 什么是各向同性和各向异性？
当需要贴图的三维表面平行于屏幕（viewport），则是各向同性的。当要贴图的三维表面与屏幕有一定角度的倾斜，则是各向异性的。
# Nearest Point Sampling（最近点采样）
这个最简单，每个像素的纹理坐标，并不是刚好对应Texture上的一个采样点texel，怎么办呢？最近点采样取最接近的texel进行采样。
当纹理的大小与贴图的三维图形的大小差不多时，这种方法非常有效和快捷。如果大小不同，纹理就需要进行放大或缩小，这样，结果就会变得矮胖、变形或模糊。