---
title: 混合(Blending)
mathjax: true
date: 2019-04-11 15:08:23
tags:
- RHI
- DirectX
categories: 
- RHI
- DirectX
---
# 混合因素

　　观察图7.1，我们将一个红色的茶壶绘制在一个木质背景上。
　　
　　图7.1
　　假设想让茶壶有一个透明度，以便我们能够透过茶壶看见背景（如图7.2）。
　　
　　图7.2
　　我们怎样才能实现这个效果呢？我们只需要在木箱子上光栅化茶壶三角形，我们需要结合像素颜色，就象通过茶壶显示木箱那样来计算茶壶的像素颜色。结合像素值的意思就是用以前写过的目标像素值去估算源像素值这被叫做混合。注意混合的效果不仅仅象是玻璃透明一样。我们有很多选项来指定颜色是怎样被混合的，就象7.2部分中看到的一样。
　　这是很重要的，认识三角形普遍利用以前写入后缓存中的像素来与之混合来光栅化。在示例图片中，木箱图片首先被画出来且它的像素在后缓存中。我们然后绘制茶壶，以便用木箱的像素来混合茶壶的像素。因此，当使用混合时，下面的规则将被遵循：
　　规则：首先不使用混合绘制物体。然后根据物体离摄象机的距离使用混合对物体拣选；这是非常有效的处理，假如物体是在视图坐标中，那么你能够利用z分量简单地拣选。最后使用从后到前的顺序混合绘制物体。
　　下面的公式是用来混合两个像素值的：
　　
　　上面的所有变量都是一个4D颜色向量（r,g,b,a），并且符号是表示分量相乘。
* OutputPixel——混合后的像素结果。
* SourcePixel——通常被计算的像素，它是利用在后缓存中的像素来被混合的。
* SourceBlendFactor——在[0，1]范围内的一个值。它指定源像素在混合中的百分比。
* DestPixel——在后缓存中的像素。
* DestBlendFactor——在[0，1]范围内的一个值。它指定目的像素在混合中的百分比。
　　源和目的混合要素使我们能够按照多种途径改变原始源和目的像素，允许实现不同的效果。7.2节列举了能够被使用的预先确定的值。
　　混合默认是被关闭的；你能够通过设置D3DRS_ALPHABLENDENABLE渲染状态为true来开启它：
Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
7.2混合要素
　　通过设置不同的源和目的要素，我们能够创造很多不同的混合效果。通过实验，使用不同的组合看看它们到底能实现什么效果。你能够通过设置D3DRS_SRCBLEND和D3DRS_DESTBLEND渲染状态来分别设置源混合要素和目的混合要素。例如我们可以这样写：
Device->SetRenderState(D3DRS_SRCBLEND, Source);
Device->SetRenderState(D3DRS_DESTBLEND, Destination);
　　这里Source和Destination能够使用下面混合要素中的一个：
* D3DBLEND_ZERO——blendFactor=(0, 0, 0, 0)
* D3DBLEND_ONE——blendFactor=(1, 1, 1, 1)
* D3DBLEND_SRCCOLOR——blendFactor=(rs, gs, bs, as)
* D3DBLEND_INVSRCCOLOR——blendFactor=(1-rs, 1-gs, 1-bs, 1-as)
* D3DBLEND_SRCALPHA——blendFactor=(as, as, as, as)
* D3DBLEND_INVSRCALPHA——blendFactor=(1-as, 1-as, 1-as, 1-as)
* D3DBLEND_DESTALPHA——blendFactor=(ad, ad, ad, ad)
* D3DBLEND_INVDESTALPHA——blendFactor=(1-ad, 1-ad, 1-ad, 1-ad)
* D3DBLEND_DESTCOLOR——blendFactor=(rd, gd, bd, ad)
* D3DBLEND_INVDESTCOLOR——blendFactor=(1-rd, 1-gd, 1-bd, 1-ad)
* D3DBLEND_SRCALPHASAT——blendFactor=(f, f, f, 1)  ,  f=min(as, 1 – ad)
* D3DBLEND_BOTHINVSRCALPHA——这种混合模式设置源混合要素为（1-as, 1-as, 1-as, 1-as,）以及目的混合要素为（as,as,as,as）。这种混合模式仅对D3DRS_SRCBLEND有效。
　　源和目的混合要素的默认值分别是D3DBLEND_SRCALPHA和D3DBLEND_INVSRCALPHA。
7.3透明度
    在以前的章节中我们忽略了颜色顶点和材质中的alpha部分，那是因为当时它并不是必须的。现在它首先被用在混合中。
    Alpha部分主要是用来指定像素的透明等级。我们为每个像素的alpha部分保留8位，alpha的有效值在[0,255]范围内，[0,255]代表不透明度[0%,100%]。因此，像素的alpha为0时，表示完全透明，像素的alpha为128时，表示50%透明，像素的alpha为255时，表示完全不透明。
    为了让alpha部分描述像素的透明等级，我们必须设置源混合要素为D3DBLEND_SRCALPHA以及目的混合要素为D3DBLEND_INVSRCALPHA。这些值碰巧也是被默认设置的。
7.3.1Alpha通道
　　代替使用Alpha部分来计算遮影，我们能够从纹理的alpha通道中得到alpha信息。Alpha通道是额外的设置位，用它来保存每一个点的alpha值。当一个纹理被映射到一个图元上时，在alpha通道中的alpha信息也被映射，并且它们利用alpha信息为每个像素赋予纹理。图7.3显示了一个带8位alpha通道的图片。
　　
　　图7.3
　　图7.4显示的是一个利用alpha通道指定透明度来渲染的一个纹理方块。
　　
　　图7.4
7.3.2指定Alpha资源
　　默认情况下，假如设置一个有alpha通道的纹理，alpha值从在alpha通道中获得。假如没有alpha通道，那么alpha值是通过顶点颜色获得。然而，你能够通过下面的渲染状态来指定使用哪一个资源：
// compute alpha from diffuse colors during shading
Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
// take alpha from alpha channel
Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
7.4使用DirectX纹理工具创建Alpha通道
    绝大多数普通图象文件格式没有存储alpha信息。在这一部分我们给你演示怎样使用DirectX纹理工具来创建一个带alpha通道的DDS文件。DDS文件是一个为DirectX应用程序和纹理设置的图象格式。DDS文件能够利用D3DXCreateTextureFromFile被读进纹理中，就象bmp和jpg文件一样。DirectX纹理工具被放在你的DXSDK目录下的\Bin\DXUtils文件夹下（我是放在C:\Program Files\Microsoft DirectX 9.0 SDK (February 2005)\Utilities\Bin\x86下的，文件名是DxTex.exe）。
    打开DirectX纹理工具，并且把本章中示例文件夹下的crate.jpg文件用工具打开。木箱被自动的按照24位RGB纹理被读取。它包含8位红色，8位绿色，以及8位蓝色。我们需要将该纹理增加为32位ARGB纹理，增加的是额外的8位alpha通道。从菜单中选择Format，选择Change Surface Format。一个象图7.5的对话框将被弹出。选择A8R8G8B8格式点击OK。

图7.5
    它创建了一个32位颜色深度的图象，它的每个象素都有8位alpha通道，8位红色，8位绿色，8位蓝色。我们下一步是向alpha通道中写入数据。我们将图7.3中的8位灰色图片信息写进alpha通道中。选择菜单中的File，选择Open Onto Alpha Channel Of This Texture。一个对话框将弹出让你选择包含你想要写入alpha通道中数据信息的图片。选择alphachannel.bmp文件。图7.6显示的是程序已经插入了alpha通道数据。

图7.6
    现在用你选择的文件名存储纹理；我们使用cratewalpha.dds文件名。
7.5实例程序：透明度
    这个实例程序是在一个木箱背景上绘制一个透明的茶壶，就象图7.2所显示的一样。在这个例子中alpha值是从材质中得到。应用程序允许我们通过按A或S键来增加/减少alpha的值。
    使用混合的必要步骤是：
1. 设置混合要素D3DRS_SRCBLEND 和 D3DRS_DESTBLEND。
2. 假如你使用alpha部分，指定资源（材质或alpha通道）。
3. 允许alpha混合渲染状态。
　　对于这个例子，我们定义下面的全局变量：
```c++
ID3DXMesh* Teapot = 0; // the teapot
D3DMATERIAL9 TeapotMtrl; // the teapot’s material

IDirect3DVertexBuffer9* BkGndQuad = 0; // background quad - crate
IDirect3DTexture9* BkGndTex = 0; // crate texture
D3DMATERIAL9 BkGndMtrl; // background material
```
Setup方法设置很多东西；我们省略了很多与本章无关的代码。关心混合，Setup方法指定alpha值的获取资源。在这个例子中，我们通过材质指定alpha值。注意我们设置茶壶的材质alpha部分为0.5，也就是说茶壶将按照50%的透明度被渲染。我们在这里也要设置混合要素。要注意的是在这个方法中我们不能将alpha混合设置为启用。理由是alpha混合要进行额外的处理并且应该仅在需要用时才被使用。举例，在这个例子中只有茶壶需要用允许alpha混合来被渲染——而方块不需要。因此，我们在Display函数中启用alpha混合。
```c++
bool Setup()
{
    TeapotMtrl = d3d::RED_MTRL;
    TeapotMtrl.Diffuse.a = 0.5f; // set alpha to 50% opacity
    BkGndMtrl = d3d::WHITE_MTRL;

    D3DXCreateTeapot(Device, &Teapot, 0);
    
    ...// Create background quad snipped    
    ...// Light and texture setup snipped
    
    // use alpha in material's diffuse component for alpha
    Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    // set blending factors so that alpha
    // component determines transparency
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    
    ...// view/projection matrix setup snipped
    return true;
}
```
在Display函数中，我们检测假如A或S键被按下那么就通过增加或减少材质的alpha值来反馈。注意这个方法要保证alpha值不会超出[0,1]的范围。我们然后渲染背景。最后，我们启用alpha混合，利用alpha混合来渲染茶壶，关闭alpha混合。
```c++
bool Display(float timeDelta)
{
    if( Device )
    {
        //
        // Update
        //
        // increase/decrease alpha via keyboard input
        if( ::GetAsyncKeyState('A') & 0x8000f )
            TeapotMtrl.Diffuse.a += 0.01f;
        if( ::GetAsyncKeyState('S') & 0x8000f )
            TeapotMtrl.Diffuse.a -= 0.01f;
        // force alpha to [0, 1] interval
        if(TeapotMtrl.Diffuse.a > 1.0f)
            TeapotMtrl.Diffuse.a = 1.0f;
        if(TeapotMtrl.Diffuse.a < 0.0f)
            TeapotMtrl.Diffuse.a = 0.0f;
        //
        // Render
        //
        Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                        0xffffffff, 1.0f, 0);
        Device->BeginScene();

        // Draw the background
        D3DXMATRIX W;
        D3DXMatrixIdentity(&W);
        Device->SetTransform(D3DTS_WORLD, &W);
        Device->SetFVF(Vertex::FVF);
        Device->SetStreamSource(0, BkGndQuad, 0, sizeof(Vertex));
        Device->SetMaterial(&BkGndMtrl);
        Device->SetTexture(0, BkGndTex);
        Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

        // Draw the teapot
        Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
        D3DXMatrixScaling(&W, 1.5f, 1.5f, 1.5f);
        Device->SetTransform(D3DTS_WORLD, &W);
        Device->SetMaterial(&TeapotMtrl);
        Device->SetTexture(0, 0);
        Teapot->DrawSubset(0);
        Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        Device->EndScene();
        Device->Present(0, 0, 0, 0);
    }
    return true;
}
```
注意：在本章中有另一个使用纹理通道来演示alpha混合的例子texAlpha。与上边的代码不同之处仅仅在于得到alpha值是从纹理而不是从材质。
```c
// use alpha channel in texture for alpha
Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
```
这个应用程序读取的是一个在7.4节中用DX Tex Tool工具创建的带有alpha通道的DDS文件。
