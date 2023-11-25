---
title: 灯光(Lighting)
mathjax: true
date: 2019-04-11 15:03:36
tags:
- RHI
- DirectX
categories: 
- RHI
- DirectX
---
# 灯光的组成
在Direct3D灯光模型中，灯光是通过灯光资源的三个成员之一来照射的，即有三种灯光。
* 环境光（Ambient Light）——这种类型的灯光将被其他所有表面反射且被用在照亮整个场景。例如，物体的各部分都被照亮，对于一个角度，甚至穿过不在光源直接照射的地方他们都能被照亮。环境光的使用是粗略的，便宜的，它模仿反射光。
* 漫反射（Diffuse Reflection）——这种灯光按照特殊方向传播。当它照射到一个表面，它将在所有方向上均匀的反射。因为漫射光在所有方向上都均匀的反射，被反射的光线将到达眼睛而与观察点无关，因此我们不必为观察者考虑。因而，漫射光仅仅需要考虑灯光方向和表面的姿态。这种灯光将成为你的资源中照射的普通灯光。
* 镜面反射（Specular Reflection）——这种灯光按照特殊方向传播。当它照射到一个表面时，它严格地按照一个方向反射。这将产生一个明亮的光泽，它能在某角度被看见。因为这种灯光在一个方向反射。明显的观察点，必须考虑灯光的方向和表面姿态，且必须按照镜面灯光等式来考虑。镜面灯光被用在物体上产生高光的地方，这种光泽只有在灯光照射在磨光的表面上才会产生。
　　镜面光比其他灯光类型要求更多的计算；因此，Direct3D提供了一个开关选择。实际上，它默认是被关闭的；要使用镜面光你必须设置D3DRS_SPECULARENABLE渲染状态。
```c
Device->SetRenderState(D3DRS_SPECULARENABLE, true);
```
每一种灯光都是通过D3DCOLORVALUE结构或者描述灯光颜色的D3DXCOLOR来描绘的。这里有几个灯光颜色的例子：
```c
D3DXCOLOR redAmbient(1.0f, 0.0f, 0.0f, 1.0f);
D3DXCOLOR blueDiffuse(0.0f, 0.0f, 1.0f, 1.0f);
D3DXCOLOR whiteSpecular(1.0f, 1.0f, 1.0f, 1.0f);
```
注意：在D3DXCOLOR类中的alpha值用在描述灯光颜色时是被忽略的。
## 材质
在现实世界中我们看到的物体颜色将由物体反射回来的灯光颜色来决定。比如，一个红色的球是红色的，因为它吸收所有的灯光颜色除了红色光。红色光是被球反射回来进入我们眼睛的，因此我们看到的球是红色的。Direct3D通过我们定义的物体材质来模拟这些所有的现象。材质允许我们定义表面反射灯光的百分比。在代码中通过D3DMATERIAL9结构描述一个材质。
```c++
typedef struct _D3DMATERIAL9 {
    D3DCOLORVALUE Diffuse, Ambient, Specular, Emissive;
    float Power;
} D3DMATERIAL9;
```
* Diffuse——指定此表面反射的漫射光数量。
* Ambient——指定此表面反射的环境光数量。
* Specular——指定此表面反射的镜面光数量
* Emissive——这个是被用来给表面添加颜色，它使得物体看起来就象是它自己发出的光一样。
* Power——指定锐利的镜面高光；它的值是高光的锐利值。
　　举例，想得到一个红色的球。我们将定义球的材质来只反射红光吸收其他颜色的所有光：
```c++
D3DMATERIAL9 red;
::ZeroMemory(&red, sizeof(red));
red.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // red
red.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // red
red.Specular = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // red
red.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // no emission
red.Power = 5.0f;
```
　　这里我们设置绿色和蓝色的值为0，这表明材质反射0%此颜色的光。我们设置红色为1，表示材质反射100%的红光。注意，我们能够控制每种灯光反射的颜色（环境、漫射和镜面光）。
　　同样假如我们定义一个只发出蓝色光的光源，对球的光照将失败因为蓝色光将被全部吸收而没有红光被反射。当物体吸收了所有光以后，物体看起来就为黑色。同样的，当物体反射100%的红、绿和蓝光，物体就将呈现为白色。
　　因为手工填充一个材质结构将是乏味的工作，我们添加下列有用的函数和全局材质常数到d3dUtility.h/cpp文件中：
```c++
D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d,
D3DXCOLOR s, D3DXCOLOR e, float p)
{
    D3DMATERIAL9 mtrl;
    mtrl.Ambient = a;
    mtrl.Diffuse = d;
    mtrl.Specular = s;
    mtrl.Emissive = e;
    mtrl.Power = p;
    return mtrl;
}
namespace d3d
{
    D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

    const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 8.0f);

    const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 8.0f);

    const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 8.0f);

    const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 8.0f);

    const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 8.0f);
}
```
顶点结构没有材质属性；一个通用的材质必须被设置。设置它我们使用IDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9 * pMaterial)方法。
假设我们想渲染几个不同材质的物体；我们将按照如下的写法去做：
```c
D3DMATERIAL9 blueMaterial, redMaterial;
// set up material structures

Device->SetMaterial(&blueMaterial);
drawSphere(); // blue sphere

Device->SetMaterial(&redMaterial);
drawSphere(); // red sphere
```
# 顶点法线
    面法线（face normal）是描述多边形表面方向的一个向量（如图5.1）。

图5.1
　　顶点法线（Vertex normals）也是基于同样的概念，但是我们与其指定每个多边形的法线，还不如为每个顶点指定（如图5.2）。
　　
　　图5.2
　　Direct3D需要知道顶点法线以便它能够确定灯光照射到物体表面的角度，并且一旦计算了每个顶点的灯光，Direct3D需要知道每个顶点的表面方向。注意顶点法线不一定和面法线相同。球体/环形物就是很好的实物例子，它们的顶点法线和三角形法线是不相同的（如图5.3）。
　　
　　图5.3
为了描述顶点的顶点法线，我们必须更新原来的顶点结构
```c++
struct Vertex
{
    float _x, _y, _z;
    float _nx, _ny, _nz;
    static const DWORD FVF;
}
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
```
注意，我们已经将上一章中使用的颜色成分去除了。这是因为我们将使用灯光来计算顶点的颜色。
　　作为一个简单的物体比如立方体和球体，我们能够通过观察看见顶点法线。对于更多复杂的网格，我们需要一个更多的机械方法。假设一个由p0,p1,p2构成的三角形，我们需要计算每个顶点的法线n0,n1,n2。
　　简单的步骤，我们列举它是为了找到由三个点构成的三角形的面法线，同时使用面法线作为顶点法线。首先计算三角形上的两个向量：
　　
　　那么面法线是：
　　
　　每个顶点的法线和面法线是相等的：
　　
　　下面是一个C函数，它通过三角形的三个顶点计算三角形的面法线。注意这个函数的三个顶点是按照顺时针方向指定的。假如不是这样，那么法线方向将是相反的。
```c++
void ComputeNormal(D3DXVECTOR3* p0,
                D3DXVECTOR3* p1,
                D3DXVECTOR3* p2,
                D3DXVECTOR3* out)
{
    D3DXVECTOR3 u = *p1 - *p0;
    D3DXVECTOR3 v = *p2 - *p0;
    D3DXVec3Cross(out, &u, &v);
    D3DXVec3Normalize(out, out);
}
```
当用三角形近似表示曲面时，使用面法线作为顶点法线不能表现一个平滑的结果。一个更好的方法是找到顶点法线的平均法线。为了找到顶点v的顶点法线vn，我们找到网格模型中所有三角形的面法线记为顶点v。vn是通过计算他们的平均面法线得到的。这里有一个例子，假设有3个三角形它们的面法线分别是n0，n1，n2，指定为顶点v。那么vn的平均法线就是：
　　
通过改变“舞台”，把顶点法线变为non-normal,这是有可能的。因此这样最好是安全的且在通过D3DRS_NORMALIZENORMALS设置渲染状态来改变“舞台”后，Direct3D从新规格化所有法线。
Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
5.4光源
　　Direct3D支持三种类型的光源。
* 点光源——这种光源在世界坐标中有一个位置且向所有方向上都照射光线。

图5.4
* 方向光源——这种光源没有位置但是向指定方向发出平行光线。

图5.5
* 聚光灯——这种类型的光源和手电筒的光类似；它有位置并且发出的光在指定方向上按照圆锥形照射。这个圆锥形有两个角度，θ和φ。角度θ描述内圆锥，φ描述外圆锥。

图5.6
　　在代码中一个灯光资源是通过D3DLIGHT9结构来表现的。
typedef struct _D3DLIGHT9 {
    D3DLIGHTTYPE Type;
    D3DCOLORVALUE Diffuse;
    D3DCOLORVALUE Specular;
    D3DCOLORVALUE Ambient;
    D3DVECTOR Position;
    D3DVECTOR Direction;
    float Range;
    float Falloff;
    float Attenuation0;
    float Attenuation1;
    float Attenuation2;
    float Theta;
    float Phi;
} D3DLIGHT9;
* Type——定义灯光类型，我们能够使用下面三种类型之一：D3DLIGHT_POINT, D3DLIGHT_SPOT, D3DLIGHT_DIRECTIONAL
* Diffuse——此光源发出的漫射光颜色。
* Specular——此光源发出的镜面光颜色。
* Ambient——此光源发出的环境光颜色。
* Position——用一个向量来描述的光源世界坐标位置。这个值对于灯光的方向是无意义的。
* Direction——用一个向量来描述的光源世界坐标照射方向。这个值不能用在点光源上。
* Range——灯光能够传播的最大范围。这个值不能比大。且不能用于方向光源。
* Falloff——这个值只能用在聚光灯上。它定义灯光在从内圆锥到外圆锥之间的强度衰减。它的值通常设置为1.0f。
* Attenuation0, Attenuation1, Attenuation2——这些衰减变量被用来定义灯光强度的传播距离衰减。它们只被用于点光源和聚光灯上。Attenuation0定义恒定衰减，Attenuation1定义线性衰减，Attenuation2定义二次衰减。适当的使用这个公式，D是代表到光源的距离，A0,A1,A2与Attenuation0，1，2相匹配。

* Theta——只用于聚光灯；指定内圆锥的角度，单位是弧度。
* Phi——只用于聚光灯；指定外圆锥的角度，单位是弧度。
　　就象初始化D3DMATERIAL9结构一样，初始化D3DLIGHT9结构是一件单调乏味的工作。我们添加下面的函数到d3dUtility.h/cpp文件中用于初始化简单灯光。
namespace d3d
{
    … …
    D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);

    D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);

    D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);
}
　　使用这些函数是非常简单的。我们现在只是演示怎样使用InitDirectionalLight。其他的也很类似：
D3DLIGHT9 d3d::InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
    D3DLIGHT9 light;
    ::ZeroMemory(&light, sizeof(light));
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Ambient = *color * 0.4f;
    light.Diffuse = *color;
    light.Specular = *color * 0.6f;
    light.Direction = *direction;
    return light;
}
　　然后创建一个方向光源，它沿着x轴正方向照射白色灯光。我们按照下面的方法来做：
D3DXVECTOR3 dir(1.0f, 0.0f, 0.0f);
D3DXCOLOR c = d3d::WHITE;
D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);
　　在把D3DLIGHT9初始化好以后，我们需要用Direct3D内在支持的灯光来注册。就象这样做：
Device->SetLight(
    0, // element in the light list to set, range is 0-maxlights
    &light);// address of the D3DLIGHT9 structure to set
　　一旦灯光注册了，我们就能使用下面的列举的例子来开或关灯光了： 
Device->LightEnable(
    0, // the element in the light list to enable/disable
    true); // true = enable, false = disable
5.5实例程序：灯光
    这一章的例子是创建如图5.7所显示的场景。它示范了怎样指定顶点法线，怎样创建材质，以及怎样创建和使用一个方向灯光。注意在这个示例程序中我们不会使用在文件d3dUtility.h/cpp中的材质和灯光函数。因为我们想展示怎样手动来做这些设置。

图5.7
    给场景增加灯光的步骤是：
1、 允许使用灯光。
2、 为每个物体创建材质并且在渲染相应物体前应将材质附予物体。
3、 创建一个或多个光源，设置它们，把它们设为可用。
4、 将其他附加光源设为可用，比如镜面高光。
　　首先我们初始化一个全局顶点缓存用他来存储“金字塔”的顶点：
IDirect3DVertexBuffer9* Pyramid = 0;
　　Setup函数包含本章的所有代码，因此我们忽略其他函数。它执行刚才讨论的步骤来给场景加入灯光。Setup方法首先允许使用灯光，当然这不是必须的因为默认设置就是允许使用灯光的。
bool Setup()
{
    Device->SetRenderState(D3DRS_LIGHTING, true);
　　下一步，我们创建顶点缓存，锁定，并且把“金字塔”的三角形顶点放入其中。顶点法线是利用5.3节中的运算法则预先计算好的。注意三角形共享顶点，但它们的法线不能共享；因此对这个物体使用索引列表并不是最有利的。例如，所有三角形都共享顶点（0,1,0）；然而，对每个三角形，它们的顶点法线是不相同的。
```C++
    Device->CreateVertexBuffer(
                12 * sizeof(Vertex),
                D3DUSAGE_WRITEONLY,
                Vertex::FVF,
                D3DPOOL_MANAGED,
                &Pyramid,
                0);
    // fill the vertex buffer with pyramid data
    Vertex* v;
    Pyramid->Lock(0, 0, (void**)&v, 0);

    // front face
    v[0] = Vertex(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
    v[1] = Vertex( 0.0f, 1.0f, 0.0f, 0.0f, 0.707f, -0.707f);
    v[2] = Vertex( 1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
    
    // left face
    v[3] = Vertex(-1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f);
    v[4] = Vertex( 0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f);
    v[5] = Vertex(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);

    // right face
    v[6] = Vertex( 1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
    v[7] = Vertex( 0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f);
    v[8] = Vertex( 1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f);

    // back face
    v[9] = Vertex( 1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
    v[10] = Vertex( 0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
    v[11] = Vertex(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);

    Pyramid->Unlock();
```
　　为物体产生了顶点数据以后，我们描述利用灯光表现各自材质的物体间是怎样相互影响的。在这个例子中，“金字塔”反射出白光，自身不发光，且会产生一些高光。
D3DMATERIAL9 mtrl;
mtrl.Ambient = d3d::WHITE;
mtrl.Diffuse = d3d::WHITE;
mtrl.Specular = d3d::WHITE;
mtrl.Emissive = d3d::BLACK;
mtrl.Power = 5.0f;
Device->SetMaterial(&mtrl);
　　接着，我们创建一个方向光并将其设为可用。方向光是沿着x轴的正方向照射的。灯光照射最强的白色漫射光（dir.Diffuse = WHITE），较弱的白色镜面光（dir.Specular = WHITE * 0.3f）以及一个中等强度的白色环境光（dir.Ambient = WHITE *0.6f）。
```c++
D3DLIGHT9 dir;
::ZeroMemory(&dir, sizeof(dir));
dir.Type = D3DLIGHT_DIRECTIONAL;
dir.Diffuse = d3d::WHITE;
dir.Specular = d3d::WHITE * 0.3f;
dir.Ambient = d3d::WHITE * 0.6f;
dir.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
Device->SetLight(0, &dir);
Device->LightEnable(0, true);
//最后，我们设置状态使法线从新规格化且把镜面高光设置为可用。
Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
Device->SetRenderState(D3DRS_SPECULARENABLE, true);
// ... code to set up the view matrix and projection matrix
// omitted
return true;
```
5.6附加实例
　　这一章中还有三个附加的例子。它们使用D3DXCreate*函数来创建组成场景的3D物体。D3DXCreate*函数创建的顶点数据是D3DFVF_XYZ | D3DFVF_NORMAL格式。在增加的函数中为我们的网格模型的每个顶点计算了顶点法线。这些实例演示了怎样使用方向光，点光源，以及聚光灯。图5.8显示的是方向光实例中的一个场景图。
　　
　　图5.8

