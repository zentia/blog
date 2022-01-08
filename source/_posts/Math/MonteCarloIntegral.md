---
title: 蒙特卡洛积分
date: 2022-01-08 09:03:00
mathjax: true
comments: true
tags:
    - Math
---
蒙特卡洛法中重要的一点是生成指定概率分布的随机数，“随机数”部分会阐述怎么生成符合指定分布的随机数。蒙特卡洛积分估计的优点在于它很简单，很容易扩展多维的情况，但问题在于收敛速度较慢。而重要性采样和拟蒙特卡洛就是加速估计的收敛速度，直观的结果就是在相同的采样数下误差更小。“重要性采样”，解决的是怎么设计最优的概率分布的问题。“拟蒙特卡罗”解决的是如何消除随机数产生聚集问题，以缩减误差。

为了方便读者的理解，本篇文章会同意符号描述，被积函数采用$f(x)$小写形式，被积函数的积分采用$F(x)$大写形式，概率密度函数采用$pdf(x)$，累计概率密度分布函数采用$cdf(x)$，大写的$X,Y$等表示随机变量，小写的$x,y$等表示具体的实数，$n$表示样本数，$N$表示维度。

推荐阅读《Physically based rendering: From theory to implementation》[1], 《概率论与数理统计，浙江大学第4版》[2]，《Advanced global illumination》[7]这三本书，以及网上 Scratchapixel https://www.scratchapixel.com/ 2.0 [3]
{% markmap 300px %}
# Testa
## test1
## test2
# Test
## Test1
## Test2
{%endmarkmap%}