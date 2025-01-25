---
title: Flutter引擎启动
---
# Flutter引擎启动工作
## Flutter启动概述
Flutter作为一款跨平台的框架，可以运行在Android、iOS等平台，本文以Android为例讲解如何从Android应用启动流程中衔接到Flutter框架，如何启动Flutter引擎的启动流程。熟悉Android的开发者，应该都了解APP启动过程，会执行Application和Activity的初始化，并调用它们的onCreate()方法。那么FlutterApplication和FlutterActivity的onCreate()方式是连接Native和Flutter的枢纽。

- FlutterApplication.java的onCreate过程：初始化配置文件/加载libflutter.so/注册JNI方法；
- FlutterActivity.java的onCreate过程：创建FlutterView、Dart虚拟机、Engine、Isolate、taskRunner等对象，最终执行到Dart的main()方法，处理整个Dart业务代码。