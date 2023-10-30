---
title: 屏幕后处理效果
mathjax: true
date: 2019-03-25 14:26:33
tags:
    Unity
categories: "Unity Shader"
---
屏幕后处理，通常指的是在渲染整个场景得到屏幕图像后，再对这个图像进行一系列操作，实现各种屏幕特效。使用这种技术，可以为游戏画面添加更多的艺术效果，例如景深（Depth of Field）、运动模糊（Motion Blur）等。
因此，想要实现屏幕后处理的基础在于得到渲染后的屏幕图像，即抓取屏幕，而Unity为我们提供了这样一个方便的接口，OnRenderImage函数。它的函数声明如下：
```C#
void OnRenderImage(RenderTexture src, RenderTexture dest)
```
当我们在脚本中声明此函数后，Unity会把当前渲染得到的图像存储在第一个参数对应的渲染纹理中，通过函数的一系列操作后，在把目标渲染纹理，即第二个桉树对应的渲染纹理显示到屏幕上。在OnRenderImage函数中，我们通常是利用Graphics.Blit函数来完成对渲染纹理处理。它有3中函数声明：
```C#
namespace UnityEngine
{
    public class Graphics
    {
        public static void Blit(Texture src, RenderTexture dest);
        public static void Blit(Texture source, RenderTexture dest, int sourceDepthSlice, int destDet\pthSlice);
        public static void Blit(Texture src, RenderTexture dest, Material mat, int pass, int destDepthSlice);
        public static void Blit(Texture src, Material mat);
    }
}
```
其中，参数src对应了源纹理，在屏幕后处理技术中，这个参数通常就是当前屏幕的渲染纹理或是上一步处理后得到的渲染纹理。参数dest是目标渲染纹理，如果它的值为null就会直接将结果显示在屏幕上。参数mat是我们使用的材质，这个材质使用的Unity Shader将会进行各种屏幕后处理操作，而src纹理将会被传递给Shader中名为_MainText的纹理属性。参数pass的默认值为-1，表示将会依次调用Shader内的所有Pass。否则，只会调用给定索引的Pass。
在默认情况下，OnRenderImage函数会在所有的不透明和透明的Pass执行完毕后调用，以便对场景中所有游戏对象都产生影响。但有时，我们希望在不透明的Pass（即渲染队列小于等于2500的Pass，内置的Background，Geometry和AlphaTest渲染队列均在此范围内）执行完毕后立即调用OnRenderImage函数，从而不对透明物体产生任何影响。此时，我们可以在OnRenderImage函数钱添加ImageEffectOpaque属性来实现这样的目的。
因此，要在Unity中实现屏幕后处理效果，过程通常如下：我们首先需要在摄像机中添加一个用于屏幕后处理的脚本。在这个脚本中，我们会实现OnRenderImage函数来获取当前屏幕的渲染纹理。然后，再调用Graphics.Blit函数使用特定的Unity Shader来对当前图像进行处理，在把返回的渲染纹理显示到屏幕上。对于一些复杂的屏幕特效，我们可能需要多次调用Graphics.Blit函数来对上一步的输出结果进行下一步处理。

# 高斯滤波
高斯模糊（Gaussian Blur），也叫高斯平滑，高斯滤波，其通常用它来减少图像噪声以及降低细节层次，尝尝也被用于对图像进行模糊。
通俗的讲，高斯模糊就是对整幅图像进行加权平均的过程，每一个像素点的值，都由其本身和领域内的其他像素经过加权平均后得到。高斯模糊的具体操作是：用一个模版（或称卷积、掩模）扫描图像中的每一个像素，用模版确定的领域内像素的加权平均灰度值去替代模版中心像素点的值。
高斯模糊同样利用了卷积计算，它适用了卷积核名为高斯核。高斯核是一个正方形大小的滤波器，其中每个元素的计算都是基于下面的高斯方程：
$G(x,y)= {1 \over 2\pi\delta^2}^{e^{x^2+y^2 \over 2\delta^2}}$
其中，$\delta$是标准方差（一般取值为1），x和y分别对应了当前位置到卷积核中心的整数距离。要构建一个高斯核，我们只需要计算高斯核中各个位置对应的高斯值。为了保证滤波后的图像不会变暗，我们需要对高斯核中的权重进行归一化，既让每个权重除以所有权重的和，这样可以保证所有权重的和为1。因此，高斯函数中e前面的系数实际不会对结果有任何影响。

高斯方程很好地模拟了领域每个像素对当前处理像素的影响程度，距离越近，影响越大。高斯核的维数越高，模糊程度越大。使用一个NxN的高斯核对图像进行卷积滤波，就需要NxNxWxH（W和H分别是图像的宽和高）次纹理采样。当N的大小不断增加时，采样次数就会变得非常巨大。幸运的是，我们可以把这个二维高斯函数拆分成两个一维函数。也就是说，我们可以使用两个一维的高斯核（图12.8中的右图）先后对图形进行滤波，它们得到的结果和直接利用二维高斯核是一样的，但采样次数只需要2xNxWxH。我们可以进一步观察到，两个一维高斯核中包含了很多重复的权重。对于一个大小为5的一维高斯核，我们实际只需要记录3个权重值即可。
{% asset_img 1.jpg %}
在本节，我们将会使用5x5的高斯核对原图进行高斯模糊。我们将先后调用两个Pass，第一个Pass将会使用竖直方向的一维高斯核对图像进行滤波，第二个Pass在使用水平方向的一维高斯核对图像进行滤波，得到最终的目标图像。在实现中，我们将还利用图像缩放进一步提高性能，并通过调整高斯滤波的应用次数来控制模糊程度（次数越多，图像越模糊）。

## 实现
```c
// Blur iterations - larger number means more blur.
[Range(0, 4)]
public int iterations = 3;

// Blur spread for each iteration - larger value means more blur 
[Range(0.2f, 3.0f)]
public float bludSpread = 0.6f;

[Range(1, 8)]
public int downSample = 2;
```
blurSpread和downSample都是出于性能的考虑。在高斯核维数不变的情况下，_BlurSize越大，模糊程度越高，但采样数却不会受到影响。但过大的_BlurSize值会造成虚影，这可能并不是我们希望的。而downSample越大，需要处理的像素数越少，同时也能进一步提高模糊程度，但过大的downSample可能会使图像像素化。

最后，我们需要定义关键的OnRenderImage函数。我们首先来看第一个版本，也就是最简单的OnRenderImage的实现：
```c
/// 1st edition: just apply blur
void OnRenderImage(RenderTexture src, RenderTexture dest) {
    if (material != null) {
        int rtW = src.width;
        int rtH = src.height;
        RenderTexture buffer = RenderTexture.GetTemporary(rtW, rtH, 0);
        
        // Render the vertical pass
        Graphics.Blit(src, buffer, material, 0);
        // Render the horizontal pass
        Graphics.Blit(buffer, dest, material, 0);
        
        RenderTexture.ReleaseTemporary(buffer);
    } else {
        Graphics.Blit(src, dest);
    }
}
```