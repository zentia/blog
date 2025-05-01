---
title: Unreal CVar
date: 2022-01-02 14:18:00
categories:
- Engine
- Unreal
tags:
    - Unreal
---
sg 是 scalability 的缩写 通过配置文件设置
# r.ScreenPercentage and sg.ResolutionQuality

https://answers.unrealengine.com/questions/735329/question-about-screen-percentage-and-resolution-qu.html?sort=oldest

如果你添加两行在DefaultScalability.ini并且运行命令"scalability auto"，引擎将设置一个sg.ResolutionQuality值基于你的GPU性能。
分辨率质量相当于屏幕百分比，所以"sg.ResolutionQuality 65"将自动设置"r.ScreenPercentage"到65。

`r.ScreenPercentage`用于减少内部实际渲染分辨率，画面会在重新放大

# r.DefaultFeature.AntiAliasing
0 关闭
1 FXAA

# stat unit
查看耗时 游戏线程时长、渲染线程时长、GPU时长

# stat Slow [-ms = 0.3] [-maxdepth = 5]

# ShowFlag
0 是 关闭 1 是开启
- ShowFlag.Translucency # 半透明
- ShowFlag.PostProcessing 
- ShowFlag.StaticMeshes
- ShowFlag.SkeletalMeshes
- ShowFlag.Particles
- ShowFlag.ReflectionEnvironment
- ShowFlag.InstancedStaticMeshed
- ShowFlag.DirectionalLights