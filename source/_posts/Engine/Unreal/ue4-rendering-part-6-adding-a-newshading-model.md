---
title: UE4渲染第6节：添加一个新的渲染模型
mathjax: true
date: 2021-06-30 20:55:59
tags:
categories: "UE4"
---

带翻译的文章：https://80.lv/articles/river-editor-water-simulation-in-real-time/ 水
https://wickedengine.net/2018/05/21/scalabe-gpu-fluid-simulation/ 流体
https://necessarydisorder.wordpress.com/2017/11/15/drawing-from-noise-and-then-making-animated-loopy-gifs-from-there/ 各种噪声，不玩了，深陷其中了

原文地址：https://medium.com/@lordned/ue4-rendering-part-6-adding-a-new-shading-model-e2972b40d72d

注意，译文和原文出入已经很大了，译文主要是按照4.26来操作的，如果有疑问，最好看着原文去操作，不过我想原文的版本大家应该都没有把。

（如果你没有读本系列的第五节，[请点击这里](https://medium.com/@lordned/unreal-engine-4-rendering-part-5-shader-permutations-2b975e503dd4)）

## 添加一个新的渲染模型
UE支持几种通用的开箱即用的渲染模型来针对大多数游戏。UE不仅支持微面元反射的默认光照，同时也支持高质量的头发和眼睛效果。但是这些可能不是最合适的对于你的游戏或者你希望修改或者新增对于你的游戏，尤其是对于高度风格化的游戏。

{% asset_img 1.png 使用阶梯照明处理的着色模型 %}

集成一个新的光照模型只需要少量的代码但是编译引擎可能花费点时间。当你决定要增加之前，最好看一下上一节，这可能会节省10分钟左右的迭代时间。

大部分的代码是基于FelixK的blog，但是已经打不开了。

我们需要修改3个地方来支持新的渲染模型，材质编辑器，材质和一些shader代码。

### 修改材质编辑器
首先打开EngineTypes.h文件，然后找到EMaterialShadingModel。这个枚举决定了在材质编辑器显示那些渲染模型。我们添加一个新的枚举`MSM_StylizedShadow`在`MSM_MAX`之前。
```C++
// Note: Check UMaterialInstance::Serialize if changed!
UENUM()
enum EMaterialShadingModel
{
    // … Previous entries omitted for brevity
    MSM_Eye UMETA(DisplayName=”Eye”),
    MSM_StylizedShadow UMETA(DisplayName=”Stylized Shadow”),
    MSM_MAX,
};
```
枚举可能是按照名字序列化的（如果存在？），但对于引擎来说任何地方的序列化的都加在最后。

UE在`UMaterialShadingModel`枚举警告开发者检查`UMaterialInstance::Serialize`函数如果我们改变了枚举。看起来我们不需要更改任何东西，当我们添加了新的渲染模型，所以我们忽略这个警告。（如果你却是对这个函数感到兴趣，它确实是改变了一些资产的加载顺序）

{% asset_img 2.png 下拉可以看到我们新加的渲染模型 %}

当我们编译完成之后（译者注：裂开了，不知道为什么要编译两千多个文件）我们可以在材质编辑器的下拉框中看到我们新加的渲染模型选项，但是它啥事也不能做。FelixK使用`Custom Data 0`来允许艺术家设置光照衰减的范围。我们需要修改代码来使`Custom Data 0`起效在我们自定的渲染模型中。

打开Material.cpp（不是Lightmass项目中的那个）然后查找`UMaterial::IsPropertyActive_Internal`(4.26是这个函数，有点慌，不知道这个文档能不能跟着做下去)函数。材质上的每个可能的引脚（PIN不知道咋翻译，知道的老铁留个言）每次都会去调用它。如果你尝试正在修改材质域（domain暂时不知道指的是啥，虽然源码中也有这个概念，等我以后知道了就改掉这块）（比如贴花，后处理等），你将需要特别小心的处理该函数的第一部分，查看他们的域对于那些引脚应该启用。如果你和我一样正在修改渲染模型，这里可能有点复杂，
你需要搞一个switch-case来针对不同的引脚做不同的处理。（我好像可以理解引脚这个概念，其实就是不同的情况吧）

在我们这个case，我们想开启`MP_CustomData0`这个引脚，我们找到`MP_CustomData0`然后添加`MSM_StylizedShadow`来结束它。
当你可以改变渲染模型在风格化窗口中的时候，说明这个引脚已经被开启了，允许你连接它在你的材质蓝图。

```c++
case MP_CustomData0:
	Active = ShadingModels.HasAnyShadingModel({ MSM_ClearCoat, MSM_Hair, MSM_Cloth, MSM_Eye, MSM_StyleizedShadow });
	break;
```
以上是4.26的写法。

理解这段代码的修改UI在材质编辑器上是重要的。
然后你还要确保你的数据支持这些引脚在你的shader上。

注意：`Custom Data 0`和`Custom Data 1`是单通道的float属性，这个可能是不够支撑你扩展你的自定义渲染模型。[Javad Kouchakzadeh](https://twitter.com/stoopdapoop) 指出你可以创建一个新的引脚，
这将让你选择如何生成你的hlsl代码。不过这有点超出我们的教程了，而且也不是这个文章的主题。你如果想体验一下，可以看一下MaterialShared.cpp文件中`InitializeAttributeMap()`这个函数。

### 修改HLSL预处理器定义

一旦我们在材质编辑器上选择了我们自己的渲染模型之后，我们得需要让我们的shader知道我们做了这件事。

HLSLMaterialTranslator.cpp然后查找`FHLSLMaterialTranslator::GetMaterialEnvironment(EShaderPlatform InPlatform, FShaderCompilerEnvironment& OutEnvironment)`函数。
(用RIDER直接两个shift就直接找函数吧。)这个函数可以看到各种变量配置（各种属性在你的材质上）然后修改`OutEnvironment`变量来添加定义。

在我们特殊的案例中我们滑倒最后的部分在switch的`Property`并且添加我们的`MSM_StylizedShadow`Case，给它一个名字在允许的匹配中。文档与当前的引擎差异已经很大了，我这都是按照语义改的，不知道是否正确，主要是引擎编译起来有点痛苦。我先不加，等全部翻译完在加。试试。

现在，我们把`MSM_StylizedShadow`设置到HLSL编译器将通过`MATERIAL_SHADINGMODEL_STYLEIZED_SHADOW`作为一个预定义的。
它将允许我们使用`#if MATERIAL_SHADINGMODEL_STYLIZED_SHADOW`在HLSL代码里面。

### Review

这里需要修改C++代码，我们已经添加我们的渲染模型在编辑器中，我们也已经改变引脚使用自定义数据，我们也保证了shader能告诉我们在这个模式。不过编译engine的代码需要花费大量时间（早说啊）。
我们也不想更改.ush/.usf文件当我们打开编辑器的时候，这回导致重新编译我们的shader。（这里好坑）

### 更新GBuffer Shading Model ID

现在他是可能的构建shader的排列（a permutation of排列是啥意思呀）通过使用我们的光照模型通过使用`MATERIAL_SHADINGMODEL_STYLEIZED_SHADOW`。第一件事情，我们需要写一个新的Shading Model ID到GBuffer。
当运行光照计算的时候，允许DeferredLIghtPixelShader知道切换那个shading model在使用着。

打开 ShadingCommon.ush然后找到`#define SHADINGMODELID_UNLIT`。在这里我们定义自己的Shading Model ID，然后更新`SHADINGMODELID_NUM`

```c++
#define SHADINGMODELID_STYLEIZED_SHADOW 12
#define SHADINGMODELID_NUM 13
```

我们需要告诉shader来写shading Model ID到GBuffer，在离开这个文件之前我们应该更新Buffer Visualization > Shading Model 颜色以便告诉那个像素在你的场景是来自你的shading model的渲染的。

在下面可以找到 `float3 GetShadingModelColor(uint ShadingModelID)`。

我们添加一个入口在`switch(ShadingModelID)`里面。

```c++
switch(ShadingModelID)
{
}
```