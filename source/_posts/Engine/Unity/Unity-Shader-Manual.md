---
title: Unity Shader Manual
date: 2018-10-15 13:20:27
categories: 
- Engine
- Unity
---
# Properties
```Shader
Shader "ShaderLab Tutorials/TestShader"
{
    Properties {
        _Range ("My Range", Range(0.02, 0.15)) = 0.07 // sliders
        _Color ("My Color", Color) = (.34, .85, .92, 1) // color
        _2D ("My Texture 2D", 2D) = "" {} // texture
        _Rect ("My Rectangle", Rect) = "name" {}
        _Cube ("My Cubemap", Cube) = "name" {}
        _Float ("My Float", Float) = 1
        _Vector ("My Vector", Vector) = (1,2,3,4)
    }
}
```
- Shader在Unity编辑器暴露给美术的参数，通过Properties来实现。
- 所有可能的参数如上所示。主要也就是Float、Vector和Texture这3类。
- 除了通过编辑器编辑Properties，脚本也可以通过Material的接口（比如SetFloat、SetTexture编辑）
- 之后的Shader程序通过\[name\]（固定管线）或者直接name（可编程Shader）访问这些属性。
- 在每一个Property前面也能类似C#那样添加Attribute，以达到额外UI功能。详见
# Semantics
# SubShader
## Tags{"TagName1"="Value1" "TagName2"="Value2"}
Tag的语法结构，通过Tags{}来表示需要添加的标识，大括号可以添加多组Tag（所以才叫Tags嘛），名称(TagName)和值（Value）是成对出现的，并且全部用字符串表示。

## Queue

渲染队列直接影响性能中的重复绘制，合理的队列可极大的提升渲染效率。
渲染队列数目小于2500的对象都被认为是不透明的物体（从前往后渲染），高于2500的被任务是半透明物体（后后往前渲染）。
"Queue"="Geometry+1"可通过值后加数字的方式来改变队列。

|Queue标签|解释|
|--|--|
|"Queue"="Background"|值为1000，此队列的对象最先进行渲染|
|"Queue"="Geometry" |默认值，值为2000，通常用于不透明对象，比如场景中的物体与角色等|
|"Queue"="AlphaTest"|值为2450，要么完全透明要么完全不透明，多用于利用贴图来实现边缘透明的效果，也就是美术常说的透贴|
|"Queue"="Transparent"|值为3000，常用于半透明对象，渲染时从后往前进行渲染，建议需要混合的对象放入此队列|
|"Queue"="Overlay"|值为4000，此渲染队列用于叠加选过。最后渲染的东西应该放在这里（例如镜头光晕等）|

## RenderType
用来区分这个Shader要渲染的对象是属于什么类型的，你可以想象成事我们把不同的物体按我们需要的类型来进行分类一样。
当然你也可以根据需要改成自定义的名称，这样并不会影响到Shader的选过。
此Tag多用于摄像机的替换材质功能（Camera.SetReplacementShader）。

|  标签  | 释义           |
|----------|-------------|
| "RenderType"="Opaque"|大多数不透明着色器|
| "RenderType"="Transparent"    | 大多数半透明着色器，比如粒子、特效、字体等。      |
| "RenderType"="TransparentCutout" | 透贴着色器，多用于植被等。      | 
| "RenderType"="Background" | 多用于天空盒着色器。 | 
| "RenderType"="Overlay" | GUI、光晕着色器等。 |
| "RenderType"="TreeOpaque" | Terrain地形中的树干。 |
| "RenderType"="TreeTransparentCutout" |Treeain地形中的树叶。 |
| "RenderType"="TreeBillboard" | Terrain地形中的永对面树。 |
| "RenderType"="Grass"               | Terrain地形中的草。 |
| "RenderType"="GrassBillboard" | Terrain地形中的用对面草。 |

指定RenderType的名称，主要是为了配合使用替代渲染的方法。

    Camera.SetReplacementShader("shader", "RenderType")

## DisableBatching
在利用Shader在模型的定点本地坐标下做一些位移动画，而当此模型有批处理时会出现效果不正确的情况，这是因为批处理将所有模型转换为世界坐标空间，因为"本地坐标空间"将丢失。

|批处理标签|解释|
|--|--|
|"DisableBatching"="True"|禁止批处理|
|"DisableBatching"="False"|不禁用批处理|

# Math
## dot(a,b)
点乘，a和b必须为三维向量或者四维向量，其计算结果是两个向量夹角的余弦值，相当于a.x\*b.x+a.y\*b.y+a.z\*b.z
a和b的位置无所谓前后，结果都是一样的
# Miscellaneous

	float3 objCenterPos = mul(unity_ObjectToWorld, float4(0,0,0,1)).xyz;

在Shader中获取当前模型的中心点，其实就是将(0,0,0)点从本地坐标转换到世界空间坐标下即可。

# 内置函数

|Syntax|Description|
|--|--|
|abs(x)|Absolute value (per component)(每个组件).|
|acos(x)|Returns the arccosine(反余弦) of each component of x.|
|ceil(x)|Returns the smallest integer(最小整数) which is greater(大于) than or equal to x.|
|clip(x)|Discards(抛弃) the current pixel,if any component of x is less than zero.|
|cross(x,y)|x向量和y向量的向量积（叉积）|
|frac(x)|返回x的小数部分|
|lerp(x,y,s)|Returns x + s(y-x)|
|noise(x)|Generates a random value using the Perlin-noise algorithm.|
|pow(x,y)|Returns $x^y$|
|saturate(x)|x: 为用于操作的标量或矢量，可以是float、float2、float3等类型。把x截取在[0,1]范围内，如果x是一个矢量，那么会对它的每一个分量进行这样的操作。|
|smoothstep(min,max,x)|如果x比min小，返回0；如果x比max大，返回1；如果x处于范围[min,max]中，则返回0和1之间的值（返回值在min和max间的比例）。|
|step(a,b)      |Returns (b>=a)?1:0 (一般用这个取代if else)|
|tex2D(s, t)    |2D texture lookup.|
|reflect(I,N)   |根据入射光方向向量I，和顶点法向量N，计算反射光方向向量。其中I和N必须被归一化，需要非常注意的是，这个I是指向顶点的；函数只对三元向量有效|
|Tex2DProj(sampler2D tex, float3 sq)|二维投影纹理查询|
|Tex2DProj(sampler2D tex, float4 szq)|二维投影纹理查询，并进行深度值比较|

# \#pragma fragementoption ARB_precision_hint_fastest
ARB_precision_hint_fastest 是用最快的方式，以最低的精度运行，提升片段着色器的运行速度，减少时间。（通常是指FP16,16bit，半精度）牺牲表现换取运行速度。

# UnpackNormal
`half3 normal = UnpackNormal(tex2D(_NormalMap, IN.uv_NormalMap));`
UnpackNormal是定义在UnityCG.cginc文件中的方法，UnpackNormal接受一个fixed4的输入，并将其转换为所对应的法线值(fixed3)。

# TANGENT_SPACE_ROTATION
```c
float3 binormal = cross(v.normal,v.tangent.xyz)*v.tangent.w;
flaot3x3 rotation = float3x3(v.tangent.xyz,binormal,v.normal);
```
也就是构造出tangent space的坐标系，定义转换world space的向量到tangent space的rotation矩阵。

# UNITY_MATRIX_IT_MV
专门用于将发现从模型空间变换到观察空间，为UNITY_MATRIX_MV的逆转置矩阵，