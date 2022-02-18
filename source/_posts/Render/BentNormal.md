---
title: BentNormal(环境法线)
categories:
- Render
---
![Bent Normal的采样示意图（从图中可以看出，Bent Normal(黄色)与原始Normal(蓝色)相比，其在考虑周围几何体分布的情况下向右侧做了修正](1.png)

用途：
- 用来改变原始的Normal，并记录光线的主要通过防线，这样在做环境贴图的采样时可以从更优化的方向来计算采样的光线。
- 用来代替原始的Normal做正常的Shading，这样可以在尽管只有Normal的情况下一定程度上得到类似于Ambient Occlusion的Soft Shadow效果。