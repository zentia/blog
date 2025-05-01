---
title: 第十章 网格模型I(Meshes Part I)
mathjax: true
date: 2019-04-11 15:14:46
tags:
- RHI
- DirectX
categories:
- RHI
- DirectX
---
# 几何信息
ID3DXBaseMesh接口包含一个用来存储网格顶点的顶点缓存和一个用来定义这些顶点怎样连接在一起组成网格三角形的索引缓存。我们能够通过使用下面的方法来得到这些缓存的指针：
```c++
HRESULT ID3DXMesh::GetVertexBuffer(LPDIRECT3DVERTEXBUFFER9* ppVB);
HRESULT ID3DXMesh::GetIndexBuffer(LPDIRECT3DINDEXBUFFER9* ppIB);
```
这里有一些使用这些方法的例子：
```c++
IDirect3DVertexBuffer9* vb = 0;
Mesh->GetVertexBuffer( &vb );
IDirect3DIndexBuffer9* ib = 0;
Mesh->GetIndexBuffer( &ib );
```
假如想锁定这些缓存来读写数据，那么我们能够使用下面的方法。注意这些方法锁定整个顶点/索引缓存。
HRESULT ID3DXMesh::LockVertexBuffer(DWORD Flags, BYTE** ppData);
HRESULT ID3DXMesh::LockIndexBuffer(DWORD Flags, BYTE** ppData);
Flags参数描述怎样锁定它。这些Flags参数在第三章中我们介绍过。ppData是函数返回的指向锁定内存的指针的地址。
　　当然在你锁定以后一定要记得解锁：
HRESULT ID3DXMesh::UnlockVertexBuffer();
HRESULT ID3DXMesh::UnlockIndexBuffer();
　　下面是另外一些与mesh几何结构有关的ID3DXMesh接口方法：
* DWORD GetFVF() — 返回顶点的格式
* DWORD GetNumVertices() — 返回顶点缓存中的顶点数
* DWORD GetNumBytesPerVertex() — 返回一个顶点所占的字节数
* DWORD GetNumFaces() — 返回在mesh中的面（三角形）数
10.2 子集和属性缓存
　　一个mesh由一个或数个子集组成。一个子集（subset）是在mesh中的使用相同属性渲染的一组三角形。这里的属性是指材质，纹理和渲染状态。图10.1显示了一座房子mesh可能被分成的几个子集。
　　
图10.1
我们通过给每个子集指定一个唯一非负整数来标识子集。这个值可以是存储在一个DWORD中的任意数值。例如，在图10.1中我们用0，1，2和3来标识子集。
　　在mesh中的每个三角形都与一个属性ID相关联，表示该三角形属于该子集。例如，图10.1中组成地板的三角形具有属性ID0，它表示这些三角形属于子集0。同样，组成墙的三角形具有属性ID1，它表示这些三角形属于子集1。
　　三角形的属性ID存储在mesh的属性缓存中，它是一个DWORD数组。因为每个面对应属性缓存中的一项，所以属性缓存中的项目数等于mesh中的面的个数。属性缓存中的项目和索引缓存中定义的三角形一一对应。即，属性缓存中的第i项和索引缓存中的第i个三角形相对应。三角形i由下面三个索引缓存中的索引项定义：
　　A = i * 3
　　B = i * 3 + 1
　　C = i * 3 + 2
图10.2显示了这个对应关系：

图10.2
　　我们可以锁定属性缓存，就象下面的代码片段：
DWORD* buffer = 0;
Mesh->LockAttributeBuffer(lockingFlags, &buffer);
// Read or write to attribute buffer...
Mesh->UnlockAttributeBuffer();
10.3 绘制
　　ID3DXMesh接口提供了DrawSubset（DWORD AttribId）方法来绘制AttribId指示的子集中的各个三角形。例如，要绘制子集0中的所有三角形，我们将这样写：
Mesh->DrawSubset(0);
　　为了绘制整个mesh，我们必须绘制mesh的所有子集。这是非常方便的用0，1，2，…，n-1来标识子集，这里的n是子集的总数。且有一个相对应的材质和纹理数组，即子集i与材质和纹理数组的第i项对应。这就使我们能够简单的用循环来渲染mesh：
```C++
for(int i = 0; i < numSubsets; i++)
{
    Device->SetMaterial( mtrls[i] );
    Device->SetTexture( 0, textures[i] );
    Mesh->DrawSubset(i);
}
```
10.4 优化
　　Mesh的顶点和索引能够被重组以便能更有效的渲染mesh。当我们这样做时，我们说我们优化了一个mesh。我们可以使用下面的方法来进行优化：
HRESULT ID3DXMesh::OptimizeInplace(
    DWORD Flags,
    CONST DWORD* pAdjacencyIn,
    DWORD* pAdjacencyOut,
    DWORD* pFaceRemap,
    LPD3DXBUFFER* ppVertexRemap
);
* Flags — 表示执行什么类型的优化方法。它可以是下面的一个或几个的组合：
* D3DXMESHOPT_COMPACT — 从mesh中移除没有用的顶点和索引项。
* D3DXMESHOPT_ATTRSORT — 根据属性给三角形排序并调整属性表，这将使DrawSubset执行更有效（参见10.5节）。
* D3DXMESHOPT_VERTEXCACHE — 增加顶点缓存的命中率。
* D3DXMESHOPT_STRIPREORDER — 重组顶点索引使三角带尽可能的长。
* D3DXMESHOPT_IGNOREVERTS — 只优化索引信息；忽略顶点信息。
注意：D3DXMESHOPT_VERTEXCACHE和D3DXMESHOPT_STRIPREORDER不能同时使用。
* pAdjacencyIn — 指向没有优化的mesh的邻接数组。
* pAdjacencyOut — 指向一个DWORD数组，它被用来填充优化好了的mesh邻接信息。该数组必须有ID3DXMesh::GetNumFaces() * 3个元素。如果不需要该信息，可以将其设置为0。
* pFaceRemap —指向一个DWORD数组，它被用来填充面重影射信息。该数组必须不小于ID3DXMesh::GetNumFaces()。当一个mesh被优化时，由索引缓存定义的面可能被移动；也就是说，在pFaceRemap中的第i项表示第i个原始面被移动到的面索引值。如果不需要该信息，可以将其设置为0。
* ppVertexRemap — 指向ID3DXBuffer指针的地址（参见11.1节），它被用来填充顶点重影射信息。这个缓存应该包含ID3DXMesh::GetNumVertices()个顶点。当一个mesh被优化后，顶点可能被移动。顶点重影射信息用来说明原来的顶点被移动到新位置；也就是说，在ppVertexRemap中的第i项表示原来的第i个顶点的新位置。如果不需要该信息，可以将其设置为0。 
　　例子：
```C++
// Get the adjacency info of the non-optimized mesh.
DWORD adjacencyInfo[Mesh->GetNumFaces() * 3];
Mesh->GenerateAdjacency(0.0f, adjacencyInfo);

// Array to hold optimized adjacency info.
DWORD optimizedAdjacencyInfo[Mesh->GetNumFaces() * 3];

Mesh->OptimizeInplace(
    D3DXMESHOPT_ATTRSORT |
    D3DXMESHOPT_COMPACT |
    D3DXMESHOPT_VERTEXCACHE,
    adjacencyInfo,
    optimizedAdjacencyInfo,
    0,
     0);
　　一个更简单的方法是Optimize方法，它输出一个优化的mesh，而不是在原来mesh的基础上进行优化：
HRESULT ID3DXMesh::Optimize(
    DWORD Flags,
    CONST DWORD* pAdjacencyIn,
    DWORD* pAdjacencyOut,
    DWORD* pFaceRemap,
    LPD3DXBUFFER* ppVertexRemap,
    LPD3DXMESH* ppOptMesh // the optimized mesh to be output
);
```
10.5 属性表
　　当一个mesh被使用D3DXMESHOPT_ATTRSORT参数来优化后，mesh的几何信息将按照属性进行排序，这样各个子集的顶点/索引将组成连续的块（如图10.3）。

图10.3
　　除了进行几何信息的排序外，D3DXMESHOPT_ATTRSORT优化项还将创建一个属性表。该表是D3DXATTRIBUTERANGE结构的一个数组。在属性表中的每一项对应mesh的一个子集并指示顶点/索引缓存中的一个连续连续内存块，这个子集的几何信息就包含在这个块中。D3DXATTRIBUTERANGE结构的定义如下：
```C++
typedef struct _D3DXATTRIBUTERANGE {
    DWORD AttribId;
    DWORD FaceStart;
    DWORD FaceCount;
    DWORD VertexStart;
    DWORD VertexCount;
} D3DXATTRIBUTERANGE;
```
* AttribId — 子集的ID。
* FaceStart — 该子集的面的起始值，FaceStart*3就是起始三角形在索引缓存中的序号。
* FaceCount — 在子集中的面（三角形）数。
* VertexStart — 该子集的起始顶点在顶点缓存中的序号。
* VertexCount — 在子集中的顶点数。
　　我们能够很容易的明白D3DXATTRIBUTERANGE结构的各个成员，如图10.3。在图10.3中mesh的属性表有三项——它们和各个子集一一对应。
　　建立了属性表以后，渲染一个子集就很容易了。仅仅查一下属性表就能找出自己的几何信息。注意如果没有属性表，每渲染一个子集就需要对属性缓存进行一次线性搜索来找出子集包含的几何信息。
　　可以使用下面的方法来访问mesh的属性表：
HRESULT ID3DXMesh::GetAttributeTable(
    D3DXATTRIBUTERANGE* pAttribTable,
    DWORD* pAttribTableSize
);
　　这个方法能够做两件事情：它可以返回属性表的属性数，也可以用属性数据来填充一个D3DXATTRIBUTERANGE结构数组。
　　要得到属性表的元素个数，可以就将第一个参数设置为0：
DWORD numSubsets = 0;
Mesh->GetAttributeTable(0, &numSubsets);
　　一旦我们知道了属性表的元素个数，我们就能够通过写属性表来填充一个D3DXATTRIBUTERANGE结构数组：
D3DXATTRIBUTERANGE table = new D3DXATTRIBUTERANGE [numSubsets];
Mesh->GetAttributeTable( table, &numSubsets );
　　我们能够使用ID3DXMesh::SetAttributeTable方法来直接设置属性表。下面的代码就是设置一个有12个子集的属性表：
D3DXATTRIBUTERANGE attributeTable[12];
// ...fill attributeTable array with data
Mesh->SetAttributeTable( attributeTable, 12);
10.6 邻接信息
　　对于mesh的某些操作，如优化，有必要了解的是三角形之间的邻接信息。Mesh的邻接数组存储了这些信息。
　　邻接数组是一个DWORD数组，其中的每一项对应了mesh中的一个三角形。例如，第i项对应的三角形由以下三个索引值定义：
A = i ??3
B = i ??3 + 1
C = i ??3 + 2
注意，使用ULONG_MAX = 4294967295表示该边没有邻接三角形。我们也可以用-1来表示，因为-1转换成DWORD就是ULONG_MAX。回想一下，DWORD就是一个unsigned32-bit整数。
　　因为每个三角形都有三条边，所以他就有三个邻接三角形（如图10.4）。
　　
　　图10.4
　　因此，邻接数组必须有三项（ID3DXBaseMesh::GetNumFaces()*3）—— 在mesh中每个三角形都可能有三个邻接三角形。
　　很多D3Dxmesh创造函数都能输出邻接信息，但我们也可以使用下面的方法：
HRESULT ID3DXMesh::GenerateAdjacency(
    FLOAT fEpsilon,
    DWORD* pAdjacency
);
* fEpsilon — 指示当两个点距离有多近时，可以认为是一个点。当两点间的距离小于epsilon时，可认为它们是同一个点。
* pAdjacency — 一个指向填充了邻接信息的DWORD数组指针。
　　例子：
DWORD adjacencyInfo[Mesh->GetNumFaces() * 3];
Mesh->GenerateAdjacency(0.001f, adjacencyInfo);
10.7 复制
　　有时我们需要将一个mesh中的数据拷贝到另一个之中。我们可以使用ID3DXBaseMesh::CloneMeshFVF方法。
```C++
HRESULT ID3DXMesh::CloneMeshFVF(
    DWORD Options,
    DWORD FVF,
    LPDIRECT3DDEVICE9 pDevice,
    LPD3DXMESH* ppCloneMesh
);
```
* Options — 用来创建mesh的一个或多个创建标志。要了解所有标志信息请查看sdk文档。现在列出一部分：
* D3DXMESH_32BIT — mesh使用32位索引。
* D3DXMESH_MANAGED — mesh数据将被放在受控的内存中。
* D3DXMESH_WRITEONLY — mesh数据只能执行写操作，不能执行读操作。
* D3DXMESH_DYNAMIC — mesh缓存将是动态的。
* FVF — 创建复制mesh的灵活顶点格式。
* pDevice — 与复制mesh有关的设备。
* ppCloneMesh — 输出复制的mesh。
　　注意这个方法允许指定与原mesh不同的options和FVF。例如我们有顶点格式为D3DFVF_XYZ的mesh，现在想复制一个顶点格式为D3DFVF_XYZ|D3DFVF_NORMAL的mesh。我们可以这样写：
// 假设_mesh和device是有效的
```C++
ID3DXMesh* clone = 0;
Mesh->CloneMeshFVF(
    Mesh->GetOptions(), // 使用与源模型同样的选项
    D3DFVF_XYZ | D3DFVF_NORMAL,// 指定克隆的FVF
    Device,
    &clone
);
```
10.8 创建一个Mesh（D3DXCreateMeshFVF）
　　我们可以使用D3DXCreate\*函数来创建mesh物体。然而，我们也可以使用  D3DXCreateMeshFVF函数来创建一个空mesh。所谓空mesh是指我们已经指定了顶点数和面数，函数D3DXCreateMeshFVF也分配了适当大小的内存给顶点、顶点索引、属性缓冲区。有了这些缓冲区后，就可以手动填写上下文数据了（需要分别向顶点缓存，索引缓存、属性缓存提供顶点、索引、属性数据）。
　　我们使用D3DXCreateMeshFVF函数来创建空mesh：
```C++
HRESULT D3DXCreateMeshFVF(
    DWORD NumFaces,
    DWORD NumVertices,
    DWORD Options,
    DWORD FVF,
    LPDIRECT3DDEVICE9 pDevice,
    LPD3DXMESH* ppMesh
);
```
* NumFaces — mesh将拥有的面数。该值必须大于0。
* NumVertices — mesh将拥有的顶点数。该值必须大于0。
* Options —用来创建mesh的一个或多个创建标志。要了解所有标志信息请查看sdk文档。现在列出一部分：
* D3DXMESH_32BIT — mesh使用32位索引。
* D3DXMESH_MANAGED — mesh数据将被放在受控的内存中。
* D3DXMESH_WRITEONLY — mesh数据只能执行写操作，不能执行读操作。
* D3DXMESH_DYNAMIC — mesh缓存将是动态的。
* FVF — mesh的顶点格式。
* pDevice — 与mesh相关的设备。
* ppMesh — 输出创建好的mesh。
　　下一节将给出实例程序，它演示了用这个函数怎样创建一个mesh以及手动填充mesh的数据内容。
　　另外，你也可以使用D3DXCreateMesh来创建空mesh。它的原型是：
```C++
HRESULT D3DXCreateMesh(
    DWORD NumFaces,
    DWORD NumVertices,
    DWORD Options,
    CONST LPD3DVERTEXELEMENT9* pDeclaration,
    LPDIRECT3DDEVICE9 pDevice,
    LPD3DXMESH* ppMesh
);
```
　　这些参数和D3DXCreateMeshFVF的参数是非常相似的，除了第四个。作为替代指定的FVF，我们指定一个D3DVERTEXELEMENT9结构，它描述了顶点格式。
```C++
HRESULT D3DXDeclaratorFromFVF(
    DWORD FVF, // input format
    D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE]//output format
);
```
注意：D3DVERTEXELEMENT9将在第17章中讨论。
　　这个函数通过输入一个FVF返回一个D3DVERTEXELEMENT9结构的数组。注意MAX_FVF_DECL_SIZE的定义如下：
typedef enum {
    MAX_FVF_DECL_SIZE = 18
} MAX_FVF_DECL_SIZE;
10.9 实例程序：创建和渲染Mesh
这一章的实例程序是渲染一个立方体（如图10.5）

图10.5
它演示了这一章中的大部分功能，包括如下一些操作：
* 创建一个空mesh。
* 用一个立方体几何信息来填充mesh。
* 根据mesh的每个面指定子集。
* 产生mesh的邻接信息。
* 优化mesh。
* 绘制mesh。
　　注意，我们忽略一些无关的代码来讨论本例。你能在叫做D3DXCreateMeshFVF的例子中找到全部的代码。
　　另外，为了更容易调试和研究mesh的构成，我们执行如下的函数来将内在内容放进文件中：
```C++
void dumpVertices(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpIndices(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAttributeBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAdjacencyBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh);
```
　　这些函数的名字就显示了它们的功能。执行这些函数是非常简单的，我们在这里讨论时忽略它们（可以看程序的原代码）。在这一节我们只展示一个dumpAttributeTable函数。
　　我们首先来浏览一下该例子，看看如下的一些全局变量：
```C++
ID3DXMesh* Mesh = 0;
const DWORD NumSubsets = 3;
IDirect3DTexture9* Textures[3] = {0, 0, 0};// texture for each subset
std::ofstream OutFile; // used to dump mesh data to file
```
　　这里我们定义了一个mesh对象的指针，我们以后要创建的。我们也定义了mesh拥有的子集数——三。在这个例子中，每个子集都用一个不同的纹理来渲染；纹理数组包含每个子集的纹理，如第i个纹理对应mesh的第i个子集。最后，Outfile变量被用来把mesh的内容输出为一个文本文件。
　　这个例子的大部分工作是在setup函数中进行。我们首先创建一个空的mesh：
```C++
bool Setup()
{
    HRESULT hr = 0;
    hr = D3DXCreateMeshFVF(
        12,
        24,
        D3DXMESH_MANAGED,
        Vertex::FVF,
        Device,
         &Mesh);
```
这里我们分配一个有12个面和24个顶点的mesh，这是描述一个盒子所必须的。
　　这样的话，mesh是空的，因此我们需要将组成盒子的顶点和索引分别写入顶点缓存和索引缓存。锁定顶点/索引缓存并手动写入数据这是很容易的：
```C++
// Fill in vertices of a box
Vertex* v = 0;
Mesh->LockVertexBuffer(0, (void**)&v);
// fill in the front face vertex data
v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
v[1] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

v[22] = Vertex( 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
v[23] = Vertex( 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
Mesh->UnlockVertexBuffer();
// Define the triangles of the box
WORD* i = 0;
Mesh->LockIndexBuffer(0, (void**)&i);
// fill in the front face index data
i[0] = 0; i[1] = 1; i[2] = 2;
i[3] = 0; i[4] = 2; i[5] = 3;

// fill in the right face index data
i[30] = 20; i[31] = 21; i[32] = 22;
i[33] = 20; i[34] = 22; i[35] = 23;
Mesh->UnlockIndexBuffer();
```
　　一旦mesh的几何信息被写入，我们必须指定每个三角形在哪个子集中。回想一下属性缓存就是存储的在mesh中每个三角形所属的子集信息。在这个例子中，我们指定索引缓存中的前四个三角形子集为0，接着的四个三角形子集为1，最后四个三角形子集为2。代码如下：
DWORD* attributeBuffer = 0;
Mesh->LockAttributeBuffer(0, &attributeBuffer);
for(int a = 0; a < 4; a++) // triangles 1-4
    attributeBuffer[a] = 0; // subset 0
for(int b = 4; b < 8; b++) // triangles 5-8
    attributeBuffer[b] = 1; // subset 1
for(int c = 8; c < 12; c++) // triangles 9-12
    attributeBuffer[c] = 2; // subset 2
Mesh->UnlockAttributeBuffer();
　　现在我们已经创建了一个包含有效数据的mesh。在这一小部分我们将渲染mesh，不过首先还是先将其优化一下。注意虽然这对于一个盒子mesh来说，优化mesh数据没有真正的效果，但是我们还是用ID3DXMesh接口方法来实践一下。为了优化一个mesh，我们首先需要计算mesh的邻接信息：
std::vector<DWORD> adjacencyBuffer(Mesh->GetNumFaces() * 3);
Mesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);
　　然后我们就能够优化mesh了：
hr = Mesh->OptimizeInplace(
        D3DXMESHOPT_ATTRSORT |
        D3DXMESHOPT_COMPACT |
        D3DXMESHOPT_VERTEXCACHE,
        &adjacencyBuffer[0],
         0, 0, 0);
　　设置好了mesh以后，我们就为渲染它做好了准备。不过在setup函数中还有最后一个问题，也就是在前面我们说的将mesh的内在数据内容写入文件的函数。这能够检查mesh的数据，它能帮助我们调试和学习mesh的结构。
OutFile.open("Mesh Dump.txt");
dumpVertices(OutFile, Mesh);
dumpIndices(OutFile, Mesh);
dumpAttributeTable(OutFile, Mesh);
dumpAttributeBuffer(OutFile, Mesh);
dumpAdjacencyBuffer(OutFile, Mesh);
OutFile.close();
...Texturing loading, setting render states, etc., snipped
return true;
} // end Setup()
　　例如，dumpAttributeTable函数将属性表的数据写入文件。它的具体实现如下：
```c++
void dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh)
{
    outFile << "Attribute Table:" << std::endl;
    outFile << "----------------" << std::endl << std::endl;
    // number of entries in the attribute table
    DWORD numEntries = 0;
    mesh->GetAttributeTable(0, &numEntries);
    std::vector<D3DXATTRIBUTERANGE> table(numEntries);
    mesh->GetAttributeTable(&table[0], &numEntries);
    for(int i = 0; i < numEntries; i++)
    {
        outFile << "Entry " << i << std::endl;
        outFile << "------" << std::endl;
        outFile << "Subset ID: " << table[i].AttribId << std::endl;
        outFile << "Face Start: " << table[i].FaceStart << std::endl;
        outFile << "Face Count: " << table[i].FaceCount << std::endl;
        outFile << "Vertex Start: " << table[i].VertexStart << std::endl;
        outFile << "Vertex Count: " << table[i].VertexCount << std::endl;
        outFile << std::endl;
    }
    outFile << std::endl << std::endl;
}
```
下面的文本文件来自于通过dumpAttributeTable函数得到的mesh Dump.txt文件。
```
Attribute Table:
----------------
Entry 0
------------
Subset ID: 0
Face Start: 0
Face Count: 4
Vertex Start: 0
Vertex Count: 8
Entry 1
------------
Subset ID: 1
Face Start: 4
Face Count: 4
Vertex Start: 8
Vertex Count: 8
Entry 2
------------
Subset ID: 2
Face Start: 8
Face Count: 4
Vertex Start: 16
Vertex Count: 8
```
我们能够了解到我们为mesh所指定的相匹配的数据——有三个子集且每个子集有4个三角形。建议你去看看本例子Dump.txt的完整信息。该文件在本示例文件目录下。
最后，我们使用下面的代码就能够非常容易地渲染mesh了；我们只需要循环每个子集，设置相关联的纹理然后在绘制子集即可。这是非常容易的，因为我们已经为每个子集指定的下标如0，1，2，…，n-1，这里的n就是子集的个数。
```c++
bool Display(float timeDelta)
{
    if( Device )
    {
        //...update frame code snipped
        Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0x00000000, 1.0f, 0);
        Device->BeginScene();
        for(int i = 0; i < NumSubsets; i++)
        {
            Device->SetTexture( 0, Textures[i] );
            Mesh->DrawSubset( i );
        }
        Device->EndScene();
        Device->Present(0, 0, 0, 0);
    }
    return true;
}
```
