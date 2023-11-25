---
title: Perfetto工具使用简介
tags:
 - Android
categories:
 - Android
---
Perfetto工具是Android下一代全新的统一的trace收集和分析框架，可以抓取平台和app的trace信息，是用来取代systrace的，但systrace由于历史原因也还会一直在，并且Perfetto抓取的trace文件也可以同样转换成systrace视图，
```
adb shell setprop persist.traced.enable 1
```
# 抓取trace
## UI操作抓取trace
### 打开操作界面
方式1：在开发者选项中打开：
- 开启`System Tracing`系统追踪