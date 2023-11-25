---
title: 遮挡显示
mathjax: true
date: 2019-04-13 12:49:05
categories:
- Engine
- Unity
---
3D游戏中主角会经常被墙壁之内的东西挡住，此时为了达到突出主角的效果，会做一些特殊处理让主角显示出来。
光栅化阶段的后期，会有一个深度测试和颜色混合的过程，此效果就是针对这个过程进行的一种特殊处理。
1. 深度就是该像素点在3D世界中距离摄像机的距离，离摄像机越近，深度值越小。
2. 深度测试：在屏幕上每一个点，都存放在一个缓存列表。如果启用深度缓存Zwrite On，那么在绘制每个像素钱，底层会将当前点的深度值和已经存储在这个位置的点的深度值进行比较。如果新的点的深度值小于原来点的深度值，则新的点会代替原来的点。反之新的点会被遮挡，其颜色值和深度值会被丢弃。

实现步骤：
1. 定义了2个pass，一个输出纯色，一个输出模型色
2. 当对象没有被遮挡是，先执行的纯色pass输出了纯色，然后模型的pass输出了模型色，最终纯色被替换显示正常模型。
3. 当对象被墙壁挡住是，如果我们什么也不做，纯色pass输出的颜色和模型pass输出的颜色都会被墙壁代替，因为它们的深度值逗逼墙壁深度值大
4. 当对象被墙壁遮挡住时，我们想要的效果是显示纯色，那么我们可以关闭纯色的ZWrite，就是不将它的深度值写入深度缓存，此时它的深度测试参数ZTest默认是LEqual，而它的深度值是大于墙壁的，所以也不会显示。
5. 被挡住时，模型色已经被剔除了，最终的颜色混合为
Blend SrcAlpha OneMinusSrcAlpha
最终色 = 纯色rgb*纯色a + 墙壁rgb*(1-纯色a)

{% asset_img 1.jpg %}

```c
Shader "Custome/XRay"
{
    Properties
    {
        _Color("Color", Color)=(1,1,1,1)
        _MainTex ("Texture", 2D) = "white" {}
        _AfterColor("After Color", Color)=(0.435,0.851,1,0.419)
    }
    SubShader
    {
        Tags{"Queue"="Geometry+1" "RenderType"="Opaque"}
        LOD  300
        Blend SrcAlpha OneMinusSrcAlpha 

        Pass
        {
            ZTest GEqual // 深度测试，大于等于当前最小中的值就会显示
            ZWrite  Off 
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
            };

            struct v2f
            {
                float4 vertex : SV_POSITION;
            };

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                return o;
            }
            fixed4 _AfterColor;
            fixed4 frag (v2f i) : SV_Target
            {
                return _AfterColor;
            }
            ENDCG
        }
        Pass
        {
            ZTest LEqual
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
                float4 vertex: SV_POSITION;
                float2 uv : TEXCOORD0;
            };
            v2f vert(appdata i)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(i.vertex);
                o.uv = i.uv;
                return o;
            }
            sampler2D _MainTex;
            fixed4 _Color;
            fixed4 frag(v2f i):SV_Target
            {
                fixed4 c = tex2D(_MainTex, i.uv) + _Color;
                return c;
            }
            ENDCG
        }
    }
}
```
