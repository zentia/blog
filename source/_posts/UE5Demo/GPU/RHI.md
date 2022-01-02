---
title: RHI
date: 2022-01-01 09:37:00
tags:
    - Unreal
---
RHI是Render Hardware Interface的缩写，虚幻引擎通过RHI把各个平台的图形API包装成统一接口，供上层渲染来使用，让业务不用过多关心API细节（实际还是需要关心RHI细节）。从代码结构上来看，RHI封装的比较贴合于现代的图形API（vulkan,metal,DX12),也支持opengl/opengles。这个接口是广义上的概念，不仅指C++的纯虚基类，也包括一些全局变量，全局函数等，具体形式就像下面的RHI.h头文件这样：
![RHI.h](/images/Unreal/RHI/1.png)