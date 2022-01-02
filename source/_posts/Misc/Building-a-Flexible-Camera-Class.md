---
title: 第十一章 网格模型II(Building a Flexible Camera Class)
mathjax: true
date: 2019-04-11 15:17:21
tags:
categories: Direct3D
---
# ID3DXBuffer
　　对ID3DXBuffer接口的一些参考是在上一章，这里我们不会详细讲解。在D3DX库中到处都能看见这个接口，因此大概介绍一下该接口还是很有必要的。
　　ID3DXBuffer接口是一个很普通的数据结构， D3DX用它将数据存储到邻接内存块中。它只有两个方法：
* LPVOID GetBufferPointer()——返回一个指向开始数据的指针。
* DWORD GetBufferSize()——返回在缓存中的字节大小。
   为了保持结构特性，它使用一个空指针。也就是说它让我们知道被存储的数据的类型。例如，D3DXLoadMeshFromX使用一个ID3DXBuffer来返回mesh的邻接信息。因为邻接信息是被存储在DWORD数组中的，所以当我们希望使用缓存中的邻接信息时，我们不得不将缓存转换为DWORD数组。
　　例如：
DWORD* info =(DWORD*)adjacencyInfo->GetBufferPointer();
D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
　　因为ID3DXBuffer是一个COM对象，当你使用完以后就必须释放它以防止内存泄漏：
adjacencyInfo->Release();
mtrlBuffer->Release();
　　我们能够使用下面的方法来创建一个空的ID3DXBuffer：
HRESULT D3DXCreateBuffer(
    DWORD NumBytes, // Size of the buffer, in bytes.
    LPD3DXBUFFER *ppBuffer // Returns the created buffer.
);
　　下面的例子是创建一个能包含4个整数的缓存：
ID3DXBuffer* buffer = 0;
D3DXCreateBuffer( 4 * sizeof(int), &buffer );
11.2 X文件
   迄今为止，我们已经使用过了简单的几何物体，如球体，圆柱体，立方体等，它们都是用D3DXCreate*函数来创建的。假如你想通过手工指定顶点来创建你自己的3D物体，你能，不用怀疑，不过这是非常枯燥乏味的事情。为了减轻建造3D物体数据的工作，专门的应用程序已经被开发出来了，我们把它们叫做3D建模工具。它们允许我们在一个虚拟的拥有丰富工具的交互环境下建造复杂的真实的mesh，在这建造这些模型都是非常容易的。例如在游戏开发中常用到的有3DSMax（www.discreet.com）,LightWave 3D（www.newtek.com）,以及Maya（www.aliaswavefront.com）。
   这些工具，当然能够输出创建好的mesh数据到文件中。因此，我们也能够写一个文件来提取在我们的3D应用程序中要用到的mesh数据。这的确是一种可行的解决办法。不过，还存在一个更方便的解决方案。它是一种叫做X文件的特殊mesh文件格式（扩展名为.X）。很多3D建模软件都能输出这种格式，当然这里存在一个将其他流行的mesh文件转换为X文件的过程。是什么使X文件这么便利呢？因为它是DirectX定义的格式，并且D3DX库很容易地支持X文件。D3DX库提供了读和写X文件的函数。因此，如果我们使用这种格式就避免了还要自己写程序文件来读/写模型文件了。
   注意：你能够下载DirectX9 SDK Extra——你能从MSDN（www.msdn.microsoft.com）上得到一些已经开发好的针对3DMax,LightWave,Maya软件导出.X文件的Direct3D工具包。
11.2.1读取X文件
   我们使用下面的函数来读取存储在X文件中的mesh数据。注意这个方法创建一个ID3DXMesh对象，且从X文件中读取几何信息数据填入其中。
HRESULT D3DXLoadMeshFromX(
    LPCSTR pFilename,
    DWORD Options,
    LPDIRECT3DDEVICE9 pDevice,
    LPD3DXBUFFER *ppAdjacency,
    LPD3DXBUFFER *ppMaterials,
    LPD3DXBUFFER* ppEffectInstances,
    PDWORD pNumMaterials,
    LPD3DXMESH *ppMesh
);
* pFilename — 读取的X文件的文件名。
* Options — 用来创建mesh的一个或多个创建标志。要了解所有标志信息请查看sdk文档。现在列出一部分：
* D3DXMESH_32BIT — mesh使用32位索引。
* D3DXMESH_MANAGED — mesh数据将被放在受控的内存中。
* D3DXMESH_WRITEONLY — mesh数据只能执行写操作，不能执行读操作。
* D3DXMESH_DYNAMIC — mesh缓存将是动态的。
* pDevice — 与复制mesh有关的设备。
* ppAdjacency — 返回一个ID3DXBuffer包含一个DWORD数组，描述mesh的邻接信息。
* ppMaterials — 返回一个ID3DXBuffer包含一个D3DXMATERIAL结构的数组，存储了mesh的材质数据。我们在下一节介绍mesh材质。
* ppEffectInstances — 返回一个ID3DXBuffer包含一个D3DXEFFECTINSTANCE结构的数组。我们现在通过指定0值来忽略这个参数。
* pNumMaterials — 返回mesh的材质数。
* ppMesh — 返回填充了X文件几何信息的ID3DXMesh对象。
11.2.2 X文件的材质
　　D3DXLoadMeshFromX的第七个参数返回的是mesh包含的材质数，第五个参数返回的是包含着材质数据的一个D3DXMATERIAL结构数组。D3DXMATERIAL结构的定义如下：
typedef struct D3DXMATERIAL {
    D3DMATERIAL9 MatD3D;
    LPSTR pTextureFilename;
} D3DXMATERIAL;
　　这是一个简单的结构；它包含一个基本的D3DMATERAIL9结构和一个用来指定与之相关联的纹理文件名的一个以null结束的字符串指针。一个X文件是不能插入纹理数据的；它只能插入文件名。因此，在使用D3DXLoadMeshFromX读取一个X文件以后，我们还必须从纹理文件中读取纹理数据。我们将在下一节中说明怎样具体实现。
　　D3DXLoadMeshFromX函数读取X文件数据以便在返回的D3DXMATERIAL数组中的第i项与第i个子集相对应。因此，子集是使用0，1，2，…，n-1标记的，n是子集和材质的数目。这也就允许使用简单的循环来渲染mesh了。
11.2.3 实例程序：X文件
　　我们现在演示本章中的第一个实例（X文件）的相关代码。该例子调用一个叫做bigship1.x的x文件，你可以在DirectX SDK下的media文件夹下找到它。完整原代码可以在相应的文件中找到。图11.1是该实例的一个截图。

图11.1
　　该实例使用下面的全局变量：
ID3DXMesh*                      Mesh = 0;
std::vector<D3DMATERIAL9>       Mtrls(0);
std::vector<IDirect3DTexture9*> Textures(0);
   这里有一个ID3DXMesh对象，它被用来存储从X文件中读取的mesh数据。也有一个材质vector和纹理vector,我们用它们来分别存储mesh的材质和纹理。
　　我们首先在Setup函数中操作。首先，我们读取X文件：
bool Setup()
{
    HRESULT hr = 0;
    //
    // Load the XFile data.
    //
    ID3DXBuffer* adjBuffer  = 0;
    ID3DXBuffer* mtrlBuffer = 0;
    DWORD        numMtrls   = 0;
    hr = D3DXLoadMeshFromX(  
        "bigship1.x",
        D3DXMESH_MANAGED,
        Device,
        &adjBuffer,
        &mtrlBuffer,
        0,
        &numMtrls,
        &Mesh);
    if(FAILED(hr))
    {
        ::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
        return false;
     }
　　读取完X文件数据以后，我们必须遍历D3DXMATERIAL数组来读取mesh中所使用的所有纹理：
    //
    // Extract the materials, and load textures.
    //
    if( mtrlBuffer != 0 && numMtrls != 0 )
    {
        D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
        for(int i = 0; i < numMtrls; i++)
        {
            // the MatD3D property doesn't have an ambient value set
            // when its loaded, so set it now:
            mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
            // save the ith material
            Mtrls.push_back( mtrls[i].MatD3D );
            // check if the ith material has an associative texture
            if( mtrls[i].pTextureFilename != 0 )
            {
                // yes, load the texture for the ith subset
                IDirect3DTexture9* tex = 0;
                D3DXCreateTextureFromFile(
                    Device,
                    mtrls[i].pTextureFilename,
                    &tex);
                // save the loaded texture
                Textures.push_back( tex );
            }
            else
            {
                // no texture for the ith subset
                Textures.push_back( 0 );
            }
        }
    }
　　d3d::Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer
    .
    . // Snipped irrelevant code to this chapter (e.g., setting up lights,
    . // view and projection matrices, etc.)
    .
    return true;
} // end Setup()
　　在Display函数中我们让mesh在每一帧中都旋转一个小角度。我们使用简单的循环，Mesh便能够被渲染了：
bool Display(float timeDelta)
{
    if( Device )
    {
        //
        // Update: Rotate the mesh.
        //

        static float y = 0.0f;
        D3DXMATRIX yRot;
        D3DXMatrixRotationY(&yRot, y);
        y += timeDelta;

        if( y >= 6.28f )
            y = 0.0f;

        D3DXMATRIX World = yRot;

        Device->SetTransform(D3DTS_WORLD, &World);

        //
        // Render
        //

        Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
        Device->BeginScene();

        for(int i = 0; i < Mtrls.size(); i++)
        {
            Device->SetMaterial( &Mtrls[i] );
            Device->SetTexture(0, Textures[i]);
            Mesh->DrawSubset(i);
        }   

        Device->EndScene();
        Device->Present(0, 0, 0, 0);
    }
    return true;
}
11.2.4 产生顶点法线
   一个X文件不包含顶点法线数据，这是很有可能的。假如是这种情况，那么手动计算顶点法线以便我们能够使用灯光这是很有必要的。在第5章中我们简要的介绍了一下怎么做。然而，现在我们知道了ID3DXMesh接口和它的父接口ID3DXBaseMesh，我们能够使用下面的函数来产生任何mesh的顶点法线：
HRESULT D3DXComputeNormals(
    LPD3DXBASEMESH pMesh, // Mesh to compute normals of.
    const DWORD *pAdjacency // Input adjacency info.
);
   这个函数通过使用平均法线的方法来产生顶点法线。假如有邻接信息，那么重复的顶点是被忽略的。假如没有邻接信息，那么重复的顶点也会被重复计算。了解这些是很重要的，我们检查pMash必须有一个包含D3DFVF_NORMAL标记的顶点格式。
   注意假如X文件不包含顶点法线数据，那么通过D3DXLoadMeshFromX创建的ID3DXMesh对象在它的顶点格式中没有指定的D3DFVF_NORMAL标记。因此，在我们能够使用D3DXComputeNormals之前，我们必须复制mesh并且为其指定包含D3DFVF_NORMAL的顶点格式。下面就是相应的代码：
// does the mesh have a D3DFVF_NORMAL in its vertex format?
if ( !(pMesh->GetFVF() & D3DFVF_NORMAL) )
{
    // no, so clone a new mesh and add D3DFVF_NORMAL to its format:
    ID3DXMesh* pTempMesh = 0;
    pMesh->CloneMeshFVF(
        D3DXMESH_MANAGED,
        pMesh->GetFVF() | D3DFVF_NORMAL, // add it here
        Device,
        &pTempMesh );

    // compute the normals:
    D3DXComputeNormals( pTempMesh, 0 );

    pMesh->Release(); // get rid of the old mesh
    pMesh = pTempMesh; // save the new mesh with normals
}
11.3渐进网格（Progressive Meshes）
   渐进网格，它通过ID3DXPMesh接口来表现，允许我们通过简化边缩减转换（edge collapse transformations，ECT）来简化mesh。每执行一次ECT就移除一个顶点和一或2个面。因为每个ECT是可逆的（它的逆过程叫顶点分裂），我们能够逆转简化过程并且恢复mesh为它的原始状态。当然，我们不可能得到比原始情况还要精细的网格。我们仅仅只能简化然后恢复简化操作。图11.2显示了同一个mesh的三种不同精细级别（levels of detail，LOD）：高，中，低。

图11.2
   渐进网格和mipmaps纹理非常相似。当使用纹理时，我们已经注意到在一个小或远的图元上使用高分辨率的纹理简直就是浪费。对于mesh也是同样的道理；一个小或远的mesh不需要太多三角形，多了也是浪费。因此，我们不会花费渲染高三角形模型的时间来渲染一个只需要表现小的低三角形模型。
   我们可以使用渐进网格来根据模型距离摄象机的距离来调整模型的LOD。也就是说，当距离减少时，我们增加mesh的细节，当距离增加时我们减少mesh的细节。
   注意我们还没有讨论渐进网格是怎样被实现的；这里我们只讲解怎样使用ID3DXPMesh接口。对此感兴趣的读者可以到渐进网格的原始页面Hoppe上查看。Hoppe的网址：http://research.microsoft.com/~hoppe/。
11.3.1 产生一个渐进网格
   我们能够使用下面的函数来创建一个ID3DXPMesh对象：
HRESULT D3DXGeneratePMesh(
    LPD3DXMESH pMesh,
    CONST DWORD *pAdjacency,
    CONST LPD3DXATTRIBUTEWEIGHTS pVertexAttributeWeights,
    CONST FLOAT *pVertexWeights,
    DWORD MinValue,
    DWORD Options,
    LPD3DXPMESH *ppPMesh
);
* pMesh— 输入原始mesh，它包含了我们想要生成的渐进网格的mesh数据。
* pAdjacency — 指向一个包含pMesh邻接信息的DWORD数组。
* pVertexAttributeWeights — 指向一个D3DXATTRIBUTEWEIGHTS数组，它的大小是pMesh->GetNumVertices（）。它的第i项与pMesh中的第i个顶点相对应并且指定的是它的品质权重。品质权重被用来确定一个顶点被删除的可能性大小。你能够将此参数设置为null，对于每个顶点一个默认的顶点品质权重将被设置。在11.3.2节中有关于顶点品质权重和D3DXATTRIBUTEWEIGHTS结构的更多信息。
* pVertexWeights — 指向一个float数组，它的大小是pMesh->GetNumVertices（），它的第i项与pMesh中的第i个顶点相对应并且指定的是它的顶点权重。顶点权重越高被删除的可能性越小。你能够将此参数设置为null，对于每个顶点一个默认的顶点品质权重1.0将被设置。
* MinValue — 我们想要简化到的最小顶点或面数。注意该值是必须的，而且与顶点/品质权重有关，最终可能达不到该值。
* Options — 只能取D3DXMESHSIMP枚举类型中的一个值：
* D3DXMESHSIMP_VERTEX — 指定在上一个参数MinValue中提到的数为顶点数。
* D3DXMESHSIMP_FACE —指定在上一个参数MinValue中提到的数为面数。
* ppPMesh — 返回生成好的渐进网格。
11.3.2 顶点品质权重
typedef struct _D3DXATTRIBUTEWEIGHTS {
    FLOAT Position;
    FLOAT Boundary;
    FLOAT Normal;
    FLOAT Diffuse;
    FLOAT Specular;
    FLOAT Texcoord[8];
    FLOAT Tangent;
    FLOAT Binormal;
} D3DXATTRIBUTEWEIGHTS;
　　顶点权重结构允许我们为每个顶点属性指定一个权值。0.0表示该属性没有权重。顶点属性的权重越高在简化过程中被移除的可能性越小。默认的权值如下：
D3DXATTRIBUTEWEIGHTS AttributeWeights;
AttributeWeights.Position = 1.0;
AttributeWeights.Boundary = 1.0;
AttributeWeights.Normal = 1.0;
AttributeWeights.Diffuse = 0.0;
AttributeWeights.Specular = 0.0;
AttributeWeights.Tex[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
　　默认的权值是被推荐的，除非你的应用程序有一个重要的理由而不使用它。
11.3.3 ID3DXPMesh方法
   ID3DXPMesh接口是继承自ID3DXBaseMesh接口。因此它拥有以前所学习过的ID3DXMesh的所有函数，下面是一些额外的方法：
DWORD GetMaxFaces(VOID)——返回渐进网格能够被设置的最大面数。
DWORD GetMaxVertices(VOID)——返回渐进网格能够被设置的最大顶点数。
DWORD GetMinFaces(VOID)——返回渐进网格能够被设置的最小面数。
DWORD GetMinVertices(VOID)——返回渐进网格能够被设置的最小顶点数。
HRESULT SetNumFaces(DWORD Faces)——这个方法允许我们设置面的个数，以便让mesh简化/复杂化。例如，假设mesh目前有50个面，我们现在想将它简化到30个面；我们将写成：
pmesh->SetNumFaces(30);
   注意调整后的面数可能并不是我们设定的面数。假如面数小于了GetMinFaces（），那么面数将为GetMinFaces（）。同样的，假如面数大于了GetMaxFaces（），那么面数将为GetMaxFaces（）。
HRESULT SetNumVertices(DWORD Vertices)——这个方法允许我们设置顶点的个数，以便让mesh简化/复杂化。例如，假设mesh目前有20个顶点，我们现在想将它增加到40个；我们将写成：
pmesh->SetNumVertices(40);
   注意调整后的顶点数可能并不是我们设定的数。假如顶点数小于了GetMinVertices（），那么顶点数将为GetMinVertices（）。同样的，假如顶点数大于了GetMaxVertices（），那么顶点数将为GetMaxVertices（）。
HRESULT TrimByFaces(
DWORD NewFacesMin,
DWORD NewFacesMax,
DWORD *rgiFaceRemap, // Face remap info.
DWORD *rgiVertRemap // Vertex remap info.
);
   这个方法允许我们设置新的最小和最大面数，分别通过NewFacesMin和NewFacesMax指定。注意新的最小和最大值必须在现有最小和最大面数之间；也就是说，必须在[GetMinFaces（），GetMaxFaces（）]之中。该函数也返回面和顶点的重影射信息。重影射信息参见10.4节。
HRESULT TrimByVertices(
DWORD NewVerticesMin,
DWORD NewVerticesMax,
DWORD *rgiFaceRemap, // Face remap info.
DWORD *rgiVertRemap // Vertex remap info.
);
   这个方法允许我们设置新的最小和最大顶点数，分别通过NewVerticesMin和NewVerticesMax指定。注意新的最小和最大值必须在现有最小和最大顶点数之间；也就是说，必须在[GetMinVertices（），GetMaxVertices（）]之中。该函数也返回面和顶点的重影射信息。重影射信息参见10.4节。
11.3.4实例程序：渐进网格
   渐进网格例子与X文件例子很相似，除了实际上我们创建和渲染的是一个渐进网格，通过ID3DXPMesh接口来表现。我们允许用户通过键盘输入进行交互式地改变渐进网格。你能通过按A键来增加mesh的面数，按S键来减少mesh的面数。
   在这个例子中使用的全局变量和X文件例子中的是一样的，不过我们增加了一个用来存储渐进网格的变量：
ID3DXMesh*                      SourceMesh = 0;
ID3DXPMesh*                     PMesh      = 0; // progressive mesh
std::vector<D3DMATERIAL9>       Mtrls(0);
std::vector<IDirect3DTexture9*> Textures(0);
   回想一下，为了得到一个渐进网格我们必须输入一个包含了数据信息的源mesh。因此，我们首先读取一个X文件数据到ID3DXMesh对象SourceMesh之中，然后再产生渐进网格：
bool Setup()
{
    HRESULT hr = 0;
    // ...Load XFile data into SourceMesh snipped.
    //
　　// ...Extracting materials and textures snipped.
   因为这一部分代码和X文件例子中的是完全一样的，在这里我们就把它省略了。一但有了源mesh，我们就能够象下面一样来生成渐进网格了：
    //
    // Generate the progressive mesh. 
    //
    hr = D3DXGeneratePMesh(
        SourceMesh,
        (DWORD*)adjBuffer->GetBufferPointer(), // adjacency
        0,                  // default vertex attribute weights
        0,                  // default vertex weights
        1,                  // simplify as low as possible
        D3DXMESHSIMP_FACE,  // simplify by face count
        &PMesh);

    d3d::Release<ID3DXMesh*>(SourceMesh);  // done w/ source mesh
    d3d::Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

    if(FAILED(hr))
    {
        ::MessageBox(0, "D3DXGeneratePMesh() - FAILED", 0, 0);
        return false;
     }
　　注意，因为顶点/品质权值的缘故，很难将Mesh简化到只有一个面，但是，如果将其指定为1，则可以将Mesh简化到最低。
　　在这一点上，渐进网格已经被产生了，但是假如你现在就渲染它，它将以最简化的方式来渲染。以为我们想开始渲染最高精度的mesh，所以我们设置它为：
    // set to original detail
    DWORD maxFaces = PMesh->GetMaxFaces();
     PMesh->SetNumFaces(maxFaces);
   在Display函数中，我们测试A键和S键并将结果输入。
bool Display(float timeDelta)
{
    if( Device )
    {
        //
        // Update: Mesh resolution.
        //

        // Get the current number of faces the pmesh has.
        int numFaces = PMesh->GetNumFaces();

        // Add a face, note the SetNumFaces() will  automatically
        // clamp the specified value if it goes out of bounds.
        if( ::GetAsyncKeyState('A') & 0x8000f )
        {
            // Sometimes we must add more than one face to invert
            // an edge collapse transformation
            PMesh->SetNumFaces( numFaces + 1 );
            if( PMesh->GetNumFaces() == numFaces )
                PMesh->SetNumFaces( numFaces + 2 );
        }

        // Remove a face, note the SetNumFaces() will  automatically
        // clamp the specified value if it goes out of bounds.
        if( ::GetAsyncKeyState('S') & 0x8000f )
　　　　　　　　PMesh->SetNumFaces( numFaces - 1 );
   这是很简单的，但是要注意当增加面时我们有时必须增加两个面来完成ECT。
　　最后，我们就能象渲染ID3DXMesh对象一样来渲染ID3DXPMesh对象。另外，为了更加直观的观察网格的三角形数的变化情况，使用黄色材质在线框模式（Wireframe Mode）下渲染Mesh的三角形。
        Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
        Device->BeginScene();
        for(int i = 0; i < Mtrls.size(); i++)
        {
            // draw pmesh
            Device->SetMaterial( &Mtrls[i] );
            Device->SetTexture(0, Textures[i]);
            PMesh->DrawSubset(i);
            // draw wireframe outline
            Device->SetMaterial(&d3d::YELLOW_MTRL);
            Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
            PMesh->DrawSubset(i);
            Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        }   
        Device->EndScene();
        Device->Present(0, 0, 0, 0);
    }
    return true;
}
　　
图11.3
11.4 界线容积（Bounding Volumes）
　　有时我们需要计算mesh的界线容积（边界范围）。常用的有两种类型：立方体和球。也有使用其它方法的，如圆柱体，椭球体，菱形体，胶囊形。图11.4演示了对同一个mesh分别使用立方体和球体类型。这一节我们只讨论立方体和球体两种边界形式。
　　
　　图11.4
　　边界盒/球常常被用来加速可见性测试，碰撞检测等。例如，假如一个mesh的边界盒/球不可见，那么我们就说mesh不可见。一个盒/球可见性测试是比分别测试mesh中的每个三角形要廉价的多。对于一个碰撞检测例子，如果一枚导弹点火起飞，我们需要检测它是否击中了同一场景中的目标。由于这些物体都是由大量三角形构成，我们可以依次检测每个对象的每个三角形，来测试导弹（可以用射线数学模型）是否碰撞到了这些三角形。这个方法需要进行多次的射线/三角形交点的运算。一个更好的方法是使用边界盒或边界球，计算射线与场景中的每个对象的边界盒/边界球的交点。如果射线与对象的边界范围相交，可以认为该对象被击中了。这是一个公平的近似方法，如果需要更高的精度，可以用边界范围法先去除那些明显不会相撞的对象，然后用更精确地方法检测很可能相撞的对象。如果边界范围检测发现相撞，则该对象就很有可能相撞。
　　D3DX库提供了计算mesh的边界盒和边界球的函数。这些函数使用顶点数组作为输入计算边界盒/球。这些函数本来就是设计的很灵活的，它们可以使用各种顶点格式：
HRESULT D3DXComputeBoundingSphere(
    LPD3DXVECTOR3 pFirstPosition,
    DWORD NumVertices,
    DWORD dwStride,
    D3DXVECTOR3* pCenter,
    FLOAT* pRadius
);
* pFirstPosition——指向在顶点数组中第一个顶点的向量，描述顶点位置。
* NumVertices——在顶点数组中的的顶点数。
* dwStride——每个顶点的字节大小。这是很需要的，因为顶点结构可能有一些额外信息如法向量和纹理坐标，这些信息对计算边界又没有用，函数需要知道应该跳过多少字节来得到下一个顶点的位置。
* pCenter——返回边界球的中心。
* pRadius——返回边界球的半径。
HRESULT D3DXComputeBoundingBox(
    LPD3DXVECTOR3 pFirstPosition,
    DWORD NumVertices,
    DWORD dwStride,
    D3DXVECTOR3* pMin,
    D3DXVECTOR3* pMax
);
前三个参数和D3DXComputeBoundingSphere的前三个参数是完全一样的。最后两个参数分别用来返回边界盒的最小和最大点。
11.4.1一些新的特殊常量
让我来介绍两个常量，它们在本书中是经常要用到的。我们把它们添加到d3d名称空间中：
namespace d3d
{
    ...
    const float INFINITY = FLT_MAX;
    const float EPSILON = 0.001f;
常量INFINITY是用来表示一个浮点数所能存储的最大数。因为我们找不到一个比FLT_MAX还要大的浮点数，我们可以将它视为无穷大。常量EPSILON是一个很小的值，我们这样定义它，凡是比它小的数就视为0。这也是很有必要的，因为得到的浮点是不精确的，一个被读作0的数可能有一点点小偏差。因此，让它和0比较相等肯定会失败。我们因此可以通过把该值与0的差值与EPSILON比较来确定是否相等：
bool Equals(float lhs, float rhs)
{
    // if lhs == rhs their difference should be zero
    return fabs(lhs - rhs) < EPSILON ? true : false;
}
11.4.2界线容积类型
   为了更容易的使用边界盒和边界球，我们将它们分别封装到两个类中。现在在d3d名称空间中定义类：
struct BoundingBox
{
    BoundingBox();
    bool isPointInside(D3DXVECTOR3& p);
    D3DXVECTOR3 _min;
    D3DXVECTOR3 _max;
};
struct BoundingSphere
{
    BoundingSphere();
    D3DXVECTOR3 _center;
    float _radius;
};
d3d::BoundingBox::BoundingBox()
{
    // infinite small bounding box
    _min.x = d3d::INFINITY;
    _min.y = d3d::INFINITY;
    _min.z = d3d::INFINITY;
    _max.x = -d3d::INFINITY;
    _max.y = -d3d::INFINITY;
    _max.z = -d3d::INFINITY;
}
bool d3d::BoundingBox::isPointInside(D3DXVECTOR3& p)
{
    // is the point inside the bounding box?
    if(p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
        p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
    {
        return true;
    }
    else
    {
        return false;
    }
}
d3d::BoundingSphere::BoundingSphere()
{
    _radius = 0.0f;
}
11.4.3实例程序：界线容积
　　在这一章中被叫做界线容积的实例程序主要是演示使用D3DXComputeBoundingSphere和D3DXComputeBoundingBox。程序读取一个X文件并且计算该mesh的边界球。它创建两个ID3DXMesh对象，一个用来作为边界球模型一个用来作为边界盒模型。X文件生成的mesh被渲染，其中的边界球或边界盒不可见（如图11.5）。你能够通过敲空格键来再边界球和边界盒之间切换。

图11.5
　　这个例子是非常简单的，我们列出你要学习的代码。我们实现的两个函数是用来计算网格的边界球和边界盒的：
bool ComputeBoundingSphere(
        ID3DXMesh* mesh, // mesh to compute bounding sphere for
        d3d::BoundingSphere* sphere) // return bounding sphere
{
    HRESULT hr = 0;

    BYTE* v = 0;
    mesh->LockVertexBuffer(0, (void**)&v);

    hr = D3DXComputeBoundingSphere(
            (D3DXVECTOR3*)v,
            mesh->GetNumVertices(),
            D3DXGetFVFVertexSize(mesh->GetFVF()),
            &sphere->_center,
            &sphere->_radius);

    mesh->UnlockVertexBuffer();

    if( FAILED(hr) )
        return false;
    return true;
}

bool ComputeBoundingBox(
        ID3DXMesh* mesh, // mesh to compute bounding box for
        d3d::BoundingBox* box) // return bounding box
{
    HRESULT hr = 0;

    BYTE* v = 0;
    mesh->LockVertexBuffer(0, (void**)&v);

    hr = D3DXComputeBoundingBox(
            (D3DXVECTOR3*)v,
            mesh->GetNumVertices(),
            D3DXGetFVFVertexSize(mesh->GetFVF()),
            &box->_min,
            &box->_max);

    mesh->UnlockVertexBuffer();

    if( FAILED(hr) )
        return false;
    return true;
}
　　注意，类型转换(D3DXVECTOR3*)v假定顶点位置成员是被存储在我们所使用的顶点结构的开始位置。同样要注意我们能够使用D3DXGetFVFVertexSize函数来得到顶点结构的大小。
11.5 摘要(略)


第十二章 创建灵活的摄像机类(Building a Flexible Camera Class)
　　迄今，我们已经使用过D3DXMatrixLookAtLH函数来计算视图空间变换矩阵。这个函数对于在固定位置布置和对准摄像机是非常好用的，不过它的用户接口对于要响应用户输入来实现摄像机移动就不那么好用了。这就激发我们用我们自己的方法来解决。在这一章我们展示了怎样实现一个Camera类，它使我们能够比D3DXMatrixLookAtLH函数更好地操作摄像机，并且可以用来作为飞行模拟摄像机和第一人称视角摄像机。
目标
* 学习怎样实现一个灵活的摄像机类，它可以用作飞行模拟摄像机和第一人称视角摄像机。
12.1 摄像机设计
　　我们定义一个相对于世界坐标系的位置和摄像机的方向，这里使用四个摄像机向量：right vector ,  up vector, look vector 以及 position vector, 如图12.1所示。这些向量用来为摄像机定义一个坐标系来描述在世界坐标中的对应关系。因为 right ，up 和 look 向量定义了摄像机在世界中的方向，我们有时把它们三个向量一起称为方向向量（orientation vectors）。方向向量必须被标准化。假如彼此互相垂直且都是单位长度，那么我们就称它们是正交标准化向量。我们做这些限制是因为等一会儿我们要将方向向量插入到一个行矩阵中。因为行向量是正交标准化的，所以该矩阵也就是直交矩阵。回忆一下，直交矩阵有一个特性就是它的逆矩阵等于它的转置矩阵。这在等一下的12.2.1.2节中是很有用的。

图12.1
　　有了这四个向量来描述摄像机，我们的摄像机就能够按照下面六种方式变化了：
* 围绕right向量旋转（pitch倾斜）
* 围绕up向量旋转（yaw 偏航）
* 围绕look向量旋转（roll 滚转）
* 沿着right向量平移（strafe）
* 沿着up向量飞行（fly）
* 沿着look向量移动（move）
通过这六种操作，我们能够沿着三个轴移动以及饶着三个轴旋转，这给了我们一个六度的自由。下面的Camera类定义了我们要的描述数据以及想要的方法：
```c++
class Camera
{
public:
    enum CameraType { LANDOBJECT, AIRCRAFT };
    Camera();
    Camera(CameraType cameraType);
    ~Camera();

    void strafe(float units); // left/right
    void fly(float units);    // up/down
    void walk(float units);   // forward/backward   
    void pitch(float angle); // rotate on right vector
    void yaw(float angle);   // rotate on up vector
    void roll(float angle);  // rotate on look vector

    void getViewMatrix(D3DXMATRIX* V); 
    void setCameraType(CameraType cameraType); 
    void getPosition(D3DXVECTOR3* pos); 
    void setPosition(D3DXVECTOR3* pos); 
    void getRight(D3DXVECTOR3* right);
    void getUp(D3DXVECTOR3* up);
    void getLook(D3DXVECTOR3* look);
private:
    CameraType  _cameraType;
    D3DXVECTOR3 _right;
    D3DXVECTOR3 _up;
    D3DXVECTOR3 _look;
    D3DXVECTOR3 _pos;
};
```
在类中我们定义了一个还没有讨论的CameraType枚举类型。目前，我们的摄像机支持两种摄像机模式，LANDOBJECT模式和AIRCRAFT模式。AIRCRAFT模式允许我们在空间中完全自由的移动。不过，在有些游戏中，比如第一人称设计游戏，人是不能飞的；因此我们必须限制它在某些轴上的运动。指定为LANDOBJECT模式的摄像机就限制了这些，你可以在下一部分看见。
12.2 执行详细资料
12.2.1计算视图矩阵
　　我们现在演示怎样根据摄像机向量来计算视图矩阵变换的。让 p = (px, py, pz), r = (rx, ry, rz), u = (ux, uy, uz) 以及 d = (dx, dy, dz) 分别表示 position, right, up 以及 look 向量。
　　回忆第二章我们所说的，视图空间变换是指在世界坐标系中进行几何变换以便将照相机平移变换到坐标系的源点并把它的方向旋转至朝向Z轴的正方向（如图12.2）。

图12.2
因此，我们希望有一个象这样的变换矩阵V ：
* pV = (0, 0, 0)—矩阵V能将摄像机移动到原点。
* rV = (1, 0, 0)—矩阵V能将摄像机的right向量与世界坐标系中的x轴对齐。
* uV = (0, 1, 0)—矩阵V能将摄像机的up向量与世界坐标系中的y轴对齐。
* dV = (0, 0, 1)—矩阵V能将摄像机的look向量与世界坐标系中的z轴对齐。
我们能将变换任务分为两个部分：1）平移部分，将摄像机的位置移动到原点；2）旋转部分，将摄像机的方向向量与世界坐标系的轴对齐。
12.2.1.1 第一部分：平移
　　平移只需要利用 –p 就可简单地将 p 移动到原点，因为 p–p=0。因此我们能够用下面的矩阵来描述视图变换中的平移部分：
　　
12.2.1.2 第二部分：旋转
　　矫正摄像机的三个方向向量使其与世界坐标系的轴对齐需要更多的工作。我们需要一个3*3的旋转矩阵A ，它能将right，up和look分别与x-，y-以及z轴对齐。这个矩阵将满足如下三个等式：
　　
注意：我们在这里使用3*3矩阵来工作是因为现在不需要额外的信息来表现旋转。等一下我们将它增加到常用的4*4矩阵。
　　因为这三个等式都有一个相同系数矩阵A ，所以我们能够把它们合在一起。我们把它们从新写到一起来：
　　
　　求A有很多方法，但是我们知道A是B逆矩阵因为BA = BB-1 = I。因为B 是一个直交矩阵（它的行向量是正交标准化的），我们知道它的逆矩阵就是它的转置矩阵。因此，将方向向量和世界坐标系中的坐标轴对齐的变换如下：
　　
12.2.1.3 将两部分合并
　　最后，将A增加为4*4矩阵，同时将平移部分合并到旋转部分形成的视图变换矩阵V：

我们在Camera::getViewMatrix方法中建立这个矩阵：
void Camera::getViewMatrix(D3DXMATRIX* V)
{
    // Keep camera's axes orthogonal to eachother
    D3DXVec3Normalize(&_look, &_look);
    D3DXVec3Cross(&_up, &_look, &_right);
    D3DXVec3Normalize(&_up, &_up);
    D3DXVec3Cross(&_right, &_up, &_look);
    D3DXVec3Normalize(&_right, &_right);

    // Build the view matrix:
    float x = -D3DXVec3Dot(&_right, &_pos);
    float y = -D3DXVec3Dot(&_up, &_pos);
    float z = -D3DXVec3Dot(&_look, &_pos);

    (*V)(0,0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
    (*V)(1,0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
    (*V)(2,0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
    (*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}
你可能想知道方法中前面几行代码是干什么的。在几次旋转后，摄像机的方向向量可能变的不相互垂直了。因此，每当该函数被调用时，我们根据look向量从新计算up和right向量，使它们保持相互垂直。新的up向量是这样计算的up = look × right。 接着新的right向量是这样计算的right = up × look。
12.2.2围绕任意轴旋转
为了实现我们的摄像机旋转方法，我们需要能够绕着任意轴旋转。D3DX库提供下面的函数来解决这个问题：
D3DXMATRIX *D3DXMatrixRotationAxis(
    D3DXMATRIX *pOut, // returns rotation matrix
    CONST D3DXVECTOR3 *pV, // axis to rotate around
    FLOAT Angle // angle, in radians, to rotate
);
　　
　　图12.3
例如，假如我们想绕向量（0.707, 0.707, 0）轴旋转π/2角度。我们可以这样写：
D3DXMATRIX R;
D3DXVECTOR3 axis(0.707f, 0.707f, 0.0f);
D3DXMatrixRotationAxis(&R, &axis, D3DX_PI / 2.0f);
D3DXMatrixRotationAxis的变换矩阵的来源你可以在Eric Lengyel的 Mathematics for 3D Game Programming &Computer Graphics中找到。
12.2.3 Pitch、Yaw和Roll
因为方向向量描述了摄像机相对于世界坐标系的方向，我们必须考虑在使用倾斜（pitch）、偏航（yaw）和滚转（roll）时及时更新方向向量。这其实也是非常简单的。图12.4，12.5，12.6分别显示了摄像机的倾斜、偏航和滚转操作。

图12.4

图12.5

图12.6
当倾斜（pitch）时，我们需要将up和look向量绕着right向量旋转一定角度。同样的，当偏航（yaw）时，我们需要将look和right向量绕着up向量旋转一定角度。最后，当滚转（roll）时，我们需要将up和right向量绕着look向量旋转一定角度。
　　我们现在明白了为什么D3DXMatrixRotationAxis函数是非常必要的，因为这三个向量中的任何一个都可能围绕世界坐标系中的任意轴旋转。
　　对于倾斜（pitch）、偏航（yaw）和滚转（roll）的执行我们已经讨论了。然而，对于LANDOBJECT模式就有一些限制。我们在偏航（yaw）方法中只围绕y轴旋转，我们完全屏蔽滚转（roll）。当然你可以根据你的程序需要来改变Camera类。我们这里只是一个示例而已。
倾斜（pitch）、偏航（yaw）和滚转（roll）方法代码的具体实现如下：
```c++
void Camera::pitch(float angle)
{
    D3DXMATRIX T;
    D3DXMatrixRotationAxis(&T, &_right, angle);

    // rotate _up and _look around _right vector
    D3DXVec3TransformCoord(&_up,&_up, &T);
    D3DXVec3TransformCoord(&_look,&_look, &T);
}

void Camera::yaw(float angle)
{
    D3DXMATRIX T;

    // rotate around world y (0, 1, 0) always for land object
    if( _cameraType == LANDOBJECT )
        D3DXMatrixRotationY(&T, angle);

    // rotate around own up vector for aircraft
    if( _cameraType == AIRCRAFT )
        D3DXMatrixRotationAxis(&T, &_up, angle);

    // rotate _right and _look around _up or y-axis
    D3DXVec3TransformCoord(&_right,&_right, &T);
    D3DXVec3TransformCoord(&_look,&_look, &T);
}

void Camera::roll(float angle)
{
    // only roll for aircraft type
    if( _cameraType == AIRCRAFT )
    {
        D3DXMATRIX T;
        D3DXMatrixRotationAxis(&T, &_look,  angle);

        // rotate _up and _right around _look vector
        D3DXVec3TransformCoord(&_right,&_right, &T);
        D3DXVec3TransformCoord(&_up,&_up, &T);
    }
}
```
12.2.4 Walking、Strafing和Flying
当提到walking时，我们的意思是在我们观察的方向上移动位置（也就是说，沿着look向量）。Strafing是说在我们观察方向的左右移动，也就是沿着right向量移动。最后，我们说flying就是沿着up向量移动。为了沿着这些轴移动，我们只需要简单地加一个向量就可以了（如图12.7）。

图12.7
就象旋转一样，我们需要对移动作一些限制。例如，LANDOBJECT不允许飞起来。因此我们把移动限制在xz平面。然而，因为LANDOBJECT能够允许爬楼梯和登山，所以，我们设置Camera::setPosition方法，它允许你手动设置你的摄像机位置来达到你的高度和位置。
    移动（walk）、平移（strafe）和飞行（fly）方法代码的具体实现如下：
void Camera::walk(float units)
{
    // move only on xz plane for land object
    if( _cameraType == LANDOBJECT )
        _pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;
    if( _cameraType == AIRCRAFT )
        _pos += _look * units;
}
void Camera::strafe(float units)
{
    // move only on xz plane for land object
    if( _cameraType == LANDOBJECT )
        _pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;
    if( _cameraType == AIRCRAFT )
        _pos += _right * units;
}
void Camera::fly(float units)
{
    // move only on y-axis for land object
    if( _cameraType == LANDOBJECT )
        _pos.y += units;
    if( _cameraType == AIRCRAFT )
        _pos += _up * units;
}
12.3实例程序：摄像机
　　这一章的实例程序是创建和渲染一个如图12.8所示的场景。你能够通过键盘输入在场景中自由地飞行。下面是相应键盘设置：
* W/S—向前/向后移动
* A/D—向左/向右平移
* R/F—向上/向下飞行
* Up/Down方向键—倾斜
* Left/Right方向键—偏航
* N/M—滚转

图12.8
　　例子的执行是非常简单的，因为所有工作都包含在摄像机类中了，这些我们都已经讨论过了。我们在Display函数中获得键盘的输入。记住，我们在全局域中实例化了一个摄像机类对象TheCamera。同样注意我们使用时间变化量来控制移动摄像机；这可以排除帧速度的影响而稳定地移动。
```c++
bool Display(float timeDelta)
{
    if( Device )
    {
        //
        // Update: Update the camera.
        //

        if( ::GetAsyncKeyState('W') & 0x8000f )
            TheCamera.walk(4.0f * timeDelta);
        if( ::GetAsyncKeyState('S') & 0x8000f )
            TheCamera.walk(-4.0f * timeDelta);
        if( ::GetAsyncKeyState('A') & 0x8000f )
            TheCamera.strafe(-4.0f * timeDelta);
        if( ::GetAsyncKeyState('D') & 0x8000f )
            TheCamera.strafe(4.0f * timeDelta);
        if( ::GetAsyncKeyState('R') & 0x8000f )
            TheCamera.fly(4.0f * timeDelta);
        if( ::GetAsyncKeyState('F') & 0x8000f )
            TheCamera.fly(-4.0f * timeDelta);
        if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
            TheCamera.pitch(1.0f * timeDelta);
        if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
            TheCamera.pitch(-1.0f * timeDelta);
        if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
            TheCamera.yaw(-1.0f * timeDelta);           
        if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
            TheCamera.yaw(1.0f * timeDelta);
        if( ::GetAsyncKeyState('N') & 0x8000f )
            TheCamera.roll(1.0f * timeDelta);
        if( ::GetAsyncKeyState('M') & 0x8000f )
            TheCamera.roll(-1.0f * timeDelta);

        // Update the view matrix representing the cameras 
        // new position/orientation.
        D3DXMATRIX V;
        TheCamera.getViewMatrix(&V);
        Device->SetTransform(D3DTS_VIEW, &V);
        //
        // Render
        //
        Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
        Device->BeginScene();
        d3d::DrawBasicScene(Device, 1.0f);
        Device->EndScene();
        Device->Present(0, 0, 0, 0);
    }
    return true;
}
```
注意：我们已经用一个新的函数DrawBasicScene更新了d3d名称空间。这个函数绘制了如图12.8的场景。我们已经将其添加进了d3d名称空间，这是因为对于建立一个基本的场景它是一个非常方便的函数。以后的例子我们就可以集中精力在例子代码中而不需要关注这些不相关的绘制场景的代码了。它是在d3dUtility.h中被声明的：
```c++
// Function references "desert.bmp" internally.  This file must
// be in the working directory.
bool DrawBasicScene(
    IDirect3DDevice9* device,// Pass in 0 for cleanup.
    float scale);            // uniform scale
```
如果该函数不能任何东西也就是什么都显示不出来，你就需要看看相应的代码了。你可以在本章的代码中找到它。注意这个函数需要调用一张desert.bmp图片用作纹理。当然该文件也可以在同一个文件夹下找到。
12.4 摘要
   我们以四个向量来描述在世界坐标系中照相机的位置和方向：right、up、look、position向量，藉由这个描述, 我们能轻易的实现一个自由的六角度照相机，为游戏中的模拟飞行器、第一人称视角的游戏玩家提供了一个灵活的照相机接口。
   

