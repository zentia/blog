---
title: Unity渲染管线
mathjax: true
date: 2019-04-09 01:32:27
tags:
- Engine
- Unity
categories:
- Engine
- Unity
---
渲染类型有4种：
1. `Unlit`无关的，UI使用
2. `ForwardLite`轻量级前向，场景使用
3. `PostUI`
4. `PlanerReflection`

OSGame渲染管线本质还是SRP模式的。

4种都会重载`OnRender(GfxRenderContext context, SRPCamera camera)`接口。
都会调用`context.SetupCameraProperties(camera);`此接口主要是设置相机属性。


# Instancing
1. `UNITY_VERTEX_INPUT_INSTANCE_ID` 用于在Vertex Shader输入 / 输出结构中定义一个语义为SV_InstanceID的元素。
2. `UNITY_INSTANCING_CBUFFER_START(name) / UNITY_INSTANCING_CBUFFER_END` 每个Instance独有的属性必须定义在一个遵循特殊命名规则的Constant Buffer中。使用这对宏来定义这些Constant Buffer。“name”参数可以是任意字符串。