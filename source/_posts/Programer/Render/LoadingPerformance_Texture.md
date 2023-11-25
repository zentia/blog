---
title: LoadingPerformance_Texture
date: 2018-07-05 11:25:56
tags:
	uwa
---

# 资源加载

资源加载是加载模块中最为耗时的部分，其CPU开销在Unity引擎中主要体现在Loading.UpdatePreloading和Loading.ReadObject两项中。

Loading.UpdatePreloading，这一项尽在调用类似LoadLevel(Async)的接口处出现，主要负责卸载当前场景的资源