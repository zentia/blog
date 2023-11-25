---
title: 标准光照模型
mathjax: true
date: 2019-04-10 13:29:47
tags:
    - Engine
    - Unity
categories: 
- Engine
- Unity
---
基本光照模型中漫反射部分的计算公式：
$c_{diffuse}=(c_{light} \cdot m_{diffuse})max(0, \vec{n} \cdot I)$
从公式可以看出，要计算漫反射需要知道4个参数：入射光线的颜色和强度$c_{light}$，表面发现$\vec{n}$以及光源方向$I$。
为了防止点击结果为赋值，我们需要使用max操作，而CG提供了这样的函数。在本例中，使用CG的另一个函数可以达到同样的目的，即saturate函数。
**函数：**saturate(x)
**参数：**x：用于操作的标量或矢量，可以是float、float2、float3等类型。
**描述：**把x截取在[0,1]范围内，如果x是一个矢量，那么会对它的每一个分量进行这样的操作。

# 逐顶点光照
```c
Shader "Unity/Diffuse Vertex"{
    Properties {
        _Diffuse ("Diffuse", Color)=(1,1,1,1) // 材质的漫反射系数
        _Specular("Specular", Color)=(1,1,1,1) // 高光系数
    }   
    SubShader{
        Pass{
            Tasg{"LightMode"="ForwardBase"} // 前向渲染，所有光源都在一个着色器中进行
        }
        CGPROGRAM
        #pragma vertex vert
        #pragma fragment frag
        #include "Lighting.cginc"
        fixed4 _Diffuse;
        struct a2v{
            float4 vertex : POSITION;
            float3 normal : NORMAL;
        };
        struct v2f {
            float4 pos : SV_POSITION;
            fixed3 color : COLOR;
        };
        v2f vert(a2v v){
            v2f o;
            o.pos = UnityObjectToClipPos(v.vertex);
            float3 ambient = UNITY_LIGHTMODEL_AMBIENT.xyz; // 环境光
            float3 worldNormal = normalize(mul(v.normal, (float3x3)_World2Object)); // 世界法线
            float3 lightNormal = normalize(_WorldSpaceLightPos0.xyz); // 光的方向
            float3 diffuse = _LightColor0.rgb * _Diffuse.rgb * saturate(dot(worldNormal, lightNormal));
            o.color = diffuse + ambient;
            return o;
        }
        fixed4 frag(v2f o) : COLOR
        {
            return fixed4(o.color,1);
        }
    }
    Fallback "Diffuse"
}
```
顶点着色器最基本的任务就是把顶点位置从模型空间转换到裁剪空间中，因此我们需要使用Unity内置的模型\*世界\*投影矩阵UNITY_MATRIX_MVP来完成这样的坐标转换。通过Unity的内置变量UNITY_LIGHTMODEL_AMBIENT得到了环境光部分。
然后，就是真正计算漫反射光照的部分。为了计算慢反射光照我们需要知道4个参数。在前面的步骤中，我们已经知道了材质的漫反射颜色_Diffuse以及顶点发现v.normal。我们还需知道光源的颜色和强度信息以及光源方向。Unity提供给我们一个内置变量_LightColor0来访问该Pass处理的光源的颜色和强度信息（注意，想要得到正确的值需要定义合适的LightMode标签），而光源方向可以由_WorldSpaceLightPos0来得到。需要注意的是，这里对光源方向的计算并不具有通用性。这里，我们假设场景中只有一个光源且该光源的类型是平行光。但如果场景中有多个光源并且类型可能是点光源等其他类型，直接使用_WorldSpaceLightPos0就不能得到正确的结果。

# 半兰伯特光照模型

$c_{diffuse}=(c_{light}\cdot m_{diffuse})(\alpha (\vec {n}\cdot I)+\beta)$
可以看出，与原兰伯特模型相比，半兰伯特光照模型没有使用max操作来防止$\vec {n}$

# Blinn-Phong光照模型

Blinn模型计算高光反射的公式如下：
<font size="4">
$c_{specular}=(c_{light} \cdot m_{specular})max(0,\vec{n} \cdot \vec{h})^{m_{glass}}$   1 
</font>
