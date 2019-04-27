---
title: 判断点和多边形关系
mathjax: true
date: 2019-03-29 21:08:55
tags:
    - 算法
categories: 算法
---
本文讨论如何判断一个点是在多边形内部，边上还是在外部。为了方便，这里的多边形默认为有向多边形，规定沿多边形的正向，边的左侧为多边形的内测域，即多边形按逆时针方向遍历，不考了自交等复杂情况。

比较常见的判断点与多边形关系的算法有射线法、面积法、点线判断法和弧长法等，算法复杂度都为O(n)，不过只有射线法可以正确用于凹多边形，其他3个只可以用于凸多边形。

# 射线法

射线法是使用最广泛的算法，这是由于相比较其他算法而言，它不但可以正确使用在凹多边形上，而且不需要考虑精度误差问题。该算法思想是从点出发向右水平做一条射线，计算该射线与多边形的边的相交个数，当点不在多边形上时，如果是奇数，那么点就一定在多边形内部，否则，在外部。
```CSharp
for (int i = 0; i < nCount; ++i)
{
	Vector3 p1 = mNodes[i];
	Vector3 p2 = mNodes[(i + 1) % nCount];
	if (p1.z == p2.z) //p1p2与y=p0.y平行;
		continue;
	if (pt.z < Mathf.Min(p1.z, p2.z)) // 交点在p1p2延长线上;
		continue;
	if (pt.z >= Mathf.Max(p1.z, p2.z)) // 交点在p1p2延长线上;
		continue;
	// 求交点的X坐标;
	float x = (pt.z - p1.z) * (p2.x - p1.x) / (p2.z - p1.z) + p1.x;
	if (x > pt.x) // 只统计单边交点;
		++nCross;
}
```
上面代码利用三角形原理，p1和p2的组成的直角三角形和p1和p组成直角三角形

# 面积法

面积法的思想是如果点在多边形内部或者边上，那么点与多边形所组成的三角形面积和等于多边形面积。多边形的面积公式可以使用叉积计算，不过计算面积是会有一定误差的，需要设置精度的误差范围。

# 点线判断法

对于多边形，如果一个点在它所有边的左边，那么这个点一定在多边形内部。利用叉积正好可以判断点与给定边的关系，即点在边的左边右边还是边上。

# 弧长法
