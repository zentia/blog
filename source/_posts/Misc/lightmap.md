---
title: 光照贴图
date: 2017-10-03 16:28:04
tags:
	- theGIbook
categories: theGIbook
---
在很多情况下，我们为了提高渲染效率，一般都会让美术同学在制作场景时，设置场景相关节点的lightmap static属性，提前给整个场景烘培出静态的光照贴图lightmap，烘培的数据保存在场景目录下的LightmapSnapshot文件中，主要包括的数据有：
lightmaps：烘培出的光照贴图数组；
gameobject uid：被烘焙的gameobject的唯一标识；
renderer的lightmapIndex：被烘培的gameobject的renderer组件所指向的光照贴图用于采样的区域坐标和宽高。
这个文件目前没有相关api读写，如果你想烘培完场景之后，把场景里面的gameobject抽出来做prefab，等切换玩场景之后再用于动态加载是不可行的，因为抽出来的prefab咋Instantiate之后将会是一个新的gameobject，uid自然和LightmapSnapshot文件里面记录的不一样，导致找不到对应的光照数据而造成模式没光照变暗或渲染错乱。
还有一种比较常见的需求是，在游戏运行时，通过更换光照贴图数据，营造场景在不同时间或季节的光照氛围，例如白天和黑夜。

由于漫反射表面的颜色可以由单个RGB颜色值描述，其记载的是辐射照度值，如果光源和整个场景是静态的，则这个辐射照度值为一个常数，因此可以被预存起来，我们称记录辐射照度值的纹理为一个辐射照度贴图（irradiance maps）或光照贴图（light maps），由于光照传输是线性的，因此其它动态光源所形成的光照可以在这个辐射照度值上进行累加。光照贴图仍然是先阶段实时渲染中漫反射间接光照的主流解决方案[Unity,2017b,Unreal Engine 4,2018b,Iwanicki and Sloan,2017]，它最早被用于[Abrash,1997]中，光照贴图的概念如图11.31所示。
{% asset_img Lightmapped_Scene_with_Lightmap.png %}
**图 11.31：**一个简单的场景（左）及其光照贴图（右），由此可以看出光照贴图的分辨率很低，因此它通常用来存储间接光照（图片来自Wikipedia）

在像素着色器中，每个像素都需要读取光照贴图中的一个辐射照度值；而另一方面，漫反射表面的每个像素还存储了一个漫反射系数值，它也是一个常数值。直观上思考，我们希望将这两个值预乘起来，形成一个直接的出射辐射亮度值。