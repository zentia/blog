---
title: Unity 动画编辑器
categories:
- Engine
- Unity
---
入口
`WindowLayout.cs` 901
`AnimationWindow.cs` 主窗口
`AnimationWindowKeyframe.cs`
这个是窗口绘制的关键帧信息
`AnimEditor.cs`实际的功能绘制和数据存储
```cs
// 播放按钮
PlayButtonOnGUI()
```
`AnimationWindowControl.cs`动画控制器，负责播放暂停等功能
```cs
ResampleAnimation(); // 采样animation真是的执行动画
```