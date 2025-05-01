---
title: AndroidProfiler
mathjax: true
date: 2019-03-23 11:12:29
categories:
- Android
---
如果你觉得应用卡顿、不够流程，不用怀疑，很大原因是没有在16ms完成你的工作。
> **著名的"16ms"原则：**
> 我们通常会提到60fps（Frame Per Second）与16ms
> 60fps：人眼与大脑之间的协作无法感知超过60fps的画面格纳更新。
> Android系统每隔16ms会发出VSYNC信号重绘我们的界面

# snapdragon 安卓抓帧

https://zhuanlan.zhihu.com/p/66094148

安卓工程需要开启 INTERNET 和 PROFILER 权限， 即在AndroidManifest.xml 配置中加入

```C++
<uses-permission android:name="android.permission.INTERNET"/>
<uses-permission android:name="com.qti.permission.PROFILER" />
```