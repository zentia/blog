---
title: MatCap
mathjax: true
date: 2019-09-03 13:03:57
tags:
- Engine
- Unity
categories: 
- Engine
- Unity
---
{% asset_img MatCap.png %}
# Description
MatCap (Material Capture) shader,for displaying objects with reflective materials with uniform surface coloring,like Zbrush or Mudbox can.It uses an image of a sphere as a view-space environment map.It's very cheap,and looks great when the camera doesn't rotate.
用于均一表面反射材质的着色器，在Zbrush/Mudbox等软件中被广泛运用。它借助一幅含有球面的图片作为视线空间的环境映射，计算成本非常低廉，当相机不转动时视觉效果极其出色。
{% asset_img 2.jpg %}
# Usage
Use a regular normal map for your object, and a render or a photo of a sphere for the MatCap.
Matcap的原理并不复杂，就是使用一张中间为球面的图片作为不同法线方向的光照颜色，然后将这些颜色根据模型的法线信息渲染到相应的位置。

例如下图图二（黑白）中，球形就是Matcap贴图，箭头代表了右下朝向的法线，车与人模型发现朝向此相同方向时，就会渲染为球形贴图上相应点的颜色----白色。
{% asset_img 3.jpg %}

# ShaderLab-MatCap.shader
```c
Shader "MatCap Bumped"{
    Properties {
        _Color ("Main Color", Color)=(0.5,0.5,0.5,1)
        _BumpMap("BumpMap (RGB)",2D)="bump"{}
        _MatCap("MatCap (RGB)",2D)="white"{}
    }
    SUbShader{
        Tags {"RenderType"="Queue"}
        Fog {Color [_AddFog]}
        Pass {
            Name "BASE"
            Tags {"LightMode"="Always"}
            
            CGPROGRAM
            #pragma exclude_renderers xbox360
            #pragma vertex vert
            #pragma fragment frag
            #pragma fragmentoption ARB_fog_exp2
            #pragma fragmentoption ARB_precision_hint_fastes
            #include "UnityCG.cginc"
            
            struct v2f{
                float4 pos : SV_POSITION;
                float2 vu : TEXCOORD0;
                float3 TtoV0 : TEXCOODR1;
                float3 TtoV1 : TEXCOORD2;
            };
            
            uniform float4 _BumpMap_ST;
            v2f vert(appdata_tan v){
                v2f o;
                o.pos = mul(UNITY_MATRIX_MVP,v.vertex);
                o.uv = TRANSFORM_TEX(v.texcoord,_BumpMap);
                TANGENT_SPACE_ROTATION
                o.TtoV0 = mul(rotation,UNITY_MATRIX_IT_MV[0].xyz);
                o.TtoV1 = mul(rotation,UNITY_MATRIX_IT_MV[1].xyz);
                return o;
            }
            uniform float4 _Color;
            uniform sampler2D _BumpMap;
            uniform sampler2D _MatCap;
            float4 frag (v2f i) : Color
            {
                float3 normal = UnpackNormal(tex2D(_BumpMap,i.uv);
                half2 vn;
                vn.x = dot(i.TtoV0,normal);
                vn.y = dot(i.TtoV1,normal);
                float4 matcapLookup = tex2D(_MatCap,vn*0.5+0.5);
                matcapLookup.a = 1;
                return _Color*mapcapLookup*2;
            }
            ENDCG
        }
    }
}
```
