# 概述
一个渲染效果一般由以下部分组成：一个顶点和/或像素着色器，一个需要设置的设备状态列表，一个或更多的渲染通道（rendering passes）。此外，有一个能在不同级别的图形硬件上渲染效果的可靠机制通常是值得的（也就是说，有不同的可用的效果版本执行同样的效果或尽可能尝试执行同样的效果）。显然，所有这些必要的任务组合在一起成为一个效果。因此，一个合理的做法是，设法将这些任务封装到一个单元中。
Direct3D效果构架提供了这样一个机制：将渲染效果的任务封装到一个效果文件。在效果文件中实现效果有两方面优势。其一，它允许我们不必重编译应用程序就能改变一个效果的执行。这是一种更新效果的过程，不管是修正一个bug，一些简单的加强，或者利用最新的3D硬件特性。第二，它将所有的效果组成部分封装到一个文件。
这一章指导你用必要信息和步骤，编写和创建一个效果文件的。我们注意到效果文件象我们的HLSL程序一样可以写在任何ASCII文件中。
# 目标
- 理解一个效果文件的结构和组织
- 找到HLSL中的一些额外的对象
- 学习如何在效果文件中指定设备状态
- 学习如何创建并使用一个效果
- 通过学习一些例子程序，取得使用效果框架上的一些经验
# 技术与传递（Techniques and Passes）
一个效果文件由一个或多个技术组成。一个技术是用一个特殊的方法渲染一些特效。所以换句话说，一个效果文件提供了渲染相同特效的一个或多个不同的传递。为什么同样的效果需要几个不同实现呢？是的，一些硬件可能不支持一个效果的一种特定实现。因此，必需在不同硬件上实现相同效果的不同版本。
注意：例如，我们可能实现一种效果的两个版本，一种用着色器实现而一种用固定管线实现。这样，那些有着色器（shader）支持的显卡用户能够利用着色器实现，而那些不支持着色器的用户仍然可以使用固定管线实现。
可以在一个效果文件中实现所有版本的效果，这让我们更完整的封装了所有的效果，也是效果框架的目标之一 ―― 封装（encapsulation）。
每种技术包括一次或多次渲染传递（passes）。一次渲染传递（rendering pass）在特定传递（pass）中封装了设备状态、采样器、和/或用于渲染几何体的着色器。
注意：一个效果不仅限于可编程管线使用。例如，它可以使用固定功能管线控制设备状态，比如灯光、材质以及纹理。
使用多次传递（multiple passes）的理由是，因为对每种特效，是通过使用不同的设备状态、着色器等等，对同样的几何体进行多次渲染来完成的。举例来说，回忆第8章 ，我们不得不在每帧里用不同的设备状态、多次渲染相同的几何体，以达到反射效果。

这个例子，是一个用两种技术实现的效果文件的框架，第一种技术包括一次传递而每二种技术包括两次传递:
```c++
// effect.txt

technique T0
{
    // first and only pass for this technique
    pass P0
    {
        [specify pass device states, shaders, samplers, etc.]
    }
}

technique T1
{
    // first pass
    pass P0
    {
        [specify pass device states, shaders, samplers, etc.]
    }
     
    // second pass
    pass P1
    {
        [specify pass device states, shaders, samplers, etc.]
    }
}
```
## 更多HLSL内置对象（ More HLSL Intrinsic Objects）
这是一些在HLSL中额外的内建对象类型。我们以前没有过早的提及，是因为它们主要用于效果框架。
### 纹理对象
   HLSL内建纹理类型描述了一个IDirect3DTexture9对象。通过使用纹理对象我们可以直接地在效果文件中对特定的采样器阶段结合纹理。纹理对象有下面的可以访问的数据成员：
* type—纹理类型 (例如：2D, 3D)
* format—纹理的像素格式
* width—纹理的宽度（单位像素）
* height—纹理的高度（单位像素）
* depth—纹理的深度（如果是3D纹理，单位像素）

注意：迄今为止我们仅仅使用纹理来存贮图形数据，但是当你学到更高级的技术，你会发现纹理可用来保存任意表格信息。换句话说，纹理仅是数据表，不是必须包含图形数据。例如，在碰撞映射（bump mapping）时我们用到一种叫做法线图的东东（normal map），就是一种在每个点上包括了法向量的纹理。
### 采样器对象与采样器状态
我们在18章讨论了采样器对象，然而，效果框架定义了新的关键字：sampler_state。使用sampler_state关键字，我们能初始化一个采样器对象（即，直接在效果方件中设置采样器对象的纹理和状态）。下面的例子说明了这点：
```C++
Texture Tex;
sampler SO = sampler_state
{
     Texture = (Tex);  // 纹理
     
     // 采样器状态
     MinFilter = LINEAR;
     MagFilter = LINEAR;
     MipFilter = LINEAR;
};
```
    这里我们给采样器S0的texture成员关联了纹理 Tex，并给状态成员设置了采样状态。我们直接明了的在效果文件中设置所有信息。
### 顶点与像素着色器对象（Vertex and Pixel Shader Objects）
vertexshader 和 pixelshader是HLSL的内建类型，分别表示顶点着色器和像素着色器。它们在效果文件中表示特定顶点和/或像素着色器，用于一个特定的渲染传递(pass)。vertexshader和/或pixelshader类型在应用程序中用ID3DXEffect::SetVertexShader和ID3DXEffect::SetPixelShader函数分别设置。例如，在效果文件中，让Effect是一个有效的ID3DXEffect对象，让VS是一个有效的IDirect3DVertexShader9对象，以及让VSHandle是一个D3DXHANDLE（是vertexshader 对象的引用）。然后，我们可以通过如下写法初始化VSHandle所引用的顶点着色器：
Effect->SetVertexShader(VSHandle, VS);
当在应用程序中设置效果文件中的变量时，多数时候我们使用SetVertexShader 和 SetPixelShader。
做为选择，我们可以直接在效果文件中写顶点和/或像素着色器。当使用一种特定的编译语法时，我们可以设置一个着色器变量。下面的例子展示了如何初始化一个pixelshader类型的变量ps。
```C++
// 定义入口函数
OUTPUT Main(INPUT input){...}

// 编译入口函数
pixelshader ps = compile ps_2_0 Main();
```
观察在pixelshader关键字之后的特定的版本名，接下来是着色器入口函数。注意，当用这种方式（style）初始化一个顶点或像素着色器对象时，入口函数必须定义在效果文件中。
   
最后，我们给一个特定传递关联一个着色器，如下：
```c++
// 定义入口函数
OUTPUT Main(INPUT input){...}

// 编译入口函数
vertexshader vs = compile vs_2_0 Main();

pass P0
{
    // 给这个传递（pass）关联一个着色器（vs）
    vertexshader = (vs);
    ...
}
```
或者更简洁的：
```c++
pass P0
{
    // 设置这个传递的顶点着色器，为入口函数" Main()"的顶点着色器
    vertexshader = compile vs_2_0 Main();
    ...
}
```
注意：这是一个相当有价值的论述，因此你至少要明白，你能用这样的语法来初始化一个vertexshader 和 pixelshader 类型：
```c++
vertexshader vs = asm { /*assembly instructions go here */ };
pixelshader ps = asm { /*assembly instructions go here */ };
```
如果你用汇编语言来写着色器，你就用这种语法。
### 字符串
最后，这是一个字符串对象，它的用法是这样地：
```C++
string filename = "texName.bmp";
```
尽管没有任何HLSL的内建函数支持字符串类型，但它可以在应用程序中读取。这样，我们能进一步封装效果使用的数据文件，比如纹理文件名和X文件字。

### 注解 (Annotations)
除我们已经描述过的语义符之外，注解可以用在变量上。注解在HLSL中是不使用的，但是它们可以被应用程序通过效果框架访问。它们仅仅服务于一个绑定 “note”的变量，这样应用程序就能够访问这个变量了。为注解加入了<annotation>语法。下面一行举例说明：
```C++
texture tex0 < string name = "tiger.bmp"; >;
```
在这个例子中的注解是<string name = "tiger. bmp";>。它关联了一个字符串到变量tex0，即保存纹理数据的文件名。很明显，用相应的文件名注解一个纹理是有益的。
注解可以使用下面函数被重新得到：
```C++
D3DXHANDLE ID3DXEffect::GetAnnotationByName(
     D3DXHANDLE hObject,
     LPCSTR pName
);
```
pName是我们要操作的注解的名字，而hObject是注解所在的父块句柄，如一个technique、pass或者结构块。一旦我们有了一个注解的句柄，我们就能通过应用ID3DXEffect::GetParameterDesc得到有关它的信息。查看DirectX SDK文档以得到更多详细的内容。
## 效果文件的设备状态（ Device States in an Effect File）
通常，为了正确执行一个效果，我们必须设置设备的状态，比如渲染状态、纹理状态、材质、灯光和纹理。将全部效果封装进一个文件使它有支持全部效果的能力，效果框架允许我们在效果文件中设置设备状态。设备状态被在渲染的传递部分（pass block）里设置，语法看起来象这样：
State= Value;
对于完整的状态的列表，在DirectX SDK文档的索引（index）中查找"states"，或者从SDK的目录（Contents）标签下，查找DirectX Graphics\Reference\Effect Reference\Effect Format\States
考虑FillMode状态。如果你看了一下刚刚提到的SDK中的内容，值与D3DFILLMODE一样，但没有D3DFILL_前缀。如果我们在SDK文档中查找D3DFILLMODE，我们找到值：D3DFILL_POINT, D3DFILL_WIREFRAME, and D3DFILL_SOLID。因而，对于效果文件我们省略了前缀，并获得下列状态FillMode的有效值：POINT, WIREFRAME, 和 SOLID。例如，你可以在效果文件中这么写-：
```C++
FillMode = WIREFRAME;
FillMode = POINT;
FillMode = SOLID;
```
注意：在后面的小节中我们将在例子程序中设置几个设备状态。多数时候能够通过状态的名字猜到它的用途，但如果你想得到更详细的描述，请查看SDK文档。
## 创建效果
    效果用ID3DXEffect接口表示，我们用下面的D3DX函数创建它：
```C++
HRESULT D3DXCreateEffectFromFile(
     LPDIRECT3DDEVICE9 pDevice,
     LPCSTR pSrcFile,
     CONST D3DXMACRO* pDefines,
     LPD3DXINCLUDE pInclude,
     DWORD Flags,
     LPD3DXEFFECTPOOL pPool,
     LPD3DXEFFECT* ppEffect,
     LPD3DXBUFFER *ppCompilationErrors
);
```
* pDevice—被创建的ID3DXEffect对象所关联的设备
* pSrcFile—我们要编译的包括效果源代码的文本文件的名字（效果文件名）
* pDefines—这个参数是可选的，在本书中指定为null
* pInclude—ID3DXInclude接口指针。这个接口被设计成由应用程序执行，因而我们可以替换默认行为。通常，默认行为就挺好，我们可以指定null忽略这个参数。
* Flags—编译效果文件中的shader的选项标志，指定0为没有标志。有效选项为：
o D3DXSHADER_DEBUG—指示编译器写入调试信息
o D3DXSHADER_SKIPVALIDATION—指示编译器不做任何代码检测。这只在你正在用到一个已知正常工作的shader时使用。
o D3DXSHADER_SKIPOPTIMIZATION—指示编译器不执行任何优化。实际上这只用于调试时，当你不想让编译器对代码做任何更改时。
* pPool—可选的ID3DXEffectPool接口指针，用于指定效果参数如何共享其它的效果实例。本例中指定null，表示我们不在参数与效果文件之间共享。
* ppEffect—返回一个ID3DXEffect接口指针，表示被创建的效果。
* ppCompilationErrors—返回一个包含错误代码字符串和消息的ID3DXBuffer指针。

这是一个调用D3DXCreateEffectFromFile的例子：
```C++
// 修建效果
ID3DXEffect* Effect = 0;
ID3DXBuffer* errorBuffer = 0;
hr = D3DXCreateEffectFromFile(
     Device,           // 关联的设备
     "effect.txt",     // 效果源文件
     0,                // no preprocessor definitions
     0,                // no ID3DXInclude interface
     D3DXSHADER DEBUG, // 编译标记
     0,                // 不共享参数
     &Effect,          // 返回创建效果的指针
     &errorBuffer);    // 返回的错误信息

// 输出错误信息
if( errorBuffer )
{
     ::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
     d3d::Release<ID3DXBuffer*>(errorBuffer);
}

if (FAILED(hr))
{
    ::MessageBox(0, "D3DXCreateEffectFromFile() - FAILED", 0, 0);
    return false;
}
```
## 设置系数（Setting Constants）
因为对于顶点和像素着色器，我们需要从程序代码中初始化效果文件中的变量。代替使用常量表，就象我们在顶点和像素着色器中做的那样，ID3DXEffect接口中有内建的设置变量的方法。我们这里不会列出所有的设置不同类型变量的方法，因为要完全列出实在是大多了—请查看DirectX SDK文档以获得完整列表。这里是一个删节的列表：
```C++
HRESULT ID3DXEffect::SetFloat(
    D3DXHANDLE hParameter,
    FLOAT f
);
Sets a floating-point variable in the effect file identified by hParameter to the value f 
HRESULT ID3DXEffect::SetMatrix(
    D3DXHANDLE hParameter,
    CONST D3DXMATRIX* pMatrix
);
Sets a matrix variable in the effect file identified by hParameter to the value pointed to by pMatrix 
HRESULT ID3DXEffect::SetString(
    D3DXHANDLE hParameter,
    CONST LPCSTR pString
);
Sets a matrix variable in the effect file identified by hParameter to the value pointed to by pString 
HRESULT ID3DXEffect::SetTexture(
    D3DXHANDLE hParameter,
    LPDIRECT3DBASETEXTURE9 pTexture
);
Sets a texture variable in the effect file identified by hParameter to the value pointed to by pTexture 
HRESULT ID3DXEffect::SetVector(
    D3DXHANDLE hParameter,
    CONST D3DXVECTOR4* pVector
);
Sets a vector variable in the effect file identified by hParameter to the value pointed to by pVector 
HRESULT ID3DXEffect::SetVertexShader(
    D3DXHANDLE hParameter,
    LPDIRECT3DVERTEXSHADER9
      pVertexShader
);
Sets a vertex shader variable in the effect file identified by hParameter to the value pointed to by pVertexShader 
HRESULT ID3DXEffect::SetPixelShader(
    D3DXHANDLE hParameter,
    LPDIRECT3DPIXELSHADER9 pPShader
);
Sets a pixel shader variable in the effect file identified by hParameter to the value pointed to by pPShader 
```
我们通过下面的方法得到变量（又叫效果参数effect parameters）句柄：
```C++
D3DXHANDLE ID3DXEffect::GetParameterByName(
    D3DXHANDLE hParent, // scope of variable - parent structure
    LPCSTR pName        // name of variable
);
```
它的用法与D3DXConstantTable::GetConstantByName方法一样。即每一个参数是一个D3DXHANDLE，它标识我们想得到的在哪个父结构中的变量句柄。对于没有父结构的全局变量，我们指定null。第二个参数是在效果文件中所显示的变量名。
做为例子，以下显示如何设置效果文件中的一些变量：
```C++
// some data to set
D3DXMATRIX M;
D3DXMatrixIdentity(&M);

D3DXVECTOR4 color(1.0f, 0.0f, 1.0f, 1.0f);

IDirect3DTexture9* tex = 0;
D3DXCreateTextureFromFile(Device, "shade.bmp", &tex);

// get handles to parameters
D3DXHANDLE MatrixHandle = Effect->GetParameterByName(0, "Matrix");
D3DXHANDLE MtrlHandle   = Effect->GetParameterByName(0, "Mtrl");
D3DXHANDLE TexHandle    = Effect->GetParameterByName(0, "Tex");

// set parameters
Effect->SetMatrix(MatrixHandle, &M);
Effect->SetVector(MtrlHandle, &color);
Effect->SetTexture(TexHandle, tex);
```
注意：对每一个ID3DXEffect::Set*方法都有相应的ID3DXEffect::Get*方法用来取得效果文件中的变量值。例如，为得到一个距阵类型的变量，我们可以用这个函数：
```C++
HRESULT ID3DXEffect::GetMatrix(
    D3DXHANDLE hParameter,
    D3DXMATRIX* pMatrix
);
```
要取得所有的方法列表，查看DirectX SDK文档。
## 使用效果
    在这一节和它的小节，我们展示一旦一个效果被创建出来后如何使用它。下面步骤概述了全部过程：
1. 得到一个在你想使用的效果文件中的技术句柄。
2. 激活想得到的技术。
3. 启动当前活动的技术。
4. 对每个激活技术中的渲染传递，渲染想要的几何体。回想一下，技术可能由几个渲染传递组成，我们必须在每个传递中渲染一次几何体。
5. 结束当前激活的技术。

### 获得效果句柄（ Obtaining a Handle to an Effect）
使用技术的第一步是获得一个技术D3DXHANDLE。可以用这个方法得到一个技术句柄：
```C++
D3DXHANDLE ID3DXEffect::GetTechniqueByName(
    LPCSTR pName // Name of the technique.
);
```
注意：实际上，一个效果文件包括几个技术，每一个都被针对一个特定的硬件能力设计。因此，应用程序通常在系统上运行一些能力测试，然后通过这些测试选择最好的技术。看下面小节中的ID3DXEffect::ValidateTechnique。
### 激活一个效果（ Activating an Effect）
一旦得到了想要的技术的句柄，我们必须激活这个技术。这可以通过下面方法实现：
```C++
HRESULT ID3DXEffect::SetTechnique(
    D3DXHANDLE hTechnique // Handle to the technique to set.
);
```
注意：在激活一项技术前你可能想用现有设备验证它。也就是说，你也许想确保硬件支持的特色、配置技术的使用。你可以用下面的方法：
```C++
HRESULT ID3DXEffect::ValidateTechnique(
    D3DXHANDLE hTechnique // Handle to the technique to validate.
);
```

回想一个效果文件可能有几个技术，每个偿试用不同的硬件特色执行一个特定效果，希望最少一个技术将在用户系统上执行。对于一个效果，你将遍例每一个技术并用ID3DXEffect::ValidateTechnique运行它，因而你能检测哪个技术是被支持的而哪个不被支持，然后进行适当的动作。
### 启动效果
为了使用一个效果渲染几何体，我们必须围绕绘图函数在ID3DXEffect::Begin 和 ID3DXEffect::End技术间调用。这些函数就是分别开启和关闭效果。
```C++
HRESULT ID3DXEffect::Begin(
    UINT* pPasses,
    DWORD Flags
);
```
* pPasses—返回在当前活动的技术中的传递的数量。
* Flags—下面标志的任何一个：
o Zero (0)—指定效果保存当前设备状态和着色状态，并在效果结束（这时ID3DXEffect::End被调用）后恢复它们。因为效果文件能够改变状态，对于可以保存启动效果前的状态来说，是很有用的。
o D3DXFX_DONOTSAVESTATE—指示效果不保存和恢复设备状态（除shader状态外）。
o D3DXFX_DONOTSAVESHADERSTATE—指示效果不保存和恢复shader状态。

### 设置当前的渲染传递（Setting the Current Rendering Pass）
在我们用效果渲染任何几何体前，我们必须指定使用的渲染传递。回想一个技术包括一个或多个渲染传递，每一个传递封装了不同的设备状态、采样器、和/或用于这一传递的着色器。渲染传递通过下面方法指定：
```C++
HRESULT ID3DXEffect::Pass(
     UINT iPass // Index identifying the pass.
);
```
一个技术的渲染传递被用标识为0...n-1的索引，共n个传递。因而，我们能用一个简单的循环遍例每一个传递，并用这一传递渲染几何体。19.6.6节有一个例子。
### 结束效果（Ending an Effect）
最后，对于每个传递，我们渲染完几何体后，停止并结束效果时使用ID3DXEffect::End方法：
HRESULT ID3DXEffect::End(VOID);

### 例子
下面的代码片断示例了以上的使用一个效果的必要的五个步骤：
```C++
// 有效果文件中
technique T0
{
    pass P0
    {
    
    }
}
```
```C++
// 在应用程序中，取得技术句柄
D3DXHANDLE hTech = 0;
hTech = Effect->GetTechniqueByName("TO");

// 激活技术
Effect->SetTechnique(hTech );

// 启动激活的技术
UINT numPasses = 0;
Effect->Begin(&numPasses, 0);

// 遍例每个传递
for(int i = 0; i < numPasses; i++)
{
    // 设置当前传递
    Effect->Pass(i);

    // 在传递中渲染几何体
    Sphere->Draw();
}

// 结束效果
Effect->End();
```

## 例子程序: Lighting and Texturing in an Effect File
做为热身，让我们创建一个在3D模型中操作灯光和纹理的效果文件。这个例子完全运行于固定功能管线，意味着效果框架不仅限于使用着色器。图19.1展示了使用灯光和纹理例子的屏幕截图。

图19.1: 灯光和纹理例子的屏幕截图. 纹理、材质和灯光状态在效果文件中指定。
 
以下是效果文件的实现：
```C++
// File: light tex.txt
// Desc: 效果文件控制光的设备状态，和纹理一个3D模型
?
// 全局变量
matrix WorldMatrix;
matrix ViewMatrix;
matrix ProjMatrix;
?
texture Tex;
?
// 过滤器
?
// Associated the texture 'Tex' with the texture stage 'S0'
// corresponds with and also set the sampler states for the sampler
// stage 'S0' corresponds with.
sampler S0 = sampler state
{
     Texture   = (Tex);
     MinFilter = LINEAR;
     MagFilter = LINEAR;
     MipFilter = LINEAR;
};

// Effect
technique LightAndTexture
{
     pass P0
     {
          // Set misc. render states.?
          pixelshader      = null;   // No pixel shader.
          vertexshader     = null;   // No vertex shader.
          fvf = XYZ | Normal | Tex1; // Flexible vertex format
          Lighting         = true;   // Enable lighting.
          NormalizeNormals = true;   // Renormalize normals.
          SpecularEnable   = false;  // Disable specular highlights.

          // Set transformation states?
          WorldTransform[0]   = (WorldMatrix);
          ViewTransform       = (ViewMatrix);
          ProjectionTransform = (ProjMatrix);

          // Set a light source at light index 0. We fill out all the
          // components for light[0] because the Direct3D
          // documentation recommends filling out all components
          // for best performance.?
          LightType[0]         = Directional;
          LightAmbient[0]      = {0.2f, 0.2f, 0.2f, 1.0f};
          LightDiffuse[0]       = {1.0f, 1.0f, 1.0f, 1.0f};
          LightSpecular[0]     = {0.0f, 0.0f, 0.0f, 1.0f};
          LightDirection[0]     = {1.0f, -1.0f, 1.0f, 0.0f};
          LightPosition[0]      = {0.0f, 0.0f, 0.0f, 0.0f};
          LightFalloff[0]        = 0.0f;
          LightRange[0]        = 0.0f;
          LightTheta[0]        = 0.0f;
          LightPhi[0]          = 0.0f;
          LightAttenuation0[0]  = 1.0f;
          LightAttenuation1[0]  = 0.0f;
          LightAttenuation2[0]  = 0.0f;
?
          // Finally, enable the light:?
          LightEnable[0] = true;
?
          // Set material components. This is like calling
          // IDirect3DDevice9::SetMaterial.?
          MaterialAmbient  = {1.0f, 1.0f, 1.0f, 1.0f};
          MaterialDiffuse  = {1.0f, 1.0f, 1.0f, 1.0f};
          MaterialEmissive = {0.0f, 0.0f, 0.0f, 0.0f};
          MaterialPower    = 1.0f;
          MaterialSpecular = {1.0f, 1.0f, 1.0f, 1.0f};
?
          // Hook up the sampler object 'S0' to sampler stage 0,
          // which is given by Sampler[0].?
          Sampler[0] = (S0);
     }
}
```
    在这个效果文件中我们主要设置设备状态，就象在19.3节所述。例如，我们直接在效果文件中设置一个光源和一个材质。此外，我们指定转换距阵和纹理及采样器状态。这些状态被指定，然后用LightAndTexture方法和渲染传递P0渲染全部几何体，。

   注意：考虑到在一个效果文件中涉及到的的变量，你必须把它们装入圆括号中。举例来说，涉及到距阵变量，你必须这样写：(WorldMatrix), (ViewMatrix), and (ProjMatrix)。不使用圆括号是违法的。

   因为大部分必需的和繁琐的工作都在效果文件里做了，比如设置灯光、材质和纹理。应用程序代码就是做一些创建效果和开启效果等简单的事情。例子中有下面一些相关的全局变量：
```C++
ID3DXEffect* LightTexEffect   = 0;

D3DXHANDLE WorldMatrixHandle  = 0;
D3DXHANDLE ViewMatrixHandle   = 0;
D3DXHANDLE ProjMatrixHandle   = 0;
D3DXHANDLE TexHandle          = 0;

D3DXHANDLE LightTexTechHandle = 0;
```
　　这些东西很没劲 —--- 只是一个ID3DXEffect指针和一些句柄。LightTexTechHandle是一个技术的句柄，因此在它的名字中有子字符串“Tech”。
     
   RestoreDeviceObjects函数执行三个主要步骤：创建效果，获得作为效果参数的我们要用的技术的句柄，并初始化一些效果参数。下面是删节的实现：
```C++
bool Setup()
{
    HRESULT hr = 0;
　　// ...省略了采样器的读取
?
    // 创建效果
    ID3DXBuffer* errorBuffer = 0;
    hr = D3DXCreateEffectFromFile(
        m_pd3dDevice ,
        "light_tex.txt",
        0,                // 没有定义预处理器
        0,                // 没有ID3DXInclude接口
        D3DXSHADER_DEBUG, // 编译标记
        0,                // 不共享参数
        &m_LightTexEffect,
        &errorBuffer);
?
    // 输出错误信息
    if( errorBuffer )
    {
        ::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
        SAFE_RELEASE(errorBuffer);
    }
?
    if(FAILED(hr))
    {
        ::MessageBox(0, "D3DXCreateEffectFromFile() - FAILED", 0, 0);
        return false;
    }
?
    // 保存经常访问的参数句柄
    m_WorldMatrixHandle  = m_LightTexEffect->GetParameterByName(0, "WorldMatrix");
    m_ViewMatrixHandle   = m_LightTexEffect->GetParameterByName(0, "ViewMatrix");
    m_ProjMatrixHandle   = m_LightTexEffect->GetParameterByName(0, "ProjMatrix");
    m_TexHandle         = m_LightTexEffect->GetParameterByName(0, "Tex");?
    m_LightTexTechHandle =
　　　　m_LightTexEffect->GetTechniqueByName("LightAndTexture");
?
    // 设置效果参数
    // 设置矩阵
    D3DXMATRIX W, P;?
    D3DXMatrixIdentity(&W);
    m_LightTexEffect->SetMatrix( m_WorldMatrixHandle, &W);?
    
    D3DXMatrixPerspectiveFovLH(
        &P,
        D3DX_PI * 0.25f, // 45 - degree
        (float)800.0f / (float)600.0f,
        1.0f,
        1000.0f);?
    m_LightTexEffect->SetMatrix( m_ProjMatrixHandle, &P);
?
    // Set texture
    IDirect3DTexture9* tex = 0;
    D3DXCreateTextureFromFile(m_pd3dDevice, "Terrain_3x_diffcol.jpg", &tex);?
    LightTexEffect->SetTexture(TexHandle, tex);
    d3d::Release<IDirect3DTexture9*>(tex);
?
    return true;
}

Disply函数很简单，运行步聚在19.6 节中简要说明:
bool Display(float timeDelta)
{
　　if( Device )
　　{
       // ...[Camera update snipped]
       // set the new updated view matrix
       LightTexEffect->SetMatrix(ViewMatrixHandle, &V);

       // Activate the technique and render
       Device->Clear(0, 0, D3DCLEAR TARGET | D3DCLEAR ZBUFFER,
                   0xffffffff, 1.0f, 0);
       Device->BeginScene();

       // set the technique to use
       LightTexEffect->SetTechnique( LightTexTechHandle );

       UINT numPasses = 0;
       LightTexEffect->Begin(&numPasses, 0);

       for(int i = 0; i < numPasses; i++)
       {
            LightTexEffect->Pass(i);

            for(int j = 0; j < Mtrls.size(); j++)
            {
               Mesh->DrawSubset(j);
            }
       }
       LightTexEffect->End();

       Device->EndScene();
       Device->Present(0, 0, 0, 0);
　　}
　　
　　return true;
}
```
19.8例子程序: Fog Effect
    非常遗憾，我们没有用一整章篇幅来介绍Direct3D雾化效果。雾化效果（以下简称雾）提高了场景的真实性，可以用它来模拟逼真的天气状况。另外，雾可以大大减少长剪裁（far-clip）平面视觉效果。
    虽然我们不能给它应有的重视，这里我们还是挤出了一个简要的雾化例程。虽然我们不涉及详细的细节，我们还是展示并解释了Direct3D代码，这是很直接的。
    Direct3D雾化是固定功能管线的一部份，受渲染状态限制。下面的效果文件设置顶点雾，以达到必要的雾化状态。

   注意：Direct3D也支持像素雾（也叫表格雾table fog），比顶点雾要更精确。
```C++
// File: fog.txt
// Desc: Effect file that handles device states for linear vertex fog.
technique Fog
{
     pass P0
     {
          // Set misc render states.?
          pixelshader      = null;
          vertexshader     = null;
          fvf              = XYZ | Normal;
          Lighting         = true;
          NormalizeNormals = true;
          SpecularEnable   = false;
?
          // Fog states?
          FogVertexMode = LINEAR;     // Linear fog function.
          FogStart      = 50.0f;       // Fog starts 50 units away from viewpoint.
          FogEnd        = 300.0f;     // Fog ends 300 units away from viewpoint.
?
          FogColor      = 0x00CCCCCC; // Gray colored fog.
          FogEnable     = true;        // Enable vertex fog.
     }
}
```
   
就象你看到的，线性顶点雾能够通过五个简单的渲染状态控制：
FogVertexMode—使用指定的雾函数用于顶点雾。雾函数指定雾如何根据距离增长，自然界的雾在近视口的地方比较薄并且根据距离增长变得厚起来了。有效的任务类型为LINEAR、EXP、EXP2。这些函数被定义为：

d 是到视口的距离(viewpoint.)

   注意：如果你用EXP或EXP2雾化函数，你不用设置FogStart 和 FogEnd，因为它们在这些雾函数类型中没被用到。代替的你必须设置雾密度（fog density）渲染状态（如，FogDensity = someFloatType）
FogStart—标记了物体将开始雾化的起始深度。
FogEnd—标记了物体将结束雾化的结束深度。
   注意：FogStart 与 FogEnd本质上定义了物体在其中被雾化的深度间隔（从视口）。
FogColor—一个DWORD 或 D3DCOLOR值，以描述雾的颜色
FogEnable—指定true以开启顶点雾或false以关闭顶点雾

   任何我们用fog.txt效果渲染的几何体将被雾化。通过这种方式，我们可以控制哪一物体得到雾化，而哪些不用雾化。这对只雾化特定区域是很有用的。例如，通常屋外是有雾的，屋里不被雾化。同样的，一定地理部分可能有雾，而另外部分可能没有。图19.2展示了这一小节的调用雾效果的例程的屏幕截图。
图19.2: 雾化效果例子程序的屏幕截图，在这个例子中我们使用线性雾函数，而且雾化渲染状态在效果文件中指定。 
19.9例子程序: Cartoon Effect
   到目前为止的2个效果文件的例子，我们没有使用着色器（shader）。因为着色器在特效中的重要部分，我们想展示一个最精简的例子。例程CartoonEffect执行了在17章中讨论的卡通着色器，但是这次应用效果框架。下面是一个删节版的效果文件：
```C++
// File: tooneffect.txt
// 在效果文件中的卡通着色器
extern matrix WorldMatrix;
extern matrix ViewMatrix;
extern matrix ProjMatrix;
extern vector Color;
extern vector LightDirection;
static vector Black = {0.0f, 0.0f, 0.0f, 0.0f};
extern texture ShadeTex;
?
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
?
// Cartoon Shader Function:
VS_OUTPUT Main(VS_INPUT input)
{
     ...[Implementation omitted for brevity.]
}
?
sampler ShadeSampler = sampler state
{
     Texture = (ShadeTex);
     MinFilter = POINT; // no filtering for cartoon shading
     MagFilter = POINT;
     MipFilter = NONE;
};
?
technique Toon
{
     pass P0
     {
          // Set P0's vertex shader.
          vertexShader = compile vs_1_1 Main();
          // Hook up the sampler object to sampler stage 0.
          Sampler[0] = (ShadeSampler);
     }
}
```
   我们注意到卡通着色器函数被定义在效果文件中，并且我们指定着色器使用一个特定的传递，在传递部分使用语法：vertexShader = compile vs_1_1_Main();。在效果文件中的设备状态象通常一样设置。
   
19.10 效果编辑（EffectEdit）
   在结束这章前，我们想提一下在DirectX SDK中的EffectEdit程序。可以在\DXSDK\Samples\C++\Direct3D\Bin文件夹中找到它。图19.3显示了一个屏幕截图。

图19.3: 一个在DirectX SDK 中的EffectEdit 程序的屏幕截图
    这个EffectEdit程序在测试和书写效果文件时是很有用的。我们推荐您在这个工具上花点时间。
 
19.11摘要 
略

相关文章：
AGP内存
AGP（Accelerate Graphical Port），加速图形接口。随着显示芯片的发展，PCI总线日益无法满足其需求。英特尔于1996年7月正式推出了AGP接口，它是一种显示卡专用的局部总线。严格的说，AGP不能称为总线，它与PCI总线不同，因为它是点对点连接，即连接控制芯片和AGP显示卡，但在习惯上我们依然称其为AGP总线。AGP接口是基于PCI 2.1 版规范并进行扩充修改而成，工作频率为66MHz。
　　AGP总线直接与主板的北桥芯片相连，且通过该接口让显示芯片与系统主内存直接相连，避免了窄带宽的PCI总线形成的系统瓶颈，增加3D图形数据传输速度，同时在显存不足的情况下还可以调用系统主内存。所以它拥有很高的传输速率，这是PCI等总线无法与其相比拟的。
　　由于采用了数据读写的流水线操作减少了内存等待时间，数据传输速度有了很大提高；具有133MHz及更高的数据传输频率；地址信号与数据信号分离可提高随机内存访问的速度；采用并行操作允许在CPU访问系统RAM的同时AGP显示卡访问AGP内存；显示带宽也不与其它设备共享，从而进一步提高了系统性能。 
　　AGP标准在使用32位总线时，有66MHz和133MHz两种工作频率，最高数据传输率为266Mbps和533Mbps，而PCI总线理论上的最大传输率仅为133Mbps。目前最高规格的AGP 8X模式下，数据传输速度达到了2.1GB/s。
　　AGP接口的发展经历了AGP1.0(AGP1X、AGP2X)、AGP2.0(AGP Pro、AGP4X)、AGP3.0(AGP8X)等阶段，其传输速度也从最早的AGP1X的266MB/S的带宽发展到了AGP8X的2.1GB/S。
AGP 1.0（AGP1X、AGP2X）
　 1996年7月AGP 1.0 图形标准问世，分为1X和2X两种模式，数据传输带宽分别达到了266MB/s和533MB/s。这种图形接口规范是在66MHz PCI2.1规范基础上经过扩充和加强而形成的，其工作频率为66MHz，工作电压为3.3v，在一段时间内基本满足了显示设备与系统交换数据的需要。这种规范中的AGP带宽很小，现在已经被淘汰了，只有在前几年的老主板上还见得到。
AGP2.0(AGP4X)
　　显示芯片的飞速发展，图形卡单位时间内所能处理的数据呈几何级数成倍增长，AGP 1.0 图形标准越来越难以满足技术的进步了，由此AGP 2.0便应运而生了。1998年5月份，AGP 2.0 规范正式发布，工作频率依然是66MHz，但工作电压降低到了1.5v，并且增加了4x模式，这样它的数据传输带宽达到了1066MB/sec，数据传输能力大大地增强了。
AGP Pro
　　AGP Pro接口与AGP 2.0同时推出，这是一种为了满足显示设备功耗日益加大的现实而研发的图形接口标准，应用该技术的图形接口主要的特点是比AGP 4x略长一些，其加长部分可容纳更多的电源引脚，使得这种接口可以驱动功耗更大（25-110w）或者处理能力更强大的AGP显卡。这种标准其实是专为高端图形工作站而设计的，完全兼容AGP 4x规范，使得AGP 4x的显卡也可以插在这种插槽中正常使用。AGP Pro在原有AGP插槽的两侧进行延伸，提供额外的电能。它是用来增强，而不是取代现有AGP插槽的功能。根据所能提供能量的不同，可以把AGP Pro细分为AGP Pro110和AGP Pro50。在某些高档台式机主板上也能见到AGP Pro插槽，例如华硕的许多主板。
AGP 3.0(AGP8X)
　　2000年8月，Intel推出AGP3.0规范，工作电压降到0.8V,并增加了8x模式，这样它的数据传输带宽达到了2133MB/sec，数据传输能力相对于AGP 4X成倍增长，能较好的满足当前显示设备的带宽需求。
AGP接口的模式传输方式
　　不同AGP接口的模式传输方式不同。1X模式的AGP，工作频率达到了PCI总线的两倍—66MHz，传输带宽理论上可达到266MB/s。AGP 2X工作频率同样为66MHz，但是它使用了正负沿（一个时钟周期的上升沿和下降沿）触发的工作方式，在这种触发方式中在一个时钟周期的上升沿和下降沿各传送一次数据，从而使得一个工作周期先后被触发两次，使传输带宽达到了加倍的目的，而这种触发信号的工作频率为133MHz，这样AGP 2X的传输带宽就达到了266MB/s×2（触发次数）＝533MB/s的高度。AGP 4X仍使用了这种信号触发方式，只是利用两个触发信号在每个时钟周期的下降沿分别引起两次触发，从而达到了在一个时钟周期中触发4次的目的，这样在理论上它就可以达到266MB/s×2（单信号触发次数）×2（信号个数）＝1066MB/s的带宽了。在AGP 8X规范中，这种触发模式仍然使用，只是触发信号的工作频率变成266MHz，两个信号触发点也变成了每个时钟周期的上升沿，单信号触发次数为4次，这样它在一个时钟周期所能传输的数据就从AGP4X的4倍变成了8倍，理论传输带宽将可达到266MB/s×4（单信号触发次数）×2（信号个数）＝2133MB/s的高度了。

　　目前常用的AGP接口为AGP4X、AGP PRO、AGP通用及AGP8X接口。需要说明的是由于AGP3.0显卡的额定电压为0.8—1.5V，因此不能把AGP8X的显卡插接到AGP1.0规格的插槽中。这就是说AGP8X规格与旧有的AGP1X/2X模式不兼容。而对于AGP4X系统，AGP8X显卡仍旧在其上工作，但仅会以AGP4X模式工作，无法发挥AGP8X的优势。


Direct3D中实现图元的鼠标拾取
索引：
　　1、什么是拾取，拾取能做什么？
　　2、拾取操作的步骤和实现
　　2.1． 变换并获得通过视点和屏幕上点击点的射线矢量（Dir）
　　 2.1.1 确定鼠标选取点的屏幕坐标
　　 2.1.2 得到Dir在观察坐标空间内的表示
　　 2.1.3 转换Dir到世界坐标空间，并得到观察点在世界坐标系中的坐标
　　2.2 使用射线矢量对场景中的所有三角形图元求交，获得三角形索引值和重心坐标。
　　 2.2.1 D3D扩展函数实现求交
　　 2.2.2射线三角面相交的数学算法
　　 2.2.3 拾取完成根据获得的中心坐标计算我们关心的常见量
　　3、结束及声明
　　4、参考文献
　　补充：重心坐标的概念
　　3D交互图形应用程序中，常常要用鼠标去选择图形，其实现的机制基于鼠标拾取算法。本文主要讲述如何在D3D中实现图元的鼠标拾取。为了讨论简单，本文假定读者理解D3D 坐标变换流程和基本的图形学知识，如果阅读有困难请参考相关资料。
1、什么是拾取，拾取能做什么？
   首先，拾取操作指当我们在屏幕上用鼠标点击某个图元，应用程序能返回该图元的一个标志和某些相关信息。有图形程序设计经验的人都知道，有这些信息就表示我们有了对该图元的控制权，我们可以删除，可以编辑，可以任意对待该图元，至于你到底想干什么，就是阁下自己的事了^_^。

2、拾取操作的步骤和实现
　　拾取算法的思想很简单：得到鼠标点击处的屏幕坐标，通过投影矩阵和观察矩阵把该坐标转换为通过视点和鼠标点击点的一条射入场景的光线，该光线如果与场景模型的三角形相交（本文只处理三角形图元），则获取该相交三角形的信息。本文讲述的方法除可以得到三角形的一个索引号以外还可以得到相交点的重心坐标。
从数学角度来看，我们只要得到射线的方向矢量和射线的发射点，我们就具备了判断射线与空间三角面是否相交的条件，本文主要讨论如何获得这些条件，并描述了射线与三角面相交判断算法和D3D的通常实现方法。 

　　根据拾取操作的处理顺序，大概可以依次分为以下几个步骤
2.1． 变换并获得通过视点和屏幕上点击点的射线矢量（Dir）
详细介绍之前，为了大家方便理解，我们要先简单说一下d3d坐标转换的大概流程，如下图:

　　 所以我们要通过一系列的反变换，得到我们关心的值在世界坐标中的表示。
　　2.1.1 确定鼠标选取点的屏幕坐标
　　这一步是非常简单的Windows给我们提供了API来完成屏幕坐标的获取，使用GetCursorPos获得鼠标指针位置，然后再利用ScreenToClient转换坐标到客户区坐标系(以窗口视区左上角为坐标原点，单位为像素)，设该坐标为（POINT screenPt）。
　　2.1.2 得到Dir在观察坐标空间内的表示
　　在观察坐标系中，Dir是一条从观察坐标原点出发的射线，所以我们只需要再确定一个该射线经过的点，就可以得到它在观察坐标系中的表示。假设我们要求的射线上的另外一点为该射线与透视投影平截头体近剪切面的交点，针对最普遍的透视投影而言，透视投影平截头体经投影变换后，变成一个1/2立方体（请允许我这么叫^_^，因为它的大小为一个正方体的一半，x,y方向边长为2，z方向为1）如图：

投影坐标系以近剪切面中心为坐标原点，该立方体从z轴负向看过去与图形程序视区相对应，最终近剪切面（前剪切面）上一点与屏幕坐标之间的对应关系如下图所示：

　　根据比例关系，screenPt与投影空间上的点projPt之间的关系为
　　假设图形程序窗口的宽为screenWidth,高为screenHeight,
　　projPt.x = (screenPt.x-screenWidth/2)/screenWidth*2; （公式1）
　　projPt.y = (screenPt.y-screenHeight/2)/screenHeight*2; （公式2）
　　projPt.z =0;（实际该值可任意取，不影响最终结果。为了处理简单，我们取改值为0，表示该点取在近剪切面上）
　　得到projPt后，我们需要做的是把该点坐标从投影空间转换到观察空间(view space),
　　根据透视投影的定义，可假设点(projPt.x，projPt.y，projPt.z)
　　对应的其次坐标为 
　　(projPt.x*projPt.w，projPt.y*projPt.w，projPt.z*projPt.w，projPt.w)
　　我们可以通过 GetTransform( D3DTS_PROJECTION, &ProjMatrix)函数获得投影矩阵ProjMatrix,则根据观察空间到投影空间的变换关系则：投影坐标 = 观察坐标×投影矩阵
　　(projPt.x*projPt.w，projPt.y*projPt.w，projPt.z*projPt.w，projPt.w)
　　= (viewPt.x，viewPt.y，viewPt.z, 1)*pProjMatrx;
　　根据定义和图形学原理
　　ProjMatrix = =
　　所以,
　　(projPt.x*projPt.w，projPt.y*projPt.w，projPt.z*projPt.w，projPt.w)
　　= ( viewPt.x*ProjMatrix._m11,
　　   viewPt.y*ProjMatrix._m22,
　　   viewPt.z*Q-QZn,
       viewPt.z)
       
　　所以
　　projPt.x*projPt.w = viewPt.x*ProjMatrix._m11
　　projPt.y*projPt.w = viewPt.y*ProjMatrix._m22
　　projPt.z*projPt.w = viewPt.z*Q-QZn （注意projPt.z = 0）
　　projPt.w = viewPt.z;
　　解得
　　viewPt.x = projPt.x*Zn/ ProjMatrix._m11;
　　viewPt.y = projPt.y*Zn/ ProjMatrix._m22;
　　viewPt.z = Zn;
　　好了，到这里为止我们终于求出了射线与近剪切面交点在观察坐标系中的坐标，现在我们拥有了射线的出发点(0,0,0)和射线方向上另外一点(viewPt.x,viewPt.y,viewPt.z),则该射线的方向矢量在观察空间中的表示可确定为（viewPt.x-0,viewPt.y-0,viewPt.z-0）,化简一下三个分量同除近剪切面z坐标Zn，该方向矢量可写作
　　DIRview = (projPt.x/projMatrix._m11,projPt.y/projMatrix._m22,1)
　　代入公式1，公式2
　　DIRview.x = (2*screenPt.x/screenWidth-1)/projMatrix._m11;
　　DIRview.y = (2*screenPt.y/screenHeight-1)/projMatrix._m22;
　　DIRview.z = 1;
     其中screenWidth和screenHeight可以通过图像显示的backBuffer的目标表面（D3DSURFACE_DESC）来获得，该表面在程序初始化时由用户创建。
     
　　2.1.3 转换Dir到世界坐标空间，并得到观察点在世界坐标系中的坐标
　　由于最终的运算要在世界坐标空间中进行，所以我们还需要把矢量DIRview从观察空间转换为世界坐标空间中的矢量DIRworld。
　　因为
　　DIRview = DIRworld*ViewMatrix;
　　其中ViewMatrix为观察矩阵，在D3D中可以用函数GetTransform( D3DTS_VIEW, &ViewMatrix )得到。
　　所以DIRworld = DIRview * inverse_ViewMatrix,其中inverse_ViewMatrix为ViewMatrix的逆矩阵。
　　观察点在观察坐标系中坐标为OriginView（0，0，0，1），所以其在世界坐标系中的坐标同样可以利用ViewMatrix矩阵，反变换至世界坐标系中，事实上我们可以很简单的判断出,其在世界坐标系中的表示为:
　　OriginWorld = (inverse_ViewMatrix._41,
　　inverse_ViewMatrix._42,
　　inverse_ViewMatrix._43,
　　1);
　　到这里为止，判断射线与三角面是否相交的条件就完全具备了。
2.2 使用射线矢量对场景中的所有三角形图元求交，获得三角形索引值和重心坐标。
　　这一步骤地实现由两种途径:
　　第一种方法非常简单，利用D3D提供的扩展函数D3DXIntersect可以轻松搞定一切。见2.1
　　第二种方法就是我们根据空间解析几何的知识，自己来完成射线三角形的求交算法。一般来讲，应用上用第一种方法就足够了，但是我们如果要深入的话，必须理解相交检测的数学算法，这样才能自由的扩展，面对不同的需求，内容见2.2
　　下面分别讲解两种实现途径：
　　2.2.1 D3D扩展函数实现求交
　　这种方法很简单也很好用，对于应用来说应尽力是用这种方式来实现，毕竟效率比自己写得要高得多。
　　实际上其实没什么好讲的，大概讲一下函数D3DXIntersect吧
　　D3D SDK该函数声明如下
　　HRESULT D3DXIntersect( 
　　LPD3DXBASEMESH pMesh,
　　CONST D3DXVECTOR3 *pRayPos,
　　CONST D3DXVECTOR3 *pRayDir,
　　BOOL *pHit,
　　DWORD *pFaceIndex,
　　FLOAT *pU,
　　FLOAT *pV,
　　FLOAT *pDist,
　　LPD3DXBUFFER *ppAllHits,
　　DWORD *pCountOfHits
　　);
　　l pMesh指向一个ID3DXBaseMesh的对象，最简单的方式是从.x文件获得，描述了要进行相交检测的三角面元集合的信息，具体规范参阅direct9 SDK
　　l pRayPos 指向射线发出点
　　l pRayDir 指向前面我们辛辛苦苦求出的射线方向的向量
　　l pHit 当检测到相交图元时，指向一个true,不与任何图元相交则为假
　　l pU 用于返回重心坐标U分量
　　l pV返回重心坐标V分量
　　l pDist 返回射线发出点到相交点的长度
　　注意：以上红色字体部分均指最近的一个返回结果（即*pDist最小）
　　l ppAllHits用于如果存在多个相交三角面返回相交的所有结果
　　l pCountOfHits 返回共有多少个三角形与该射线相交
　　补充：重心坐标的概念
三角形的重心坐标：　　
P1，P2，P3为空间三角形的三个顶点矢量， (U,?V)就称为三角形的重心坐标
在空间三角形平面上的点可以表示为：　P?=?P1?+?U?*?(P2?-?P1)?+?V?*?(P3?-?P1)
当0?<?U?<?1，0?<?V?<?1，0?<?U?+?V?<?1时，这个点P就在这个三角形的内部
                                                                             收集
　　
　　其中pU和pV用到了重心坐标的概念，下面稍作描述
　　一个三角形有三个顶点，在迪卡尔坐标系中假设表示：
　　V1(x1,y1,z1),
　　V2(x2,y2,z2),
　　V3(x3,y3,z3),
　　则三角形内任意一点的坐标可以表示为（pV为任意点）： 
　　pV = V1 + U(V2-V1) + V(V3-V1)
　　所以已知三个顶点坐标的情况下，任意一点可用坐标(U,V)来表示，其中 参数U控制V2在结果中占多大的权值，参数V控制V3占多大权值，最终V1占多大权值 = 1 - U - V，这种坐标定义方式就叫重心坐标。
　　
　　2..2.2射线三角面相交的数学算法
　　使用d3d扩展函数，毕竟有时不能满足具体需求，掌握了该方法，我们才能够获得最大的控制自由度，任意修改算法。
　　已知条件: 射线源点orginPoint,三角形三个顶点 v1,v2,v3,射线方向 Dir（均以三维坐标向量形式表示）。
　　算法目的: 判断射线与三角形是否相交，如果相交求出交点的重心坐标(U,V)和射线原点到交点的距离T。
　　我们可先假设射线与三角形相交则交点
　　(注以下均为向量运算：
dot(X,Y)       点乘；
　　cross(X，Y)    叉乘；
　　U，V，T      标量(这三个值为X、Y、Z轴坐标)) 
　　
　　则：（IntersectPoint为三角形上的交点）
　　IntersectPoint = V1 + U×(V2-V1) + V×(V3-V1) ;
　　IntersectPoint = originPoint + T×Dir；
　　所以：
　　orginPoint + T×Dir = V1 + U×(V2-V1) + V×(V3-V1);
　　整理得：
　　orginPoint - V1 = U×(V2-V1) + V×(V3-V1) - T×Dir;
?
这是一个简单的线性方程组，若有解则行列式［-Dir, V2-V1, V3-V1］不为0。
   根据T,U,V的含义当T>0, 0<U<1,0<V<1,0<U+V<1时该交点在三角形内部，解此方程组即可获得我们关心的值,具体解法不再赘述，克莱姆法则就够了（详细见线性代数）:射线原点到相交点的距离T,和交点的中心坐标(U,V)。
下面给出Direct 9 SDK示例程序中的实现代码：
```C++
IntersectTriangle( const D3DXVECTOR3& orig,
      const D3DXVECTOR3& dir, D3DXVECTOR3& v0,
     D3DXVECTOR3& v1, D3DXVECTOR3& v2,
     FLOAT* t, FLOAT* u, FLOAT* v )
{
??? // 算出两个边的向量
??? D3DXVECTOR3 edge1 = v1 - v0;
??? D3DXVECTOR3 edge2 = v2 - v0;
?
   D3DXVECTOR3 pvec;
   D3DXVec3Cross( &pvec, &dir, &edge2 );

   // 如果det为0，或接近于零则射线与三角面共面或平行，不相交
   //此处det就相当于上面的[-Dir, V2-V1, V3-V1]，
??? FLOAT det = D3DXVec3Dot( &edge1, &pvec );
?
??? D3DXVECTOR3 tvec;
??? if( det > 0 )
??? {
??????? tvec = orig - v0;
??? }
??? else
??? {
??????? tvec = v0 - orig;
??????? det = -det;
??? }
?
??? if( det < 0.0001f )
??????? return FALSE;
?
??? // 计算u并测试是否合法（在三角形内）
??? *u = D3DXVec3Dot( &tvec, &pvec );
??? if( *u < 0.0f || *u > det )
??????? return FALSE;
?
??? // Prepare to test V parameter
??? D3DXVECTOR3 qvec;
??? D3DXVec3Cross( &qvec, &tvec, &edge1 );
?
??? //计算u并测试是否合法（在三角形内）
??? *v = D3DXVec3Dot( &dir, &qvec );
??? if( *v < 0.0f || *u + *v > det )
??????? return FALSE;
?
??? /*计算t,并把t,u,v放缩为合法值（注意前面的t,v,u不同于算法描述中的相应量，乘了一个系数det）,注意：由于该步运算需要使用除法，所以放到最后来进行，避免不必要的运算，提高算法效率*/
??? *t = D3DXVec3Dot( &edge2, &qvec );
 FLOAT fInvDet = 1.0f / det;
 *t *= fInvDet;
 *u *= fInvDet;
 *v *= fInvDet;

 return TRUE;
}
```
2.2.3? 拾取完成根据获得的中心坐标计算我们关心的常见量
根据重心坐标（U,V）,我们可以很容易的算出各种相关量比如纹理坐标和交点的差值颜色，假设以纹理坐标为例设V1,V2,V3的纹理坐标分别为T1(tu1,tv1),T2(tu2,tv2),T3(tu3,tv3)则交点的坐标为
?
IntersectPointTexture = T1 + U(T2-T1) + V(T3-T1)

3、结束及声明
　　Ok, 到这里为止关于拾取的相关知识就介绍完了，小弟第一次写这种文章，不知道有没有把问题说清楚，希望对大家有所帮助，有任何问题可以给我发email: jzhang1@mail.xidian.edu.cn
　　或者到我的网站留言： www.heavysword.com
　　声明：
　　本文写作的目的是为了广大D3D学习者方便学习服务，文中算法为作者参考相关文献总结，作者无意把这些据为自己的成果，所有权原算法提出者所有（参阅参考文献），文中代码为D3d SDK的示例内容，由笔者进行了必要的解释，代码版权归microsoft所有。
4、参考文献
　　【1】Microsoft DirectX 9.0 SDK,microsoft
　　【2】fast,Minimun Storage Ray/Triangle Intersection,Tomas Moler,Ben Trumbore

?BY

克莱姆（Cramer）法则
一、线性方程组
　　元线性方程组是指形式为：
　　　　　　　　（1）
的方程组，其中代表个未知量，是方程的个数，， ; 称为方程组的系数，称为常数项。
　　线性方程组的一个解是指由个数组成的有序数组， 当个未知量分别用代入后，式（1）中每个等式都成为恒等式。方程组（1）的解的全体称为它的解集合，如果两个线性方程组有相同的解集合，就称它们是同解方程组。
　　为了求解一个线性方程组，必须讨论以下一些问题：
　　(1).这个方程组有没有解？
　　(2).如果这个方程组有解，有多少个解？
　　(3).在方程组有解时,解之间的关系,并求出全部解。
　　本节讨论方程的个数与未知量的个数相等(即)的情形。

二、克莱姆法则

　　定理1（克莱姆法则）如果线性方程组
　　　　　 （2）
的系数行列式：
　　　　
那么这个方程组有解，并且解是唯一的，这个解可表示成：
　　　　　　　　（3）
其中是把中第列换成常数项所得的行列式，即
　　　　。
　　分析：定理一共有3个结论：方程组有解；解是唯一的；解由公式（3）给出。因此证明的步骤是：
　　第一，把 代入方程组，验证它确实是解。这样就证明了方程组有解，并且（3）是一个解，即证明了结论与。
　　第二，证明如果是方程组（２）的一个解，那么一定有。这就证明了解的唯一性，即证明了结论。
　　证明：先回忆行列式的一个性质，设阶行列式，则有：
　
接下来证明定理。首先，证明（3）确实是（2）的解。将行列式按第列展开得：
　　　　，
其中是行列式中元素的代数余子式。现把
代入第个方程的左端，得：




这说明将（3）代入第个方程后，得到了一个恒等式，所以（3）是（2）的一个解。
　　其次，设是方程组（2）的一个解，那么，将代入（2）后，得到个恒等式：
　　　　　　　　　　（4）
用系数行列式的第列的代数余子式依次去乘（4）中个恒等式，得到：
　　　　
将此个等式相加，得：

从而有：。这就是说，如果是方程组（2）的一个解，那么一定有，所以方程组只有一个解。

三、齐次线性方程组
　　在线性方程组中，有一种特殊的线性方程组，即常数项全为零的方程组，称为齐次线性方程组。显然，齐次线性方程组总是有解的，因为就是它的解，这个解称为零解；其他的，即不全为零的解（如果还有的话），称为非零解。所以，对于齐次线性方程组，需要讨论的问题，不是有没有解，而是有没有非零解。这个问题与齐次线性方程组解的个数是有密切关系的。如果一个齐次线性方程组只有零解，那么这个方程组就只有唯一解；反之， 如果某个齐次线性方程组有唯一解， 那么由于零解是一个解，所以这个方程组不可能有非零解。
　　对于方程个数与未知量个数相同的齐次线性方程组，应用克莱姆法则，有 
　　推论1? 如果齐次线性方程组
　　　　　　　　（5）
的系数行列式不等于零，那么（5）只有零解。
　　推论2　齐次线性方程组
　　　　
有非零解的必要条件是它的系数行列式等于零。

四、例子
　　例1　解线性方程组
　　　　
　　解：方程组的系数行列式：
　　　　
所以根据克莱姆法则，这个线性方程组有唯一解。又因
　
　
所以这个线性方程组的唯一解为：
　　　　
　　例2　解线性方程组
　　　　
　　解：方程组的系数行列式：
　　　　
所以根据克莱姆法则，这个线性方程组有唯一解。又因
　　　
　　　
所以这个线性方和组的唯一解为：
　　　　
　　例3???????? 已知三次曲线在四个点处的值分别为：，试求其系数。
　　解：将三次曲线在4点处的值代入其方程，得到关于的线性方程组：
　　　　
它的系数行列式是范德蒙行列式：
　　　　
所以根据克莱姆法则，这个线性方程组有唯一解。又因
　　　
　　　
所以，即所求的三次曲线方程为。
　　例4　如果齐次线性方程组
　　　　
有非零解，那么必须满足什么条件？
　　解：由克莱姆法则知，齐次线性方程组有非零解的必要条件是其系数行列式等于零，因此有
　　　　
又由：，从而必须满足的条件为。
　　注　用克莱姆法则求解系数行列式不等于零的元非齐次线性方程组，需要计算个阶行列式，它的计算工作量很大。实际上关于数字系数的线性方程组（包括系数行列式等于零及方程个数和未知量个数不相同的线性方程组）的解法，一般都采用后续章节介绍的方法来求解。克莱姆法则主要是在理论上具有重要的意义，特别是它明确地揭示了方程组的解和系数之间的关系。 