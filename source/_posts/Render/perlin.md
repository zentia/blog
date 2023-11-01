---
title: Perlin噪声
mathjax: true
date: 2019-04-01 20:29:28
tags:
    - Render
categories: Render
---
# Perlin噪声
Ken Perlin在1993年提出了一种渐变噪声，Perlin在1985年的SIGGRAPH有该算法的描述，称之为经典Perlin噪声（Classical Perlin Noise）。为了简化计算，方便使用硬件实现，并解决经典Perlin噪声中存在的错误，到2001年，Ken Perlin再次对原始的噪声算法进行了改进，称之为Simplex噪声（Simplex Noise），这两种算法都可以成为Perlin噪声。但是，我们有时候也把分形噪声成为Perlin噪声，甚至在严肃的学术论文中都有这种叫法。为了避免歧义，本文指的Perlin噪声特指经典的Perlin噪声和Simplex噪声。

Stefan Gustavson指出：Simplex噪声有更小的算法复杂度，要求更少的乘法，在N维空间上，经典Perlin噪声的算法复杂度为$log(2^N)$，但是Simplex噪声的算法复杂度为$log(N^2)$。

# 经典Perlin噪声

经典Perlin噪声是Ken Perlin在1983年提出的噪声，Ken Perlin提供了一维、二维、三维算法的C实现。代码如下：

```c
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define B 0x100
#define BM 0xff

#define N 0x1000
#define NP 12 /* 2^M */
#define NM 0xfff

static p[B + B + 2];
static float g3[]
```