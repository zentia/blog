---
title: Drawing in Direct3D
mathjax: true
date: 2019-04-11 14:55:06
tags:
- RHI
- DirectX
categories: 
- RHI
- DirectX
---
# 顶点/索引缓存
顶点和索引缓存有相似的接口并且共享相似的方法；一个顶点缓存是一块连续的存储了顶点数据的内存。同样的，一个索引缓存是一块连续的存储了索引数据的内存。我们使用顶点和索引缓存保存我们的数据是因为它们能被放置在显存中。渲染显存中的数据要比渲染系统内存中的数据快的多。
在代码中，一个顶点缓存是通过IDirect3DVertexBuffer9接口来定义的。类似的，一个索引缓存是通过IDirect3DIndexBuffer9接口来定义。
## 创建一个顶点和索引缓存
我们能使用下面两个方法创建一个顶点缓存和索引缓存：
```C++
HRESULT IDirect3DDevice9::CreateVertexBuffer(
    UINT Length,
    DWORD Usage,
    DWORD FVF,
    D3DPOOL Pool
    IDirect3DVertexBuffer9** ppVertexBuffer,
    HANDLE* pSharedHandle
);

HRESULT IDirect3DDevice9::CreateIndexBuffer(
    UINT Length,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DIndexBuffer9** ppIndexBuffer,
    HANDLE* pSharedHandle
);
```
这两个方法大部分参数是相同的，因此我们一起介绍它们。
* Length —— 分配给缓存的字节大小。假如想得到一个能存储8个顶点的顶点缓存，那么我们就要在顶点结构中设置这个参数为 8 * sizeof ( Vertex ) 。
* Usage —— 指定关于怎样使用缓存的额外信息。这个值可以是0，没有标记，或者是下面标记的一个或多个的组合：
* D3DUSAGE_DYNAMIC——设置这个参数可以使缓存是动态的。在下一页说明静态和动态缓存。
* D3DUSAGE_POINTS——这个参数指定缓存存储原始点。原始点将在第14章粒子系统中介绍。这个参数仅仅用在顶点缓冲中。
* D3DUSAGE_SOFTWAREPROCESSING——使用软件顶点处理
* D3DUSAGE_WRITEONLY——指定应用程序只能写缓存。它允许驱动程序分配最适合的内存地址作为写缓存。注意如果从创建好的这种缓存中读数据，将会返回错误信息。
* FVF —— 存储在缓存中的顶点格式
* Pool —— 缓存放置在哪一个内存池中
* ppVertexBuffer ——返回创建好的顶点缓存的指针。
* pSharedHandle ——没有使用；设置为0。
* Format ——指定索引的大小；使用D3DFMT_INDEX16设置16位索引，使用D3DFMT_INDEX32设置32位索引。注意并非所有设备都支持32位索引；请检查设备能力。
* ppIndexBuffer ——返回创建好的索引缓存的指针。

注意：不使用D3DUSAGE_DYNAMIC参数创建的缓存被叫做静态缓存。静态缓存通常被放置在显存中，在其中的数据能被很有效的处理。然而，对于静态缓存，从中读取和写入数据是很慢的，因为访问显存是很慢的。因为这个原因我们用静态缓存存储静态数据（不需要被经常改变的数据）。对于静态缓存地形和建筑物是很好的后选例子，因为在应用程序中他们通常不需要被改变。静态缓存应该在应用程序初始话的时候就被填充好，而不是在运行时才做。

注意：使用D3DUSAGE_DYNAMIC参数创建的缓存被叫做动态缓存。动态缓存通常被放在AGP内存中，这种内存中的数据能被很快的更新。处理动态缓存中的数据不会比处理静态缓存中的数据快，因为这些数据必须在渲染前被转移到显存中，动态缓存的好处是它们能够被稍微快点地被更新（比CPU写快）。因此，假如你需要经常更新缓存中的数据，那么你就应该使用动态缓存。对于动态缓存粒子系统是很好的一个应用，因为它们是动态的，并且他们通常每一帧都会被更新。

注意：在程序中读取显存和AGP内存都是非常慢的。因此，假如你在运行时需要读取你的几何物体，最好的方案是指定一块系统内存，都在其中拷贝并且读取数据。
　　  下边是创建一个静态顶点缓存的例子，该缓存能存储8个顶点。
```C++
IDirect3DVertexBuffer9* vb;
device->CreateVertexBuffer(
    8 * sizeof( Vertex ),
    0,
    D3DFVF_XYZ,
    D3DPOOL_MANAGED,
    &vb,
    0);
```
## 访问缓冲内存
　　为了访问一个顶点/索引缓存，我们需要得到一个指针。我们通过一个指针获得缓存数据必须使用Lock方法。当我们访问完缓存后必须对它解锁。一旦有一个指向内存的指针，我们就能对它进行读写。
```C++
HRESULT IDirect3DVertexBuffer9::Lock(
    UINT OffsetToLock,
    UINT SizeToLock,
    BYTE** ppbData,
    DWORD Flags
);
HRESULT IDirect3DIndexBuffer9::Lock(
    UINT OffsetToLock,
    UINT SizeToLock,
    BYTE** ppbData,
    DWORD Flags
);
```
　　图3.1
　　这两个方法的参数都是完全相同的。
* OffsetToLock —— 偏移量，以字节为单位，从缓存开始位置到锁定开始位置的距离。如图3.1。
* SizeToLock —— 锁定的字节数。
* ppbData —— 一个指向锁定内存开始位置的指针。
* Flags —— 标记描述怎样锁定内存。它可能是0或者是下面参数中的1个或多个的组合：
* D3DLOCK_DISCARD——这个参数仅仅会在动态缓存时被使用。它指示硬件丢弃缓存并返回一个指向新分配的缓存的指针。这是很有用，因为当我们存取一个新分配的缓存时它允许硬件继续从丢弃的缓存渲染。这防止了硬件延迟。
* D3DLOCK_NOOVERWRITE——这个参数仅仅会在动态缓存时被使用。它声明你将向缓存中添加数据。即，你不能向已经渲染的内存中写数据。这是有好处的因为他允许你在添加新数据到缓存的同时让硬件继续渲染。
* D3DLOCK_READONLY——这个参数声明你锁定的缓存只能从中读取数据而不能写数据。这允许一些内在的优化。
    用参数D3DLOCK_DISCARD和D3DLOCK_NOOVERWRITE的地址实际上就是缓存的一部分被使用（正在渲染）时它被锁定。假如情况允许这些标记被使用，当在锁定时他们防止渲染停止。
    下边的例子展示了通常怎样使用Lock方法。注意当我们使用完以后要调用Unlock方法。
Vertex* vertices;
_vb->Lock(0, 0, (void**)&vertices, 0); // 锁定整个缓存
vertices[0] = Vertex(-1.0f, 0.0f, 2.0f); // 向缓存里写顶点
vertices[1] = Vertex( 0.0f, 1.0f, 2.0f); 
vertices[2] = Vertex( 1.0f, 0.0f, 2.0f);
_vb->Unlock(); // 当你访问完缓存时，解锁缓存
3.1.3 找回顶点和索引缓存信息
有时我们需要得到顶点/索引缓存信息。下面的例子示范了用于获得这些信息的方法：
D3DVERTEXBUFFER_DESC vbDescription;
_vertexBuffer->GetDesc(&vbDescription); // 取得顶点缓存信息

D3DINDEXBUFFER_DESC ibDescription;
_indexBuffer->GetDesc(&ibDescription); //取得索引缓存信息
　　
　　D3DVERTEXBUFFER_DESC和D3DINDEXBUFFER_DESC结构的定义如下：
typedef struct _D3DVERTEXBUFFER_DESC {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;
    DWORD FVF;
} D3DVERTEXBUFFER_DESC;

typedef struct _D3DINDEXBUFFER_DESC {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;
} D3DINDEXBUFFER_DESC;
3.2 渲染状态
   Direct3D提供了多种渲染状态，它影响几何物体怎样被渲染。渲染状态有默认值，因此假如你的应用程序需要不同于默认设置的渲染时，你仅仅改变它即可。一种渲染效果会一直起作用，直到你下一次改变渲染状态为止。为了设置一个渲染状态，我们使用下面的方法：
HRESULT IDirect3DDevice9::SetRenderState(
    D3DRENDERSTATETYPE State, // 更改的渲染状态
    DWORD Value // 新的状态值
);

例如，在这一章的例子中我们将使用线框模式渲染我们的物体。因此，我们设置如下的渲染状态：
_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
注意：查看DirectX SDK中关于D3DRENDERSTATETYPE的信息。其中详细介绍了所有的渲染状态。
3.3 绘制准备
　　一旦我们创建好一个顶点缓存以及一个索引缓存（可选的）后，我们就为渲染其中的内容准备得差不多了，但是在渲染前我们还有3个步骤必须先做。
1、 设置资源流。设置资源流与一个顶点缓存挂钩，此流就是一个流入渲染管线的几何信息的流。
　　下面的方法是用于设置一个资源流：
HRESULT IDirect3DDevice9::SetStreamSource(
    UINT StreamNumber,
    IDirect3DVertexBuffer9* pStreamData,
    UINT OffsetInBytes,
    UINT Stride
);
* StreamNumber——确定我们的顶点缓存与哪一个资源流挂钩。在这本书中我们不使用多重流；因此我们总是使用0号流。
* pStreamData——一个指向我们想与流挂钩的那个顶点缓存的指针。
* OffsetInBytes——相对流开始处的偏移量。以字节为单位，它指定被填入渲染管线的顶点数据的开始位置。通过检查D3DCAPS9结构中的D3DDEVCAPS2_STREAMOFFSET标志，假如你的设备支持，那么这个参数就有一些非0值。
* Stride——我们在顶点缓存中操作的每个部分的流的字节大小。
　　例如，假设vb是一个已经填充了顶点信息的顶点缓存：
_device->SetStreamSource( 0, vb, 0, sizeof( Vertex ) );

2、 设置顶点格式。在这里我们指定后面用来绘图调用的顶点的顶点格式。
_device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );

3、 设置索引缓存。假如我们使用了索引缓存，我们必须设置后面要用于绘制操作的索引缓存。每次我们只能使用一个索引缓存；因此假如你需要用一个不同的索引缓存绘制一个物体时，你必须转换到另一个上。下面的代码设置一个索引缓存：
_device->SetIndices( _ib ); // 传递一个索引缓存指针的拷贝
3.4用顶点/索引缓存绘制
　　在我们创建好顶点/索引缓存以及做好准备工作以后，我们就能绘制我们的几何物体了。这是通过使用DrawPrimitive或者DrawIndexedPrimitive传送几何信息到达渲染管线。这些方法从顶点流中获得顶点信息以及从索引缓存中获得索引信息。
3.4.1 IDirect3DDevice9::DrawPrimitive
　　这个方法不使用索引信息绘制图元。
HRESULT IDirect3DDevice9::DrawPrimitive(
    D3DPRIMITIVETYPE PrimitiveType,
    UINT StartVertex,
    UINT PrimitiveCount
);
* PrimitiveType——我们绘制的图元类型。比如，我们能绘制点和线以及三角形。以后我们使用三角形，用D3DPT_TRIANGLELIST参数。
* StartVertex——索引到在顶点流中的一个元素。设置渲染顶点中的开始点。这个参数给予我们一定的机动性，可以绘制一个顶点缓存中的某部分。
* PrimitiveCount——绘制图元的个数。
　　例子：
// 绘制4个三角形
_device->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 4);
3.4.2 IDirect3DDevice9::DrawIndexedPrimitive
　　这个方法使用索引信息来绘制图元。
HRESULT IDirect3DDevice9::DrawIndexedPrimitive(
    D3DPRIMITIVETYPE Type,
    INT BaseVertexIndex,
    UINT MinIndex,
    UINT NumVertices,
    UINT StartIndex,
    UINT PrimitiveCount
);
* Type——我们绘制的图元类型。比如，我们能绘制点和线以及三角形。以后我们使用三角形，用D3DPT_TRIANGLELIST参数。
* BaseVertexIndex——一个基本数字，在调用中用它去加上索引。参看下面的说明。
* MinIndex——将被引用的最小索引值。
* NumVertices——在此调用中将被引用的顶点数。
* StartIndex——索引到索引缓存中的某个位置，它标记开始渲染的开始索引点。
* PrimitiveCount——绘制图元的个数。
　　例子：
_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
注意：BaseVertexIndex参数需要一些特别的解释。在解释过程中将会用到的图3.2。

图3.2
   在索引缓存中定位顶点相应的也就在顶点缓存中定位了。然而，假设我们想将球，盒子，圆柱体的顶点放置到一个公共的顶点缓存中。对于每一个物体，我们将不得不再计算在公共顶点缓存中的索引。这个新的索引值是通过与一个偏移量相加得到。注意这个偏移量是标准的顶点，而不是字节。
    我们需要计算物体在公共顶点缓存中的索引值。Direct3D允许我们通过设置BaseVertexIndex参数得到一个顶点偏移量，随后Direct3D就能利用顶点自身的索引重新计算新的索引。
3.4.3 开始/结束场景
   最后一点就是所有绘制方法都必须在IDirect3DDevice9::BeginScene和IDirect3DDevice9::EndScene方法之间被调用。例如，我们将这样写：
_device->BeginScene();
　　// 绘制场景
    _device->DrawPrimitive(...);
_device->EndScene();
3.5 D3DX几何物体
　　通过在代码中建造每个三角形来建造3D物体是一件非常枯燥的事。幸运的是，D3DX库已经为我们提供了一些方法来产生简单3D物体的网格数据。
　　D3DX库提供如下6种网格生成函数。
* D3DXCreateBox
* D3DXCreateSphere
* D3DXCreateCylinder
* D3DXCreateTeapot
* D3DXCreatePolygon
* D3DXCreateTorus

图3.3
　　这6种函数的使用都很类似，并且使用D3DX网格数据结构ID3DXMesh就象使用ID3DXBuffer接口一样。这些接口回在第10章和11章中讲解。现在，我们忽视它们的详细信息，只需简单使用它们即可。
HRESULT D3DXCreateTeapot(
    LPDIRECT3DDEVICE9 pDevice, // 与mesh关联的设备
    LPD3DXMESH* ppMesh, // 返回的mesh
    LPD3DXBUFFER* ppAdjacency // 现在设成0
);
　　
　　一个使用D3DXCreateTeapot函数的例子：
ID3DXMesh* mesh = 0;
D3DXCreateTeapot(_device, &mesh, 0);
　　一旦生成了网格数据，我们就能使用ID3DXMesh::DrawSubset方法绘制图形了。这个方法有一个参数，它用来识别网格的一个子集。这个网格是通过上面的D3DXCreate*函数中的一个子集创建的，因此可以给这个参数指定0值。一个渲染网格的例子：
_device->BeginScene();
    mesh->DrawSubset(0);
_device->EndScene();
   
   使用了网格以后，必须释放（release）它：
mesh->Release();
_mesh = 0;
3.6 实例程序：三角形、立方体、茶壶、D3DXCreate*
　　这里有4个例子。
* 三角形——这是非常简单的应用程序，它示范了在线框模式下怎样创建并渲染一个三角形。
* 立方体——只比三角形稍微复杂一点，这个程序渲染一个线框立方体。
* 茶壶——这个程序使用D3DXCreateTeapot函数创建并渲染一个纺纱茶壶。
* D3DXCreate——这个程序创建并渲染几种不同的能够使用D3DXCreate*函数创建的3D物体。
让我们简单讨论一下创建立方体的例子。通过对它的学习你自己就能很快地理解其他例子。
    这个简单的绘制和渲染立方体的程序的运行结果如图3.4。

图3.4
　　首先我们定义下边两个全局变量来保存立方体的顶点和索引数据：
IDirect3DVertexBuffer9* VB = 0;
IDirect3DIndexBuffer9* IB = 0;
　　下一步，我们定义两个全局常量，由它们来指定我们的屏幕大小：
const int Width = 800;
const int Height = 600;
　　接下来定义我们的顶点结构以及结构中顶点的格式。在这个例子中顶点结构只保存顶点的位置信息：
```C++
struct Vertex
{
    Vertex(){}
    Vertex(float x, float y, float z)
    {
        _x = x; _y = y; _z = z;
    }
    float _x, _y, _z;
    static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;
```
让我们把它迁移到框架程序（见1.53节）上。Setup函数创建顶点和索引缓存，锁定它们，把构成立方体的顶点写入顶点缓存，以及把定义立方体的三角形的索引写入索引缓存。然后把摄象机向后移动几个单位以便我们能够看见在世界坐标系中原点处被渲染的立方体。
```C++
bool Setup()
{
    // 创建顶点、索引缓存
    Device->CreateVertexBuffer(
        8 * sizeof(Vertex),
        D3DUSAGE_WRITEONLY,
        Vertex::FVF,
        D3DPOOL_MANAGED,
        &VB,
        0);

    Device->CreateIndexBuffer(
        36 * sizeof(WORD),
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16,
        D3DPOOL_MANAGED,
        &IB,
        0);

    // 向立方体的顶点缓存填充数据
    Vertex* vertices;
    VB->Lock(0, 0, (void**)&vertices, 0);

    // vertices of a unit cube
    vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
    vertices[1] = Vertex(-1.0f, 1.0f, -1.0f);
    vertices[2] = Vertex( 1.0f, 1.0f, -1.0f);
    vertices[3] = Vertex( 1.0f, -1.0f, -1.0f);
    vertices[4] = Vertex(-1.0f, -1.0f, 1.0f);
    vertices[5] = Vertex(-1.0f, 1.0f, 1.0f);
    vertices[6] = Vertex( 1.0f, 1.0f, 1.0f);
    vertices[7] = Vertex( 1.0f, -1.0f, 1.0f);

    VB->Unlock();

    // 定义立方体的三角形
    WORD* indices = 0;
    IB->Lock(0, 0, (void**)&indices, 0);

    // 前面
    indices[0] = 0; indices[1] = 1; indices[2] = 2;
    indices[3] = 0; indices[4] = 2; indices[5] = 3;

    // 背面
    indices[6] = 4; indices[7] = 6; indices[8] = 5;
    indices[9] = 4; indices[10] = 7; indices[11] = 6;

    // 左面
    indices[12] = 4; indices[13] = 5; indices[14] = 1;
    indices[15] = 4; indices[16] = 1; indices[17] = 0;

    // 右面
    indices[18] = 3; indices[19] = 2; indices[20] = 6;
    indices[21] = 3; indices[22] = 6; indices[23] = 7;

    // 顶部
    indices[24] = 1; indices[25] = 5; indices[26] = 6;
    indices[27] = 1; indices[28] = 6; indices[29] = 2;

    // 底部
    indices[30] = 4; indices[31] = 0; indices[32] = 3;
    indices[33] = 4; indices[34] = 3; indices[35] = 7;

    IB->Unlock();

    // 照相机位置（视图矩阵）
    D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
    D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &position, &target, &up);

    Device->SetTransform(D3DTS_VIEW, &V);

    // 投影矩阵
    D3DXMATRIX proj;
    D3DXMatrixPerspectiveFovLH(
        &proj,
        D3DX_PI * 0.5f, // 90 - degree
        (float)Width / (float)Height,
        1.0f,
        1000.0f);
    Device->SetTransform(D3DTS_PROJECTION, &proj);

    // 渲染状态（填充模式：框架填充）
    Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    return true;
}
```
　　Display方法有两个任务；它必须更新场景并且紧接着渲染它。既然想旋转立方体，那么我们将对每一帧增加一个角度使立方体能在这一帧旋转。对于这每一帧，立方体将被旋转一个很小的角度，这样我们看起来旋转就会更平滑。接着我们使用IDirect3DDevice9::DrawIndexedPrimitive方法来绘制立方体。
```C++
bool Display(float timeDelta)
{
    if( Device )
    {
        // 旋转立方体
        D3DXMATRIX Rx, Ry;
        // x轴旋转45弧度
        D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);

        // 每一帧中增加y轴的弧度
        static float y = 0.0f;
        D3DXMatrixRotationY(&Ry, y);
        y += timeDelta;

        //当y轴旋转2周时，重新回到0弧度
        if( y >= 6.28f )
            y = 0.0f;

        // 结合x轴与y轴的旋转矩阵
        D3DXMATRIX p = Rx * Ry;
        Device->SetTransform(D3DTS_WORLD, &p);

        // 清空目标缓存和深度缓存（用0xffffffff, 1.0f）
        Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
          0xffffffff, 1.0f, 0);
        
        Device->BeginScene(); // 开始绘制场景

        Device->SetStreamSource(0, VB, 0, sizeof(Vertex)); // 设置资源流
        Device->SetIndices(IB); // 设置索引缓存
        Device->SetFVF(Vertex::FVF); // 设置顶点格式
　　　　// 利用索引缓存绘制
        Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

        Device->EndScene(); // 结束绘制场景
        Device->Present(0, 0, 0, 0); // 翻转表面
    }
    return true;
}
```
　　最后，我们释放使用过的所有内存。这意味着释放顶点和索引缓存接口：
```c++
void Cleanup()
{
    d3d::Release<IDirect3DVertexBuffer9*>(VB);
    d3d::Release<IDirect3DIndexBuffer9*>(IB);
}
```


