---
title: UE4中用球面高斯函数实现移动端SSS效果
---
https://zhuanlan.zhihu.com/p/139836594
目前市面用的是LUT图去算SSS，包括我们游戏。
但是手机GPU芯片在不断增加ALU，但是iPhone消耗占大头的还是ALU，这篇文章说的是，不用LUT了，要用ALU，节省带宽。

有点：
- 灵活指定不同的Diffusion Profile(扩散刨面)、模拟人类皮肤、非人类皮肤或普通的次表面散射皆可；
- 不用采样LUT图，节约带宽

缺点：
- 额外的ALU占用率（实测这部分仍然是大头）
- 没有完整的实时SG SSS实现参考

