---
title: Unity ShaderLab
date: 2018-05-19 10:56:45
tags:
  - Unity
categories: "Unity Shader"
---
# CG类型

|Type|Description|
|--|--|
| float |     32位浮点数据|
| half  |     16位浮点数据|
| int   |     32位整形数据|
| fixed |     12位定点数|
| bool  |     布尔数据|
| sampler|    纹理对象的句柄，分为sampler、sampler1D、sampler2D、sampler3D、samplerCUBE和samplerRECT。|

float2x4 matrix // 表示2x4阶矩阵，包含8个float类型数据

使用技巧：

- 精度够用就好
- 颜色和单位向量，使用fixed
- 其他情况，尽量使用half（即范围在[-6万，）

内置数据类型：基于基础数据类型，如float3，表示float3类型的三维向量；同理，bool2表示布尔类型的二维向量。

向量最长不能超过四元，如float5 vector; //编译错误

Swizzle操作符：它可以将一个向量的成员取出组成一个新的向量。对于坐标或者角度等其它多维向量，Swizzle操作符(.)后接x、y、z、w分别表示原始向量的第一个、第二个、第三个和第四个元素；同样，对于颜色可以后接r、g、b和a来表示同样的索引。
例如：
    
    float4(a,b,c,d).xwz 等价于 float4(a,d,c)
    float4(a,b,c,d).xxy 等价于 float4(a,a,b)

Swizzle操作符只能对结构体和向量使用，不能对数组使用。