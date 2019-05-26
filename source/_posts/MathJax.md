---
title: MathJax
mathjax: true
date: 2019-03-09 12:03:24
tags:
    - Hexo
categories:
---

# 希腊字母

|小写       |Tex|大写|Tex|
|-----------|--|--|---|
|$\alpha$   |\alpha     |
|$\beta$    |\beta  |
|$\gamma$   |\gamma     |
|$\delta$   |\delta |
|$\epsilon$ |\epsilon   |
|$\zeta$    |\zeta  |
|$\eta$     |\eta       |
|$\theta$   |\theta |
|$\upsilon$ |\upsilon   |
|$\phi$     |\phi   |
|$\omega$   |\omega     |$\Omega$   |\Omega |
|$\in$      |\in        |
|$\lambda$  |\lambad    |$\Lambda$  |\Lambda|
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

# 矢量

\vec{a} \cdot \vec{b} = 0 显示 $\vec{a} \cdot \vec{b} = 0$