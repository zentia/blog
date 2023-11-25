---
title: 第十五章 选取(Picking)
mathjax: true
date: 2019-04-11 15:26:08
tags:
- RHI
- DirectX
categories:
- RHI
- DirectX
---

# 概览(OVERVIEW)
   如果用户点击了屏幕上的点 s = (x, y)。 从图15.1 我们能看到用户选取了茶壶。 无论如何，应用程序无法根据给定的s点就立即确定茶壶是被选取。所以，我们必须拿出计算这个动作技巧来，叫做选取技巧。
   
   图15.1 用户正在选择茶壶
   
   我们知道一些知识：关于茶壶和它的关联点s，茶壶投影在围绕s点的区域，更准确的说是：它投影到投影窗口上围绕p点的区域，与它对应的屏幕点是s。因为这个问题依赖于3D物体与它的投影之间的关系，我们看图15.2就可以了解。
   
图15.2放射线穿过点p点将会相交于围绕p点投影的对象。 注意：在投影窗口上的点 p与荧屏上被按下了点s相关联。

   图15.2我们看到如果我们发射一条选取射线，从原点发出，经过点p，会与围绕p点投影的对象相交，即茶壶。所以一旦我们计算选取射线，我们可以遍例场景中的每个对象并测试，看射线是否与它相交。与射线相交的对象即是用户选择的对象，在这个例子中用户选取的对象是茶壶。
   上面的例子讲解了点s与茶壶的关系。通常我们任意点击屏幕上的点，我们遍例场景中的每个对象，如果对象与射线相交，那么这个对象就是用户选取的对象。例如，图15.1中，如果用户没有点击5个对象中的一个，而是点击了白色的背景区域，射线将不能相交任何对象。因此，结论是：如果射线没有与场景中的任何对象相交，则用户没有点击任何一个对象，其它的我们不关心。
   “选取”适用于所有种类的游戏和3D程序。例如，玩家通过用鼠标点击来影响3D世界中的不同对象，玩家可能点击向敌人射击，或点击拾取物品。好的程序会适当做出反应，程序需要知道哪个对象被选取（是敌人还是物品），和在3D空间中的位置（开枪会击中哪？或玩家将要移动到哪去拾取物品？）。选取回答了我们这些问题。
   
目标
学习如何计算选取算法并了解它是如何工作的，我们将选取分解成四步：
1) 给一个屏幕点s，找出它在投影窗口上相交的点，即p。
2) 计算射线，它是从原点出发并经过点p。
3) 转换射线与模型到同一空间。
4) 测试与射线相交的对象，相交的对象即是屏幕上点击的对象。
15.1 屏幕到投影窗口的转换
首先，转换屏幕点到投影窗口，视口变换矩阵是：


根据屏幕上的点s = (sx, sy)，通过视口转换，得到在投影窗口上的点p = (px, py, pz)：


回忆一下2D图形部分：视口转换后z轴是不用保存的，而被保存在z缓存中。
给出屏幕点s，我们要找到点p，使用下列公式：


假定视口成员x和y都是0，通常我们能进一步得到：


因为前面的定义，投影窗口就是z=1的平面，所以pz = 1。

可是我们还什么都没做，投影矩阵缩放投影窗口上的点，来模拟不同的视角。为了返回缩放前的点值，我们必须用与缩放相反的操作来转换点。P是投影矩阵，因为P00 和 P11转换距阵缩放点的x和y坐标，我们得到：

15.2 计算射线
   回忆一下，射线能够描述参数方程：p(t) = p0 + tu。其中p0是射线的起点，用来描述它的位置，u是向量，用来描述它的方向。
   如图15.2，我们知道射线的起点总是视图空间的原点，所以p0 = (0, 0, 0)，如果p是射线穿过投影窗口上的点，方向向量u给出：u = p - p0 = (px, py, 1) - (0, 0, 0) = p。
   下面的方法用来计算选取射线（从屏幕空间点击的点所对应的视图空间的点x、y坐标）：
```C++
d3d::Ray CalcPickingRay(int x, int y)
{
     float px = 0.0f;
     float py = 0.0f;

     D3DVIEWPORT9 vp;
     Device->GetViewport(&vp);
     D3DXMATRIX proj;
     Device->GetTransform(D3DTS_PROJECTION, &proj);

     px = ((( 2.0f*x) / vp.Width)  - 1.0f) / proj(0, 0);
     py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

     d3d::Ray ray;
     ray._origin    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
     ray._direction = D3DXVECTOR3(px, py, 1.0f);

     return ray;
}
```
where Ray is defined as:
struct Ray
{
     D3DXVECTOR3 _origin;
     D3DXVECTOR3 _direction;
};
   我们更新d3dUtility.h文件，在d3d命名空间中加入选取射线Ray。
   
15.3 变换射线
   上一节讲到，选取射线的计算被描述在视图空间，为了完成射线的相交的测试，射线和对象必须在同一个坐标系统。通常转换射线到世界空间（甚至对象在本地空间）要好于将所有对象转换到视图空间。
   我们能够将一个变换矩阵转换为一条原点为p0，方向为u的射线r(t) = p0 + tu，注意：原点转换为一个点，方向转换为一个向量，在本章的选取例子中，下列函数转换一条射线：
void TransformRay(d3d::Ray* ray, D3DXMATRIX* T)
{
      // transform the ray's origin, w = 1.
      D3DXVec3TransformCoord(
           &ray->_origin,
           &ray->_origin,
           T);

      // transform the ray's direction, w = 0.
      D3DXVec3TransformNormal(
           &ray->_direction,
           &ray->_direction,
           T);

      // normalize the direction
      D3DXVec3Normalize(&ray->_direction, &ray->_direction);
}
   D3DXVec3TransformCoord和D3DXVec3TransformNormal接受一个Ray类型参数（包含二个3D向量成员）。 D3DXVec3TransformCoord函数中，射线的原点（_origin）向量的第四部分w = 1。相反，函数D3DXVec3TransformNormal中，射线的方向（_direction）向量的第四部分w = 0。
   这样，当我们向世界空间转换时，能够用D3DXVec3TransformCoord转换一个点，用D3DXVec3TransformNormal转换一个向量。
   
15.4 射线－对象 交点
   我们将射线和对象转换到同一坐标系统后，准备测试哪个对象与射线相交。因为我们将对象描述为三角形组成的网络，下面详细说明这种方法。遍例场景中每个对象的三角形列表并测试，如果射线相交于一个三角形，它就与三角形所在的对象相交。
   然而，通过遍例场景中的每个三角形来实现射线相交在计算上会增加时间，一种比较快的方法，虽然准确性会差一点。它将每个对象围成一个近似的球形（边界球），这样我们就能通过遍例每个边界球来测试射线相交。用边界球来描述相交的对象。
   注意：射线可能相交多个对象，然而离照相机近的对象会被选取。因为近距离对象遮挡了后面的对象。
   给出一个边界球的圆心c和半径r，使用下列恒等式能够测试点p是否在边界球上：
   ||p-c||-r = 0
   如果恒等式满足，则点p在边界球上。如图15.3
   
   图15.3 向量p到c的长度表示为：||p - c||，如果等于半径则表示点p在边界球上。注意：我们使用边界球是为了方便，但这将扩展出三个种情况。
   假定射线p(t) = p0 + tu相交于边界球，我们将射线代入球的恒等式中，使参数t满足了球的恒等式，给出了满足相交点的参数。
   
将射线p(t) = p0 + tu代入球的恒等式：
||p(t) - c|| - r = 0   -->   ||p0 + tu - c|| - r = 0

通过以上推导，我们得到二次方程：
At2 + Bt + C = 0

其中A = u · u, B = 2(u · (p0 - c))，而C = (p0 - c) . (p0 - c) – r 2。
如果u是标准化的，那么A = 1。
因为u是标准化的，我们解t0 和 t1：
   
   
   图15.4显示可能返回的t0 和 t1，并显示了一些返回值的几何意义：
   
   图15.4 (a)射线从球边上擦过；(b)射线在球前；(c)射线在球的内部；(d)射线相交于球；(e)射线是球的切线。
   下列方法射线通过并与边界球相交，返回true；射线错过边界球，返回false。
 bool PickApp::raySphereIntersectionTest(Ray* ray,
                                        BoundingSphere* sphere)
{
     D3DXVECTOR3 v = ray->_origin - sphere->_center;
     float b = 2.0f * D3DXVec3Dot(&ray->_direction, &v);
     float c = D3DXVec3Dot(&v, &v) - (sphere->_radius * sphere->
                                      _radius);

     // find the discriminant
     float discriminant = (b * b) - (4.0f * c);

     // test for imaginary number
     if( discriminant < 0.0f )
          return false;

     discriminant = sqrtf(discriminant);

     float s0 = (-b + discriminant) / 2.0f;
     float s1 = (-b - discriminant) / 2.0f;

     // if a solution is >= 0, then we intersected the sphere
     if( s0 >= 0.0f || s1 >= 0.0f )
          return true;

     return false;
}
   当然，我们已经准备了一个边界球，为了便于理解我们再次显示它的定义：
bool PickApp::raySphereIntersectionTest(Ray* ray,
                                        BoundingSphere* sphere)
{
struct BoundingSphere
{
     BoundingSphere();

     D3DXVECTOR3 _center;
     float       _radius;
};
   
15.5 例子程序：选取
图15.5显示了本章例子程序的屏幕截图，茶壶绕着屏幕移动，你可以用鼠标试着点击它。如果你点击到茶壶的边界球上，一个消息框将弹出，表示你点中了。我们通过测试WM_LBUTTONDOWN消息来处理鼠标点击事件：
case WM_LBUTTONDOWN:

// compute the ray in view space given the clicked screen point
d3d::Ray ray = CalcPickingRay(LOWORD(lParam), HIWORD(lParam));

// transform the ray to world space
D3DXMATRIX view;
Device->GetTransform(D3DTS_VIEW, &view);

D3DXMATRIX viewInverse;
D3DXMatrixInverse(&viewInverse, 0, &view);

TransformRay(&ray, &viewInverse);

// test for a hit
if( RaySphereIntTest(&ray, &BSphere) )
     ::MessageBox(0, "Hit!", "HIT", 0);

break;

图15.5 这章例子程序的屏幕截图

15.6 摘要
* 选取技巧通过鼠标点击，来确定与3D对象对应的屏幕上显示的2D投影对象。
* 选取线是一个射线，源自视图空间的原点，穿过投影窗口上的点关联到屏幕上点击的点。
* 我们能够变换一个射线r(t) = p0 + tu，通过变换原点p0和通过矩阵变换得到方向u。注意：原点变换自一个（w=1）的点，方向是（w=0）的向量。
* 测试射线与对象相交，我们能测试射线是否相交于组成对象的三角形，或测试射线是否相交于围绕对象的一个体积，比如边界球。

