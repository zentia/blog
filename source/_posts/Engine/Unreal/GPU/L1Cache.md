---
title: Nvidia GPU 架构
date: 2022-01-03 20:16:00
tags:
    - Unreal
---
# Nvidia GPU不同架构L1 cache的一些区别
```
按照经验来看，GPU通常长使用两级的缓存机制，L2是所有SM/CU共用，L1一般是单个SM/CU独自享用，nvidia不同GPU架构在L1的设计上有一些差异。
```
## 1. Kepler（开普勒架构）
## 2. Maxwell (麦克斯韦架构)