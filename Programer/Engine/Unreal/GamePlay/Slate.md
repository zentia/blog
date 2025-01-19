---
title: Unreal Slate
date: 2022-01-02 11:24:00
categories:
- Engine
- Unreal
- GamePlay
tags:
    - Engine-Unreal-GamePlay
---
# [UEInside]Slate合批机制刨析

https://zhuanlan.zhihu.com/p/346275251

# UE4移动平台UI优化

https://www.slideshare.net/EpicGamesChina/unreal-open-day-2017-optimize-in-mobile-ui

# Unreal Engine 4 中的UI优化技巧

https://gameinstitute.qq.com/community/detail/113852

- Widget Tree: 每一个User Widget都是存储成树状结构。
- Panel Widget: 不会渲染出来，用于对Child Widget进行布局，如Canvan Panel, Grid panel, Horizontal Box等。
- Common Widget:  用于渲染，会生成最后的Draw Element中，如Button, Image, Text

在游戏线程（Game Thread），Slate Tick每一帧会遍历两边Widget Tree。
Prepass:从上到下遍历树，计算每一个Widget的理想尺寸（Desired Size）
OnPaint:从上到下遍历树，计算渲染所需的Draw Elements.