---
title: 深入理解Flutter消息机制
date: 2022-11-12 15:39:00
tags: Flutter
categories:
    - Flutter
---
# 概述
在深入理解Flutter引擎启动已经介绍了引擎启动阶段会创建AndroidShellHolder对象，在该过程会执行ThreadHost初始化，MessageLoop便是在这个阶段启动的。
## 消息流程图
### FlutterMain::Init
在Flutter引擎启动时执行FlutterActivity的onCreate()过程，会调用FlutterMain::Init()方法
