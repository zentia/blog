---
title: 图形学习步骤
mathjax: true
date: 2019-04-11 08:27:42
tags:
  - 图形学
categories: 图形学
---
1. 3D图形学分3大块的学习内容:
  a.空间几何数学:   空间几何变换, 加速算法, 多边形技术, 曲线和曲面, 相交测试, 碰撞测试。
  b.光照着色系统:   光照, 纹理贴图, 高级象素着色光照, 艺术性渲染.
  c.程序技术性应用: 公告板, 精灵, 天空盒, 体绘制, 材质系统, 场景图, 渲染队列
2. 在实际学习过程中,3个部分相互制约,故不能单方面突进,应保持一种平衡发展,使得相互促进,深化理解,达到比较流畅的学习曲线.
3. 相对重要性以a,b,c序减(时间将无情淘汰现有的程序技术性应用),深刻掌握图形学基础才是王道。
4. 3个部分都略有小成后,大量快速阅读网上各方面的杂家资料,应用基础知识去分析理解,在短时间内掌握各种程序性技巧。
5. 再次通观全局基础,遍览所有细节,以图有更深的理解.

如何学习API (OpenGL/Direct3D等)
首先，我认为API是工具，不是本质，OpenGL/Direct3D的本质是图形学,而不是OpenGL/Direct3D的本身,API的本身只是一些Interface而已.如果你明白图形学的原理.那么你很容易这些接口的作用,以及为什么要有这些接口的存在.所以,我要说的第一点是:你要学习3D编程,不是学会了OpenGL/Direct3D就可以了。甚至会不会这些API都不是那么的重要(虽然这么说,或许很多人不太赞同).最重要的,最根本的是,你要明白这些API背后的图形学的原理---因为那才是根本中的根本.
   
下面我来介绍我对API学习的看法.

我认为API的学习有两种方法:一是正向学习.二是反向学习.
    
一:正向学习,所谓的正向学习,就是学习API的本身.我觉得这种方法是一种Brute Force行为.不是很好.我们只要看看API的特性,有那些部分.就可以了。比如学习Direct3D的时候,我们要知道它如何初始化,以及它和操作系统的结合.它在Direct3D8里引入了VS/PS.最后就是创建一个Direct3D应用的步骤和方法.这些就足够了。要不然.Direct3D那么多的函数,一个几十个参数。每一个都会要了我的命.
    
正向学习的第二个作用就是你在熟悉了图形学和大概了解了API后,有空就来看看API的细节,然后思索一下API里提供的一些特性对你的程序有什么作用.比如Direct3D里的Two Side Stencil.OpenGL里的TextureCombine等。

二:逆向学习.这是根本的方法,到了这一步,你就可以真正的算是图形学入门了。这要求你要有一定的图形学基础.比如,你现在开始做一个demo.你预计你的demo里有一堆眩眩的效果,当然你也要明白你的这些眩眩的效果要怎么实现的.然后你去找API里对应的功能,我想如果你的想法正常,一般的功能在Direct3D/OpenGL里应该都会有的.当然你也会碰到你想要的功能在这些API里没有,但是这不重要,重要的是你又学到新东西了---这个特性在XX API里不支持。

通常我是采用先正向,再逆向,同时再进行正向学习的方法.希望以上的建议,对各位初学者有一定的帮助.

# 关于计算机图形学的学习

## 引言
什么是计算机图形学？ 本文尽量避免给它做严格的定义，但是通常来说，计算机图形学是数字图象处理的逆过程，这只是一个不确切的定义，后面我们会看到，实际上，计算机图形学、数字图象处理和计算机视觉在很多地方的区别不是非常清晰的，很多概念是相通的。
计算机图形学是用计算机来画东西的学科，数字图象处理是把外界获得的图象用计算机进行处理的学科。
如何学习计算机图形学呢？除了计算机图形学的基础知识以外，你还需要有以下的知识。
* 英语， 你一定要把英语学好，如果你想学习计算机图形学的话，尽量看英文的书籍和资料
* 数学， 计算机图形学里面的数学用的比较多，，我们可以列举一些常用的：
高等数学，数值分析，微分几何，拓扑，概率， 插值理论，（偏）微分方程…
* 物理， 如果你要进行基于物理的建模，一些物理理论是要学习的：
力学（运动学，动力学，流体力学…），光学，有限元…
* 编程语言： C或C++是计算机图形学最通用的‘普通话’，
* 数据结构： 你需要数据结构来描述你的图形对象，除了通用的链表、树等数据结构外，图形学还有自己特殊的数据结构
* 其他类别： 有的时候你需要其他学科的知识，根据你的需要去学习吧

上面列举的不是你必须学习的东西，而是计算机图形学可能会用到的东西，一定要记住，不要指望通过一本教材就学会计算机图形学，它比你想象的要复杂的多。

## 图形学的问题

每个学科都有自己学科的特定问题，图形学要解决的是如何画出图来，得到需要的效果，当然这是图形学最大的一个问题。
在开始学习计算机图形学的时候，找一本简单的书看，对计算机图形学有个大概的认识，你就可以开始图形学之旅了：

OpenGL Programming Guide: The Official Guide to Learning OpenGL, Version 1.4, Fourth Edition
OpenGL SuperBible (3rd Edition) 

是比较好的学习计算机图形学的入门教材，在练中去学，一开始就去啃

Foley的Computer Graphics: Principles and Practice, Second Edition in C 
不是好主意，会看的一头雾水，一本什么都讲的书的结果往往是什么都没讲清楚。
当你把OpenGL的基本内容掌握之后，你对图形学就有了大概的了解了

那么下面你可以来学习一下计算机图形学的数据结构和算法，下面的书比较适合Joseph O'Rourke 的Computational Geometry in C，书里面有C的源代码，讲述简单，清晰，适合程序员学习

总的来说，计算机图形学涉及到2大部分：建模和渲染

### 建模

你想画一个东西，首先要有它的几何模型，那么这个几何模型从什么地方来呢？下面的书很不错的：
Gerald Farin 的Curves and Surfaces for CAGD: A Practical Guide

这本书算是CAGD (计算机辅助几何设计)的经典图书，CAGD方面的全貌，还有2本很好的讲述曲面的书Bezier和Nurbs的书

Les A. Piegl, Wayne Tiller 的The Nurbs Book 
书里面有NURBS曲线、曲面的程序伪代码，很容易改成C的，书讲的通俗、易懂，但是你要有耐心看的：）

曲线与曲面的数学
这本书是法国人写的中文翻译版，里面还有Bezie本人写的序J，翻译的很不错的，看了你就掌握Bezier曲面技术了
注意：在后面会有这样的章节，标明
里面是我认为的一些高级话题，跳过他们不影响你学习计算机图形学，但是要学好就要注意了，呵呵
还有其他的一些造型技术，比如：
隐式曲面(Implicit Surface)的造型：
就是用函数形式为F( x ,y ,z ) = 0的曲面进行造型，这样的造型技术适合描述动物器官一样的肉乎乎的东西，有2本书推荐大家
Jules Bloomenthal编辑的Introduction to Implicit Surfaces，是一本专著，讲述了Implicit Surface建模型(Modeling)，面片化(Polygonization)，渲染(Rendering)的问题
Luiz Velho 的 Implicit Objects Computer Graphics 也是一本专著，讲述个更新的一些进展

细分曲面（Subdivision Surface）造型
当用NURBS做造型的时候，曲面拼接是复杂的问题，在动画的时候，可能产生撕裂或者褶皱，Subdivision Surface用来解决这个问题
Joe Warren的Subdivision Methods for Geometric Design: A Constructive Approach就是这方面的专著

从实际物体中得到造型，现在的技术可以用三维扫描仪得到物体表面的点，然后根据这些点把物体的表面计算出来，称为重建(Reconstruction)，因为这些技术之在文章中论述，所以我们省略对它的描述
下面还是一个高级话题
在你的几何模型做好之后，有一些问题需要对这个模型进一步处理，得到适合的模型，当面片很多的时候，或者模型很复杂的时候，需要对几何模型进行简化，才可以满足一些实时绘制的需要，这个技术叫做层次细节（LOD-Level of Detail）。下面的书就是讲这个的：
David Luebke编著的 Level of Detail for 3D Graphics

### 渲染

有了模型，怎么把这个几何模型画出来呢？这个步骤就是渲染
如果你看了上面的OpenGL的书，那么你就知道一些渲染的知识了，但是别高兴的太早，OpenGL使用的是局部光照模型（Local Illumination Model），不要被这个词吓住了

Local illumination Model指的是在做渲染的时候只考虑光源和物体之间的相互作用，不考虑物体和物体之间的影响，所以OpenGL不支持阴影，一个（半）透明物体的效果..，这些需要考虑物体之间的影响才可以实现。

OpenGL本身不支持，但是通过一些方法可以实现的，用Google搜索一下
Shadow Volume, OpenGL就找到答案啦

Global Illumination Model 这类模型考虑的就比较全啦。现在关于Global Illumination的技术有3大类，具体的技术就不在这里介绍了，如果想了解，可以联系我，大家一起讨论：

光线追踪(Ray Tracing)
关于Ray Tracing的好书有2本：
Andrew Glassner 的An Introduction to Ray tracing 
Glasser是图形界的名人，这本书也是Ray Tracing的经典
R. Keith Morley, Peter Shirley 的Realistic Ray Tracing, Second Edition
这本书第一版是伪代码，第二版是C代码。它的结构不是很清楚，虎头蛇尾的感觉。
辐射度(Radiosity)
关于Radiosity的好书有4本：
Michael Cohen 的Radiosity and Realistic Image Synthesis ， Cohen获得SIGGRAPH 1998计算机图形学成就奖，他把Radiosity变成实际可用，现在Cohen在MSR图形 http://research.microsoft.com/~cohen/CohenSmallBW2.jpg

Francois X. Sillion的Radiosity and Global Illumination ， Sillion是法国人，他的主要研究方向是Radiosity，这本书写的很不错的，非常清晰

Philip Dutre 的新书Advanced Global Illumination ，看起来还不错，刚拿到手，还没看，呵呵，所以不好评价

Ian Ashdown的Radiosity: A Programmer's Perspective 

Photon mapping
Henrik Wann Jensen的Realistic Image Synthesis Using Photon Mapping
Henrik Wann Jensen是Photon mapping技术的发明者

### 这些也是图形学吗？ 图形和图象的区别模糊了

#### 非真实性图形学（Non-Photorealistic Graphics）
真实性不是计算机图形学的唯一要求，比如：你给我画一个卡通效果的图出来，或者我要用计算机画水彩画怎么办？或者：把图象用文字拼出来怎么做？，解决这些问题要用到非真实性图形学， 好书继续推荐！！！
Bruce Gooch, Amy Ashurst Gooch的 Non-Photorealistic Rendering 

#### 体图形学(Volume Graphics)

用CT机做很多切片（比如头骨），那么能通过这些切片得到3D的头骨吗？Volume Graphics就是解决这样的问题的
Min Chen 编著的Volume Graphics 
上面的2个图形学技术就和图象的界限不明显了，实际上他们是图形图象的综合

## 还有其他的书吗

Graphics Gems I ~ V，一大帮子人写的书，包括研究人员，程序员…
有计算机图形学的各种数据结构，编程技巧

Tomas Akenine-Moller 等人编著的Real-Time Rendering (2nd Edition) 
许多最新的计算机图形学进展

David Ebert等人的Texturing & Modeling: A Procedural Approach, Third Edition 
讲述如何通过程序实现纹理、山、地形等图形学要素
F. Kenton Musgrave号称分形狂(Fractal Mania)
Ken Perlin就是Perlin噪声的发明者，用过3d软件的人对Perlin Noise不会陌生的

关于图形学的特定对象，有特定的专题图书，
Evan Pipho Focus On 3D Models,对于图形学的常用模型格式，进行了讲解
Trent Polack的 Focus On 3D Terrain Programming ，讲地形的
Donald H. House 的Cloth Modeling and Animation ，讲布料的
Nik Lever的Real-time 3D Character Animation with Visual C++ ，讲角色动画的

Richard Parent的 Computer Animation: Algorithms and Techniques，当然是讲动画的啦，呵呵。
David H. Eberly的3D Game Engine Design : A Practical Approach to Real-Time Computer Graphics ，有代码的啊！呵呵：）

最后，没事情的时候，看看下面的书吧
Alan H. Watt， 3D Computer Graphics (3rd Edition) 

James D. Foley等人的 Computer Graphics: Principles and Practice in C (2nd Edition) ，这本圣经没事的时候再看吧，呵呵

# 游戏程序员养成计划

与玩游戏相比,写游戏要复杂上千万倍,除了需要掌握通用的编程技巧以外，还要有相当的图形学，物理，数学基础，特别是在国内，由于相关资料的缺乏，更是让初学者无从下手。下面总结了一些入门方法和比较容易入手的资料。

首先你要精通一门高级语言，比如C++或者C#，其次，要有良好的英文阅读能力。对游戏开发者来说英文阅读能力是最重要也是最基本的工具之一，因为你遇到的大部分资源都将是英文的，不要总等着别人为你翻译。慢慢尝试着阅读英文资料，你会发现其实也并没有那么难:)

刚开始，你要做的就是选择一门图形API，一般就是DirectX或者OpenGL之间选一个。如果考虑到跨平台，那么OGL是首选. 如果只在ms的平台，则DX是首选。我对OGL并不是很了解，所以下面大部门资料都是和DX相关的。

当然，作为准备工作之一，你首先要到DirectX Develop Center下载最新版的DirectX SDK。

入门书籍非常重要，推荐<<Introduction to 3D Game Programming with DirectX 9.0>>（好像去年出了中文版）也就是传说中的龙书，这可以说是最好的DX入门教材，Frank Luna从浅入深，讨论了DX的方方面面。另外再配上<< Advanced 3D Game Programming With DirectX 9.0>>，书名虽然是advanced，但实际上没有多少advanced级别的内容。看完这两本书，你基本上已经对DirectX比较熟悉了。如果你希望学习XNA，也是一样的，毕竟XNA是以DX为基础。

不要一开始就看图形学的书，这个时候你对图形编程还没有一个基本的感性认识，因此八成看的云里雾里。不要以网上的教程和论坛提问作为主要学习途径，找一本好书，系统学习，效率才最高。不要马上看SDK里的例子，很多图形学的基本原理仅仅通过读代码是不会明白的。某些年代太过久远的书就不要看了，比如《windows游戏编程大师技巧》（总看到有人在找这本书）。有人说基本的思想总是不变的，可惜对于现代GPU来说，很多早期的技术和优化技巧早就过时了。图形编程是发展的非常快的技术，看看GPU的发展速度，1~2年就是一代产品的革新。

好了，入门之后，是你巩固和拓展视野的阶段。现在看计算机图形学就比较合适了。吐血推荐<<Real-Time-Rendering>>,这本书算得上是所有图形程序员的必读书籍和参考手册了。最近刚出了第三版。可惜国内只有第二版，稍微有点老，如果实在找不到第三版，还是值得一读。国内其他所有以图形学命名的书都有一个共同点：枯燥，过时。只需看看其中二维三维变换和曲线曲面表示的部分即可。如果这个时候发现你当年数学没有学好，那么有三本数学书是为游戏程序员量身定制的：<<3D Math Primer for Graphics and Game Development>>, <<Mathematics for 3D Game Programming and Computer Graphics>>和<<Essential Mathematics Guide 2nd Edition>>，第一本书有中文版，最后一本则是08年才出的新书。

其实入门之后，就没有固定的学习路线了，最好根据你感兴趣的方向来学习。

Shader方面：《Cg_tutorial》和《The Complete Effect and HLSL Guide》都是不错的入门材料，当然还有SDK文档。<<Shaders for Game Programmers and Artists>>有大量入门的例子。<<Advanced Lighting And Materials With Shaders>>详细介绍了各种光照模型和技术。<<GPU Gems>> 1~3册肯定是必读的，虽然有1，2有中文版，但某些翻译并不是很理想，强烈建议直接看英文版。ShaderX系列也是很经典的系列，每年出版一本，包含了最新的实时渲染技术，如今已经出了第6册了。不过网络上只能找到1~3册。1，2册大部分shader都是用asm写的，不过看懂原理，转换为HLSL也并不难。另外Nvidia SDK和ATI SDK也是学习shader的重要资源。最后还有刚出的<< Programming Vertex, Geometry, and Pixel Shaders>>

地形：<<Real Time 3D Terrain Engines Using C++ And DX9>>非常全面的讨论了关于地形渲染的各种技术，至少应该把第5~9章都浏览一遍。之后便可以 到virtual terrain查阅近期的地形渲染技术。

模型导入和动画：<<Advanced Animation with DirectX>>，仅此一本足以。

物理：<<Game Physics>>和<<Game Physics Engine Development>>都不错。<<Real-time Collision Detection>>是碰撞检测方面最好的书，可惜目前还没有电子版。
LOD：<<Level of Detail for 3D Graphics>>
Ray tracing:<< Physical-Based Rendering - From Theory to Implementation>>
引擎设计：说实话，这方面还没有特别好的书，大概越是核心的内容，越少有人愿意写吧。<<3D Game Engine Architecture Engineering Real-Time Applications with Wild Magic>>只有第三章值得一读。<<3D Game Engine Programming>>可以选部分感兴趣的章节看看，不过总的来说，讲的比较浅。
AI：<<Programming Game AI by Example>>非常不错，好像还有中文版，备选<<Artificial Intelligence for Games>>。当然<<AI Programming Wisdom>>系列也是必读作品，不过目前网络上只有1～2册。
综合：<<Game Programming Gems>>系列，不过由于内容涉及的过于广泛，文章质量参差不齐，选择性阅读就可以了。历年GDC, Gamefest t,Siggraph等大型会议的paper也是应该关注的。
除了书以外，再推荐一些不错的网络资源：
www.GameDev.NET 除了大量教程以外，论坛里多年累计下来的内容保罗万象。好好利用论坛搜索，你会发不管你多厉害，那里总有一些比你强大的人在很久以前就遇到了和你同样的问题，并且给出了解决方案。
Nvidia和ATI的开发者中心
creators.xna.com XNA官方网站
www.gamasutra.com 与GameDev类似
www.beyond3d.com 这里的除了讨论软件以外，还能看到对硬件构架的分析
www.ziggyware.com 最好的XNA教程网站
www.gameres.com 国内唯一比较专业的游戏编程网站，可惜和GameDev相比就显得太寒碜了
http://imgtec.eetrend.com/
www.alanzucconi.com/category/shader/
http://shaderbits.com/blog/tiling-within-subuv-or-volume-textures
http://www.iquilezles.org/www/index.htm
http://www.opengpu.org/forum.php?mod=forumdisplay&fid=21
https://www.gamedev.net/
http://www.flipcode.com/archives/Light_Mapping_Theory_and_Implementation.shtml
http://www.realtimerendering.com/
http://kesen.realtimerendering.com/