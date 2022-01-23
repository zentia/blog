---
title: RHI
date: 2022-01-22 09:27:00
categories:
- Engine
- Unreal
---
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