---
title: Unity 2D Outline
date: 2017-10-05 23:25:51
tags:
    - Unity
---

# 像素着色器描边：
首先最直观的想法，就是使用fragment shader找出2D角色贴图透明像素和非透明像素的边界，然后通过边界周边Alpha值Blur的方式，给边界上描边色。

```c
Shader "Hidden/NewImageEffectShader"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Offset ("Offset", Range(0,1)) = 0.1 // 偏移
        _Color ("Color", Color) = (1,0,0,1) // 边缘色
        _AlphaThreshold("Alpha Threshold", Range(0,1)) = 0.5
    }
    SubShader
    {
        Tags {"Queue"="Transparent"}
        Blend SrcAlpha OneMinusSrcAlpha

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            
            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv[5] : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;
            fixed _Offset; // 偏移
            fixed4 _Color; // 边缘色
            fixed _AlphaThreshold; // Alpha 阀值

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv[0] = v.uv;
                o.uv[1] = v.uv + float2(0, _Offset);// 上
                o.uv[2] = v.uv + float2(0, -_Offset); // 下
                o.uv[3] = v.uv + float2(-_Offset, 0); // 左
                o.uv[4] = v.uv + float2(_Offset, 0); // 右
                return o;
            }
            
            fixed4 frag (v2f i) : SV_Target
            {
                fixed4 col = tex2D(_MainTex, i.uv[0]);
                fixed alpha = col.a;
                fixed p1 = tex2D(_MainTex, i.uv[1]).a;
                fixed p2 = tex2D(_MainTex, i.uv[2]).a;
                fixed p3 = tex2D(_MainTex, i.uv[3]).a;
                fixed p4 = tex2D(_MainTex, i.uv[4]).a;
                alpha = (alpha + p1 + p2 + p3 + p4) / 5;
                fixed ret = step(alpha, _AlphaThreshold);
                col.rgb = ret * _Color.rgb + col.rgb * (1-ret);
                return col;
            }
            ENDCG
        }
    }
}
```
```
Shader "Hidden/NewImageEffectShader"
{
    Properties
    {
        _Edge ("Edge", Range(0,0.5)) = 0.1 // 边缘
        _EdgeColor ("Edge Color", Color) = (1,0,0,1) // 边缘色
        _FlowColor ("Flow Color", Color) = (0,1,0,1) // 流动色
        _Speed ("Speed", Range(0, 2)) = 1 // 旋转速度
        _MainTex("MainTex", 2D) = "white" {} // 主纹理
    }
    SubShader
    {
        Tags {"Queue" = "Transparent"}
        Blend SrcAlpha OneMinusSrcAlpha
        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            
            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0; // 纹理uv坐标
            };

            struct v2f
            {
                float4 vertex : SV_POSITION;
                float2 uv : TEXCOORD0;
            };
            sampler2D _MainTex;
            float4 _MainTex_ST;
            fixed _Edge; // 边缘阀值
            fixed4 _EdgeColor; // 边缘色
            fixed4 _FlowColor; // 流动色
            fixed _Speed; // 旋转速度

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;
                return o;
            }
            
            fixed4 frag (v2f i) : SV_Target
            {
                fixed x = i.uv.x;
                fixed y = i.uv.y;
                if (x < _Edge || abs(1 - x) < _Edge || y < _Edge || abs(1 - y) < _Edge) // 求边缘
                {
                    x -= 0.5;
                    y -= 0.5;
                    fixed w = _Speed * _Time.y;
                    fixed temp = saturate(x* cos(w) - y* sin(w));
                    return (temp )* _EdgeColor + (1-temp)*_FlowColor;
                }
                return tex2D(_MainTex, i.uv);
            }
            ENDCG
        }
    }
}

```
# 后处理+像素着色器描边：
　　于是，很自然的想到，使用Postprocessing 的方式来处理屏幕最终渲染出来的仅包含有需要描边的角色的图片，然后进行上述的fragment描边处理。
　　首先，在场景中添加一个专用的描边相机，Depth设为比默认相机高一级，视口大小、位置全部和默认相机一致。另外给场景物体添加一个"Outline1"layer，设置到专用相机的cullingmask，需要描边的角色的layer都设置为"Outline1"。

　　这个时候专用相机只渲染描边物体，然后给相机添加Monobehavior脚本，在OnRenderImage里对相机渲染的图像进行像素着色器的描边处理，得到如下效果：
　　描边效果的确是预期的效果，但是还是有两个问题：
　　一个是：由于我们游戏UI是使用Screen space - camera的方式，它和游戏中对象都是在默认相机渲染的，专用相机的depth比默认相机高一级，所以所有渲染内容都会在UI层次之上，这显然也不是我们想要的。
　　另一个是：角色的不同行动状态需要用不同的描边颜色来区分，上述方式只支持一种描边颜色。

# RenderTarget + 像素着色器描边：
　　现在我们着重来解决上述两个问题：
第一个遮挡UI的问题：
　　描边效果需要在默认相机中进行z轴排序，让其渲染在UI之后，所以，我们需要让描边效果作为一个默认相机渲染的常规游戏对象放到游戏中。于是，我想到的是给它创建一个和屏幕尺寸同大小的Mesh，再将描边渲染的图片放到Mesh上，所以，做法是给专用相机分配一个屏幕尺寸同大小的RenderTarget，然后将RenderTarget渲染到创建的Mesh上。如下图所示，该Mesh在骨骼动画和相机之间创建。

第二个颜色区分的问题：
　　查询Unity Camera的API有两个回调接口可以用：
　　OnPreRender 在相机渲染场景之前被调用。
　　OnPostRender 在相机完成场景渲染之后被调用。
　　我们给专用相机添加这两个回调，
在OnPreRender里：
　　给所有需要描边的物体更换Material，该Material负责绘制该物体的状态纯颜色到RenderTarget上，并保存原有Material。
在OnPostRender里：
　　将原有Material又置换回来供默认相机正常渲染。得到的RenderTarget如下，
　　这个时候，再用像素着色器对应用了RenderTarget的Mesh进行描边，得到如下效果
