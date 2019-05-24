---
title: 【基于物理的渲染（PBR）白皮书】（一）开篇：PBR核心知识体系总结与概览
date: 2018-05-14 19:46:49
mathjax: true
tags:
    - PBR
---
### 光与物质
光与物质：基于物理的渲染与着色理论
光是一种复杂的现象，它同时展现出波与粒子两种特性。于是，为了描述光的表现特性，人们建立了各种模型。
作为材质艺术家，我们感兴趣的是光线模型（Light Ray Model）因为它描述了光与物质的交互作用。
对我们来说，理解光线如何与物质表面交互非常重要，因为我们的工作是创作描述物体表面的材质。
我们创作出的纹理与材质再虚拟世界中与光交互，对光线表现特效理解的越多，创作出的材质就会更好。
再这篇指南中，我们会讨论物理理论，而基于物理的渲染(physically-based rendering)(PBR)模型正是建立在其上的。
我们从光线开始，逐步讲解PBR中的关键点。
### 光线
光线模型阐明，光线在均匀透明介质（如空气）中的轨迹为直线。光线模型同时也阐明，当光传播遇到表面是，例如遇到不透明物体或穿过不同介质如从空气折射入水中，光的表现是可预测的。这样，当光从一个起点传播到某点最终转换为其他形式例如热时，我们可以将它的轨迹视觉化。
击中一个表面的光线成为入射光，而它击中的角度称为入射角，如图01所示。
一条光线射入两个不同介质间的平面交界面。
先放出PBR知识体系的架构图：
{%asset_img 1.jpg%}

# 系列文章前言
基于物理的渲染（Physically Based Rendering，PBR）技术，自迪士尼SIGGRAPH 2012上提出了著名的“迪士尼原则的BRDF（Disney Principled BRDF）”之后，由于其高度的易用性以及方便的工作流，已经被电影和游戏业界广泛的使用。
http://renderwonk.com/publications/s2010-shading-course/
https://blog.selfshadow.com/publications/s2012-shading-course/
https://blog.selfshadow.com/publications/s2013-shading-course/
https://blog.selfshadow.com/publications/s2014-shading-course/
https://blog.selfshadow.com/publications/s2015-shading-course/
https://blog.selfshadow.com/publications/s2016-shading-course/
https://blog.selfshadow.com/publications/s2017-shading-course/
《Physically Based Rendering:From Theory to Implementation,Third Edition》这本书专注离线渲染，实时渲染只能用到里面很少的一部分。
http://www.pbr-book.org/3ed-2018/contents.html
《Real-Time Rendering 4th》中PBR的相关章节
《Physically Based Shader Development for Unity》，主要是PBR在Unity引擎中的使用，而且是以Surface Shader的方式，准入门级，比较浅。
# PBR知识体系概览
这篇文章接下来的部分，是这个系列文件PBR知识体系的精华浓缩版。涉及八个部分的内容：
- 核心PBR理论
- 渲染方程与BxDF
- 迪士尼原则的BxDF（Disney Principled BxDF）
- 漫反射BRDF模型（Diffuse BRDF）
- 镜面反射BRDF模型（Specular BRDF）
- 基于物理的环境光照（Physically Based Environment Lighting）
- 离线渲染相关（Offline Rendering Related）
- 进阶渲染主题（Advanced Rendering Topics）

# PBR核心理论与渲染原理
PBR核心知识体系的第一部分自然是PBR的核心理论以及相关的渲染原理。

{%asset_img 2.jpg%}
基于物理的渲染（Physically Based Rendering，PBR）是指使用基于物理原理和微平面理论建模的着色/光照模型，以及使用从现实中测量的表面参数来准确表示真实世界材质的渲染理念。
{%asset_img 3.jpg%}
以下是对PBR基础理念的概括：
- 微平面理论（Microfacet Theory）。微平面理论是将物体表面建模成无数微观尺度上有随机朝向的理想镜面反射的小平面（microfacet）的理论。在实际的PBR工作流中，这种物体表面的不规则性用粗糙度贴图或者高光贴图来表示。
- 能量守恒（Energy Conservation）。出射光线的能量永远不能超过入射光线的能量。随着粗糙度的上升镜面反射区域的面积会增加，作为平衡，镜面反射区域的平均亮度则会下降。
- 菲涅尔反射（Fresnel Reflectance）。光线以不同角度入射会有不同的反射率。相同的入射角度，不同的物质也会有不同的反射率。万物皆有菲涅尔反射。F0是即0度角入射的菲涅尔反射。大多数非金属的F0范围是0.02~0.04，大多数金属的F0范围是0.7~1.0。
- 线性空间（Liner Space）。光照计算必须在线性空间完成，shader中输入的gamma空间的贴图比如漫反射贴图需要被转成线性空间，在具体操作时需要根据不同引擎和渲染器的不同做不同的操作。而描述物体属性的贴图如粗糙度，高光贴图，金属贴图等必须保证是线性空间。
- 色调映射（Tone Mapping）。也称色调复制（tone reproduction），是将宽范围的照明级别拟合到屏幕有限色域内的过程。因为基于HDR渲染出来的亮度值会超过显示器能够现实最大亮度，所以需要使用色调映射，将光照结果从HDR转换微显示器能够正常显示的LDR。
- 物质的光学特性（Substance Optical Properties）。现实世界中有不同类型的物质可分为三大类：绝缘体（Insulators），半导体（semi-conductors）和导体（conductors）。在渲染和游戏领域，我们一般只对其中的两个感兴趣：导体（金属）和绝缘体（电解质，非金属）。其中非金属具有单色/灰色镜面反射颜色。而金属具有彩色的镜面反射颜色。即非金属的F0是一个float。而金属的F0是一个float3，如下图。
{%asset_img 4.jpg 金属和非金属材质的F0范围 %}

## 光与非光学平坦表面的交互原理
光在与非光学平坦表面（Non-Optically-Flat Surfaces）的交互时，非光学平坦表面表现得像一个微小的光学平面表面的大集合。表面上的每个点都会以略微不同的方向对入射光反射，而最终的表面外观是许多具有不同表面取向的点的聚合结果。
{%asset_img 5.jpg 来自非光学平坦表面的可见光反射是来自具有不同方向的许多表面点的反射的总体结果%}
在微观尺度上，表面越粗糙，反射越模糊，因为表面取向与整个宏观表面取向的偏离更强。
{%asset_img 6.jpg%}
图片顶部所示的表面，表面相对光滑；表面取向仅略有变化，从而产生更清晰的反射。图片底部所示的表面较粗糙；表面上的不同点具有广泛变化的方向取向，导致反射光方向的高度变化，并因此导致模糊的反射。注意，两个表面在肉眼可见尺度下看起来是光滑的，粗糙度差异仅在微观尺度上。
出于着色的目的，我们通常会去用统计方法处理这种微观几何现象，并将表面视为在每个点处在多个方向上反射（和折射）光。
{%asset_img 7.jpg 从宏观上看，非光学平面可以被视为多个方向上反射（和折射）光%}
从表面反射的光的行为很好理解，那么，从表面折射的光会发生什么变化？这取决于对象本身的特性：
- 对于金属，折射会立刻被吸收-能量被自由电子立即吸收。
- 对于非金属（也成为电介质或绝缘体），一旦光在其内部折射，就表现为常规的参与介质，表现出吸收和散射两种行为。

{%asset_img 8.jpg 在金属中，所有折射的光能立即被自由电子吸收 %}
{%asset_img 9.jpg 在非金属中，折射的光会进行散射，直到从表面重新射出，而这通常会在经过部分吸收之后 %}
## 漫反射和次表面散射本质相同
另外，漫反射和次表面散射其实是相同的物理现象，本质都是折射光的次表面散射的结果。唯一的区别是相对于观察尺度的散射距离。散射距离相较于像素来说微不足道，次表面散射便可以近似为漫反射。也就是说，光的折射现象，建模为漫反射还是次表面散射，取决于观察的尺度，如下图。
{%asset_img 10.jpg%}
在左上角，像素（带有红色边框的绿色圆形）大于光想离开表面之前所经过的距离。在这种情况下，可以假设出射光从入口点（右上）射出，可以当作漫反射，用局部着色模型处理。在底部，像素小于散射距离；如果需要更真实的着色效果，则不能忽略这些距离的存在，需当作次表面散射现象进行处理。
## 一、PBR的范畴（Scope of PBR）
寒霜（Frostbite）引擎在SIGGRAPH 2014的分享《Moving Frostbite to PBR》中提出，基于物理的渲染的范畴，由三部分组成：
- 基于物理的材质（Material）
- 基于物理的光照（Lighting）
- 基于物理适配的摄像机（Camera）
{%asset_img 11.jpg%}
完整的这三者，才是真正完整的基于物理的渲染系统。而很多同学一提到PBR，就说PBR就是镜面反射采用微平面Cook-Torrance模型，其实是不太严谨的。
## 二、渲染方程与BxDF
PBR核心知识体系的第二部分是渲染方程与BxDF。渲染方程作为渲染领域中的重要理论，将BxDF代入渲染方程是求解渲染问题的一般方法。
{%asset_img 12.jpg%}
### 2.1渲染方程与反射方程
渲染方程（The Rendering Equation）作为渲染领域中的重要理论，其描述了光能在场景中的流动，是渲染中不可感知方面的最抽象的正式表示。根据光学的物理学原理，渲染方程在理论上给出了一个完美的结果，而各种各样的渲染技术，知识这个理想结果的一个近似。

渲染方程的物理基础是能量守恒定律。在一个特定的位置和方向，出射光$L_o$是自发光$L_e$与反射光线之和，反射光线本身是各个方向的入射光Li之和诚意表面反射率及入射角。

这个方程金国交叉点将出射光线与入射光线联系在一起，它代表了场景中全部的光线传输。所有更加完善的算法都可以看作是这个方程的特殊形式的解。

某一点p的渲染方程，可以表示为：
$L_o=L_e + $<font size="5">$\int_\Omega $</font>${f_r\cdot L_i \cdot (w_i\cdot n)\cdot dw_i}$
其中：
- $L_o$是p点的出射光亮度。
- $L_e$是p点的发出的光亮度。
- $f_r$是p点入射方向到出射方向光的反射比例，即BxDF，一般为BRDF。
- $L_i$是p点入射光亮度。
- $(w_i\cdot n)$是入射角带来的入射光衰减。
- <font size="5">$\int_\Omega $</font>$...dw_i$是入射方向半球积分（可以理解为无穷小的累加和）。

{%asset_img 13.jpg%}
而在实时渲染中，我们常用的反射方程（The Reflctance Equation），则是渲染方程的简化的版本，或者说是一个特例：
$L_o=L_e + $<font size="5">$\int_\Omega $</font>${f_r\cdot L_i \cdot (w_i\cdot n)\cdot dw_i}$
同样，其中：
- $L_o$是p点的出射光亮度。
- $f_r$是p点入射方向到出射方向光的反射比例，即BxDF，一般为BRDF。
- $L_i$是p点入射光亮度。
- $(w_i\cdot n)$是入射角带来的入射光衰减。
- <font size="5">$\int_\Omega $</font>$...dw_i$是入射方向半球积分（可以理解为无穷小的累加和）。

## 2.2 BxDF
BxDF一般而言是对BRDF、BTDF、BSDF、BSSRDF等几种双向反射分布函数的一个统一的表示。
其中，BSDF可以看作BRDF和BTDF更一般的形式，而且BSDF=BRDF+BTDF。
而BSSRDF和BRDF的不同之处在于，BSSRDF可以指定不同的光线入射位置和出射位置。
在上述这些BxDF中，BRDF最为简单，也最为常用。因为游戏和电影中的大多数物体都是不透明的，用BRDF就完全足够。而BSDF、BTDF、BSSRDF往往更多用于半透明材质和次表面散射材质。
{%asset_img 14.jpg BSDF:BRDF+BTDF %}
我们时常讨论的PBR中的BxDF，一般都为BRDF，对于进阶的一些材质的渲染，才会讨论BSDF等其它三种BxDF。
另外，BxDF即上文所示渲染方程以及反射方程中的$f_r$项。
## 2.3 BRDF的分类
{%asset_img 15.jpg%}

# 三、迪士尼原则的BxDF（Disney Principled BxDF）

PBR核心知识体系的第三部分是迪士尼原则的BxDF。迪士尼动画工作室在SIGGRAPH 2012上著名的talk《Physically-based shading at Disney》中提出了迪士尼原则的BRDF（Disney Principled BRDF）,奠定了后续游戏行业和电影行业PBR的方向和标准。了解Disney Principled BxDF，是深入理解PBR的重要一环。
{%asset_img 16.jpg%}
基于物理的渲染，其实早在20世纪就已经在图形学业界有了一些套路怒，2010年在SIGGRAPH上就已经有公开讨论的Course《SIGGRAPH 2010 Course:Physically-Based Shading Models in File and Game Production》，而直到2012~2013年，才正式进入大宗的视野，渐渐被电影和游戏业界广泛使用。

迪士尼动画工作室则是这次PBR革命的重要推动者。迪士尼的Brent Burley与SIGGRAPH 2012上进行了著名的talk《Physically-based shading at Disney》，提出了迪士尼原则的BRDF（Disney Principled BRDF），由于其高度的通用性，将材质复杂的物理属性，用非常直观的少量变量表达了出来（如金属度metallic和粗糙度roughness），在电影业界和游戏业界引起了不小的轰动。从此，基于物理的渲染正式进入大众的视野。
{%asset_img 17.jpg SIGGRAPH 2012 《Physically-based shading at Disney》 %}

在2012年受到Diney的启发后，以下是主流游戏引擎从传统渲染转移到基于物理的渲染时间节点：
- [SIGGRAPH 2013]UE4 : [Real shading in unreal engine 4]
- [SIGGRAPH 2014] Frostbite （寒霜）:[Moving Frostbite to PBR]
- [GDC 2014] Unity : [Physically Based Shading in Unity]

## 3.1 迪士尼原则的BRDF（Disney Principled BRDF）
### 3.1.1 Disney Principled BRDF核心理念
在2012年迪士尼原则的BRDF被提出之前，基于物理的渲染都需要大量复杂而不直观的参数，此时PBR的优势，并没有那么明显。

在2012年迪士尼提出，他们的着色模型是艺术导向（Art Directable）的，而不一定要是完全物理正确（physically correct）的，并且对为平面BRDF的各项都进行了严谨的调查，并提出了清晰明确而简单的解决方案。

迪士尼的理念是开发一种“原则性”的易用模型，而不是严格的物理模型。正因为这种艺术导向的易用性，能让美术同学非常直观的使用少量参数，以及非常标准化的工作流，就能快速实现涉及大量不同材质的真实感的渲染工作。而这对于传统的着色模型来说，是不可能完成的任务。

迪士尼原则的BRDF（Disney Principled BRDF）核心理念如下：
1. 应使用直观的参数，而不是物理类的晦涩参数。
2. 参数应尽可能少。
3. 参数在其合理范围内应该为0到1。
4. 允许参数在有意义时超出正常的合理范围。
5. 所有参数组合应尽可能健壮和合理。

以上五条原则，很好地保证了迪士尼原则的BRDF的易用性。
### 3.1.2 Disney Principled BRDF参数
以上理念为基础，迪士尼动画工作室对每个参数的添加进行了把关，最终得到了颜色参数（baseColor）和下面描述的是个标量参数：
- **baseColor（基础色）**：表面颜色，通常由纹理贴图提供。
- **subsurface（次表面）**：使用次表面近似控制漫反射形状。
- **metallic（金属度）**：金属（0=电介质，1=金属）。这是两种不同模型之间的线性混合。金属模型没有漫反射成分，并且还具有等于基础色着色入射镜面反射。
- **specular（镜面反射强度）**：入射镜面反射两。用于取代折射率。
- **specularTint（镜面反射颜色）**：对美术控制的让步，用于对基础色（base color）的入射镜面反射进行颜色控制。掠射镜面反射仍然时非彩色的。
- **roughness（粗糙度）**：表面粗糙度，控制漫反射和镜面反射。
- **anisotropic（各向异性强度）**：各向异性强度。用于控制镜面反射高光的纵横比。（0=各向同性，1=最大各向异性）
- **sheen（光泽度）**：一种额外的掠射分量（grazing component），主要用于布料。
- **sheenTine（光泽颜色）**：对sheen（光泽度）的颜色控制。
- **clearcoat（清漆强度）**：有特殊用途的第二个镜面波瓣（specular lobe）。
- **clearcoatGloss（清漆光泽度）**：控制透明涂层光泽度，0=“缎面（satin）”外观，1=“光泽（gloss）”外观。

每个参数的效果的渲染示例如下图所示。
{%asset_img 18.jpg Disney Principled BRDF，每行的参数从0到1变化，其他参数保持不变 %}

## 3.2 迪士尼原则的BSDF（Disney Principled BSDF）
随后的2015年，迪士尼动画工作室在Disney Principled BRDF的基础上进行了修订，提出了Disney Principled BSDF [Extenting the Disney BRDF to a BSDF with Integrated Subsurface Scattering,2015]

以下是开源三维动画软件Blender实现的Disney Principled BSDF的图示：
{%asset_img 19.jpg Disney Principled BSDF %}

# 四、漫反射BRDF模型（Diffuse BRDF）
为了求解渲染方程，需要分别求解Diffuse BRDF和Specular BRDF。所以PBR核心知识体系的第四部分是Diffuse BRDF。
{%asset_img 20.jpg%}
Diffuse BRDF可以分为传统型和基于物理型两大类。其中，传统型主要是总所周知的Lambert。
而基于物理型，从1994年的Oren Nayar开始，这里一直统计到今年（2019年）。
其中较新的有GDC 2017上提出的适用于GGX+Smith的基于物理的漫反射模型（PBR diffuse for GGX+Smith），也包含了最近在SIGGRAPH2018上提出的，来自《使命召唤：二战》的多散射漫反射BRDF（MultiScattrering Diffuse BRDF）：

- Oren Nayar[1994]
- Simplified Oren-Nayar[2012]
- Disney Diffuse[2012]
- Renormalized Disney Diffuse[2014]
- Gotanda Diffuse[2014]
- PBR diffuse for GGX+Smith[2017]
- MultiScattrering Diffuse BRDF[2018]

# 五、镜面反射BRDF模型（Specular BRDF）
PBR核心知识提醒的第五部分是Specular BRDF。这也是基于物理的渲染领域中最活跃，最主要的部分。
{%asset_img 21.jpg%}
上图中加粗部分为目前业界较为主流的模型。

游戏业界目前最主流的基于物理的镜面反射BRDF模型是基于微平面理论（microfacet theory）的Microfacet Cook-Torrance BRDF。

而微平面理论（microfacet theory）源自将微观几何（microgeometry）建模为微平面（microfacets）的集合思想，一般用于描述来自非光学平坦（non-optically flat）表面的表面反射。

微平面理论的基本假设是微观几何（microgeometry）的存在，微观几何的尺度小于观察尺度（例如着色分辨率），但大于可见光波长的尺度（因此应用几何光学如衍射一样的波效应等可以忽略）。且微平面理论在2013年和以前时仅用于推导单反射（single-bounce）表面反射的表达式；而随着领域的深入，最近几年也出现了使用microfacet理论对多次反弹表面的一些探讨。

由于假设微观几何尺度明显大于可见光波长，因此可以将每个表面点视为光学平坦的。如上文所述，光学平坦表面将光线分成两个方向：反射和折射。

每个表面点将来自给定进入方向的光反射到单个出射方向，该方向取决于微观几何法线（microgeometry normal）m的方向。在计算BRDF项时，指定光方向I和视图方向**v**。这意味着所有表面点，只有那些恰好正确朝向可以将**I**反射到**v**的那些小平面可能有助于BRDF值（其他方向有正有负，积分之后，相互抵消）。

在下图中，我们可以看到这些“正确朝向”的表面点的表面法线**m**正好位于**I**和**v**之间的中间位置。**I**和**v**之间的矢量成为半矢量（half-vector）或半角矢量（half-angle vector）；我们将其表示为**h**。
{%asset_img 22.jpg 仅m=h的表面点的朝向才会将光线I反射到视线v的方向，其他表面点对BRDF没有贡献。 %}

并非所有的**m = h**的表面点都会积极地对反射做出共享；一个被**I**方向（阴影shadowing），v方向（掩蔽masking）或两者的其他表面区域阻挡。Microfacet理论假设所有被遮蔽的光（shadowed light）都从镜面反射项中消失；实际上，由于多次表面反射，其中一些最终将是可见的，但这在目前常见的微平面理论中一般并未去考虑，各种类型的光表面相互作用如下图所示。
{%asset_img 23.jpg%}
上图在左侧，我们看到一些表面点从I的方向被遮挡，因为它们被遮挡并且不接受光（因此它们不能反射任何）。在中间，我们看到从视图方向v看不到一些表面点，因此当然不会看到从它们反射的任何光。在这两种情况下，这些表面点对BRDF没有共享。实际上，虽然阴影区域没有接受从I接受任何直射光，但它们确实接受（并因此反射）从其他表面区域反射的光（如右图所示）。microfacet理论忽略了这些互相反射。

## 5.1 从物理现象到BRDF
利用这些假设（局部光学平坦表面，没有相互反射），可以很容易推导出一个被成为Microfacet Cook-Torrnace BRDF的一般形式的Specular BRDF项。此Specular BRDF具有以下形式：

$f(l,v)={D(h)F(v,h)G(l,v,h)\over 4(n\cdot l)(n \cdot v)}$

其中：

- **D(h)**:法线分布函数（Normal Distribution Function），描述微面元发现分布的概率，即正确朝向的发现的浓度。即具有正确朝向，能够将来自**I**的光反射到v的表面点的相对于表面面积的浓度。
- **F(I,h)**:菲涅尔方程（Fresnel Equation），描述不同的表面角下表面所反射的光线所占的比率。
- **G(l,v,h)**:几何函数（Geometry Function），描述微平面自成阴影的属性，即m=h的未被遮蔽的表面点的百分比。
- **分母4(n\cdot l)(n \cdot v)**:校正因子（correctionfactor），作为微观几何的局部空间和整个宏观表面的局部空间之间变换的微平面量的校正。

关于Cook-Torrance BRDF，需要强调的两点注意事项：

- 对于分母中的点击，仅仅避免负值时不够的，也必须避免零值。通常通过在常规的clamp或绝对值操作之后添加非常小的正值来完成。
- **Microfacet Cook-Torrance BRDF是实践中使用最广泛的模型，实际上也是人们可以想到的最简单的微平面模型。它仅对几何光学系统中的单层为表面上的单个散射进行建模，没有考虑多次散射，分层材质，以及衍射。Microfacet模型，实际上还有很长的路要走。**

下面对Microfacet Cook-Torrance BRDF中的D、F、G项分别进行简单说明。

## 5.2 Specular D

法线分布函数（Normal Distribution Function, NDF）D的常见模型可以总结如下：

- Beckmann[1963]
- Blinn-Phong[1977]
- GGX[2007]/Trowbridge-Reitz[1975]
- Generalized-Trowbridge-Reitz(GTR)[2012]
- Anisotropic Beckmann[2012]
- Anisotropic GGX[20115]

其中，业界较为主流的法线分布函数是GGX（Trowbridge-Reitz），因为具有更好的高光长尾：

$D_{GGX}$