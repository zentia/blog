---
title: Unity Lerp
date: 2018-04-26 16:54:28
categories:
- Engine
- Unity
---

有时，我们在做游戏时会发现有些跟随动作不够圆滑或者需要一个缓冲的效果，这时，一般会考虑到插值。（比如摄像机跟随主角）

插值是数学上的一个概念，公式： from + (to - from) * t;
from是起始地位置，to是目标位置，按照数字t在from到to之间插值。
