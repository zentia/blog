---
title: ExecuteInEditMode
mathjax: true
date: 2019-03-25 17:17:52
tags:
    Unity
categories: Unity
---
# 描述
使脚本实例可以在编辑模式运行
默认的情况下，MonoBehaviours只能在Play模式下工作。通过添加这个属性，任何MonoBehaviour实例的回调函数也会在Edit模式下执行。
此属性正在逐步淘汰，因为它不考虑预制模式。如果在预制模式下编辑了具有此属性的MonoBehavi的预制体，并且输入了播放模式，则编辑器将退出预制模式，以防止意外修改仅用于播放模式的逻辑引起的预制体。

要指示MonoBehaviour正确考虑预制模式并且在播放模式下预制模式下打开是安全的，可以使用属性ExecuteAlways而不是ExecuteInEdiMode。

Update：在Scene发生变化时被调用
OnGUI：在Game视图接受到Event时被调用
OnRenderImage：在Scene视图或Game视图重绘时被调用
