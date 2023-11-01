---
title: 三角形中心坐标
mathjax: true
date: 2019-05-13 21:25:25
tags:
- Render
categories: 
- Render
---
P用A,B,C来表示
对于三角形内任意一点P
{%asset_img 1.jpg%}
其实学过向量的就能想到，$\vec{AP}$，$\vec{AB}$，$\vec{AC}$线性相关。
{%asset_img 2.jpg%}
可以写成：$\vec{AP}=u\vec{AB}+v\vec{AC}$
展开：$A-P=u(A-B)+v(A-C)$
得到：$P=(1-u-v)A+uB+vC$
这也是P的表示，其中$0\le{u,v}\le{1}$，来取特殊一点的u，v可以得到A，B，C。
这个公式长得很像AB上任意一点D的计算：
$D=A+t(B-A),0\le{t}\le{1}$
也就是线性插值：$D=(1-t)A+tB,0\le{t}\le{1}$
# 计算
当然也可以把ABC看成坐标系，始于A点，基于：$\vec{AB}$和$\vec{AC}$。所以这个叫做重新坐标系（barycentric，bary-重的）也能得到公式：
$\vec{AP}=u\vec{AB}+v\vec{AC}$
转换成：$u\vec{AB}+v\vec{AC}+\vec{PA}=0$
甚至我们还可以把它写成矩阵形式：
$\begin{bmatrix}
{u}&{v}&{1}\\
\end{bmatrix}
\begin{bmatrix}
{\vec{AB_x}}\\{\vec{AC_x}}\\{\vec{PA_x}}\\
\end{bmatrix} = 0$
$\begin{bmatrix}
{u}&{v}&{1}\\
\end{bmatrix}
\begin{bmatrix}
{\vec{AB_y}}\\{\vec{AC_y}}\\{\vec{PA_y}}\\
\end{bmatrix} = 0$
实际上我们都可以看做是我们在寻找向量(u,v,1)同时垂直于向量$(\vec{AB_x},\vec{AC_x},\vec{PA_x})$和向量$(\vec{AB_y},\vec{AC_y},\vec{PA_y})$。这不就是叉乘吗？
同时这给了我们一个有了P点，求u和v的思路。

    xvector = (B_x - A_x, C_x - A_x, A_x - P_x)
    yvector = (B_y - A_y, C_y - A_y, A_y - P_y)
    u=xvector x yvector
    # 因为我们讨论的是二维三角形，如果u的z分量不等于1则说明P点不在三角形内

# 编码
然后我们来代码阶段，因为我们的计算中涉及到浮点数，可能u的z分量不会一定等于1.0，令u的三个分量是(a, b, c)，我们代入原公式：
$a\vec{AB}+b\vec{AC}+c\vec{PA}=0$
$P=(1-a/c-b/c)A+a/cB+b/cC,c\neq{0})$
```c
Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P){
    Vec3f s[2];
    for (int i = 2; i > 0;i--){
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2)
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z,u.x/u.z);
    return Vec3f(-1,1,1);
}
```
```python
def barycentric(A,B,C,P)
    """
    A,B,C,P:Vector3, points
    return u: Vector3, barycentric coordinate of P
    """
    s1 = Vector3(B.x - A.x, C.x - A.x, A.x - P.x)
    s2 = Vector3(B.y - A.y, C.y - A.y, A.y - P.y)
    u = s1.cross(s2)
    if abs(u.z) > EPSILON
        return Vector3(1-(u.x+u.y)/u.z,u.x/u.z,u.y/u.z)
    return Vector3(-1,1,1)
```
# 应用
重心坐标在CG中的应用可以有以下：
- 填充三角形
之前我也想过这个问题，因为我们生成图像，归根结底是在于画像素点，那么如果我们要填充一个三角形，对于一个一个的像素点，我们只需要放到上述函数里面去，就可以判断P是否在三角形内，我们就去画它。这样填一个三角形的算法可以就是O(image.width*image.height)，我们甚至可以进一步降低复杂度，对于一个三角形来说，我们可以由它的bounding box，只需要检测它的bounding box里面的点，然后就可以用来填充。
- z-buffer
其实这里也不止于z-buffer，我们已经把P点表示成了A，B，C的线性组合形式，那么对于P点的Z，我们也可以这样来看，我们把P点z值也可以看成A，B，C的线性组合，其实不仅仅是P点的z，对于P点的任意性质，只要我们觉得可以用线性组合来看的，我们都可以用这个坐标系统。
