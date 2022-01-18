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

https://blog.csdn.net/yinfourever/article/details/109822330?utm_source=app&app_version=4.21.0&code=app_1562916241&uLinkId=usr1mkqgl919blen

# 渲染漫谈：early-z、z-culling、hi-z、z-perpass到底是什么？

这四种技术本质上都是解决传统渲染管线中的同一个问题--过度绘制（OverDraw）。一个经典的渲染管线通常会依次经历**顶点阶段、光栅化、片元阶段和逐像素处理**。其中片段阶段会进行复杂的光照计算，是整个管线的性能瓶颈。而在逐像素阶段会对计算出来的片元值进行各种测试以判断这个片元会不会最终显示到屏幕上。