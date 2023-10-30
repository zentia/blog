---
title: ZBrush Manual
mathjax: true
date: 2019-05-11 10:14:20
tags:
    - ZBrush
categories: Artist
---
# 常用快捷键
空格键 激活鼠标指针下的菜单（按住不放再拖拉鼠标指针可定位文档） 
左键在空白处单击并拖动＝旋转视角 
alt建＋左键在空白处单击并拖动＝平移视角 
alt键＋左键然后放开alt键在空白处拖动并平移＝缩放视角 
+ 放大文档 
- 缩小文档 
0（零） 查阅文档的实际大小 
旋转中配合 shift ，捕捉在正交视点处 
旋转中配合 ALT ，以相对平滑方式旋转视图 
按下 DEL ，可以将物体在画布中最大化显示 
Ctrl+0 查阅文档实际大小的一半，抗锯齿（还影响输出） 
ctrl+shift+拖动=未选中的部分将隐藏。 
ctrl+shift+点击空白处=恢复显示 
ctrl+shift+拖动+释放（ctrl、shift）=选中的部分将隐藏 
Ctrl+f 用当前texture填充canvas当前layer 
Ctrl 遮罩功能 
Ctrl+d 细份一次 
Tab 隐藏/显示浮动调控板 
H 隐藏/显示Zscript窗口 
C 在指针下面拾取颜色 
S 绘图大小 
I RGB强度 
Shift+I Z强度 
Shift+d 绘制贴图的时候让模型上的网格线隐藏 
Q 绘图指示器（默认为所有工具） 
W 移动模式 
E 缩放模式 
R 旋转模式 
T 编辑模式 
Shift+s 备份物体 
M 标记物体 
点中marker ctrl+M 取消标记 
Ctrl+Z 撤销 
Shift+Ctrl+Z 重做 
Alt/Option 影响转换大多数工具 
G 打开Projection Master 
Alt+A Alpha调控板 
Alt+C Color颜色调控板 
Alt+T tool调控板 
Alt+T Transform调控板 
Alt+R Render调控板 
Alt+鼠标左 绘图模式下可以删除Z求点 

Shift+crease--------------crease everything 
Shift_uncrease------------uncrease everything 
A ------------------zSphere to Z mesh 
ctrl+N ----------------- clear the layer
# 笔刷
1. **Standard(标准笔刷)**:使用Standard笔刷进行雕刻的时候,我们可以塑造出截面为半椭圆形的突起,如图所示
{% asset_img 1.webp %}
2. **Smooth(光滑笔刷)**:在选择任何笔刷的情况下,按住Shift键,都会切换到Smooth笔刷,该笔刷可以使物体避免的形状进行融合,进而雕刻出较为平滑三维表面,如图所示
{% asset_img 2.webp %}
3. **Move(移动笔刷)**:Move和Standard不同,不能对物体表面进行连续的形变,每次只能对不大于笔刷大小的区域进行锥拉操作,如图所示,但是Move在对形体进行调整时,有非常良好的表现
{% asset_img 3.webp %}
4. **Clay(粘土笔刷)**:Clay与ClayTubes和ClayBuildup笔刷一样,属于粘土类型的笔刷,该中类型的笔刷雕刻起来感觉类似传统的泥塑,就像用泥巴一层一层的添加结构,它是应用最广发的笔刷之一.
5. **Clay Tubes(粘土管笔刷)**:此笔刷可以作为Clay的变种笔刷,由于加载了方形的Alpha,所以在塑造形体的时候,边缘更清晰,而且有层次感.
6. **Clay Buildup(粘土堆积笔刷)**:同ClayTubes笔刷相比,ClayBuildup笔刷更细腻,凹起程度也更高.不同的是,Clay和ClayTubes在雕刻时会使形体产生平坦的突起,但是ClayBuildup会产生边界较为锐利但表面弧状的突起.
{% asset_img 4.webp %}
7. **Dam Standard**:给人感觉用刀雕刻的感觉。
{% asset_img 1.jpg %}
8. **HPolish Brush**可以画出坚硬的牙齿效果。
9. **Slash3**

Tray是用来放置菜单的.
Canvas
Light Box
Shelf
Brush
Project位于上面的File菜单中.也可以通过快捷键Ctrl+S访问这个选项,这样就可以保存好一个Project了,Project中可以保存全部的Tools.可以将其中所有的Tools都保存好.也可以保存好对材质面板的改变.以及Render面板中的一些设置和Light面板中的一些设置.一定要记住Project中保存了所有的Tools
Tool
# 快捷键操作
摁住Alt鼠标左键点击空白处移动,可以移动画面.
默认外拉面数,Alt内推面数
Zadd,Zsub
Zadd是所有笔刷默认使用的模式,不是所有的笔刷,你只要按Alt就可以切换到Zsub模式,向内雕刻模型,
摁住Shift你会注意到,鼠标光标会变成蓝色,现在处于Smooth Brush模式,你可以使模型表面变得平滑
Smoothey Intelligently
Clay笔刷不用理会拓扑结构是怎样的.无论是光滑的还是凹陷的结构.它与拓扑结构无关.
Ctrl添加Mask，Ctrl+Alt消除Mask
做耳朵技巧：先用Mask画出一个耳朵，然后更改画刷为Move，然后修改细分，然后拉出一只耳朵来。
单独按D键，可以向上移动一个层级
Deformation
Smart ReSym
# 设置快捷键
1键切换到标准笔刷，2键切换到Move笔刷，3键表示Clay4，4键表示Tray Dynamic，5键表示Clay Buildup
在Deformation(变形工具)中可以调整Inflate（充气）选项。让模型变得臃肿。Inflate Balloon(快速充气)注意这个选项运算量很大，所以最好修改一下细分。
1. **Polish**：磨光
2. **Polish Grisp Edge**

Masking(遮罩工具)
1. **Mask By Cavity**对内向的地方进行了遮罩。

Sub Tool
Sphere3D制作头发
Geometry
DynaMesh
Resolution 128
Transparency
Ghost

Can't adjust divide
Try use Dynamesh,but this operator may be make your mode have a few of triangle.

|快捷键|说明|
|Ctrl+Shift+Alt然后框选物体|剪切掉框选的物体|
|Ctrl+Shitf然后框选物体|剪切掉除了框选之外的物体|