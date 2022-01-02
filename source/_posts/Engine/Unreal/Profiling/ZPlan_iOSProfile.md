---
title: iOSProfile
date: 2022-01-02 15:38:00
password: zplan
tags:
    - Unreal
---
本篇文章是私密文章，不会开放到外网。
以下测试均已iPhone8测试标准
# SkyLight
## 开启
![EnableSkyLight](/images/Unreal/ZPlan/EnableSkyLight.PNG)
![ActorScene](/images/Unreal/ZPlan/ActorScene.png)
角色和场景绘制耗时4.15ms(此时是LOD2的)
PostAO占了1.98ms
## 关闭
![NoSkyLightShadowDepth](/images/Unreal/ZPlan/NoSkyLightShadowDepth.png)
# Skeleton LOD
## MinLOD0
## MinLOD1
## MinLOD2
## ForceLOD2
# Shadow Quality
## ShadowDepth
不管如何设置，大小都是2048 x 2048，有点奇怪。
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth.png)
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth1.png)
这里可以看出顶点数越多，画阴影的时间也会越多。
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth2.png)
这里又比较奇怪了，顶点数也比较多，但是耗时却比较少。怀疑跟屏占比有关系。
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth3.png)
此时开启天光的阴影整体耗时为402.12us
![NoSkyLightShadowDepth](/images/Unreal/ZPlan/NoSkyLightShadowDepth.png)

![LUT](/images/Unreal/ZPlan/SkyLightLUTS.png)
耗时319.65us
![Tonemap](/images/Unreal/ZPlan/Tonemap.png)
Tonemap 674.33us
FXAA 538.36us
UpScale (300.97us)
## Quality 0
## Quality 1
# AntiAntiAliasing
iOS是必须开的，不开会白屏，目前不知道什么原因。
# Model RMA (Roughtess, Metallic, Ambient)
其实没啥意义，现在瓶颈主要是在GPU，而贴图卡顿主要是带宽，如果带宽不足的话，其实不管会卡GPU，而且会卡Draw，SkyLight就是这样子的，有一个2000多的uniformBuffer。
# ScreenPercentage
## 100
## 75
## 50
# Bloom
## Open
## Close
# SSR
## Open
## Close
# HDR(Tonemapper)
不支持，开了就卡死，材质找不到提示。暂时没时间去细察。PC上效果如下所示：
![LDR](/images/Unreal/ZPlan/LDR.png)
![LDRProfiling](/images/Unreal/ZPlan/LDR-Profiling.png)
可以看出，当关了HDR之后是没有tonemapper。

# PlanarReflection
![PlanarReflection](/images/Unreal/ZPlan/planarReflection.png)
比较不解的是，已经关掉了平面反射，但是 iOS还在跑，需要实验删掉看看情况是否好转。