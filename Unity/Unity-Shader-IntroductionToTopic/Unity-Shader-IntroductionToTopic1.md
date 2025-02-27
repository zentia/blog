### 光源

在光学里，我们使用辐照度（irradiance）来量化光。对于平行光来说，它的辐照度可通过计算在垂直于l的单位平面上的单位时间内穿过的能量来得到。

#### 吸收和散射

光线由光源发射出来后，就会与一些物体相交。通常，相交的结构有两个：散射（scattering）和吸收（absorption）。
散射只改变光线的方向，但不改变光线的密度和颜色。而吸收只改变光线的密度和颜色，但不改变光线的方向。

#### BRDF光照模型

我们已经了解了光线在和物体表面相交时会发生哪些现象。当已知光源位置和方向、视角方向时，我们就需要知道一个表面是和光照进行交互的。例如，当光线从某个方向照射到一个表面时，有多少光线被反射？反射的方向有哪些？而BRDF（Bidirectional Reflectance Distribution Function）就是用来回答这些问题的。当给定模型表面上的一个点时，BRDF包含了对该点外观的完整的描述。在图形学种，BRDF大多使用一个数学公式来表示，并且提供了一些参数来调整材质属性。通俗来讲，当给定入射光线的方向和辐照度后，BRDK可以给出某个出射方向上的光照能量分布。本章涉及的BRDF都是对真实场景进行理想化和简化后的模型，也就是说，它们并不能真实地反映物体和光线之间地交互，这些光照模型被成为是经验模型。尽管如此，这些经验模型仍然在实时渲染领域被应用了多年。

### 标准光照模型

基本方法是，把进入到摄像机的光线分为4部分，每个部分使用一种方法来计算它的贡献度

- 自发光(emissive)
- 高光反射(specular)
- 漫反射(diffuse)
- 环境光(ambient)

#### 环境光

在标准光照模型中，使用一种被称为环境光的部分来近似模拟间接光照。环境光的计算非常简单，它通常是一个全局变量，即场景中的所有物体都使用这个环境光。下面的等式给出了计算环境光的部分：

$c_{ambient}=g_{ambient}$

#### 自发光

光线也可以直接由光源发射进入摄像机，而不需要经过任何物体的反射。标准光照模型使用自发光来计算这个部分的贡献度。它的计算也很简单，就是直接使用了该材质的自发光颜色：

$c_{emissive}=m_{emissive}$

通常在实时渲染中，自发光的表面往往并不会照亮周围的表面，也就是说，这个物体并不会被当成一个光源。

#### 漫反射

漫反射光照是用于对那些被物体表面随机散射到各个方向的辐射度进行建模的。在漫反射中，视角的位置是不重要的，因为反射是完全随机的，因此可以认为在任何反射方向上的分布都是一样的。但是，入射光线的角度很重要。
漫反射光照符合**兰伯特定律（Lambert's law）**：反射光线的强度与表面发现和光源方向之间夹角的余弦值成正比。因此，漫反射部分的计算如下：

$c_{diffuse} = (c_{light} \cdot m_{diffuse})max(0, n \cdot I)$

#### 高光反射

这里的高光反射是一种经验模型，也就是说，它并不完全符合真实世界中的高光反射现象。它可用于计算那些沿着完全镜面反射方向被反射的光线，这可以让物体看起来是有光泽的，例如金属材质。
计算高光反射需要知道的信息比较多，如表面法线、视角方向、光源方向、反射方向等。
反射计算公式：
$r = 2(\vec{n} \cdot I)\vec{n} - I$
推导公式见Light Reflect
这样，我们就可以利用Phong模型来计算高光反射的部分：
<font size="4">$c_{spscular} = (c_{light} \cdot m_{specular})max(0, \vec{v} \cdot r)^{m_{glass}}$</font>
其中，$m_{gloss}$是材质的**光泽度（gloss）**，也被称为**反光度（shinness）**。它用于控制高光区域的“亮点”有多宽，$m_{gloss}$越大，亮点就越小。$m_{spscular}$是材质的高光反射颜色，它用于控制该材质对于高光反射的强度和颜色。$c_{light}则是光源的颜色和强度。$

#### 逐像素还是逐顶点

#### 总结

### 单张纹理

#### 实践

#### 纹理的属性

### 凹凸映射

#### 高度纹理

#### 法线纹理

#### 实践

#### 法线纹理类型

### 渐变纹理

### 遮罩纹理

#### 实践

#### 其他遮罩纹理