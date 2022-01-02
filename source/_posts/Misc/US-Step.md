---
title: Unity Shader:优化GPU代码，用step()代替if sele等条件语句
mathjax: true
date: 2019-03-09 11:06:24
top:
tags:
    - Unity
categories: "Unity Shader"
---

# 普通的卡通着色Shader

先看一个Shader，卡通着色。由于卡通着色需要对不同渲染区域进行判定，比较适合做案例。
