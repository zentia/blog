---
title: Introduction-to-Vertex-Shaders
mathjax: true
date: 2019-04-11 15:28:59
tags:
- RHI
- DirectX
categories:
- RHI
- DirectX
---
# 顶点着色器入门(Introduction to Vertex Shaders)
概览
   顶点着色器（vertex shader）是一个在图形卡的GPU上执行的程序，它替换了固定功能管线（fixed function pipeline）中的变换（transformation）和光照（lighting）阶段。（这不是百分之百的正确，因为顶点着色器可以被Direct3D运行时（Direct3D runtime）以软件模拟，如果硬件不支持顶点着色器的话）。图16.1说明了管线中顶点着色器替换的部件。
   
图16.1：顶点着色器替换固定功能管线的光照和变形阶段

   从图16.1，我们知道，顶点以局部坐标输入到顶点着色器，并且必须输出齐次剪裁空间的有颜色的顶点。（为了保持简单，本书中我们没有深入研究投影变换的细节。但是经投影矩阵变换顶点后的空间称作齐次剪裁空间（homogeneous clip space）。因此，要把一个顶点从局部空间变换到齐次坐标空间，我们必须应用下列变换序列：世界变换（world transformation），视图变换（view transformation）和投影变换（projection transformation），它们分别由世界矩阵，视图矩阵和投影矩阵来完成。）对于点元（point primitive），顶点着色器也被用于操作每个顶点的顶点大小。

   由于顶点着色器是我们（在HLSL中）写的一个自定义程序，因此我们在图形效果方面获得了我们能够达到的极大的自由性。我们不再受限于Direct3D的固定光照算法。此外，应用程序操纵顶点位置的能力也有了多样性，例如：cloth simulation，粒子系统的点大小操纵，还有顶点混合/morphing。此外，我们的顶点数据结构更自由了，并且可以在可编程管线中包含比在固定功能管线中多得多的数据。

   顶点着色器仍然是相对新的特性，并且许多图形卡不支持它们，特别是随DirectX 9发布的较新版本的顶点着色器。通过检查D3DCAPS9结构的VertexShaderVersion成员，可以测试顶点着色器的版本。下列代码段展示了这一点：
// If the device's supported version is less than version 2.0
if( caps.VertexShaderVersion < D3DVS VERSION(2, 0) )
     // Then vertex shader version 2.0 is not supported on this device.

我们看到D3D_VERSION的两个参数分别接收主和次版本号。现在，D3DXCompileShaderFromFile函数支持顶点着色器版本1.1和2.0。

目标
* 学习如何在可编程管线中定义顶点结构的分量
* 了解顶点分量的不同用法
* 学习如何创建、设置和销毁一个顶点着色器
* 学习如何使用顶点着色器实现卡通动画渲染效果

17.1顶点声明
   到现在为止，我们已经使用自由顶点格式（flexible vertex format，FVF）来描述顶点结构中的各分量。但是，在可编程管线中，顶点数据包含的数据比用FVF所能表达的多很多。因此，我们通常使用更具表达性并且更强大的顶点声明（vertex declaration）。

注意：如果FVF能够描述我们的顶点格式 我们仍然可以在可编程管线中使用它。不管用何种方法，只是为了方便，同样FVF会在内部被转换为一个顶点声明。

17.1.1 描述顶点声明
   我们将一个顶点声明描述为一个D3DVERTEXELEMENT9结构的数组。D3DVERTEXELEMENT9数组中的每个成员描述了一个顶点的分量。所以，如果你的顶点结构有三个分量（例如：位置、法线、颜色），那么其相应的顶点声明将描述3个D3DVERTEXELEMENT9结构的数组。这个D3DVERTEXELEMENT9结构定义如下：
```C++
typedef struct _D3DVERTEXELEMENT9 {
     BYTE Stream;
     BYTE Offset;
     BYTE Type;
     BYTE Method;
     BYTE Usage;
     BYTE UsageIndex;
} D3DVERTEXELEMENT9;
```

* Stream——指定关联到顶点分量的流
* Offset——偏移，按字节，相对于顶点结构成员的顶点分量的开始。例如，如果顶点结构是：
struct Vertex
{
     D3DXVECTOR3 pos;
     D3DXVECTOR3 normal;
};
……pos分量的偏移是0，因为它是第一个分量；normal分量的偏移是12，因为sizeof(pos) == 12。换句话说，normal分量以Vertex的第12个字节为开始。
* Type——指定数据类型。它可以是D3DDECLTYPE枚举类型的任意成员；完整列表请参见文档。常用类型如下：
* D3DDECLTYPE_FLOAT1——浮点数值
* D3DDECLTYPE_FLOAT2——2D浮点向量
* D3DDECLTYPE_FLOAT3——3D浮点向量
* D3DDECLTYPE_FLOAT4——4D浮点向量
* D3DDECLTYPE_D3DCOLOR—D3DCOLOR类型，它扩展为RGBA浮点颜色向量(r g b a)，其每一分量都是归一化到区间[0, 1]了的。

* Method——指定网格化方法。我们认为这个参数是高级的，因此我们使用默认值，标识为D3DDECLMETHOD_DEFAULT.。
* Usage——指定已计划的对顶点分量的使用。例如，它是否准备用于一个位置向量、法线向量、纹理坐标等？有效的用途标识符（usage identifier）是D3DDECLUSAGE枚举类型的：
typedef enum _D3DDECLUSAGE {
     D3DDECLUSAGE_POSITION     = 0,  // Position.
     D3DDECLUSAGE_BLENDWEIGHTS = 1,  // Blending weights.
     D3DDECLUSAGE_BLENDINDICES = 2,  // Blending indices.
     D3DDECLUSAGE_NORMAL       = 3,  // Normal vector.
     D3DDECLUSAGE_PSIZE        = 4,  // Vertex point size.
     D3DDECLUSAGE_TEXCOORD     = 5,  // Texture coordinates.
     D3DDECLUSAGE_TANGENT      = 6,  // Tangent vector.
     D3DDECLUSAGE_BINORMAL     = 7,  // Binormal vector.
     D3DDECLUSAGE_TESSFACTOR   = 8,  // Tessellation factor.
     D3DDECLUSAGE_POSITIONT    = 9,  // Transformed position.
     D3DDECLUSAGE_COLOR        = 10, // Color.
     D3DDECLUSAGE_FOG          = 11, // Fog blend value.
     D3DDECLUSAGE_DEPTH        = 12, // Depth value.
     D3DDECLUSAGE_SAMPLE       = 13  // Sampler data.
} D3DDECLUSAGE;
   
   D3DDECLUSAGE_PSIZE类型用于指定一个顶点的点的大小。它用于点精灵，因此我们可以基于每个顶点控制其大小。一个D3DDECLUSAGE_POSITION成员的顶点声明意味着这个顶点已经被变换，它通知图形卡不要把这个顶点送到顶点处理阶段（变形和光照）。

注意：这些中的少数用途类型（usage type）未在本书中提及，例如BLENDWEIGHTS, BLENDINDICES, TANGENT, BINORMAL, 和TESSFACTOR

* UsageIndex——用于标识多个相同用途的顶点分量。这个用途索引是位于区间[0, 15]间的一个整数。例如，假设我们有三个用途为D3DDECLUSAGE_NORMAL的顶点分量。我们可以为第一个指定用途索引为0，为第二个指定用途索引为1，并且为第三个指定用途索引为2。按这种方式，我们可以通过其用途索引标识每个特定的法线。

顶点描述声明的例子：假设我们想要描述的顶点格式由位置向量和三个法线向量组成。顶点声明可以指定如下：
D3DVERTEXELEMENT9 decl[] =
{
{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, 
　D3DDECLUSAGE_POSITION, 0},
　
{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, 
　D3DDECLUSAGE_NORMAL, 0},
　
{0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, 
　D3DDECLUSAGE_NORMAL, 1},
　
{0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, 
　D3DDECLUSAGE_NORMAL, 2},

D3DDECL_END()
};
D3DDECL_END宏用于初始化D3DVERTEXELEMENT9数组的最后一个顶点元素。同样的，注意法线向量的用途索引标签。

17.1.2 创建顶点声明
一旦你描述了一个顶点声明为D3DVERTEXELEMENT9数组，我们就可以使用下面的方法获得一个IDirect3DVertexDeclaration9接口指针：
HRESULT IDirect3DDevice9::CreateVertexDeclaration(
     CONST D3DVERTEXELEMENT9* pVertexElements,
     IDirect3DVertexDeclaration9** ppDecl
);
* pVertexElements——D3DVERTEXELEMENT9结构数组，它描述我们想要创建的顶点声明。
* ppDecl——用于返回创建的IDirect3DVertexDeclaration9接口指针
例子调用，其中decl是一个D3DVERTEXELEMENT9数组：
IDirect3DVertexDeclaration9* _decl = 0;
hr = _device->CreateVertexDeclaration(decl, &_decl);

17.1.3 使用一个顶点声明
   回忆一下：自由顶点格式是一个方便的特性并且在内部转换成了顶点声明。因此，当直接使用顶点声明，我们不再需要调用：Device->SetFVF( fvf );
相反，我们调用：Device->SetVertexDeclaration( _decl );
其中，_decl是一个IDirect3DVertexDeclaration9接口指针。
17.2顶点数据用途
考虑这个顶点声明：
D3DVERTEXELEMENT9 decl[] =
{
{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, 
　D3DDECLUSAGE_POSITION, 0},

{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, 
　D3DDECLUSAGE_NORMAL,   0},

{0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, 
　D3DDECLUSAGE_NORMAL,   1},

{0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
  D3DDECLUSAGE_NORMAL,   2},

D3DDECL_END()
};

   我们需要一种方式，来定义一个顶点声明的元素到顶点着色器的Input结构的数据成员的映射。我们在Input结构中通过指定每个数据成员的语义（: usage-type [usage-index]）定义这个映射。语义通过元素的用途类型和用途索引标识顶点声明中的一个元素。由数据成员的语义标识的顶点元素是得以映射到数据成员的元素。例如，对应于前面的顶点声明的输入结构是：
struct VS_INPUT
{
     vector position    : POSITION;
     vector normal     : NORMAL0;
     vector faceNormal1 : NORMAL1;
     vector faceNormal2 : NORMAL2;
};

注意：如果我们遗漏了用途索引，就意味着用途索引为零。例如，POSITION和POSITION0是同一样东西。

   这里decl中的元素0，由用途POSITION和用途索引0标识，它映射到position。decl中的元素1，由用途NORMAL和用途索引0标识，它映射到normal。decl中的元素2，由NORMAL和用途索引1标识，它映射到faceNormal1。decl中的元素3，由用途NORMAL和用途索引2标识，它映射到faceNormal2。

受支持的顶点着色器输入用途（input usage）是：
* POSITION [n]——位置
* BLENDWEIGHTS [n]——混合权重
* BLENDINDICES [n]——混合索引
* NORMAL [n]——法线向量
* PSIZE[n]——顶点大小
* DIFFUSE [n]——散射颜色
* SPECULAR [n]——镜面颜色
* TEXCOORD [n]——纹理坐标
其中，n是一个位于区间[0, 15]的可选整数。

注意：再重复一遍，这些用途类型中的少数未在本书中提及，如：BLENDWEIGHTS, TANGENT, BINORMAL, BLENDINDICES, 和TESSFACTOR。

   此外，对于输出结构，我们必须指定每个成员是用来做什么的。例如，数据成员应该被作为位置向量、颜色、纹理坐标等对待吗？图形卡没主意，除非你强制的告诉它。这也需要通过语法的语义来完成：
struct VS_OUTPUT
{
     vector position  : POSITION;
     vector diffuse   : COLOR0;
     vector specular  : COLOR1;
};

受支持的顶点着色器输出用途是：
* POSITION—位置
* PSIZE—顶点大小
* FOG—雾混合值
* COLOR [n]—顶点颜色。注意：可以有多个顶点颜色被输出，并且这些颜色可以被混合在一起以产生最终的颜色。
* TEXCOORD [n]—顶点纹理坐标。注意：多个顶点纹理坐标可以被输出。
其中，n是一个位于区间[0, 15]的可选整数。

17.3使用顶点着色器的步骤
下面的列表概括了创建和使用顶点着色器的必须步骤：
1. 编写并编译顶点着色器
2. 创建一个IDirect3DVertexShader9接口以引用已编译的着色器代码上的顶点着色器。
3. 用IDirect3DDevice9:: SetVertexShader方法使用这个顶点着色器。

当然，在我们做完这些之后，我们还得销毁这个顶点着色器。下面的各小节将更详细的迈入这些步骤。

17.3.1 编写并编译顶点着色器
   首先，我们必须编写一个顶点着色器程序。在本书中的HLSL一章中，我们已经编写了我们的着色器（译者注：参见我翻译的译文第一章中各节）。一旦着色器代码写好之后，我们就使用D3DXCompileShaderFromFile函数编译这个着色器，如16.2.2节所述。回忆一下，这个函数返回一个ID3DXBuffer指针，它包含已编译的着色器代码。

17.3.2 创建顶点着色器
   一旦我们拥有了编译好的着色器代码，我们就能够获得一个IDirect3DVertexShader9接口的指针，它代表一个顶点着色器——通过使用下面的方法：
HRESULT IDirect3DDevice9::CreateVertexShader(
     const DWORD *pFunction,
     IDirect3DVertexShader9** ppShader
);
pFunction——已编译着色器代码的指针
ppShader——返回一个IDirect3DVertexShader9接口的指针

例如，假设变量shader是一个包含已编译的，着色器代码的ID3DXBuffer指针。然后要获得一个IDirect3DVertexShader9接口，我们可以写：
IDirect3DVertexShader9* ToonShader = 0;
hr = Device->CreateVertexShader(
           (DWORD*)shader->GetBufferPointer(),
           &ToonShader);
注意：重申一遍，D3DXCompileShaderFromFile是一个函数，它将返回已编译着色器的代码（shader）。

17.3.3 建立顶点着色器
   在我们获得了一个代表我们的顶点着色器的IDirect3DVertexShader9接口的指针之后，我们就能够使用下面的方法使用它：
HRESULT IDirect3DDevice9::SetVertexShader(
     IDirect3DVertexShader9* pShader
);
这个方法仅接受一个参数，我们在其中传递一个想要使用的顶点着色器的指针。要使用这个我们在17.3.2节创建的着色器，我们可以写：Device->SetVertexShader(ToonShader);

17.3.4 销毁顶点着色器
   和所有的Direc3D接口一样，要清除他们，我们就必须在用完它们之后调用其的Release方法。仍然以我们在17.3.2节创建的顶点着色器为例，我们写：
d3d::Release<IDirect3DVertexShader9*>(ToonShader);

17.4样例应用程序：散射光照
   作为创建并使用顶点着色器的热身，我们写一个顶点着色器，它用一个方向（平行）光对每个顶点进行标准的散射光照。简而言之，散射光照根据顶点法线和光线向量（它的点朝向光源方向）的角度计算顶点接收到的光线的数量。角度越小，则顶点接收到的光线就越多；而角度越大，则顶点接收到的光线就越少。如果角度大于等于90度，顶点就接收不到光线了。

我们以检阅着色器代码作为开始：
// File: diffuse.txt
// Desc: Vertex shader that does diffuse lighting.
// Global variables we use to hold the view matrix, projection matrix,
// ambient material, diffuse material, and the light vector that
// describes the direction to the light source. These variables are
// initialized from the application.

matrix ViewMatrix;
matrix ViewProjMatrix;
vector AmbientMtrl;
vector DiffuseMtrl;
vector LightDirection;

// 环境光强度，漫射光强度
// 这些变量定义在着色器代码中
vector DiffuseLightIntensity = {0.0f, 0.0f, 1.0f, 1.0f};
vector AmbientLightIntensity = {0.0f, 0.0f, 0.2f, 1.0f};



// Input and Output structures.
struct VS_INPUT
{
     vector position : POSITION;
     vector normal   : NORMAL;
};

struct VS_OUTPUT
{
     vector position : POSITION;
     vector diffuse  : COLOR;
};

//Main
VS_OUTPUT Main(VS_INPUT input)
{
     // zero out all members of the output instance.
     VS_OUTPUT output = (VS_OUTPUT)0;
     
     // 变换位置到齐次坐标空间，保存到output.position成员中
     output.position = mul(input.position, ViewProjMatrix);
     
     // 变换光和法线到视图空间，设置w分量为0，是因为变换的向量不是点 
     LightDirection.w = 0.0f;
     input.normal.w  = 0.0f;
     LightDirection   = mul(LightDirection, ViewMatrix);
     input.normal    = mul(input.normal, ViewMatrix);

     // 计算光与法线夹角的余弦
     float s = dot(LightDirection, input.normal); 

     // 回忆一下，如果法线和光的夹角大于90度，则表面接收不到光。
     if( s < 0.0f )
         s = 0.0f;

     // 环境光反射是执行一个叉积（环境材质向量与环境光强度向量），
     // 漫射光反射是执行一个叉积（漫射材质向量与漫射光强度向量，
     // 更进一步讲，我们测量着色器的颜色，基于顶点从光源处接收到多少光
     //环境光和漫射光综合起来，决定一个顶点的最终颜色
     output.diffuse = (AmbientMtrl * AmbientLightIntensity) +
                      ((DiffuseMtrl * DiffuseLightIntensity) * s);
     return output;
}

   既然我们已经看到了实际的顶点着色器的代码，那么就让我们改变方式来看看应用程序的代码。这个应用程序有下列相关的全局变量：
IDirect3DVertexShader9* DiffuseShader = 0;
ID3DXConstantTable* DiffuseConstTable = 0; 
ID3DXMesh* Teapot               = 0;
D3DXHANDLE ViewMatrixHandle     = 0;
D3DXHANDLE ViewProjMatrixHandle = 0;
D3DXHANDLE AmbientMtrlHandle    = 0;
D3DXHANDLE DiffuseMtrlHandle    = 0;
D3DXHANDLE LightDirHandle       = 0;
D3DXMATRIX Proj;
   有代表顶点着色器及其常量表的变量，有茶壶网格的变量，接着是一组D3DXHANDLE，其名字描述了他们引用的变量：

Setup函数执行下列任务：
* 创建茶壶网格
* 编译顶点着色器
* 根据已编译代码创建顶点着色器
* 通过常量表获取着色器程序中的几个变量的句柄
* 通过常量表初始化着色器的这几个变量

注意：对于本应用程序，我们的顶点结构不需要任何自由顶点格式没有的额外的分量。因此，在本例中，我们使用一个自由顶点格式来代替顶点声明。回想一下，自由顶点格式描述最终在内部被转换为一个顶点声明。
bool Setup()
{
     HRESULT hr = 0;

     // Create geometry:
     D3DXCreateTeapot(Device, &Teapot, 0); 

     // Compile shader
     ID3DXBuffer* shader      = 0;
     ID3DXBuffer* errorBuffer = 0;
     
     hr = D3DXCompileShaderFromFile(
          "diffuse.txt",
          0,
          0,
          "Main", // entry point function name
          "vs_1_1",
          D3DXSHADER_DEBUG,
          &shader,
          &errorBuffer,
          &DiffuseConstTable);

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

     // Create shader
     hr = Device->CreateVertexShader(
          (DWORD*)shader->GetBufferPointer(),
          &DiffuseShader);

      if(FAILED(hr))
     {
        ::MessageBox(0, "CreateVertexShader - FAILED", 0, 0);
        return false;
     }

     d3d::Release<ID3DXBuffer*>(shader);

     // Get Handles
     LightDirHandle    = DiffuseConstTable->GetConstantByName(0, "LightDirection");
     ViewMatrixHandle = DiffuseConstTable->GetConstantByName(0, "ViewMatrix");
     ViewProjMatrixHandle = DiffuseConstTable->GetConstantByName(0, "ViewProjMatrix");
     AmbientMtrlHandle = DiffuseConstTable->GetConstantByName(0, "AmbientMtrl");
     DiffuseMtrlHandle = DiffuseConstTable->GetConstantByName(0, "DiffuseMtrl");
     

     // Set shader constants:
     // Light direction:
     D3DXVECTOR4 directionToLight(-0.57f, 0.57f, -0.57f, 0.0f);
     DiffuseConstTable->SetVector(Device, LightDirHandle,  &directionToLight); 

     // Materials:
     D3DXVECTOR4 ambientMtrl(0.0f, 0.0f, 1.0f, 1.0f);
     D3DXVECTOR4 diffuseMtrl(0.0f, 0.0f, 1.0f, 1.0f);
     DiffuseConstTable->SetVector(Device,AmbientMtrlHandle,&ambientMtrl);
     DiffuseConstTable->SetVector(Device,DiffuseMtrlHandle,&diffuseMtrl);
     
     DiffuseConstTable->SetDefaults(Device);

     // Compute projection matrix.
     D3DXMatrixPerspectiveFovLH(
       &Proj, D3DX PI * 0.25f,
       (float)Width / (float)Height, 1.0f, 1000.0f);

     return true;
}

   Display函数非常简单。它检测用户输入（译者注：这里指的是用户输入的传入着色器程序的变量），并相应的更新视图矩阵。但是，因为我们在着色器中执行这个视图矩阵变换，所以我们还必须更新着色器中的视图矩阵变量。我们用常量表完成这件事情。
bool Display(float timeDelta)
{
     if( Device )
     {
          // Update view matrix code snipped...
          D3DXMATRIX V;
          D3DXMatrixLookAtLH(&V, &position, &target, &up);
          DiffuseConstTable->SetMatrix(Device, ViewMatrixHandle, &V);
          D3DXMATRIX ViewProj =V *Proj;

          DiffuseConstTable->SetMatrix(Device, ViewProjMatrixHandle,
                                      &ViewProj);

          // Render
          Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                        0xffffffff, 1.0f, 0);
          Device->BeginScene();
          Device->SetVertexShader(DiffuseShader);
          Teapot->DrawSubset(0);
          Device->EndScene();
          Device->Present(0, 0, 0, 0);
     }
     return true;
}

同样注意，就在DrawSubset调用之前，我们允许了这个我们希望使用的顶点着色器。
清理也需要被完成；我们简单的释放了这个已分配的接口：
void Cleanup()
{
     d3d::Release<ID3DXMesh*>(Teapot);
     d3d::Release<IDirect3DVertexShader9*>(DiffuseShader);
     d3d::Release<ID3DXConstantTable*>(DiffuseConstTable);
}

17.5 卡通渲染
   作为第二个顶点着色器的例子，让我们编写两个顶点着色器，它们以卡通风格的绘画方式对网格着色（shade）并画轮廓（outline）。图17.2展示了这一点：

图17.2：（a）使用卡通着色法着色的对象（注意着色间的尖锐过渡）。（b）增强卡通效果，轮廓边（silhouette edge）被勾出。（c）使用标准散射光照着色的对象

注意：卡通渲染是一种特定类型的非写实渲染（non-photorealistic rendering），有时被称作风格化渲染（stylistic rendering）。

   虽然卡通渲染不适用于所有游戏，例如激烈的第一人称射击游戏，但是它仍然可以增强一些希望表现卡通感觉类型游戏的气氛。此外，卡通渲染是漂亮的，并易于实现。让我们好好的演示一个顶点着色器。

我们将卡通渲染分为两步：
1. 卡通绘画的特点是：在一个顶点到下一个顶点的强烈转换时，有少量的阴影强度级别；我们看一下这个卡通阴影（cartoon shading）。在图17.2（a）中，我们看到网络着色使用了三种阴影强度（亮、中、暗），而且其间的过渡是不平滑的——不像图17.2（c），其明暗过渡是平滑的。
2. 卡通绘图的主要特点是：在其外框上勾画轮廓，如图17.2（b）所示。
这两个步骤都需要其各自的顶点着色器。

17.5.1 卡通着色
   要实现卡通着色，我们采用Lander在2000年3月发表在Game Developer Magazine的文章“Shades of Disney: Opaquing a 3D World”中所描述的方法。它像这样工作：我们创建一个带强度级别的灰度纹理，它包含我们需要的不同的着色强度。图17.3显示了我们在样例程序中使用的这个纹理。

图 17.3：用来保存着色强度的着色纹理。注意观察不连续的着色间过渡和纹理着色强度必须从左到右增加。
   然后在顶点着色器中，我们执行标准散射点积运算（standard diffuse calculation dot product）来确定顶点法线N和光线向量L之间角度的余弦，用以确定顶点接收到多少光线：s=L·N
   
   如果s＜0，就表示光线向量和顶点法线之间的角度大于90度，也就表示该表面接收不到光线。因此，如果s＜0，我们就让s＝0。所以s ∈ [0, 1]。
   
   现在，在通常的散射光照模型中，我们使用s来标记颜色向量。这样，顶点颜色的明暗取决于接收到的光照的数量：diffuseColor = s(r, g, b, a)
   但是，这将会导致从亮到暗之间平滑的着色。这是与我们期望的卡通着色相反的。我们想要一种在几个不同着色器间突然转换颜色的效果（对卡通渲染来说，在2至4种着色器工作起来还是挺不错的）。
   
   不使用s来标记颜色向量，我们将使用s作为早先提到的强度纹理的u纹理坐标——如图17.3。
注意：标量（scalar）s必定是一个有效的纹理坐标，因为s ∈ [0, 1]，这是通常的纹理坐标区间。

   按这种方式，顶点不会被平滑着色，而是间断的。例如，强度纹理可能被分成3种着色，如图17.4所示：

图17.4：那么，s ∈ [0, 0.33]的值使用shader0着色，s ∈ [ 0.33，0.66]的值使用shader1着色，s ∈ [0.66,1]的值使用shader2着色。当然，从这些着色的一种到另一种的过渡是不平滑的，这就赋予了我们期望的效果。

注意：我们还为卡通着色关闭了纹理过滤，因为这种过滤会试图使着色过渡变平滑。这对于我们要求的不连续过渡是多余的。

17.5.2 卡通着色的顶点着色器代码
   我们现在介绍卡通着色的顶点着色器。这个着色器的主要任务只是根据s=L·N计算并设置纹理坐标。注意观察输出结构，我们已经增加了一个数据成员来存储已被计算过的纹理坐标。同时还需注意，我们仍然输出顶点颜色，虽然我们不修改它，不过当颜色被与强度纹理组合起来的时候，它呈现为被着色的。
// File: toon.txt
// Desc: Vertex shader that lights geometry so it appears to be
// drawn in a cartoon style.

// Globals
extern matrix WorldViewMatrix;
extern matrix WorldViewProjMatrix;
extern vector Color;
extern vector LightDirection;
static vector Black = {0.0f, 0.0f, 0.0f, 0.0f};

// Structures
struct VS_INPUT
{
     vector position : POSITION;
     vector normal   : NORMAL;
};

struct VS_OUTPUT
{
     vector position : POSITION;
     float2 uvCoords : TEXCOORD;
     vector diffuse  : COLOR;
};

// Main
VS_OUTPUT Main(VS_INPUT input)
{
      // zero out each member in output
　　　VS_OUTPUT output = (VS_OUTPUT)0;
　　　
      // transform vertex position to homogenous clip space
　　　output.position = mul(input.position, WorldViewProjMatrix);
　　　
      // Transform lights and normals to view space.  Set w
      // components to zero since we're transforming vectors.
      // Assume there are no scalings in the world
      // matrix as well.
      LightDirection.w = 0.0f;
      input.normal.w   = 0.0f;
      LightDirection   = mul(LightDirection, WorldViewMatrix);
      input.normal     = mul(input.normal, WorldViewMatrix);

      // Compute the 1D texture coordinate for toon rendering.
　　　float u = dot(LightDirection, input.normal);
　　　
      // Clamp to zero if u is negative because u
      // negative implies the angle between the light
      // and normal is greater than 90 degrees.  And
      // if that is true then the surface receives no light.
      if(u < 0.0f)
         u = 0.0f;

      // Set other tex coord to middle.
      float v = 0.5f;
      output.uvCoords.x = u;
      output.uvCoords.y = v;

      // save color
　　　output.diffuse = Color;
　　　
      return output;
}

两点注解：
* 我们假设世界矩阵没有执行任何缩放。因为如果它执行，它就会弄乱乘以它的顶点的长度和方向。
* 我们总是设置v纹理坐标为纹理的中点。这意味着我们仅使用纹理中一条单一的线，那就是说我们可以使用1D强度纹理来代替2D的那个纹理。不管怎样，1D和2D纹理都能工作。本例中，我们使用了2D纹理而不是1D纹理，这是没有什么特别的原因的。

17.5.3轮廓勾勒
要完成卡通效果，我们还需要勾勒（outline）轮廓边（silhouette edge）。这比卡通着色稍微复杂一点。

17.5.3.1 边的表示法
我们将一个网格的一条边表示为一个四元组（构建自2个三角形）——参见图17.5。

图 17.5：表示边的四元组

   我们选择四元组有两个原因：我们可以通过调整四元组的维容易的改变边的厚度，并且我们可以渲染退化的四元组来隐藏某些边，也即非轮廓边。在Direct3D中，我们从两个三角形来构建一个四元组。退化四元组（degenerate quad）是从两个退化三角形构建而来的四元组。退化三角形（degenerate triangle）是一个面积为零的三角形，或者换句话说，是一个三点位于一线上的三角形。如果我们传入一个退化三角形到渲染管线，则该三角形显示为空。这是很有用的，因为如果我们希望隐藏特定三角形，我们可以简单的退化它而不需要实际的从三角形列表（顶点缓冲）移除它。回想一下，我们只需要显示轮廓边——而不是网格的每一条边。

   当我们首先创建一条边的时候，我们指定其四个顶点，并使其退化，这意味着边将会被隐藏（渲染时不显示）。

图17.6：由两个三角形共用边描述的退化四元组

   注意图17.6中的两个顶点v0和v1，我们设置其顶点法线向量为零向量。然后当我们将边的顶点送入顶点着色器的时候，顶点着色器将会检测顶点是否位于轮廓边上；如果是，则顶点着色器将按顶点法线的方向偏移顶点位置的标量。观察法线向量为零的顶点，它不会被偏移。

因此，我们最终以一个非退化四元组（non-degenerate quad）来表示轮廓边，如图17.7所示。

图17.7：位于轮廓边上的顶点v2和v3被按照其各自的顶点法线n2和n3进行偏移。观察顶点v0和v1仍然保持在其固定位置，因为其顶点法线等于零向量，因此对于它们来说没有偏移发生。按这种方式，四元组成功的重新生成来表示轮廓边。

备注：如果我们没有设置顶点v0和v1的顶点法线为零向量，那么那些顶点就同样会被偏移。但是如果偏移描述轮廓边的所有四个顶点，那么我们仅是平移了该退化四元组。通过保持顶点v0和v1固定并仅仅偏移顶点v2和v3，我们重新生成了四元组。

17.5.3.2 轮廓边测试
   若两个三角面face0和face1在视图方向上与两个不同方向的面共享同一条边，则该边为轮廓边。也就是说，如果一个面是前面（front facing）而另一个面是后面（back facing），那么这条边就是一条轮廓边。图17.8给出了一个轮廓边和一个非轮廓边的例子。

图17.8：在（a）中，由v0 和v1定义的共享边的一个面是前面，而共享边另一个面是背面，因此该边是轮廓边。在（b）中，由v0 和v1定义的这两个共享边面都是前面，因此该边不是轮廓边。

   接下来，为了检测一个顶点是否在轮廓边上，我们必须以每个顶点为基础了解face0 和 face1的法线向量。我们的边的顶点数据结构反映如下：
struct VS_INPUT
{
     vector position    : POSITION;
     vector normal      : NORMAL0;
     vector faceNormal1 : NORMAL1;
     vector faceNormal2 : NORMAL2;
};

   前两个分量很直接，但让我们看看两个额外的法线向量，它们是faceNormal1和faceNormal2。这些向量描述了两个三角面的面法线，共享边的顶点位于这两个面的共享边上，这两个面是face0和face1。

   实际检测顶点是否在共享边上的数学如下。假设我们在视图空间中，令v为一原点指向检测顶点的向量——图17.8，令n0为face0的面法线且n1为face0的面法线，若下面的不等式为真，则顶点位于轮廓边上：
（1）（v·n0）（v·n1）＜0

   若两点积符号相异，则不等式为真，使得不等式左边为负。回想一下点积的性质：两个点积的符号相异，这意味着一个三角面是前面而另一个是后面。

   现在，考虑一条边只有一个三角形共享它的情况，如图17.9，其法线将会被存储在faceNormal1中。

图 17.9：顶点v0和v1定义的边只有一个三角面共享它

   我们定义这种边总为轮廓边。要确保顶点着色器将这种边作为轮廓边处理，我们要让faceNormal2 = -faceNormal1。因此，反向的面法线和不等式（1）为真，表示该边为一轮廓边。

17.5.3.3 边的生成
   生成网格的边是微不足道的；我们简单的遍历网格的每个三角面并为三角面上每条边计算一个四元组（退化的，如图17.6所示）。
注意：每个三角面有三条边，因为每个三角形有三条边。

   对于每条边上的顶点，我们同样需要知道共享边的两个三角面。一个面是边所在的三角形。例如，如果要计算第1个面的一条边，那么第1个面共享该边。共享该边的另一个面可以使用网格的邻接信息找到。
17.5.4 轮廓边顶点着色器代码
   我们现在呈现渲染轮廓边的顶点着色器代码。这个着色器的主要任务就是确定传入的顶点是否在轮廓边上。如果是，顶点着色器就以一定的值，按顶点法线的方向偏移顶点。
// File: outline.txt
// Desc: Vertex shader renders silhouette edges.

// Globals

extern matrix WorldViewMatrix;
extern matrix ProjMatrix;
static vector Black = {0.0f, 0.0f, 0.0f, 0.0f};

// Structures
struct VS_INPUT
{
     vector position : POSITION;
     vector normal : NORMAL0;
     vector faceNormal1 : NORMAL1;
     vector faceNormal2 : NORMAL2;
};
struct VS_OUTPUT
{
     vector position : POSITION;
     vector diffuse : COLOR;
};

// Main
VS_OUTPUT Main(VS_INPUT input)
{
      // zero out each member in output
      VS_OUTPUT output = (VS_OUTPUT)0; 

      // transform position to view space
      input.position = mul(input.position, WorldViewMatrix); 

      // Compute a vector in the direction of the vertex
      // from the eye. Recall the eye is at the origin
　　　// in view space - eye is just camera position.
      vector eyeToVertex = input.position; 

      // transform normals to view space.  Set w
      // components to zero since we're transforming vectors.
      // Assume there are no scalings in the world
      // matrix as well.
      input.normal.w      = 0.0f;
      input.faceNormal1.w = 0.0f;
      input.faceNormal2.w = 0.0f;
      input.normal      = mul(input.normal,      WorldViewMatrix);
      input.faceNormal1 = mul(input.faceNormal1, WorldViewMatrix);
      input.faceNormal2 = mul(input.faceNormal2, WorldViewMatrix); 

      // compute the cosine of the angles between
      // the eyeToVertex vector and the face normals.
      float dot0 = dot(eyeToVertex, input.faceNormal1);
      float dot1 = dot(eyeToVertex, input.faceNormal2);

      // if cosines are different signs (positive/negative)
      // then we are on a silhouette edge. Do the signs
      // differ?
      if( (dot0 * dot1) < 0.0f )
      {
           // yes, then this vertex is on a silhouette edge,
           // offset the vertex position by some scalar in the
           // direction of the vertex normal.
           input.position += 0.1f * input.normal;
      }

      // transform to homogeneous clip space
      output.position = mul(input.position, ProjMatrix);

      // set outline color
      output.diffuse = Black; 

      return output;
}

17.6 摘要
* 使用顶点着色器，我们可以替换固定功能管线的变换和光照阶段。通过用我们自己的程序（顶点着色器）替换此固定处理，我们可以在图形效果方面获得我们能够达到的极大的自由性。
* 顶点声明用于描述顶点格式。它们和自由顶点格式相似，但是更加自由并允许我们描述FVF不能描述的顶点格式。注意，如果顶点可以用FVF描述，我们仍然可以使用它们；不管怎样，在内部它们被转换为顶点声明。
* 对于输入，用途语义指定了顶点分量如何被从顶点声明映射到HLSL程序中的变量。对于输出，用途语义指定了顶点分量是用来做什么的（例如：位置、颜色、纹理坐标，等等）。


