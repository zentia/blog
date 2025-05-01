---
title: RTR 4th Chapter2 图形渲染管线
mathjax: true
date: 2019-04-12 23:14:20
tags:
    图形学
categories: RHI
---
整个渲染流水线的目标是实时地通过一个虚拟的camera，把光源，3D物体（包括上面的纹理）等渲染到2D图像上，流水线是并行化执行的，但是会出现stall，直到流水线中最慢的一个阶段完成它的任务
整条流水线的架构，主要依次分为四个阶段：
Application,Geometry Processing,Rasterization和Pixel Processing
<div align="center">{% asset_img 1.jpg %}</div>
每个阶段内部也有自己的pipeline的。
我们要区别“阶段（functional stage）”和具体的实现结构，也就是说具体的某个实现结合可能合并了几个stage的内容到一个单元（unit），使得其过程时间消耗更短。

# 简要介绍

Application阶段主要是在CPU上完成的，可以利用现代CPU的多和特性进行多线程优化，这一部分通常包括碰撞检测，全局加速算法，动画，物理模拟等待，这些取决于application的类型。
Geometry Processing阶段主要处理转换（transforms），投影（projections）等等其他所有类型的几何操作，这个阶段计算了什么将会被渲染，怎样被渲染，在哪里被渲染，主要在GPU上完成。
Rasterization阶段一般接受三个顶点，构建出一个三角形，找到所有被这个三角形包含着的像素点，然后把这些像素点传递给Pixel Processing阶段，这部分完全在GPU上完成。
Pixel Processing阶段执行逐像素的程序，计算每个像素的color，也许会进行Z-Test等进行可见性判断，也许会进行blend操作等，这部分也完全在GPU上完成。

# 具体介绍每个阶段

## Application Stage

这部分是开发者完全控制的阶段，因为这部分通常是基于CPU执行的，对于这部分的改变会影响后续阶段的performance（比如这里可以通过算法减低要被渲染的三角形的数量）但是现在在这部分平台上也可以使用计算着色器来借GPU的力量加速。这个阶段的最后，渲染图元（点、线、三角形）传入到Geometry Procssing阶段。
这个部分是不能被分割成子部分的，因为这个是基于软件的实现方式，我们最多只能借助CPU的多核编写多线程程序来提升性能
这个阶段也是用来处理外接设备的输入，两个物体的碰撞检测，加速算法（比如部分剔除算法）

## Geometry Processing

用来负责逐三角形和逐顶点的操作。这部分可以被分为4个阶段：Vertex Shading，Projection，Clipping和Screen Mapping，下面逐一介绍：
Vertex Shading：这个子阶段主要处理两个任务：1.计算顶点位置 2.计算每个顶点包含的其他性质，如发现，纹理坐标等
以前会在这里实现逐顶点的光照，所以叫顶点着色器，现在顶点着色器变成了一个更加通用的用来设定与操作每个顶点相关的数据的计算单元（比如可以实现顶点动画等）
这个阶段同时是转换坐标的阶段，把坐标从模型空间转换到世界空间，为了便于后续的投影和裁剪，在使用view transform转换到相机空间，同时保存每个顶点对应的material，normal等信息，接下来把它转换到一个单位(-1,-1,-1)(1,1,1)中进行投影（z也可能是[0,1]），首先是进行投影，投影可分为正交投影和透视投影，在进行顶点着色器的操作
这个子阶段的输出（包含颜色，纹理坐标等）被送到Rasterization和Pixel Processing阶段被插值，用来计算表面的shading，这一步结束后，z-轴坐标不再存储在image中，而是z-buffer内，也就是说，这一步把3D->2D。

Optional Vertex Processing：这个阶段是可选的，而且不同的GPU对这部分的硬件支持程度也是不同的。简单来说依次是曲面细分，几何着色和流输出。

Clipping：只有那些全部部分在view volume中的渲染图元才能够pass这个阶段进入后续的阶段，使用投影矩阵意味着把transform后的图元裁剪单位立方体中。

{% asset_img 2.jpg %}
Screen Mapping：把还是3D的坐标转换成屏幕坐标（屏幕坐标原点取决于API）

## Rasterization
目标：找到所有被渲染图元包含或覆盖的像素点
这部分分为两个部分：三角形建立和三角形遍历。把2D的顶点在屏幕空间转换到屏幕上的像素，在这里，三角形通过传入的3个点得以简历，最后把所有图元的像素点传递到Pixel Processing中。这个阶段通常是由硬件实现的。
## Pixel Processing
这个阶段通常分为2个子阶段，分别是Pixel Shading和Mergeing
Pixel Shading：所有逐像素的计算都在这里完成，这里的输入是通过shading data的插值得到的。这里是基本上完全可编程的，通过自定义片元着色器（或者叫像素着色器）在GPU上完成光照，纹理等计算
Mergeing：每个像素存储来一个叫颜色缓冲的地方，我们需要一个Merge阶段处理片元着色器出来的颜色和buffer中本来存着的颜色之间的关系，这部分也用来负责可见性问题（使用z-buffer算法）
模版缓冲是一个离屏缓冲用来记录渲染图元的位置，每个像素通常为8bit，可以用来控制后续图元的渲染情况（通过模版测试的方式）