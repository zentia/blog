---
title: 溶解
mathjax: true
date: 2019-03-31 23:03:23
tags:
	- Shader
categories: Unity
---
溶解，也就是让这个模型逐渐消失。那么，最简单的，直接让这个像素的Fragment Shader操作discard，这个像素就消失了。然后，我们要做的就是让这个溶解的对象一部分消失，另一部分存在，所以，这个时候我们就需要一个Mask图进行控制，然后用这个Mask值与我们设定的一个阀值来进行比较，小于阀值的部分discard，大于的部分正常计算。最终，我们将这个阀值从0逐渐增加到1，就可以实现模型的一部分像素先消失，直至整个模型完全消失的效果。