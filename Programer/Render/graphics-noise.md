---
title: 图形学-噪声
mathjax: true
date: 2019-04-01 08:27:42
tags:
	- Render
categories: Render
---

由程序产生噪声的方法大致可以分为两类

|类别|名称|
|--|--|
|基于晶格的方法（Lattice based）|又可细分为两种：第一种是梯度噪声（Gradient noise），包括Perlin噪声，Simplex噪声，Wavelet噪声等；第二种是Value噪声（Value noise）。|
|基于点的方法（Point based）|Worley噪声|

{% asset_img 1.png 随机噪声 %}{% asset_img 2.png 分形噪声 %}{% asset_img 3.png Simplex噪声 %}

# 白噪声
噪声的基础是随机数，如果给屏幕上的每个像素点赋予一个0和1之间的随机数来表示像素点的亮度，就会得到一副杂乱无章的图片。这就是白噪声，
# Perlin噪声

实现步骤：
1. 定义一个晶格结构，每个晶格的顶点有一个“伪随机”的梯度向量。对于二维的Perlin噪声来说，晶格结构就是一个平面网络，三维的就是一个立方体网格。
2. 输入一个点（二维的话就是二维坐标，三维就是三维坐标），我们找到和它相邻的那些晶格顶点（二维下有4个，三维下有8个，n维下有$2^n$个），计算该点到各个晶格顶点的距离向量，在分别与顶点上的梯度向量做点乘，得到$2^n$个点乘结果。
3. 使用缓和曲线（ease curves）来计算它们的权重和。在原始的Perlin噪声实现中，缓和曲线是$s(t) = 3t^2 - 2t^2$，在2002年论文中，Perlin改进为$s(t) = 6t^5 - 15t^4 + 10t^3$。

```c
Shader "Shadertoy/Template" 
{
    Properties
    {
        iMouse ("Mouse Pos", Vector) = (100, 100, 0, 0)
        iChannel0("iChannel0", 2D) = "white" {}  
        iChannelResolution0 ("iChannelResolution0", Vector) = (100, 100, 0, 0)
    }
 
    CGINCLUDE    
    #include "UnityCG.cginc"   
    #pragma target 3.0      
 
    #define vec2 float2
    #define vec3 float3
    #define vec4 float4
    #define mat2 float2x2
    #define mat3 float3x3
    #define mat4 float4x4
    #define iTime _Time.y
    #define mod fmod
    #define mix lerp
    #define fract frac
    #define texture2D tex2D
    #define iResolution _ScreenParams
    #define gl_FragCoord ((_iParam.scrPos.xy/_iParam.scrPos.w) * _ScreenParams.xy)
 
    #define PI2 6.28318530718
    #define pi 3.14159265358979
    #define halfpi (pi * 0.5)
    #define oneoverpi (1.0 / pi)
 
    fixed4 iMouse;
    sampler2D iChannel0;
    fixed4 iChannelResolution0;
 
    struct v2f 
    {
        float4 pos : SV_POSITION;    
        float4 scrPos : TEXCOORD0;   
    };              
 
    v2f vert(appdata_base v) 
    { 
        v2f o;
        o.pos = UnityObjectToClipPos (v.vertex);
        o.scrPos = ComputeScreenPos(o.pos);
        return o;
    }  
 
    vec4 main(vec2 fragCoord);
 
    fixed4 frag(v2f _iParam) : COLOR0 
    {
        vec2 fragCoord = gl_FragCoord;
        return main(gl_FragCoord);
    }  
 
    vec3 hash3(vec2 p)
    {
        vec3 q = vec3(dot(p, vec2(127.1,311.7)),dot(p,vec2(269.5,183.3)),dot(p,vec2(419.2,317.9)));
        return fract(sin(q)*43758.5453);
    }

    float iqnoise(in vec2 x, float u, float v)
    {
        vec2 p = floor(x);
        vec2 f = fract(x);

        float k = 1.0+63.0*pow(1.0-v,4.0);

        float va = 0.0;
        float wt = 0.0;
        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                vec2 g = vec2(float(i),float(j));
                vec3 o = hash3(p + g)*vec3(u,u,1.0);
                vec2 r = g - f + o.xy;
                float d = dot(r,r);
                float ww = pow(1.0-smoothstep(0.0,1.414,sqrt(d)),k);
                va += o.z*ww;
                wt += ww;
            }
        }
        return va/wt;
    }

    vec4 main(vec2 fragCoord) 
    {
        vec2 uv = fragCoord.xy / iResolution.xx;
        vec2 p = 0.5 - 0.5*sin(iTime*vec2(1.01,1.71));
        if (iMouse.w > 0.001) p = vec2(0.0, 1.0) + vec2(1.0, -1.0)*iMouse.xy/iResolution.xy;
        p = p*p*(3.0-2.0*p);
        p = p*p*(3.0-2.0*p);
        p = p*p*(3.0-2.0*p);
        float f = iqnoise(24.0*uv,p.x,p.y);
        return vec4(f, f, f, 1);
    }
    ENDCG    
    SubShader 
    {    
        Pass 
        {    
            CGPROGRAM    
            #pragma vertex vert    
            #pragma fragment frag    
            #pragma fragmentoption ARB_precision_hint_fastest     
            ENDCG    
        }    
    }     
    FallBack Off    
}
```
