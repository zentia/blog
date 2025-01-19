---
title: Unity Shader 后处理：Bloom全屏泛光
date: 2019-03-30 23:11:43
categories: 
- Engine
- Unity
---
全屏Bloom效果，也叫Glow效果，中文称作“全屏泛光”，这是一种可以模拟出HDR的全屏后处理效果，但是实现原理和HDR相差很远，效果比HDR差一下，但是性能高很多。

# HDR

HDR(High Dynamic Range)，高动态范围。High值得是亮度的范围更高。正常屏幕上一个像素是由RGB三原色组成的，每个通道用八位二进制表示，也就是0-255，而真实世界的亮度的最大值要远远超过屏幕上能够显示的最大亮度值，比如太阳的亮度会是屏幕亮度的几万倍，这个就是所谓的High。
HDR可以模拟高范围的亮度分布。实现这样功能的技术叫做ToneMapping，翻译为色调映射技术。这种技术会让画面对比度更加柔和，将高的亮度范围更加平滑地缩放到0-255这一低光照范围，主要运用的原理是局部适应性。人眼在比较暗的地方，也能看清东西，但是突然到了一个比较亮的地方，就会感觉模糊，需要一会儿才能适应当前的亮度水平。

# Bloom
Bloom可以模拟出HDR的效果，但是原理上和HDR是截然不同的。HDR实际上是通过映射技术，来达到整体调整全局亮度属性的，这种调整是颜色、强度等都可以进行调整，而Bloom仅仅是能够将光照范围调高达到过饱和，也就是让亮的地方更亮。不过Bloom效果实现起来简单，性能效果也小，却可以达到不错的效果。

实现原理：首先需要设置一个泛光的亮度阀值，第一遍处理时，需要对原场景图进行帅选，所有小于这个阀值的像素都被筛掉，所有大于该值的像素都被留下，这样，我们就得到了一张只包含需要泛光部分的贴图，其余部分是黑色的；泛光效果是由衍射效果产生的，我们现实世界中看到的泛光效果，最亮的地方实际上是会向暗的地方扩散的，也就是说在亮的地方，边界是不明显的，所以我们就需要对泛光也就是上一步操作的结果图片进行模糊操作，达到光移除的效果，最后，我们将处理过的图像和原图像紧急性叠加，就得到了最终的效果。
```CSharp
using UnityEngine;
[ExecuteAlways]
public class RenderImage : MonoBehaviour
{
    // 分辨率
    public int downSample = 1;
    public float grayScaleAmout = 1.0f;
    public Material curMaterial;
    // 高亮部分提取阀值
    public Color colorThreshold = Color.gray;
    // 采样率
    public int samplerScale = 1;
    // Bloom泛光颜色
    public Color bloomColor = Color.white;
    // Bloom权值
    [Range(0.0f, 1.0f)]
    public float bloomFactor = 0.5f;
    void OnRenderImage(RenderTexture src, RenderTexture dest)
    {
        // 申请两块RT，并且分辨率按照downSample降低
        RenderTexture temp1 = RenderTexture.GetTemporary(src.width >> downSample, src.height >> downSample);
        RenderTexture temp2 = RenderTexture.GetTemporary(src.width >> downSample, src.height >> downSample);

        // 直接将场景图拷贝到低分辨率的RT上达到降分辨率的效果
        Graphics.Blit(src, temp1);
        // 根据阀值提取高亮部分，使用pass0进行高亮提取
        curMaterial.SetVector("_colorThresshold", colorThreshold);
        Graphics.Blit(temp1, temp2, curMaterial, 0);

        // 高斯模糊，两次模糊，横向纵向，使用pass1进行高斯模糊
        curMaterial.SetVector("_offsets", new Vector4(0, samplerScale, 0, 0));
        Graphics.Blit(temp2, temp1, curMaterial, 1);
        curMaterial.SetVector("_offset", new Vector4(samplerScale, 0, 0, 0));
        Graphics.Blit(temp1, temp2, curMaterial, 1);

        // Bloom，将模糊后的图作为Material的Blur图参数
        curMaterial.SetTexture("_BlurTex", temp2);
        curMaterial.SetVector("_bloomColor", bloomColor);
        curMaterial.SetFloat("_bloomFactor", bloomFactor);
        // 使用pass2进行景深效果计算，清晰场景图直接从source输入到shader的_MainTex中
        Graphics.Blit(src, dest, curMaterial, 2);

        // 释放申请的RT
        RenderTexture.ReleaseTemporary(temp1);
        RenderTexture.ReleaseTemporary(temp2);
    }
}

```
```c
Shader "Hidden/BloomEffect"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
		_BlurTex("BlurTex", 2D) = "white" {}
    }
    CGINCLUDE
    #include "UnityCG.cginc"

    // 用于阀值提取亮度部分
    struct v2f_threshold
    {
        float4 pos : SV_POSITION;
        float2 uv : TEXCOORD0;
    };

    // 用于blur
    struct v2f_blur
    {
        float4 pos  : SV_POSITION;
        float2 uv   : TEXCOORD0;
        float4 uv01 : TEXCOORD1;
        float4 uv23 : TEXCOORD2;
        float4 uv45 : TEXCOORD3;
    };

    // 用于bloom
    struct v2f_bloom
    {
        float4 pos  : SV_POSITION;
        float2 uv   : TEXCOORD0;
        float2 uv1  : TEXCOORD1; 
    };

    sampler2D _MainTex;
    float4 _MainTex_TexelSize;
    sampler2D _BlurTex;
    float4 _BlurTex_TexelSize;
    float4 _offsets;
    float4 _colorThreshold;
    float4 _bloomColor;
    float _bloomFactor;

    // 高亮部分提取shader
    v2f_threshold vert_threshold(appdata_img v)
    {
        v2f_threshold o;
        o.pos = UnityObjectToClipPos(v.vertex);
        o.uv = v.texcoord.xy;
        //dx中纹理从左上角为初始坐标，需要反向
#if UNITY_UV_STARTS_AT_TOP
        if (_MainTex_TexelSize.y < 0)
            o.uv.y = 1 - o.uv.y;
#endif
        return o;
    }

    fixed4 frag_threshold(v2f_threshold i) : SV_Target
    {
        fixed4 color = tex2D(_MainTex, i.uv);
        // 仅当color大于设置阀值的时候才输出
        return saturate(color - _colorThreshold); 
    }
    
    // 高斯模糊 vert shader
    v2f_blur vert_blur(appdata_img v)
    {
        v2f_blur o;
        _offsets *= _MainTex_TexelSize.xyxy;
        o.pos = UnityObjectToClipPos(v.vertex);
        o.uv = v.texcoord.xy;

        o.uv01 = v.texcoord.xyxy + _offsets.xyxy * float4(1,1,-1,-1);
        o.uv23 = v.texcoord.xyxy + _offsets.xyxy * float4(1,1,-1,-1)*2.0;
        o.uv45 = v.texcoord.xyxy + _offsets.xyxy * float4(1,1,-1,-1)*3.0;

        return o;
    }

    // 高斯模糊 pixel shader
    fixed4 frag_blur(v2f_blur i) :SV_Target
    {
        fixed4 color = fixed4(0,0,0,0);
        color += 0.40 * tex2D(_MainTex, i.uv);
        color += 0.15 * tex2D(_MainTex, i.uv01.xy);
        color += 0.15 * tex2D(_MainTex, i.uv01.zw);
        color += 0.10 * tex2D(_MainTex, i.uv23.xy);
        color += 0.10 * tex2D(_MainTex, i.uv23.zw);
        color += 0.05 * tex2D(_MainTex, i.uv45.xy);
        color += 0.05 * tex2D(_MainTex, i.uv45.zw);
        return color;
    }

    // Bloom效果 vertex shader
    v2f_bloom vert_bloom(appdata_img v)
    {
        v2f_bloom o;
        o.pos = UnityObjectToClipPos(v.vertex);
        o.uv.xy = v.texcoord.xy;
        o.uv1.xy = o.uv.xy;
#if UNITY_UV_STARTS_AT_TOP
        if (_MainTex_TexelSize.y < 0)
            o.uv.y = 1 - o.uv.y;
#endif
        return o;
    }

    fixed4 frag_bloom(v2f_bloom i) : SV_Target
    {
        // 取原始清晰图片进行uv采样
        fixed4 ori = tex2D(_MainTex, i.uv1);
        // 取模糊图片进行uv采样
        fixed4 blur = tex2D(_BlurTex, i.uv);
        // 输出 = 原始图像叠加bloom权值*bloom颜色*泛光颜色
        fixed4 final = ori + _bloomFactor * blur * _bloomColor;
        return final;
    }
    ENDCG
    SubShader
    {
        // pass 0: 提取高光部分
        Pass
        {
            ZTest Off
            Cull Off
            ZWrite Off
            Fog{ Mode Off }

            CGPROGRAM
            #pragma vertex vert_threshold
            #pragma fragment  frag_threshold 
            ENDCG 
        }

        // pass 1: 高斯模糊
        Pass
        {
            ZTest Off
            Cull Off
            ZWrite Off
            Fog{ Mode Off }
            CGPROGRAM
            #pragma vertex vert_blur
            #pragma fragment frag_blur
            ENDCG
        }
        // pass 2: 泛光颜色
        Pass
        {
            ZTest Off
            Cull Off
            ZWrite Off
            Fog { Mode Off }
            CGPROGRAM
            #pragma vertex vert_bloom
            #pragma fragment frag_bloom
            ENDCG
        }
    }
}

```