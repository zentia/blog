---
title: Camera
---
1. 屏幕坐标和世界坐标的相互转换
   1. Camera有一个参数`Constrain Aspect Ratio`约束宽高比，当勾上此参数会重新计算相机原点，导致通过目标点推到出来的世界点和相机位置不在同一条直线上。
   2. 每个窗口有一个Viewport，可以指定使用哪个相机