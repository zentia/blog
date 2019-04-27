---
title: 第十八章 像素着色器入门(Introduction to Pixel Shaders)
mathjax: true
date: 2019-04-11 15:29:49
tags:
    - 图形学
categories: Direct3D
---
   像素着色器是一个执行在图形卡的GPU上的程序，它运行在对每个像素进行光栅化处理时。（不像顶点着色器，Direct3D不会以软件模拟像素着色器的功能。）它实际上替换了固定功能管线的多纹理化阶段（the multitexturing stage），并赋予我们直接操纵单独的像素和访问每个像素的纹理坐标的能力。这种对像素和纹理坐标的直接访问使我们可以达成各种特效，例如：多纹理化（multitexturing）、每像素光照（per pixel lighting）、景深（depth of field）、云状物模拟（cloud simulation）、焰火模拟（fire simulation）、高级阴影技术（sophisticated shadowing technique）。

   图形卡支持的像素着色器的版本可以通过D3DCAPS9结构的PixelShaderVersion成员和D3DPS_VERSION宏进行检查。下列代码片断展示了这点：
// If the device's supported version is less than version 2.0
if( caps.PixelShaderVersion < D3DPS_VERSION(2, 0) )
     // Then pixel shader version 2.0 is not supported on this device.

目标
* 获得对多纹理化概念的基本理解
* 学习如何编写、创建并使用像素着色器
* 学习如何使用像素着色器实现多纹理化效果

18.1多纹理化概览
   多纹理化（Multitexturing）可能是用像素着色器实现的最简单的技巧了。此外，因为像素着色器替换多纹理化阶段，那么接下来我们应该对多纹理化“是什么”和“做什么”有一个最基本的理解。本节介绍多纹理化的简明概览。

   当我们一开始讨论纹理化（texturing）的时候（第6章），我们忽略了固定功能管线中对多纹理化的讨论，这有两个原因：第一，多纹理化是有一点棘手的过程，我们考虑到这在当时是一个高级话题；此外，固定功能多纹理化阶段被新的和更强有力的像素着色器替换掉了。因此花时间在已经过时的固定功能纹理化阶段上是无意义的。

   多纹理化后面的概念有一点和混合（blending）相关。在第七章中我们了解到：可以将正要被光栅化的像素与之前写入后台缓冲的像素进行混合来达成一种特效。我们延伸这种相同的思想到多纹理化中（multiple texture）。也就是说，我们一次使用几个纹理，然后定义这些纹理如何被混合在一起，以达到一种特殊效果。多纹理化的一个通常的用法是执行光照。作为在顶点处理阶段使用Direct3D的光照模型的替代，我们使用一种叫做“光照图”（light map）的特殊纹理贴图（texture map），它编码（encode）表面是如何被光照的。例如，假设我们希望一盏聚光灯（spotlight）照在一个大木箱上，我们要么可以定义一个D3DLIGHT9结构的聚光灯，要么可以将代表木箱的纹理贴图与代表聚光灯的光照映射混合在一起，如图18.1所示。

图18.1：使用多纹理化渲染一个通过聚光灯照亮的木箱。这里我们通过将相应的纹理像素（texels）相乘来将这两个纹理组合起来。

注意：用第七章里的混合，结果图像依赖于纹理被混合的方式。在固定功能管线的多纹理化阶段，混合方程式被纹理渲染状态（texture render state）控制。用像素着色器，我们 能够以可编程的方式在代码中写出混合函数的简单表达式。这使我们可以用任何我们想要的方式混合纹理。我们将在讨论本章的例子程序时详细讨论纹理混合。

混合多个纹理（本例中是两个）来照亮木箱比起Direct3D的光照来有两个好处：
* 光照是是预先在聚光灯的光照贴图里计算好的。因此，光照不需要在运行时被计算，这节省了处理时间。当然，只有静态对象和静态灯光的光照可以被预先计算。
* 因为光照图是预先计算好的，我们能够使用比Direct3D的（光照）模型多的多的更加精确的和成熟的光照模型。（更好的光照可以产生更真实的场景。）

备注：多纹理化阶段的典型应用是实现静态对象的完全光照引擎（full lighting engine）。例如，我们可以用一个纹理贴图保存对象的颜色，比如木箱的纹理贴图。然后我们可以用一个散射光照贴图（diffuse light map）保存散射表面着色（diffuse surface shade），一个单独的镜面光照贴图保存镜面表面着色，一个雾状物贴图（fog map）保存覆盖在表面的雾状物的总量，还有可以用一个详细贴图（detail map）保存小的、高访问率的表面的细节。当所有这些纹理被组合起来，只需到这些预先计算的纹理中检索，就可以有效的照亮、着色并且增加细节到场景中去。

注意：聚光灯光照贴图在很基础的光照贴图中是一个价值不高（trivial）的例子。一般的的程序通过给定的场景和光源来生成光照贴图。生成光照贴图超越了本书的范围。有兴趣的读者可以参考Alan Watt和Fabio Policarpo在《3D Games: Real-time Rendering and Software Technology》中描述的光照贴图。

18.1.1 允许多个纹理
   回忆一下，纹理是用IDirect3DDevice9::SetTexture方法设置，而采样器状态（sampler state）是用IDirect3DDevice9::SetSamplerState方法设置，原型如下：
HRESULT IDirect3DDevice9::SetTexture(
     DWORD Stage, // specifies the texture stage index
     IDirect3DBaseTexture9 *pTexture
);

HRESULT IDirect3DDevice9::SetSamplerState(
     DWORD Sampler, // specifies the sampler stage index
     D3DSAMPLERSTATETYPE Type,
     DWORD Value
);

注意：一个特定的采样器阶段索引I联合第i个纹理阶段（texture stage）。即第i个采样器阶段指定采样器状态是第i集（set）纹理。

   纹理/采样器阶段索引标识了我们希望设置的纹理/采样器的纹理/采样器阶段。因此，我们可以允许多个纹理并通过使用不同的阶段索引设置其相应的采样器状态。在本书前面的部分中，我们总是指定0，来指示第一个阶段，因为我们一次仅使用一个纹理。所以例如，假设我们要允许三个纹理，我们像这样使用阶段0,1和2：
// Set first texture and corresponding sampler states.
Device->SetTexture(0, Tex1);
Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); 

// Set second texture and corresponding sampler states.
Device->SetTexture(1, Tex2);
Device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
Device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
Device->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); 

// Set third texture and corresponding sampler states.
Device->SetTexture(2, Tex3);
Device->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
Device->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
Device->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
这段代码使用Tex1, Tex2和Tex3，并设置每个纹理的过滤模式。

18.1.2 多纹理坐标
   回忆一下第六章，对于每个3D三角形，我们应该在纹理上定义一个三角形以映射该3D三角形。我们通过对每个顶点增加纹理坐标完成映射。因此，每三个顶点定义一个三角形，它对应于纹理上的三角形。

   因为我们现在使用多纹理，每三个顶点定义一个三角形，我们需要在每个被使用的纹理上定义一个相应的三角形。我们通过给每个顶点增加额外的一套纹理坐标——每个顶点一套，对应于每个使用的纹理。举个例子，如果我们混合三个纹理到一起，那么每个顶点必须有三套纹理坐标以索引到三个使用的纹理。因此，一个包含三个纹理的多纹理化顶点结构看起来可能像这样：
struct MultiTexVertex
{
     MultiTexVertex(float x, float y, float z,
                    float u0, float v0,
                    float u1, float v1,
                    float u2, float v2)
     {
          _x =  x;   _y = y; _z = z;
          _u0 = u0;  _v0 = v0;
          _u1 = u1;  _v1 = v1;
          _u2 = u2;  _v2 = v2;
     } 

     float _x, _y, _z;
     float _u0, _v0; // Texture coordinates for texture at stage 0.
     float _u1, _v1; // Texture coordinates for texture at stage 1.
     float _u2, _v2; // Texture coordinates for texture at stage 2. 

     static const DWORD FVF;
};
const DWORD MultiTexVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX3;

注意，指定自由顶点格式标记D3DFVF_TEX3表明顶点结构包含3套纹理坐标。固定功能管线支持最多8套纹理坐标。如果多于8套，你必须使用顶点声明和可编程顶点管线。

注意：在新版本像素着色器中，我们可以使用一套纹理坐标集来索引多个纹理，并因此消除了对多个纹理坐标的需要。当然这得假设每个纹理阶段使用相同的纹理坐标。如果每个阶段的纹理坐标不同，则我们仍然需要多纹理坐标。
# 像素着色器输入和输出
有两样东西要输入到像素着色器：颜色和纹理坐标。两样都是以每像素为单位的。
注意：回想一下，顶点颜色是在图元的面（face of primitive）间进行插值的。
每个像素的纹理坐标就是简单的 (u , v) ，它指定了纹理的哪个图素被映射到像素上。在输入到像素着色器前，Direct3D根据顶点颜色和顶点纹理坐标，为每个像素计算颜色和纹理坐标。输入到像素着色器的颜色和纹理坐标的数值依赖于顶点着色器输出的颜色和纹理坐标的数值。例如，如果一个顶点着色器输出了两个颜色和三个纹理坐标，那么Direct3D将会为每个像素计算两个颜色和三个纹理坐标并且把它们把它们输入到像素着色器。我们使用带语意的语法（semantic syntax）映射输入颜色和纹理坐标进我们的着色器程序的变量里。用前面的例子，我们可以这样写：
```c++
struct PS_INPUT
{
     vector c0 : COLOR0;
     vector c1 : COLOR1;
     float2 t0 : TEXCOORD0;
     float2 t1 : TEXCOORD1;
     float2 t2 : TEXCOORD2;
};
```
对于输出，像素着色器只输出一个计算过的该像素的颜色值：
```c++
struct PS_OUTPUT
{
     vector finalPixelColor : COLOR0;
};
```
# 使用像素着色器的步骤
下面的列表概述了创建和使用像素着色器的必要步骤：
1. 编写并编译像素着色器
2. 创建一个IDirect3DPixelShader9接口来代表基于已编译代码的像素着色器
3. 用IDirect3DDevice9::SetPixelShader方法允许该像素着色器

当然，用完顶点着色器之后我们必须销毁它。下面几个小节将深入这些步骤。

18.3.1 编写并编译像素着色器
   我们用与编译顶点着色器一样的方式编译像素着色器。首先，我们必须编写一个像素着色器程序。本书中，我们用HLSL编写我们的着色器。一旦写好着色器代码，我们就可以用D3DXCompileShaderFromFile函数编译该着色器了，如16.2节所述。回忆一下，这个函数返回一个ID3DXBuffer指针，它包含已编译的着色器代码。

注意：因为我们使用的是像素着色器，所以要记得把编译目标改成像素着色器目标（比如：ps_2_0），而不是顶点着色器目标（比如：vs_2_0）。编译目标通过D3DXCompileShaderFromFile函数的一个参数指定。详见16.2节。
18.3.2 创建像素着色器
   一旦我们编译了着色器代码，我们就可以获得一个IDirect3DPixelShader的接口指针，它代表一个像素着色器，使用下面的方法：
HRESULT IDirect3DDevice9::CreatePixelShader(
      CONST DWORD *pFunction,
      IDirect3DPixelShader9** ppShader
);
pFunction——已编译着色器代码的指针
ppShader——返回一个IDirect3DPixelShader9接口的指针

例如，假设变量shader是一个包含已编译着色器代码的ID3DXBuffer接口指针。那么要获得IDirect3DPixelShader9接口，我们应该写：
IDirect3DPixelShader9* MultiTexPS = 0;
hr = Device->CreatePixelShader(
           (DWORD*)shader->GetBufferPointer(),
           &MultiTexPS);
注意：重申一遍，D3DXCompileShaderFromFile是一个可以返回已编译着色器代码（shader）的函数。

18.3.3 建立像素着色器
   在我们获得一个代表我们的像素着色器的IDirect3DPixelShader9接口的指针之后，我们可以使用下面的方法使用它： 
HRESULT IDirect3DDevice9::SetPixelShader(
      IDirect3DPixelShader9* pShader
);
   这个方法只接受一个参数，我们通过它传递一个我们希望使用的指向像素着色器的指针。要使用我们在18.3.2节创建的像素着色器，我们应该写：
Device->SetPixelShader(MultiTexPS);

18.3.4 销毁像素着色器
   和其它所有Direct3D接口一样，要清除这些接口，我们必须在使用完毕后调用它们的Release方法。继续使用我们在18.3.2节创建的像素着色器，我们写：
d3d::Release<IDirect3DPixelShader9*>(MultiTexPS);

18.4 HLSL采样器对象
   在像素着色器中使用HLSL的内建函数tex*XXXX给纹理采样。
注意：采样时引用纹理上图素的坐标索引和采样器状态来生成像素。
   看16.7节详细地解释了这些函数，通常这些函数需要我们做2件事：
* 使用纹理中的索引建立(u, v)纹理坐标。
* 给特定的纹理中编入索引。

   将纹理坐标（u, v）输入到像素着色器，在一个指定的HLSL对象中的像素着色器中，我们想编入索引的纹理是在像素着色器中被定义过的，在HLSL中叫作采样器。（The particular texture that we want to index into is identified in the pixel shader by a special HLSL object called a sampler.），我们可以把采样器对象想象成定义纹理和采样器阶段的对象。例如：假如我们使用3张纹理，这意味着我们需要在像素着色器里能够引用3个阶段中的每个一个。在像素着色器中我们这样写：
sampler FirstTex;
sampler SecondTex;
sampler ThirdTex;
   Direct3D将给每个采样器对象连接一个唯一的纹理级别(stage)，在应用程序中我们找出与采样器对象相关联的阶段，并设置相应的纹理和采样器状态给该阶段。下列代码将举例说明如何在应用程序中设置纹理并把采样器状态设置为FirstTex：
 // 创建
IDirect3DTexture9* Tex;
D3DXCreateTextureFromFile(Device, "tex.bmp", &Tex);
… …
// 取得常量FirstTex的句柄
FirstTexHandle = MultiTexCT->GetConstantByName(0, "FirstTex");

// 取得常量的描述
D3DXCONSTANT_DESC FirstTexDesc;
UINT count;
MultiTexCT->GetConstantDesc(FirstTexHandle, &FirstTexDesc, &count);
… …
// 为FirstTex设置纹理和采样器状态. We identify
// the stage FirstTex is associated with from the
// D3DXCONSTANT_DESC::RegisterIndex member:
Device->SetTexture(FirstTexDesc.RegisterIndex, Tex);

Device->SetSamplerState(FirstTexDesc.RegisterIndex, 
                        D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
Device->SetSamplerState(FirstTexDesc.RegisterIndex,
                        D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
Device->SetSamplerState(FirstTexDesc.RegisterIndex,
                        D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
   注意：作为选择，替换使用采样器类型，你可以使用更多特殊的、强大的类型，如：sampler1D，sampler2D，sampler3D，和samplerCube类型，这些类型更安全并且它们只使用tex* 函数。例如：一个sampler2D对象只使用tex2D*函数，同样一个sampler3D对象只使用tex3D*函数。
18.5 例子程序：Multitexturing in a Pixel Shader
   这章中的例子演示了在像素着色器中使用多纹理，这个例子将纹理一个基于图18.2方格，渲染的目标是一个木箱纹理，一个聚光灯纹理，和一个包含字符串的纹理。这就是例子程序：Pixel Shader。
   
   图18.2: 混合纹理. 让我们分别取得木箱纹理上、聚光灯纹理和字符串纹理上相关联的像素颜色：b，s和t，然后定义如何将这些颜色混合： c = b × s + t 。
   这个例子可以不使用像素着色器来实现，但实现这个程序是简单直接，它允许我们示范如何写，创建，而且使用像素着色器实现一些特效不必使用那些复杂的算法。
   虽然在这个例子中一次只使用3张纹理，检查采样器对象的成员以确定每个像素着色器能够使用的版本，这是值得的。换句话说，我们一次能使用多少纹理这依赖  于使用的像素着色器的版本。
* 像素着色器的版本ps_1_1 到 ps_1_3支持4个纹理采样器。
* 像素着色器的版本ps_1_4支持6个纹理采样器。
* 像素着色器的版本ps_2_0到 ps_3_0支持16个纹理采样器。
```c++
// File: ps_multitex.txt

// Desc: Pixel shader that does multitexturing.
// Globals
sampler BaseTex;
sampler SpotLightTex;
sampler StringTex;

// Structures
struct PS_INPUT
{
     float2 base      : TEXCOORD0;
     float2 spotlight : TEXCOORD1;
     float2 text      : TEXCOORD2;
};

struct PS_OUTPUT
{
     vector diffuse : COLOR0;
};

// Main
PS_OUTPUT Main(PS_INPUT input)
{
     // zero out members of output
     PS_OUTPUT output = (PS_OUTPUT)0;

     // sample appropriate textures
     vector b = tex2D(BaseTex,      input.base);
     vector s = tex2D(SpotLightTex, input.spotlight);
     vector t = tex2D(StringTex,    input.text);

     // combine texel colors
     vector c =b *s +t;
     // increase the intensity of the pixel slightly
     c += 0.1f;
     // save the resulting pixel color
     output.diffuse = c;

     return output;
}
```
首先像素着色器定义了3个sampler对象，要渲染的每个纹理，接下来定义是input和output结构。注意：我们没有将任何的颜色值输入到像素着色器中，这是因为我们使用纹理自己的颜色和光照；即BaseTex保存表面的颜色，SpotLightTex是光照图。像素着色器输出只一个简颜色值，指定了我们计算过的这个特定像素的颜色。
Main函数使用tex2D函数采样3  个纹理，即它取得每个纹理的图素，计算映射到的像素，这通常依赖于指定的纹理坐标和采样器对象。然后我们混合图素的颜色用公式：c = b * s + t。接下来我们让全部的像素变亮一个bit，给每个部分增加0.1f。最后我们保存结果像素颜色并返回它。
现在我们看到了的像素着色器的代码，现在我们改变并考虑应用程序的代码。应用程序有下列相应的全局变量：
```c++
IDirect3DPixelShader9* MultiTexPS = 0;
ID3DXConstantTable* MultiTexCT    = 0;

IDirect3DVertexBuffer9* QuadVB = 0;

IDirect3DTexture9* BaseTex      = 0;
IDirect3DTexture9* SpotLightTex = 0;
IDirect3DTexture9* StringTex    = 0;
D3DXHANDLE BaseTexHandle      = 0;
D3DXHANDLE SpotLightTexHandle = 0;
D3DXHANDLE StringTexHandle    = 0;

D3DXCONSTANT_DESC BaseTexDesc;
D3DXCONSTANT_DESC SpotLightTexDesc;
D3DXCONSTANT_DESC StringTexDesc;
```
多纹理顶点结构的例子如下：
```c++
struct MultiTexVertex
{
     MultiTexVertex(float x, float y, float z,
                    float u0, float v0,
                    float u1, float v1,
                    float u2, float v2)
     {
          _x =  x;   _y =  y; _z = z;
          _u0 = u0;  _v0 = v0;
          _u1 = u1;  _v1 = v1;
          _u2 = u2,  _v2 = v2;
     }

     float _x,  _y,  _z;
     float _u0,  _v0;
     float _u1,  _v1;
     float _u2,  _v2;

     static const DWORD FVF;
};
const DWORD MultiTexVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX3;
```
它包含3个纹理坐标系统。
   
Setup函数执行下列功能：
* 填充方形的顶点缓存
* 编译着像素色器
* 创建像素色器
* 读取纹理
* 设置投影矩阵，不使用光照
* 取得采样器(sampler)对象的句柄
* 取得采样器对象的描述
```c++
bool Setup()
{
    HRESULT hr = 0;

    // Create quad geometry.
    Device->CreateVertexBuffer(
         6 * sizeof(MultiTexVertex),
        D3DUSAGE_WRITEONLY,
        MultiTexVertex::FVF,
        D3DPOOL_MANAGED,
        &QuadVB,
        0);

    MultiTexVertex*v =0;
    QuadVB->Lock(0, 0, (void**)&v, 0);

    v[0] = MultiTexVertex(-10.0f, -10.0f, 5.0f,
                       0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    v[1] = MultiTexVertex(-10.0f, 10.0f, 5.0f,
                       0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[2] = MultiTexVertex( 10.0f, 10.0f, 5.0f,
                       1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

    v[3] = MultiTexVertex(-10.0f, -10.0f, 5.0f,
                       0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    v[4] = MultiTexVertex( 10.0f, 10.0f, 5.0f,
                       1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    v[5] = MultiTexVertex( 10.0f, -10.0f, 5.0f,
                       1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    QuadVB->Unlock();

    // Compile shader
    ID3DXBuffer* shader      = 0;
    ID3DXBuffer* errorBuffer = 0;

    hr = D3DXCompileShaderFromFile(
         "ps_multitex.txt",
        0,
        0,
        "Main", // entry point function name
        "ps_1_1",
        D3DXSHADER_DEBUG,
        &shader,
        &errorBuffer,
        &MultiTexCT);

    // output any error messages
    if( errorBuffer )
    {
        ::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
        d3d::Release<ID3DXBuffer*>(errorBuffer);
    }

    if(FAILED(hr))
    {
        ::MessageBox(0, "D3DXCompileShaderFromFile() - FAILED", 0, 0);
        return false;
    }

    // Create Pixel Shader
    hr = Device->CreatePixelShader((DWORD*)shader->GetBufferPointer(),&MultiTexPS);

    if(FAILED(hr))
    {
        ::MessageBox(0, "CreateVertexShader - FAILED", 0, 0);
        return false;
    }

    d3d::Release<ID3DXBuffer*>(shader);

    // Load textures.
    D3DXCreateTextureFromFile(Device, "crate.bmp", &BaseTex);
    D3DXCreateTextureFromFile(Device, "spotlight.bmp", &SpotLightTex);
    D3DXCreateTextureFromFile(Device, "text.bmp", &StringTex);

    // Set projection matrix
    D3DXMATRIX P;
    D3DXMatrixPerspectiveFovLH(&P, D3DX_PI * 0.25f, (float)Width / (float)Height, 1.0f, 1000.0f);

    Device->SetTransform(D3DTS_PROJECTION, &P);

    // Disable lighting.
    Device->SetRenderState(D3DRS_LIGHTING, false);

    // Get handles
    BaseTexHandle      = MultiTexCT->GetConstantByName(0, "BaseTex");
    SpotLightTexHandle = MultiTexCT->GetConstantByName(0, "SpotLightTex");
    StringTexHandle    = MultiTexCT->GetConstantByName(0, "StringTex");

    // Set constant descriptions:
    UINT count;

    MultiTexCT->GetConstantDesc(BaseTexHandle, &BaseTexDesc, &count);
    MultiTexCT->GetConstantDesc(SpotLightTexHandle, &SpotLightTexDesc, &count);
    MultiTexCT->GetConstantDesc(StringTexHandle,&StringTexDesc, &count);

    MultiTexCT->SetDefaults(Device);
    return true;
}
```

Display函数设置像素着色器，使用2个纹理，并且在渲染方格前设置他们对应的采样器状态。
```c++
bool Display(float timeDelta)
{
    if( Device )
    {
        // ...camera update code snipped
        // Render
        Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                   0xffffffff, 1.0f, 0);
        Device->BeginScene();

        // set the pixel shader
        Device->SetPixelShader(MultiTexPS);
     Device->SetFVF(MultiTexVertex::FVF);
     Device->SetStreamSource(0, QuadVB, 0, sizeof(MultiTexVertex));

     // base tex
     Device->SetTexture(BaseTexDesc.RegisterIndex, BaseTex);
     Device->SetSamplerState(BaseTexDesc.RegisterIndex,
                             D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
     Device->SetSamplerState(BaseTexDesc.RegisterIndex,
                             D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
     Device->SetSamplerState(BaseTexDesc.RegisterIndex,
                             D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

     // spotlight tex
     Device->SetTexture(SpotLightTexDesc.RegisterIndex, SpotLightTex);
     Device->SetSamplerState(SpotLightTexDesc.RegisterIndex,
                             D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
     Device->SetSamplerState(SpotLightTexDesc.RegisterIndex,
                             D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
     Device->SetSamplerState(SpotLightTexDesc.RegisterIndex,
                             D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

     // string tex
     Device->SetTexture(     StringTexDesc.RegisterIndex, StringTex);
     Device->SetSamplerState(StringTexDesc.RegisterIndex,
                             D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
     Device->SetSamplerState(StringTexDesc.RegisterIndex,
                             D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
     Device->SetSamplerState(StringTexDesc.RegisterIndex,
                             D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

     // draw the quad
     Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

     Device->EndScene();
     Device->Present(0, 0, 0, 0);
}
return true;
}
```
当然我们必须想着在Cleanup函数中释放我们自己分配的接口。
```C++
void Cleanup()
{
    d3d::Release<IDirect3DVertexBuffer9*>(QuadVB);
    d3d::Release<IDirect3DTexture9*>(BaseTex);
    d3d::Release<IDirect3DTexture9*>(SpotLightTex);
    d3d::Release<IDirect3DTexture9*>(StringTex);

    d3d::Release<IDirect3DPixelShader9*>(MultiTexPS);
    d3d::Release<ID3DXConstantTable*>(MultiTexCT);
}
```
## 摘要
* 像素着色器取代了固定功能管线的多纹理级别（stage），而且，像素着色器给我们更改单独像素的能力，以任何方式选择和访问纹理数据。因而，使我们能实现很多使用固定功能管线所不能完成的特殊效果。
* 多纹理是一次使用几个纹理，并渲染它们一起创造出一个想要的结果的一个过程。多纹理代表性的用法是用它为静态几何图形实现光引擎。
* HLSL内建的采样器（sampler）对象，标识特定的纹理/采样器级别（stage）。A采样器常用于从像素着色器中引用一个纹理/采样器级别。

注意：一旦你懂得了如何去实现顶点和像素着色器，你需要的一些特效的创意，可以用它们去实现。得到特效创意最好的方法是，学习现有的用顶点和像素着色器实现的特效。《Direct3D ShaderX: Vertex and Pixel Shader Tips》和《Tricks edited by Wolfgang Engel》这2本书是众多出版物中最好的，像Nvidia和ATI的开发站点：http://developer.nvidia.com/ 和 http://ati.com/developer/index.html。另外我们推荐CG方面：由Randima Fernando 和 Mark J. Kilgard写的《The Cg Tutorial by Randima Fernando》，这本书对于使用Cg的3D图形编程是一本相当好的指南，它基本上和Direct3D's HLSL相同。

