---
title: MathJax
mathjax: true
date: 2019-03-09 12:03:24
tags:
    - Hexo
categories:
---

# 希腊字母

|序号|小写       |Tex|大写|Tex|汉语注音|
|--|-----------|--|--|---|--|
|1|$\alpha$   |\alpha     |||阿尔法|
|2|$\beta$    |\beta  |||贝塔|
|3|$\gamma$|\gamma|$\Gamma$|\Gamma|伽马|
|4|$\delta$   |\delta |$\Delta$|\Delta|德尔塔|
|5|$\epsilon$ |\epsilon   |||伊普西隆|
|6|$\zeta$    |\zeta  |||泽塔|
|7|$\eta$     |\eta       |
|$\theta$   |\theta |
|$\upsilon$ |\upsilon   |
|$\phi$     |\phi   |
|$\omega$   |\omega     |$\Omega$   |\Omega |
|$\in$      |\in        |
|$\lambda $  |\lambda    |$\Lambda$  |\Lambda|
||$\varrho$|\varrho|
||$\varsigma$|\varsigma|
||$\varphi$|\varphi|
- 若需要大写希腊字母，将命令首字母大写即可。 \Omega 呈现为 $\Omega$
- 需需要斜体希腊字母，将命令前面加上var。\varpi 呈现为 $\varPi$

# 字母修饰

# 上下标

- 上标：^
- 下标：_
- 举例：c_n^2呈现为$c_n^2$

# 分数

有两种方法来显示分数，一种是 \frac a b 显示$\frac a b$，另一种是用\over，如{a+1 \over b+1}显示${a+1 \over b+1}$

# 求和与积分

\sum用来表示求和符号，其下标表示求和上线，商标表示上限。如\sum_1^n：$\sum_1^n$

\int
    \int_0^\infty{fxdx} 呈现为$\int_0^\infty{fxdx}$

# 特殊符号和记号

- \lt \gt \le \ge \neq 表示 $\lt \gt \le \ge \neq$，还可以在不等号上加\not，如\not\lt 表示 $\not\lt$
- \times \div \pm \mp 表示 $\times \div \pm \mp$，点乘用\cdot表示，如 x \cdot y 表示 $x \cdot y$
- $\in$ : \in

# 矢量

\vec{a} \cdot \vec{b} = 0 显示 $\vec{a} \cdot \vec{b} = 0$

# 如何插入公式大括号

```
方法一：
$ f(x)=\left\{
\begin{aligned}
x & = & \cos(t) \\
y & = & \sin(t) \\
z & = & \frac xy
\end{aligned}
\right.
$
```
方法一：
$ f(x)=\left\{
\begin{aligned}
x & = & \cos(t) \\
y & = & \sin(t) \\
z & = & \frac xy
\end{aligned}
\right.
$

# 开方
\sqrt{a+b} : $\sqrt{a+b}$
# 矩阵
`\begin{matrix}...\end{matrix}`，每一行以\结尾，hexo markdown中`\\\\`。&分割矩阵元素。
括号除了上面的\left...\right方式外，还可以pmatrix $\begin{pmatrix} 1&2\\\\3&4 \end{pmatrix}$，bmatrix $\begin{bmatrix} 1&2\\\\3&4 \end{bmatrix}$，Bmatrix $\begin{Bmatrix} 1&2\\\\3&4 \end{Bmatrix}$，vmatrix $\begin{vmatrix} 1&2\\\\3&4 \end{vmatrix}$，Vmatrix $\begin{Vmatrix} 1&2\\\\3&4 \end{Vmatrix}$。