---
title: iOSProfile
date: 2022-01-02 15:38:00
password: zplan
tags:
    - Unreal
---
本篇文章是私密文章，不会开放到外网。
以下测试均已iPhone8测试标准
通过我们大量测试，Game线程 Draw线程 RHI(GPU)三者只要有一个高于10ms，过一段时间必然发烫降频。所以，目标是在所有机型，这三个线程的耗时需要控制到10ms以内。
# SkyLight
## 开启
![EnableSkyLight](/images/Unreal/ZPlan/EnableSkyLight.PNG)
### 阴影
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth3.png)
此时开启天光的阴影整体耗时为402.12us
### 角色场景
![ActorScene](/images/Unreal/ZPlan/ActorScene.png)
角色和场景绘制耗时4.15ms(此时是LOD2的)
PostAO占了1.98ms
总共6.6ms
### 后处理
![LUT](/images/Unreal/ZPlan/SkyLightLUTS.png)
耗时319.65us
![Tonemap](/images/Unreal/ZPlan/Tonemap.png)
Tonemap 674.33us
FXAA 538.36us
UpScale (300.97us)
总共1.814.14ms
总体耗时：0.4+6.6+1.8=8.8ms

## 关闭
![NoSkyLight](/images/Unreal/ZPlan/NoSkyLight.PNG)
### 阴影 (r.ShadowQuality)
![NoSkyLightShadowDepth](/images/Unreal/ZPlan/NoSkyLightShadowDepth.png)
227us
### 角色场景
![NoSkyLightRender](/images/Unreal/ZPlan/NoSkyLightRender.png)
6.37ms
### 后处理
![NoSkyLightPostProcess](/images/Unreal/ZPlan/NoSkyLightPostProcess.png)
3.8ms
图中框住的部分，可以通过关闭HDR去掉，但是去掉的效果就是会很油腻，很黑。
也就是说场景分了3部分渲染，第一部分是Shadow第二部分是正常绘制，第三部分是后处理总共大概
6.37+1.778+0.227=8.375ms左右，但是手机上显示16ms左右，还有其它几毫秒的偏差，暂时不知道哪里造成的。（也可能不是同一帧截的，暂时忽略）
结论：在iPhone8 上开启天光和关闭天光只有0.4ms的差距，可以开启，但是Android之前测试的情况，会有一些莫名的uniformbuffer导致性能下降，等定位到问题，android在开。
# LOD
LOD 这里只测试强制LOD，因为不一定能截取到同一画面。导致数据差异性会大一些。
这里的LOD只是角色的LOD场景的LOD并没有测试。
可以看出会有1.3ms的左右的提升，内存也会有所降低，DrawCall降低了一半以上。但是性能提升效果其实并不明显，但是比天光要好一些。
## ForceLOD1
![LOD1](/images/Unreal/ZPlan/LOD1.PNG)
## ForceLOD2
![LOD2](/images/Unreal/ZPlan/LOD2.PNG)

# Shadow Quality
这里主要比较开启和关闭就可以了，主要是为了达到android效果，开太多也没有必要。
这里也可以看出，开启shadow和关闭shadow大概也是1.3ms左右的效率。
## Quality 0
![Shadow0](/images/Unreal/ZPlan/Shadow0.PNG)
![Shadow1](/images/Unreal/ZPlan/Shadow1.PNG)
## Quality 1
# AntiAntiAliasing (r.DefaultFeature.AntiAliasing)
iOS是必须开的，不开会白屏，目前不知道什么原因。
# Model RMA (Roughtess, Metallic, Ambient)
其实没啥意义，现在瓶颈主要是在GPU，而贴图卡顿主要是带宽，如果带宽不足的话，其实不管会卡GPU，而且会卡Draw，SkyLight就是这样子的，有一个2000多的uniformBuffer。
# ScreenPercentage （r.screenpercentage)
这里可以看出，每个梯度都有一点多毫秒的下降。
## 100
![ScreenPercentage100](/images/Unreal/ZPlan/Shadow1.PNG)
## 75
![ScreenPercentage75](/images/Unreal/ZPlan/ScreenPercentage75.PNG)
## 50
![ScreenPercentage50](/images/Unreal/ZPlan/ScreenPercentage50.PNG)
# Bloom (r.bloomquality 0)
这里也是测试0 和 1 就可以
可以看出，在开启Bloom和关闭Bloom之前的相差差不多也是1ms多点。
## Open
![Bloom1](/images/Unreal/ZPlan/Bloom1.PNG)
## Close
![Bloom0](/images/Unreal/ZPlan/Bloom0.PNG)
# SSR
开启和关闭大概有3ms的差距，所以一般还是关闭比较好，怀疑这个甚至比PPR还要耗(抽空测试一下)
## Open
![SSR](/images/Unreal/ZPlan/SSR.PNG)
## Close
![NoSSR](/images/Unreal/ZPlan/NoSSR.PNG)
# HDR(Tonemapper)
不支持，开了就卡死，材质找不到提示。暂时没时间去细察。PC上效果如下所示：
![LDR](/images/Unreal/ZPlan/LDR.png)
![LDRProfiling](/images/Unreal/ZPlan/LDR-Profiling.png)
可以看出，当关了HDR之后是没有tonemapper。
手机效果可以用`r.Mobile.TonemapperFilm 0`来看看，不过效果是有的，但是抓帧里面还有tonemapper。
# PlanarReflection
![PlanarReflection](/images/Unreal/ZPlan/planarReflection.png)
比较不解的是，已经关掉了平面反射，但是 iOS还在跑，需要实验删掉看看情况是否好转。

# UI
想不到UI还可以占2ms。可怕
![UI](/images/Unreal/ZPlan/UI.png)

# ShadowDepth
不管如何设置，大小都是2048 x 2048，有点奇怪。
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth.png)
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth1.png)
这里可以看出顶点数越多，画阴影的时间也会越多。
![ShadowDepth](/images/Unreal/ZPlan/ShadowDepth2.png)
这里又比较奇怪了，顶点数也比较多，但是耗时却比较少。怀疑跟屏占比有关系。

关闭天光的阴影真题耗时为227.41us
![NoSkyLightShadowDepth](/images/Unreal/ZPlan/NoSkyLightShadowDepth.png)

# MobileBasePass_PostAO(环境光遮蔽)
`r.Mobile.PixelProjectedReflectionQuality`
![PostAO](/images/Unreal/ZPlan/postAO.png)
这里控制开启和关闭
## Open
![OpenPostAO](/images/Unreal/ZPlan/OpenPostAO.png)
## Close (sg.PostProcessQuality 0)

# Fog
```C++
static TAutoConsoleVariable<int32> CVarFog(
	TEXT("r.Fog"),
	1,
	TEXT(" 0: disabled\n")
	TEXT(" 1: enabled (default)"),
	ECVF_RenderThreadSafe | ECVF_Scalability);
```
