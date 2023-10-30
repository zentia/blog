---
title: Light Reflect
mathjax: true
date: 2019-03-12 15:33:16
tags:
    Unity
categories: "Unity Shader"
---

假设我们有一个发着光的太阳，太阳底下有一个“理想镜面”，如下图：
{% asset_img 1.png %}
这时候我们抽象成数学图来进行计算，如下图：
{% asset_img 2.png %}
这幅图可以看出，我们计算反射光线OB，则转换成了计算OP，这里OP是AO在法向量N上的投影，接下来我们推导投影向量的计算，如下图：
{% asset_img 3.png %}
我们根据点积计算出$cos\theta$，然后通过$OA' = |OA| \cdot cos\theta \cdot n$(n是单位法向量)就能得到，向量的投影公式。
这个时候我们在回过头来推导$OB = AO + 2OP$的结果，如下图：
![](4.png)