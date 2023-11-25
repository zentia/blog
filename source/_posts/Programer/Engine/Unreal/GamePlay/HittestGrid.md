---
title: HittestGrid点击机制刨析
date: 2022-01-02 11:26:00
categories:
- Engine
- Unreal
- GamePlay
---
# [UEInside] HittestGrid点击机制刨析

https://zhuanlan.zhihu.com/p/346460371

## 一 区块划分
HittestGrid是以Window为单位进行划分的,一整个编辑器界面既是一个Window, 默认情况下HittestGrid把一个Window按照一个区块(即Cell)128x128大小,划分多个Cell.

每个Cell存储了和该Cell相交的空间,以加速查询过程.所以往往一个控件会被多个Cell所持有,等到真正响应点击的时候,还会用控件和鼠标做一次包含判断,Cell划分只是一种加速算法.

## 二 Grid生成

### 2.1 每帧生成

Grid每帧都会重新生成,和控件的绘制一起执行.SWindow::PaintWindow窗口每帧绘制开始时,会调用`HittestGrid->SetHittestArea`清理HittestGrid,并重新生成HittestGrid.

### 2.2 与绘制同时进行

每帧从SWindow的根节点开始分发Paint，在OnPaint之中调用FHittestGrid::AddWidget将自身加入FHittestGrid中。由于UE4的控件绘制已经时深度优先的遍历了，所以在往FHittestGrid加入自身的时候天然就生成了一个从根节点到叶节点的链状关系。

### 2.3 独立的RetainerBox

由于RetainerBox的特殊性质,它在绘制的时候会将自己的children绘制到一张RT上,所以它的HittestGrid也是独立生成.在SRetainerWidget的OnPaint中,会生成一个新的HittestGrid,并在onPaint结束的时候Append到RootGrid中.这样就完成了一整个HittestGrid的生成