---
title: 第六章 纹理(Texturing)
mathjax: true
date: 2019-04-11 15:06:54
tags:
- RHI
- DirectX
categories: 
- RHI
- DirectX
---
Direct3D使用一个纹理坐标系统，它是由用水平方向的u轴和竖直方向v轴构成。由u,v坐标决定纹理上的元素，它被叫做texel。注意v轴是向下的（如图6.2）。
　　
　　图6.2
同样，注意规格化的坐标间隔，[0，1]，它被使用是因为它给Direct3D一个固定的范围用于在不同尺寸的纹理上工作。
对每一个3D三角形，我们都希望在给它贴图的纹理上定义一个用相应的三角形。（如图6.3）。
　　
　　图6.3
　　  我们再一次修改原来的顶点结构，添加一个用于在纹理上定位的纹理坐标。
```C++
struct Vertex
{
    float _x, _y, _z;
    float _nx, _ny, _nz;
    float _u, _v; // texture coordinates
    static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
```
　　我们在顶点格式中添加了一个D3DFVF_TEX1，它是说我们的顶点结构中包含了一个纹理坐标。
　　现在每个三角形都通过顶点的三个对象来建立，同时也通过纹理坐标定义了一个相应的纹理三角形。
6.2创建并赋予材质
　　纹理数据通常是从存储在磁盘中的图片文件中读取的，且被读进IDirect3DTexture9对象中。我们能够使用下面的D3DX函数完成这项工作：
HRESULT D3DXCreateTextureFromFile(
    LPDIRECT3DDEVICE9 pDevice, // device to create the texture
    LPCSTR pSrcFile, // filename of image to load
    LPDIRECT3DTEXTURE9* ppTexture // ptr to receive the created texture
);
这个函数能够读取下面图片格式中的任意一种：BMP,DDS,DIB,JPG,PNG,TGA。
　　例如，用一个名为stonewall.bmp的图片创建一个纹理，我们将按照下面的例子来写：
IDirect3Dtexture9* _stonewall;
D3DXCreateTextureFromFile(_device, "stonewall.bmp", &_stonewall);
　　设置当前纹理，我们使用下面的方法：
HRESULT IDirect3DDevice9::SetTexture(
    DWORD Stage, // A value in the range 0-7 identifying the texture
                // stage – see note on Texture Stages
    IDirect3DBaseTexture9* pTexture // ptr to the texture to set
);
　　例子：
Device->SetTexture(0, _stonewall);
注意：在Direct3D中，你能够设置八个纹理，它们能够组合起来创建更多细节的图象。这又被叫做多重纹理。在本书的第四部分以前我们不会使用多重纹理；因此现在我们总是设置stage为0。
　　为了销毁一个纹理，我们设置pTexture为0。例如，假如不想用一个纹理来渲染物体，那么我们就这样写：
Device->SetTexture(0, 0);
renderObjectWithoutTexture();
假如场景中有使用不同纹理的三角形，我们就必须添加与下面类似的一些代码：
Device->SetTexture(0, _tex0);
drawTrisUsingTex0();

Device->SetTexture(0, _tex1);
drawTrisUsingTex1();
6.3过滤器
    就象以前提及的，纹理被映射到屏幕中的三角形上。通常纹理三角形和屏幕三角形是不一样大的。当纹理三角形比屏幕三角形小时，纹理三角形会被适当放大。当纹理三角形比屏幕三角形大时，纹理三角形会被适当缩小。这两种情况，变形都将会出现。过滤（Filtering）是一种Direct3D用它来帮助这些变形变的平滑的技术。
    Direct3D提供了三种不同的过滤器；每种都提供了一个不同的品质级别。越好的品质越慢，因此你必须在品质与速度之间取得一个平衡。纹理过滤器是用IDirect3DDevice9::SetSamplerState方法来设置的。
* Nearest point sampling——这是默认的过滤方法且返回最差的效果，但是它的计算是最快的。下面的代码就是设置Nearest point sampling作为缩小放大的过滤器：
Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
* Linear filtering——这种过滤产生还算比较好的效果，在今天的硬件上处理它还是非常快的。它是被推荐使用的。下面的代码就是设置Linear filtering作为缩小放大的过滤器。
Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
* Anisotropic filtering——这种过滤产生最好的效果，但是处理时间也是最长的。下面的代码就是设置Anisotropic filtering作为缩小放大的过滤器。
Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
当使用Anisotropic filtering时，我们必须设置D3DSAMP_MAXANISOTROPY等级，它决定处理的质量。该值越高处理的效果越好。检查D3DCAPS9结构确认你的显卡是否支持此功能。下面的代码设置该值为4：
Device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 4);
6.4 Mipmaps
　　就象6.3节所说的，在屏幕上的三角形和纹理三角形通常是不一样大的。为了使这个大小差异变小，我们为纹理创建mipmaps链。也就是说将一个纹理创建成连续的变小的纹理，但是对它们等级进行定制过滤，因此对我们来说保存细节是很重要的（如图6.4）。
　　
　　图6.4
6.4.1 Mipmaps过滤器
    mipmap过滤器是被用来控制Direct3D使用mipmaps的。设置mipmap过滤器，你可以这样写：
Device->SetSamplerState(0, D3DSAMP_MIPFILTER, Filter);
在Filter处你能用下面三个选项中的一个：
* D3DTEXF_NONE——不使用mipmap。
* D3DTEXF_POINT——通过使用这个过滤器，Direct3D将选择与屏幕三角形大小最接近的mipmap等级。一旦等级选定了，Direct3D就将按照指定的过滤器进行缩小和放大过滤。
* D3DTEXF_LINEAR——通过使用这个过滤器，Direct3D将选择两个最接近的mipmap等级，缩小和放大过滤每个等级，然后线性联合计算它们两个等级来得到最终的颜色值。
6.4.2 Direct3D中使用Mipmaps
    在Direct3D中使用Mipmaps是很简单的。假如你的显卡支持Mipmaps，那么使用D3DXCreateTextureFromFile将为你产生一个Mipmap链。Direct3D自动选择与屏幕三角形最匹配的Mipmap。因此Mipmap有非常广泛的应用，且它能被自动设置。
6.5 寻址模式
    以前，我们规定纹理坐标必须指定在[0，1]之间。从技术上来说这是不正确的；他们能够超出这个范围。纹理坐标也可以在[0，1]的范围之外，它通过Direct3D的寻址模式来定义。这里有四种寻址模式：环绕纹理寻址模式、边框颜色纹理寻址模式、截取纹理寻址模式、镜像纹理寻址模式，这里分别给出了它们的示意图6.5，6.6，6.7，6.8。
  
图6.5（环绕）                          图6.6（边框）
  
图6.7（截取）                          图6.8（镜像）
在这些图片中，纹理坐标通过（0,0）（0,3）（3,0）（3,3）顶点来定义。在u轴和v轴上方块又被分成子块放进3×3的矩阵中。假如，你想让纹理按5×5的方格来平铺，你就应该指定环绕纹理寻址模式并且纹理坐标因该设置为（0,0）（0,5）（5,0）（5,5）。
    下面的代码片段列举的是怎样设置这四种寻址模式：
// set wrap address mode
if( ::GetAsyncKeyState('W') & 0x8000f )
{
    Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}
// set border color address mode
if( ::GetAsyncKeyState('B') & 0x8000f )
{
    Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
    Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
    Device->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x000000ff);
}
// set clamp address mode
if( ::GetAsyncKeyState('C') & 0x8000f )
{
    Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
}
// set mirror address mode
if( ::GetAsyncKeyState('M') & 0x8000f )
{
    Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
    Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
}
6.6实例程序：有纹理的方块
    这个例子是怎样为方块加上纹理以及设置一个纹理过滤器（如图6.9）。假如你的显卡支持，通过D3DXCreateTextureFromFile函数一个mipmap链将被自动创建。

图6.9
注意：还提供了其他两个例子大家就自己看看了。
为一个场景增加纹理的必要步骤是：
1. 用纹理坐标指定的，创建物体的顶点。
2. 用D3DXCreateTextureFromFile函数读取一个纹理到IDirect3DTexture9接口中。
3. 设置缩小倍数，放大倍数以及mipmap过滤器。
4. 在你绘制一个物体前，用IDirect3DDevice9::SetTexture设置与物体关联的纹理。
我们先定义几个全局变量；一个是顶点缓存，它存储方块的顶点。另外一个是我们为方块映射的纹理：
IDirect3DVertexBuffer9* Quad = 0;
IDirect3DTexture9*      Tex  = 0;
Setup程序是很容易读懂的；我们用已经定义了纹理坐标的两个三角形创建一个方块。然后把文件dx5_logo.bmp读进IDirect3DTexture9接口中。接着使用SetTexture方法赋予纹理。最后设置缩小和放大过滤器进行线性过滤，我们也可以设置mipmap过滤器来进行D3DTEXF_POINT: 
```c++
bool Setup()
{
    //
    // Create the quad vertex buffer and fill it with the
    // quad geoemtry.
    //

    Device->CreateVertexBuffer(
        6 * sizeof(Vertex), 
        D3DUSAGE_WRITEONLY,
        Vertex::FVF,
        D3DPOOL_MANAGED,
        &Quad,
        0);

    Vertex* v;
    Quad->Lock(0, 0, (void**)&v, 0);

    // quad built from two triangles, note texture coordinates:
    v[0] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    v[1] = Vertex(-1.0f,  1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    v[2] = Vertex( 1.0f,  1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

    v[3] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    v[4] = Vertex( 1.0f,  1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
    v[5] = Vertex( 1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

    Quad->Unlock();

    //
    // Create the texture and set filters.
    //

    D3DXCreateTextureFromFile(
        Device,
        "dx5_logo.bmp",
        &Tex);

    Device->SetTexture(0, Tex);

    Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

    //
    // Don't use lighting for this sample.
    //
    Device->SetRenderState(D3DRS_LIGHTING, false);

    //
    // Set the projection matrix.
    //

    D3DXMATRIX proj;
    D3DXMatrixPerspectiveFovLH(
            &proj,
            D3DX_PI * 0.5f, // 90 - degree
            (float)Width / (float)Height,
            1.0f,
            1000.0f);
    Device->SetTransform(D3DTS_PROJECTION, &proj);

    return true;
}
```
我们现在可以渲染方块了，且通常已经为它赋予了纹理：
```c++
bool Display(float timeDelta)
{
    if( Device )
    {
        Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
        Device->BeginScene();

        Device->SetStreamSource(0, Quad, 0, sizeof(Vertex));
        Device->SetFVF(Vertex::FVF);
        Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

        Device->EndScene();
        Device->Present(0, 0, 0, 0);
    }
    return true;
}
```