---
title: Unreal CVar
date: 2022-01-02 14:18:00
tags:
    - Unreal
---
sg 是 scalability 的缩写 通过配置文件设置
# r.ScreenPercentage and sg.ResolutionQuality

https://answers.unrealengine.com/questions/735329/question-about-screen-percentage-and-resolution-qu.html?sort=oldest

如果你添加两行在DefaultScalability.ini并且运行命令"scalability auto"，引擎将设置一个sg.ResolutionQuality值基于你的GPU性能。
分辨率质量相当于屏幕百分比，所以"sg.ResolutionQuality 65"将自动设置"r.ScreenPercentage"到65。

# r.DefaultFeature.AntiAliasing
0 关闭
1 FXAA

# stat unit
查看耗时