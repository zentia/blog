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
};
```