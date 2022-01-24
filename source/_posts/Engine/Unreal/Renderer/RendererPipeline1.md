---
title: RenderPipeline1
date: 2022-01-06 10:47:00
categories:
- Engine
- Unreal
- Renderer
---
游戏画质与性能、功能不可兼得，我们要在三者之间寻找平衡。
![1](/images/Unreal/Renderer/RendererPipeline/1.png)

1.2 Scene Draw Order
![1](/images/Unreal/Renderer/RendererPipeline/3.jpg)

- Z-PrePass: 场景中的opaque和mask材质写一遍深度。
- Compute light grid: 场景中的灯光按照屏幕空间分成相应的grid，仅限点光源聚光灯。
- Build HZB: 生成场景的Hierarchical Z。
- Shadow Depth Pass: 根据不同灯光类型渲染出2DShadowmap或cubemapshadowmap，供后面lightingmass使用。
- BasePass: 渲染出GBuffer, customdepth/stencil，还有velocity buffer用于后面的motion blur和TAA。
- PreLightingPass: 计算DeferredDecal和SSAO。
- LightingPass: 计算直接光照阴影和间接光照阴影。
- Reflections: 计算SSR（屏幕空间反射）或光追反射。
- Additional: 各种无效，半透明。
- Post Process: Temproal AA、EyeAdaption、Motion Blur、Bloom、Tone Mapping、PP Material等。

https://interplayoflight.wordpress.com/2017/10/25/how-unreal-renders-a-frame/

