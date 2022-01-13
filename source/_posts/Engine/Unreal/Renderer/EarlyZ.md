---
title: EarlyZ的渲染优化
date: 2022-01-13 16:54:00
categories:
- Renderer
---
https://zhuanlan.zhihu.com/p/44245258
传统的3D硬件渲染管线中，Z test所处的位置如下：
-----------------
multitex | PixelShader?
fog effects
alpha test
stencil\depth test * 
alpha blending
dither
-----------------
可以看到，实际上在传统管线中，z test不可能用来剔除pixel shader片断的执行。但是，在当前的一些显卡中，很多都把z test 提前到了pixel shader之前的执行一次，就被称为Early-Z culling优化。
但是从管线次序中可以清楚地看到，alpha test成为了z test的限制，因为一旦打开了alpha test，对于alpha test中失败的像素Early-Z将不能正确的被判断。因此对于部分显卡硬件，一旦被关闭了alpha test，Early-Z Culling就会被自动打开。

为了充分体现出
