---
title: RHI
date: 2022-01-01 09:37:00
categories:
- Engine
- Unreal
- GPU
- RHI
tags:
    - Engine
    - Unreal
    - GPU
    - RHI
---
RHI是Render Hardware Interface的缩写，虚幻引擎通过RHI把各个平台的图形API包装成统一接口，供上层渲染来使用，让业务不用过多关心API细节（实际还是需要关心RHI细节）。从代码结构上来看，RHI封装的比较贴合于现代的图形API（vulkan,metal,DX12),也支持opengl/opengles。这个接口是广义上的概念，不仅指C++的纯虚基类，也包括一些全局变量，全局函数等，具体形式就像下面的RHI.h头文件这样：
![RHI.h](1.png)

Shader枚举类型
```C++
enum FShaderFrequency : uint8
{
    SF_Vertex   = 0, // 顶点着色器
    SF_Hull     = 1, // 曲面细分着色器
    SF_Domain   = 2, // 域着色器
    // 上面2个都是用来做曲面细分的
    SF_Pixel    = 3, // 像素着色器
    
    SF_Geometry = 4, // 几何着色器
    SF_Compute  = 5, // 计算着色器
    SF_RayGen   = 6,
    SF_RayMiss  = 7, // 
    SF_RayHitGroup = 8,
    SF_RayCallable = 9,

    SF_NumFrequencies = 10,

    // SM5风格的标准图形管线所需要的着色器（不包括计算着色器）
    SF_NumGraphicsFrequencies = 5,
    // SM5风格的标准图形管线所需要的着色器（包括计算着色器）
    SF_NumStandardFrequencies = 6,

    SF_NumBits = 4,
};
```