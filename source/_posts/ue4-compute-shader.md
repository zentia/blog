---
title: UE4 使用Compute Shader
mathjax: true
date: 2021-07-02 21:03:31
tags:
    - UE4
categories: UE4
---
本文是对 https://medium.com/realities-io/using-compute-shaders-in-unreal-engine-4-f64bac65a907 的一篇译文 
<!--more-->

如果你想在你的UE4项目中使用[Compute Shader](https://docs.microsoft.com/en-us/windows/win32/direct3d11/direct3d-11-advanced-stages-compute-shader) ，
这篇文章可以帮助你节省一些事件，我花费了两周的事件去研究文档和代码才学会如何自定义compute shader。因此我决定写下这篇文章来教你如何写compute shader。

本文讲了写啥？
- 如何添加一个全局的compute shader。
- 如何使用global compute shader。

例子仓库在：
https://github.com/AyoubKhammassi/CustomComputeShaders

（老哥是4.24，抽空我用4.26实验一下，由于学习时间太紧张，基本都是先留个坑，然后在填）

文章中我将不会解释所有的东西，但是仓库中解释了。本文，主要讲述以下几点：

- 添加一个新的模块
- 添加一个shader 文件。
- 创建一个全局的shader type然后实现它。
- 为这个shader type创建一个shader manager

# 新模块

在我们设置shader之前，我们先创建一个新模块。因为以下几点理由：

- 模块分离的好处，我想不言而喻了吧。加快编译，生殖隔离之类的。
- UE4做不到我们添加一个shader type通过动态加载模块，动态模块其实也不支持的，除非你在加载编辑器的前面。（老外的语法把我整的不会翻译了，明明知道啥意思，就是翻译不好）
分离模块也让我们不会乱搞主游戏模块的加载。
  
本文不会告诉你咋新建一个模块，你直接看视频吧。这是链接 https://www.youtube.com/watch?v=piPkLJerpTg&list=PLHadbgEqCTxAorD5QoSxggjV2mXhhmkyI

