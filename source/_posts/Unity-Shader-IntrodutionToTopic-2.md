---
title: Unity Shader第三部分
date: 2018-10-22 18:12:38
tags:
  - 图形
mathjax: true
categories: "Unity Shader"
---

# 透明效果
透明是游戏中经常要使用的一种效果。在实时渲染中要实现透明效果，通常会在渲染模型时控制它的**透明通道（Alpha Channel）**。当开启透明混合后，当一个物体被渲染到屏幕上时，每个片元除了颜色值和深度值之外，它还有透明度。当透明度为1时，表示该像素是完全不透明的，而当其为0时，则表示该像素完全不会显示。
在Unity中，我们通常使用两种方法来实现透明效果：第一种是使用**透明度测试（Alpha Test）**，这种效果其实无法得到真正的半透明效果；另一种是**透明度混合（Alpha Blending）**。
在之前的学习中，我们从没有强调过渲染顺序的问题。也就是说，当场景中包含很多模型时，我们并没有考虑是先渲染A，在渲染B，最后在渲染C，还是按照其他的顺序来渲染。事实上，对于不透明（opaque）物体，不考虑它们的渲染顺序也能得到正确的排序效果，这是由于强大的深度缓冲（depth buffer，也被称为z-buffer）的存在。在实时渲染中，深度缓冲是用于解决可见性（visibility）问题的，它可以决定哪个物体的哪些部分会被渲染在前面，而哪些部分会被其他物体遮挡。它的基本思想是：根据深度缓冲中的值来判断该片元距离摄像机的距离，当渲染一个片元时，需要把它的深度值和已经存在于深度缓冲中的值进行比较（如果开启了深度测试），如果它的值距离摄像机更远，那么说明这个片元不应该被渲染到屏幕上（有物体遮挡住了它）；否则，这个片元应该覆盖掉此时颜色缓冲中的像素值，并把它的深度值更新到深度缓冲中（如果开启了深度写入）。

## 透明度混合

透明度混合的实现要比透明度测试复杂一些，这是因为我们在处理透明度测试时，实际上跟对待普通的不透明物体几乎是一样的，只是在片元着色器中增加了对透明度判断并裁剪片元的代码。而想要实现透明度混合就没这么简单了。我们回顾之前提到的透明度混合的原理。
**透明度混合：**这种方法可以得到真正的半透明效果。它会使用当前片元的透明度作为混合因子，与已经存储在颜色缓冲区种的颜色进行混合，得到新的颜色。但是，<font color=red>**透明度混合需要关闭深度写入**</font>，这使得我们要非常小心物体的渲染顺序。
为了进行混合，我们需要使用Unity提供的混合命令Blend。Blend是Unity提供的设置混合模式的命令。想要实现半透明的效果就需要把当前自身的颜色和已经存在于颜色缓冲区种的颜色值进行混合，混合时使用的函数就是由该指令决定的。

|语义     | 描述    |
|----|----|
|Blend Off|关闭混合|
|Blend SrcFactor DstFactor|开启混合，并设置混合因子。源颜色（该片元产生的颜色）会乘以SrcFactor，而目标颜色（已经存在于颜色缓存的颜色）会乘以DstFactor，然后把两者相加后在存入颜色缓冲中|
|Blend SrcFactor DstFactor, SrcFactorA DstFractorA|和上面一样，只是使用不同的因子来混合透明通道|
|BlendOp BlendOperation|并非是把源颜色和目标颜色简单相加后混合，而是使用BlendOperation对它们进行其他操作|

我们介绍第二种语义，即Blend SrcFactor DstFactor来进行混合。需要注意的是，这个命令在设置混合因子的同时也开启了混合模式。这是因为，只有开启了混合之后，设置片元的透明通道才有意义，而Unity在我们使用Blend命令的时候就自动帮我们打开了。很多初学者抱怨为什么自己的模型没有任何透明效果，这往往是因为他们没有在Pass中使用Blend命令，一方面是没有设置混合因子，更重要的是没有打开混合模式。我们会把源颜色的混合因子SrcFactor设为SrcAlpha，而目标颜色的混合因子DstFactor设为OneMinusSrcAlpha。这意味着，经过混合后新的颜色是：

$DstColor_{new} = SrcAlpha*SrcColor+(1-SrcAlpha)*DstColor_{old}$

### 开启深度写入的半透明效果

### ShaderLab的混合命令

#### 混合等式和参数

#### 混合操作

#### 常见和混合类型

### 双面渲染的透明效果

#### 透明度测试的双面渲染

#### 透明度混合的双面渲染

## 渲染路径Rendering Path

|LightMode    |描述 |
|------ |-------|
|Always |不管使用哪种渲染路径，该Pass总是会被渲染，但不会计算任何光照       |
|ForwardBase|用于前向渲染。该Pass会计算环境光、最重要的平行光、逐顶点/SH光源和Lightmaps|
|ForwardAdd|用于前向渲染。该Pass会计算额外的逐像素光源，每个Pass对应一个光源
|Deferred|用于延迟渲染。该Pass会渲染G缓冲（G-buffer）|
|ShadowCaster|把物体的深度信息渲染到阴影映射纹理（shadowmap）或一张深度纹理中|
|PrepassBase |用于遗留的延迟渲染。该Pass会渲染法线和高光反射的指数部分|
|PrepassFinal |用于遗留的延迟渲染。该Pass通过合并纹理、光照和自发光来渲染得到最后的颜色|
|Vertex、VertexLMRGBM和VertexLX|用于遗留的顶点照明渲染|

Unity在处理多光源的情况时为我们提供了三种模式；修改的地方在Edit->Project Settings--Player--Other Settings--Redering Path

- 顶点光Vertex Lit
- 方向性Forward(默认)
- 延迟照明 Deferred Lighting

shader默认使用Forward

#### 前向渲染路径

每进行一次完整的前向渲染，需要渲染该对象的渲染图元，并计算两个缓冲区的信息：一个是颜色缓冲区，一个是深度缓冲区。利用深度缓冲区来决定一个片元是否可见，如果可见就更新颜色缓冲区的的颜色值。我们可以用下面的伪代码来描述前向渲染路径的大致过程：

```C++
Pass 
{
    for (each primitive in this model) 
    {
        if (failed in depth test)
        {
            // 如果没有通过深度测试，说明该片元是不可见的
            discard;
        }
        else
        {
            // 如果该片元可见
            // 就进行光照计算
            float4 color = Shading(materialInfo, pos, normal, lightDir, viewDir);
            // 更新帧缓冲
            writeFrameBuffer(fragment, color);
        }
    }
}
```

对于每个逐像素光源，我们都需要进行上面一次完整的渲染流程。如果一个物体在多个逐像素光源的影响区域内，那么该物体就需要执行多个Pass，每个Pass计算一个逐像素光源的光照结果，然后在帧缓冲中把这些光照结果混合起来得到最终的颜色值。假设，场景中有N个物体，每个物体物体受M个光源的影响，那么要渲染整个场景一共需要N*M个Pass。可以看出，如果有大量逐像素光照，那么需要执行的Pass数目也会很大。因此，渲染引擎通常会限制每个物体的逐像素光照的数目。

##### Unity中的前向渲染

事实上，一个Pass不仅仅可以用来计算逐像素光照，它也可以用来计算逐顶点等其他光照。这取决于光照计算所处流水线阶段以及计算时使用的数学模型。当渲染一个物体时，Unity会计算哪些光源照亮了它，以及这些光源照亮该物体的方式。
在Unity中，前向渲染路径有3种处理光照（即照亮物体）的方式：逐顶点处理、逐像素处理，球谐函数（Spherical Harmonics, SH）处理。

#### 顶点照明渲染路径


#### 延迟渲染路径

这是一种可以按照你的需求在场景中使用任意数目的光源的方法，而且这个方法还能同时保证性能仍然保持在一个合理的范围。它也不限制阴影的数量，如果场景中的对象实在光照范围之内的话，也不会增加额外的渲染批次（如果对象投影阴影的话则是例外）。

在游戏中的实时光照，一般有三种常用的方法

1. 一遍渲染多个光源：所有光源都在一个着色器中进行。但一个着色器指令数量有限，所以这个技术只适用于光源数量较少的情况。
2. 多遍渲染多光源
3. 延迟渲染

### 菲涅尔反射

在实时渲染中，经常会使用**菲涅耳反射（Fresnelreflection）**来根据视角方向控制反射程度。菲涅耳反射描述了一种光学现象，即当光线照射到物体表面上时，一部分发生反射，一部分进入物体内部，发生折射或散射。被反射的光和入射光之间存在一定的比率关系，这个比率关系可以通过菲涅耳等式进行计算。一个经常使用的例子是，当你站在湖边，直接低头看脚边的水面时，你会发现水几乎是透明的，你可以直接看到水底的小鱼和石子；但是，当你抬头看远处的水面时，会发现几乎看不到水下的情景，而只能看到水面反射的环境。这就是所谓的菲涅耳效果。事实上，不仅仅是水、玻璃这样的反光物体具有菲涅耳效果，几乎任何物体都或多或少包含了菲涅耳效果，这是基于物理的渲染中非常重要的一项高光反射计算因子。
那么，如何计算菲涅耳反射呢？这就需要使用菲涅耳等式。真实世界的菲涅耳等式是非常复杂的，但在实时渲染中，通常会使用一些近似公式来计算。其中一个著名的近似公式就是Schlick 菲涅耳近似等式：
<font size="4" color="red">
$F_{Schlick}(v,n) = F_{0}+(1-F_{0})(1-v \cdot n)^5$
</font>

其中，$F_{0}$是一个反射系数，用于控制菲涅耳反射的强度， v 是视角方向， n 是表面法线。另一个应用比较广泛的等式是Empricial菲涅耳近似等式：
<font size="4" color="red">
$F_{Empricial}(v,n) = max(0,min(1,bias+scale x (1-v \cdot n)^power))$
</font>
其中，bias、scale和power是控制项。
使用上面的菲涅尔近似等式，可以在边界处模拟反射光强和折射光强/漫反射光强之间的变化。在许多车漆、水面等材质的渲染中，会经常用到菲涅尔反射来模拟更加真实的反射。

### 广告牌

广告牌技术的本质就是构建旋转矩阵，而我们知道一个变换矩阵需要3个基向量。广告牌技术使用的基向量通常就是**表面法线（normal）、指向上的方向（up）以及指向右的方向（right）**。除此之外，我们还需要指定一个**锚点（anchor location）**，这个锚点在旋转过程中是固定不变的，以此来确定多边形在空间中的位置。
广告牌技术的难点在于，如何根据需求构建3个相互正交的基向量。计算过程通常是，
CPU写法：
```
public Vector3 Normal;
Quaternion direction;
void Start () {
    direction = Quaternion.FromToRotation(new Vector3(0, 0, 1), Normal);
}
void Update () {
    transform.rotation = Camera.main.transform.rotation * direction;
}
```

## 屏幕后处理效果

### 建立一个基本的屏幕后处理脚本系统

    MonoBehaviour.OnRenderImage(RenderTexture src, RenderTexture dest);

当在脚本中声明该函数后，Unity会把当前渲染得到的图像存储在第一个参数对应的源渲染纹理中，把第二参数对应的渲染纹理显示到屏幕上。

    public static void Blit(Texture src, RenderTexture dest);
    public static void Blit(Texture src, RenderTextrue dest, Material mat, int pass = -1);
    public static void Blit(Texture src, Material mat, int pass = -1);

参数pass的默认值胃-1，表示将会依次调用Shader内的所有Pass。否则，只会调用给定索引的Pass。

## 使用深度和法线纹理

### 消融

噪声纹理+透明度测试，我们使用对噪声纹理采样的结果和某个控制消融程度的阀值比较，如果小于阀值，就使用clip函数把它对应的像素裁减掉。

```Shader
Shader "Test"
{
    Properties
    {
        _BurnAmount("Burn Amount", Range(0.0, 1.0)) = 0.0 // 控制消融程度，0为正常，1完全消融
        _LineWith("Burn Line Width", Range(0.0, 0.2)) = 0.1 // 模拟烧焦效果的线宽，值越大，火焰边缘的蔓延范围越宽
        _MainText ("Base (RGB)", 2D) = "white" {} // 漫反射纹理
        _BumpMap ("Normal Map", 2D) = "bump" {} // 法线纹理
        _BurnFirstColor("Burn First Color", Color) = (1,0,0,1) // 火焰边缘颜色
        _BurnSecondColor("Burn Second Color", Color) = (1,0,0,1) // 火焰边缘颜色
        _BurnMap("Burn Map", 2D) = "white" {} // 消融纹理
    }   
    SubShader {
        Pass {
            CGPROGRAM
            #pragma vertex vert
            #pragma vertex frag

            ENDCG
        }   
    }   
}
```

### 水波效果

在模拟实时水面的过程中，往往也会使用噪声纹理。此时，噪声纹理通常会用作一个高度图，以不断修改水面的法线方向。为了模拟水不断流动的效果，我们会使用和时间相关的变量来对噪声纹理进行采样，当得到法线信息后，在进行正常的反射+折射计算，得到最后的水面波动效果。
```
SubShader {
    Tags {"Queue"="Transparent" "RenderType"="Opaque"}
    GrabPass {"_RefractionTex"}
}
```

首先把SubShader的标签中渲染队列设置成Transparent，并把后面的RenderType设置为Opaque。把Queue设置成Transparent可以确保该物体渲染时，其他所有不透明的物体都已经被渲染到屏幕上了，否则就可能无法正确得到“透过水面看到的图像”。而设置RenderType则是为了在使用着色器替换（Shader Replacement）时，该物体可以在需要时被正确渲染。
