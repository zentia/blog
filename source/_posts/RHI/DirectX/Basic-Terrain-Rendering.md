---
title: 地形渲染基础(Basic Terrain Rendering)
mathjax: true
date: 2019-04-11 15:21:54
tags:
- RHI
- DirectX
categories: 
- RHI
- DirectX
---

实际上，地形网格不比三角形网格复杂，图13.1.(a)所示，网络的每个顶点指定了高度，格子模型用这种方式显示从山脉到河流的平滑过渡。图13.1 (b)，模拟自然地形。当然，我们可以用漂亮的纹理表现沙石地，绿色的山丘。图13.1.(c)雪山效果。

图 13.1: (a) 三角网格. (b) 平滑高度过渡的三角网格. (c) 光和纹理，我们在这一章节中写的例子的一个屏幕截图。
   这一章的内容是实现一个Terrain（地形）类。这个类的功能很强。我们的意思是，它只是储存整个地形的顶点/索引数据，然后渲染它。因为如果游戏需要一个小的地形，那么它能够在现代图形卡支持的硬件顶点处理下工作。然而实际上，游戏需要大量的地形，你必须对细节做某种（级别）程度的捡选，因为模型需要大量的几何数据，这样大的地形对于再强大的处理方法也是无法处理的。

目标 
* 学习怎样生成地形的高度信息，它能使山丘、河流等地带的平滑的过度，模拟自然界的地形。
* 了解怎样生成地形的顶点和三角形数据。
* 学习使用地形的纹理和光照.
* 找到控制地形上照相机位置的方法，以便模拟在地形上走动。

13.1 Heightmaps（高度图）
   我们使用高度图去描述地形上的山丘、河流。高度图是一个数组，数组中的每个成员指定地形顶点描述中的高度信息。我们经常把高度图想像成一个矩阵，因为每个元素都一一对应于每个地形网格中的顶点。
   当我们保存高度图到磁盘上时，我们通常为高度图的每个元素分配1个byte的内存，所以高度的范围是0..255，0..255的范围对于地形的高度之间保持平滑过渡是足够用的。但为了在我们的程序中匹配3D世界中的物体，可能需要的范围在0..255以外。例如，我们在3D世界中的测量单位是英尺，那么0..255的范围对于表现任何有趣的东西是不够的。因此，当我们读取数据进应用程序时，给每个高度元素分配一个整型数（或浮点型），它允许我们很好的缩放0..255范围之外的任何大小的物品。
   高度图图形表示法这一是灰度图(grayscale map)。较黑的值表示地形中较低的地方，较白的值表现地形中较高的地方。
     
     图13.2: 高度图的灰度图表示
     
13.1.1 创建高度图（Heightmap）
   高度图不是用程序生成就是用图像编辑器生成，比如：Adobe Photoshop。使用图像编辑器大概是最容易的方法了。当你想生成地形时，可以交互式的可视化的创建。你可以利用图像编辑器的功能，比如：过滤器，创建一个有趣的高度图，图13.3显示了一个用Adobe Photoshop图像编辑器的工具创建的金字塔形的高度图。注意：当创建图像时我们指定一个灰度图类型。
     
     图13.3 一幅用Adobe Photoshop创建的灰度图
     一但你画完了你的高度图，你必须将它保存为一个8bit的RAW文件。RAW文件只图像的逐个字节。我们的应用程序可以非常容易的读这样的图像。你的软件可能告诉你保存的RAW文件是有文件头的还是没有文件头的。
     注意：用RAW格式保存高度信息不是必须的；你可以用符合你需要的任何格式。RAW格式是我们能使用的的格式之一。我决定使用RAW格式是因为很多流行的图像编辑器支持导出这种格式，而且应用程序读取RAW文件的数据非常简单。这章中有使用8-bit RAW文件的例子。
     
13.1.2 读取RAW文件
   RAW文件与一段连续的bit内存块没什么分别。我们能用很简单的方法读取这段内存块，注意：变量_heightmap是Terrain类的一个成员，定义如下 ：
std::vector<int> _heightmap;
bool Terrain::readRawFile(std::string fileName)
{
     // A height for each vertex
     std::vector<BYTE> in(  numVertices );
     std::ifstream inFile(fileName.c_str(), std::ios_base::binary);
     if( inFile == 0 )
          return false;
     inFile.read(
          (char*)&in[0], // buffer
          in.size());// number of bytes to read into buffer
     inFile.close();

     // copy BYTE vector to int vector
     _heightmap.resize( _numVertices );
     for(int i = 0; i < in.size(); i++)
          _heightmap[i] = in[i];

     return true;
}
我们COPY一个bytes向量到一个整形向量，这样做我们能够缩放 [0,255]以外的高度。这个方法唯一限制是：RAW文件必须读入至少与地形的顶点数一样多的高度信息。因此，如果你读取一个256x256 的RAW文件，你的地形也必须包含256x256个顶点。
   
13.1.3 访问与修改Heightmap           
     Terrain类提供以下2个方法访问和修改Heightmap的入口。
int Terrain::getHeightmapEntry(int row, int col)
{
      return _heightmap[row * _numVertsPerRow + col];
}

void Terrain::setHeightmapEntry(int row, int col, int value)
{
     _heightmap[row * _numVertsPerRow + col] = value;
}
   这些方法允许我们以行和列来访问入口，并且隐藏方法。当使用它去描述矩阵时，我们必须将一个线性数组编入索引。
（These methods allow us to refer to an entry by row and column and hide the way we must index a linear array when using it to describe a matrix）
   
13.2 生成地形几何数据
   图13.4显示Terrain类的一些属性、词汇和我们提到的一些关键点。我们定义地形的大小，指定每行、每列顶点的数量，和单元的间隔。传递这些值到Terrain类的构造器中。另外，也传递地形所关联的设备，一个包含高度图数据的字符串文件名，一个用来缩放高度图成员的高度缩放值。
   
         图13.4：三角形网络的属性，延着方格线上的点是地形的顶点。
         
class Terrain
{
public:
     Terrain(
          IDirect3DDevice9* device,
          std::string heightmapFileName,
          int numVertsPerRow,
          int numVertsPerCol,
          int cellSpacing,    // space between cells
          float heightScale); // value to scale heights by

     ... methods snipped
private:
     ...device/vertex buffer etc snipped

     int _numVertsPerRow;
     int _numVertsPerCol;
     int _cellSpacing;

     int _numCellsPerRow;
     int _numCellsPerCol;
     int _width;
     int _depth;
     int _numVertices;
     int _numTriangles;

     float _heightScale;
};
Terrain类定义的全部的源代码，实在是太多了，无法在这里全部包含进来。根据传递给构造器的值，我们能够计算Terrain类的其他变量：
```c++
_numCellsPerRow  = _numVertsPerRow - 1;
_numCellsPerCol  = _numVertsPerCol - 1;
_width           = _numCellsPerRow * _cellSpacing;
_depth           = _numCellsPerCol * _cellSpacing;
_numVertices     = _numVertsPerRow * _numVertsPerCol;
_numTriangles    = _numCellsPerRow * _numCellsPerCol * 2;
```
Terrain类定义的顶点结构：
```c++
struct TerrainVertex
{
     TerrainVertex(){}
     TerrainVertex(float x, float y, float z, float u, float v)
     {
          _x = x; _y = y; _z = z; _u = u; _v = v;
     }
     float _x, _y, _z;
     float _u, _v;

     static const DWORD FVF;
};
```
   注意：TerrainVertex是Terrain类内部的一个嵌套类（译者：看样子是结构啊？），之所以这么做，是因为它在Terrain类外部基本没有什么用处。
13.2.1 计算顶点
   在图13.4中，计算三角形网格上的顶点，我们只是在开始产生顶点的地方，一行一行的生成顶点数据，直到结束为止。单元格的顶点与顶点之间有一块空白区域，这会让我们取得x、z坐标，但y坐标是什么呢？得到y坐标很容易，当读取高度图数据结构时会找到对应的入口。
   注意：这个操作使用一个巨大的顶点缓存去保存所有地形上的所有顶点。这可能会引起硬件局限性的问题。例如：一个原始计数界限的最大值和3D设备设定的最大的顶点索引界限。检查MaxPrimitiveCount和D3DCAPS9结构的MaxVertexlndex成员，查看你的设备的限定值，在13.7节讨论，使用顶点缓存时存在问题和解决方法。
   计算纹理坐标，看图13.5，给我们一个简单的设定，允许我们用(u, v)纹理坐标去对应地形顶点坐标。
   
   图13.5：地形顶点与纹理顶点之间一一对应。
   u = j uCoordIncrementSize 
   v = i vCoordIncrementSize 
   And where:
   
   
   最后，用代码生成顶点：
bool Terrain::computeVertices()
{
     HRESULT hr = 0;

     hr = _device->CreateVertexBuffer(
          _numVertices * sizeof(TerrainVertex),
          D3DUSAGE_WRITEONLY,
          TerrainVertex::FVF,
          D3DPOOL_MANAGED,
          &_vb,
          0);

     if(FAILED(hr))
          return false;

     // 对应第一个生成的顶点坐标
     int startX = -_width / 2;
     int startZ =  _depth / 2;

     // 对应最后一个生成的顶点坐标
     int endX =  _width / 2;
     int endZ = -_depth / 2;

     // compute the increment size of the texture coordinates
     // from one vertex to the next.
     float uCoordIncrementSize = 1.0f / (float)_numCellsPerRow;
     float vCoordIncrementSize = 1.0f / (float)_numCellsPerCol;

     TerrainVertex* v = 0;
     _vb->Lock(0, 0, (void**)&v, 0);

     int i = 0;
     for(int z = startZ; z >= endZ; z -= _cellSpacing)
     {
          int j = 0;
          for(int x = startX; x <= endX; x += _cellSpacing)
          {
               // compute the correct index into the vertex buffer
               // and heightmap based on where we are in the nested
               // loop.
               int index = i * _numVertsPerRow + j;

               v[index] = TerrainVertex(
                    (float)x,
                    (float)_heightmap[index],
                    (float)z,
                    (float)j * uCoordIncrementSize,
                    (float)i * vCoordIncrementSize);

               j++; // next column
          }
          i++; // next row
     }

     _vb->Unlock();

     return true;
}
   
   
13.2.2 计算索引-定义三角形
   计算三角形网格的索引，只需要循环访问每一个格子，从左上到右下，如图13.4，并且计算组成格子的2个三角形。

这里的技巧是：提取出计算第ij格子的2个三角形的公式。用图13.6去推导公式，找到第ij的格子：

?ABC = {i · numVertsPerRow + j i·numVertsPerRow + j + 1 (i + 1). numVertsPerRow + j} 
?CBD = {(i + 1) numVertsPerRow + j i·numVertsPerRow + j + 1 (i·l) numVertsPerRow + j + 1} 


图13.6 方格的顶点

代码生成索引：
bool Terrain::computeIndices()
{
     HRESULT hr = 0;

     hr = _device->CreateIndexBuffer(
          _numTriangles * 3 * sizeof(WORD), // 每个三角形有3个索引
          D3DUSAGE_WRITEONLY,
          D3DFMT_INDEX16,
          D3DPOOL_MANAGED,
          &_ib,
          0);

     if(FAILED(hr))
          return false;

     WORD* indices = 0;
     _ib->Lock(0, 0, (void**)&indices, 0);

     // 将组成一个方格的2个三角形的一组6个索引的开始位置编入索引
     int baseIndex = 0;

     // 从头到尾计算每一个格子中的三角形
     for(int i = 0; i < _numCellsPerCol; i++) //行循环
     {
          for(int j = 0; j < _numCellsPerRow; j++) //列循环
          {
               indices[baseIndex]     =   i   * _numVertsPerRow + j;
               indices[baseIndex + 1] =   i   * _numVertsPerRow +
                                                     j + 1;
               indices[baseIndex + 2] = (i+1) * _numVertsPerRow + j;

               indices[baseIndex + 3] = (i+1) * _numVertsPerRow + j;
               indices[baseIndex + 4] =   i   * _numVertsPerRow +
                                                     j + 1;
               indices[baseIndex + 5] = (i+1) * _numVertsPerRow +
                                                     j + 1;

               // next quad
               baseIndex += 6;
          }
     }
     _ib->Unlock();

     return true;
};
   
# 纹理
Terrain类提供2个方法去处理地形的纹理。最简单的方法是简单地读取一个已经制作好的纹理文件并使用它，下面的方法使用Terrain类实现将一个文件读取纹理到_tex成员中，然后指向一个IDirect3DTexture9接口的指针。关键是，在地形渲染之前先用Terrain: :draw方法设置_tex。
```c++
bool Terrain::loadTexture(std::string fileName)
{
     HRESULT hr = 0;

     hr = D3DXCreateTextureFromFile(
          _device,
          fileName.c_str(),
          &_tex);

     if(FAILED(hr))
          return false;
     return true;
}
```
     
13.3.1 程序上的处理方法
   一个可选择的方法是用程序计算地形的纹理，就是说，我们创建一个空纹理，根据定义的参数用代码计算每一个部分的颜色，在例子中，参数是地形的高度。
   我们用Terrain::genTexture方法用程序去生成纹理，首先用D3DXCreateTexture方法创建一个空的纹理，锁定高度级别（top level，纹理图的一个成员，有多个级别），不断的循环每一个texel（图素）并给它上色，texel的颜色取决于与方格对应的高度（近似高度）。我们的想法是：地形中较低的地方是沙滩色，中间的地方像是绿色的小山丘，较高的地方颜色好像雪山。我们定义的高度是方格中左上角的近似高度。
   一旦每个texel都有了颜色，我们想让每一个texel变暗或是变亮，这基于光打在格子中对应的texel上的角度，由Terrain::lightTerrain方法实现。（Once we have a color for each texel, we want to darken or brighten each texel based on the angle at which sunlight (modeled by a directional light) strikes the cell to which the texel corresponds. This is done in the Terrain::lightTerrain method）
   Terrain::genTexture方法通过计算lower mipmap级别的texels来得出结论，它是通过D3DXFilterTexture函数实现。用代码生成纹理：
bool Terrain::genTexture(D3DXVECTOR3* directionToLight)
{
     // Method fills the top surface of a texture procedurally. Then
     // lights the top surface. Finally, it fills the other mipmap
     // surfaces based on the top surface data using
     // D3DXFilterTexture.

     HRESULT hr = 0;

     // texel for each quad cell
     int texWidth  = _numCellsPerRow;
     int texHeight = _numCellsPerCol;

     // create an empty texture
     hr = D3DXCreateTexture(
          _device,
          texWidth, texHeight,  // dimensions
          0,                    // create a complete mipmap chain
          0,                    // usage - none
          D3DFMT_X8R8G8B8,      // 32-bit XRGB format
          D3DPOOL_MANAGED,      // memory pool
          &_tex);

     if(FAILED(hr))
          return false;

     D3DSURFACE DESC textureDesc;
     _tex->GetLevelDesc(0 /*level*/, &textureDesc);

     // make sure we got the requested format because our code
     // that fills the texture is hard coded to a 32-bit pixel depth.
     if( textureDesc.Format != D3DFMT_X8R8G8B8 )
          return false;

     D3DLOCKED_RECT lockedRect;
     _tex->LockRect(0/*lock top surface*/, &lockedRect,
           0 /* lock entire tex*/, 0/*flags*/);

     // fill the texture
     DWORD* imageData = (DWORD*)lockedRect.pBits;
     for(int i = 0; i < texHeight; i++)
     {
          for(int j = 0; j < texWidth; j++)
          {
          D3DXCOLOR c;

          // get height of upper-left vertex of quad.
          float height = (float)getHeightmapEntry(i, j)/_heightScale;

          // set the color of the texel based on the height
          // of the quad it corresponds to.
          if( (height) < 42.5f )       c = d3d::BEACH SAND;
          else if( (height) < 85.0f )  c = d3d::LIGHT YELLOW GREEN;
          else if( (height) < 127.5f ) c = d3d::PUREGREEN;
          else if( (height) < 170.0f ) c = d3d::DARK YELLOW GREEN;
          else if( (height) < 212.5f ) c = d3d::DARKBROWN;
          else                         c = d3d::WHITE;

          // fill locked data, note we divide the pitch by four
          // because the pitch is given in bytes and there are
          // 4 bytes per DWORD.
          imageData[i * lockedRect.Pitch / 4 + j] = (D3DCOLOR)c;
          }
     }

     _tex->UnlockRect(0);

     // light the terrain
     if(!lightTerrain(directionToLight))
     {
           ::MessageBox(0, "lightTerrain() - FAILED", 0, 0);
           return false;
     }

     // fill mipmaps
     hr = D3DXFilterTexture(
          _tex,// texture to fill mipmap levels
          0,   // default palette
           0,   // use top level as source for lower levels
           D3DX_DEFAULT); // default filter

     if (FAILED (hr))
     {
           ::MessageBox(0, "D3DXFilterTexture() - FAILED", 0, 0);
           return false;
     }

     return true;
}

注意：颜色常量BEACH_SAND等定义在d3dUtility.h.文件中。
   
13.4 光照
   Terrain::genTexture方法会调用Terrain::lightTerrain，顾名思义，光照使地形更接近于现实。当我们已经计算完地形纹理以后，我们只需要计算阴影系数（shade factor），使一个定义了光源的地形区域变亮或变暗。在这一节中，我们检验这样一个技巧，你会惊讶于为什么我们照亮地图却没有让Direct3D来做。我们自己来计算有三个好处：
* 内存中不必保存顶点法线。
* 因为纹理是静态的，所以不能随意的移动光源。虽然我们可以重新计算光源，但因此采用Direct3D实时的照亮地形是很耗时的。
* 我们获得了一些数学上的经验，熟悉了一些基本的光照概念，并且是用Direct3D函数实践的。

13.4.1概览(OVERVIEW)
   光照是计算地形阴影（shade）的一个最基本的技巧之一，一般认为的光是漫射光（diffuse lighting），我们定义一个平行光源，指定光的方向，延着光线的相反方向是散发平行光的光源。因此，如果我们想让光线从空中笔直落下，那么lightRaysDirection = (0, -1, 0)，按相反的方向：directionToLight = (0, 1, 0)。注意：创建光照向量要使用单位向量。
   注意：虽然指定方向的光是从光源发射出来的，这么说更直接一点，指定方向的光在计算上要比漫谢光更合得来。
   对于地形中的每个方格，我们计算光的向量与方格的面法线之间的角度。
   在图13.7中我们看到，当角度变得比较大时，方格的面离光源越来越远，接收的光越少。反过来说，角度变小，方格的面则离光源越来越近，相应的会接收更多的光。注意：一旦光向量与法线角度大于90度，表面就接收不到光。
   
   图13.7 光向量与平面法线的关系，我们能够创建一个阴影（shading） 标量，用0..1之间的范围来表示表面能接收到光的多少。使用阴影标量，角度大则标量接近于0。当颜色与一个阴影标量接近0的值相乘时，得到的结果是：颜色变暗。相反，乘以一个阴影标量的值接近1的值时，颜色则接近于原始亮度。
   
13.4.2 计算方格的阴影（Shade）
   光源的方向是一个单位向量，为了计算光源方向与面法线间的夹角，首先需要找到面法线，这是叉积的一小部分应用，但首先必须在方格里找到二个共面的非0并且不平行的向量。看图 13.8有两个这样的向量：
   
   图13.8: 计算在同一方格中的共面的二个向量
   
u = (cellSpacing, by - ay, 0) 
v = (0, cy, -ay, -cellSpacing) 

关于u和v，方格的法线N = u × v，当然要把N标准化：

找到光线与法线的夹角，回忆一下点积，是二个3维空间中的单位向量组成的夹角的余弦。
   
   它的标量是在-1..1的范围，因为-1..0的sin值符合夹角角度且大于90度，在图13.7中接受不到光照，如果它在-1..0之间那么夹角是0度。
float cosine = D3DXVec3Dot(&n, directionToLight);

if(cosine < 0.0f)
     cosine = 0.0f;

   现在s的夹角大于90度，s的阴影标量将在0..1之间。因为光线与法线的角度从0增加到90度时，s的值将从1到降到0。这是我们想要的结果，具体讲解请看13.4.1节。
   给指定的格子计算阴影系数用Terrain::computeShade方法，它需要参数：行和列来确定方格，还有平行方向光的光源。
float Terrain::computeShade(int cellRow, int cellCol,
                            D3DXVECTOR3* directionToLight)
{
     // 取得方格中三个顶点的高度（从高度图中）
     float heightA = getHeightmapEntry(cellRow,   cellCol);
     float heightB = getHeightmapEntry(cellRow,   cellCol+1);
     float heightC = getHeightmapEntry(cellRow+1, cellCol);

     // 创建方格中的二个顶点
     D3DXVECTOR3 u( cellSpacing, heightB - heightA, 0.0f);
     D3DXVECTOR3 v(0.0f, heightC - heightA, - cellSpacing);

     //用方格中的二个向量的叉积找到面法线
     D3DXVECTOR3 n;
     D3DXVec3Cross(&n, &u, &v);
     D3DXVec3Normalize(&n, &n);

     float cosine = D3DXVec3Dot(&n, directionToLight);

     if(cosine < 0.0f)
          cosine = 0.0f;

     return cosine;
}

   
13.4.3 地形阴影（Shading）
   一旦知道了如何给指定的方格加阴影，我们就能给地形上所有的方格加阴影。只要遍例每一个方格，计算方格的阴影值，并测量方格对应的texel颜色。光照少则方格会变暗。下面一段代码展示了Terrain::lightTerrain方法的重要部分：

DWORD* imageData = (DWORD*)lockedRect.pBits;
for(int i = 0; i < textureDesc.Height; i++)
{
     for(int j = 0; j < textureDesc.Width; j++)
     {
          int index = i * lockedRect.Pitch / 4 + j;

          // get current color of cell
          D3DXCOLOR c( imageData[index] );

          // shade current cell
          c *= computeShade(i, j, lightDirection);;

          // save shaded color
          imageData[index] = (D3DCOLOR)c;
     }
}
   
13.5 在地形上“行走”
   构造了一个地形以后，我们想要有移动照相机的能力，以便模拟在地形上行走的效果。我们需要调整照相机的高度，这依赖于地形部分的知识，好的，我们继续往下看。我们首先需要找到照相机所在的方格的位置，并给出x轴和z轴坐标，Terrain::getHeight函数能做到这些，它能提供x轴、y轴坐标参数，返回照相机需要被设置在地形上的高度值，现在看实现部分。
float Terrain::getHeight(float x, float z)
{
     // Translate on xz-plane by the transformation that takes
     // the terrain START point to the origin.
     x = ((float) width / 2.0f) + x;
     z = ((float) depth / 2.0f) - z;

     // Scale down by the transformation that makes the
     // cellspacing equal to one. This is given by
     // 1 / cellspacing since cellspacing * 1 / cellspacing = 1.
     x /= (float) cellSpacing;
     z /= (float)_cellSpacing;
   我们首先转换地形的起始点为原点，然后，我们按反方向去测量空间变量（we scale by the inverse of the cell spacing variable），设置单元空间间隔为1。我们切换到一个新的参考框架，z轴正方向是向下的。当然，没有代码转换参考框架，但现在我们知道+z是向下的。图13.9显示了这些步骤：

图13.9：地形网格在转换前的第一个点，转换后为原点。单元格的空间为1，转换z轴。
我们看到我们转换的坐标系统与矩阵的行和列相对应，也就是说左上为原点，列数的增加向右，行数的增加向下。因此，在图13.9中我们知道了单元格的空间是1，通过以下的方法我们马上就能得到单元格行和列：
float col = ::floorf(x);
float row = ::floorf(z);

换句话说，在x轴部分列是整数，z轴部分行也是整数。回忆floor(t)函数，。
现在我们将取得方格的四个顶点的高度。
        //  A   B
        //  *--*
        //  | / |
        //  *--*
        //  C   D
float A = getHeightmapEntry(row,   col);
float B = getHeightmapEntry(row,   col+1);
float C = getHeightmapEntry(row+1, col);
float D = getHeightmapEntry(row+1, col+1);

   现在我们知道了方格的四个顶点的高度，我们需要找到照相机所在的位置的方格的高度，因为一个方格可能同时向几个方向倾斜，这可能会稍微难一点，见图 13.10:

图13.10: 照相机所在的位置的方格的高度
   为了找到高度，我们需要知道我们在方格中的哪个三角形里。方格是由二个三角形渲染成的，找到我们所在的三角形，我们要取得我们所在的方格并且转换它，它的左上点是原点。
   自从用行和列来描述我们所在的方格左上顶点的位置以来，我们必须转换列x轴与行z轴，转换x、z坐标：
   float dx = x - col;
   float dz = z - row;

图13.11: 我们所在的方格在转换前与转换后，左上顶点变成了原点。
   .现在解释当我们在方格中的上三角形时如何找到高度，这和在下三角形是相似的。马上你会看到这两种情况的代码。在上三角形时，构造2个向量：u = (cellSpacing, B -A, 0) and v = (0, C - A, - cellSpacing)，三角形的边上并且在矢量q = (qx, A, qz)终点点开始的地方，如图13.12(a)。
   
   图13.12 (a) 计算三角形的邻边和对边这两个向量。 (b)使用线性差值创建高度
   注意：我们只关心改变的高度值，我们只修改y值，忽视其他部分，因此，Height=sum A + dxuy + dzvy
   以下是Terrian::getHeight函数的实现代码：
   （Note that since we are only concerned about the interpolated height value, we can just interpolate the y-components and ignore the other components. Thus, the height is obtained by the sum A + dxuy + dzvy.）
if(dz < 1.0f - dx) // upper triangle ABC
     {
          float uy = B - A; // A->B
          float vy = C - A; // A->C

          height = A + d3d::Lerp(0.0f, uy, dx) +
                       d3d::Lerp(0.0f, vy, dz) ;
     }
     else // lower triangle DCB
     {
          float uy = C - D; // D->C
          float vy = B - D; // D->B

          height = D + d3d::Lerp(0.0f, uy, 1.0f - dx) +
                       d3d::Lerp(0.0f, vy, 1.0f - dz);
     }    return height;
}
   
   Lerp函数是一个沿着一维直线的基本线性插值算法，实现如下：
float d3d::Lerp(float a, float b, float t)
{
     return a - (a*t) + (b*t);
}
   
13.6 例子程序: Terrain
   这章的例子是用一个包含高度信息的RAW文件创建一个地形，纹理和光源。用方向键在地形上行走。注意，下列函数中不相关的代码被省略了，被省略的代码用(...)表示，依赖你的硬件，这个例子可能运行得很慢，请偿试运行一个小地形。
   首先，增加全局变量：地形、照相机、每秒帧数。
Terrain* TheTerrain = 0;
Camera   TheCamera(Camera::LANDOBJECT);
FPSCounter* FPS = 0;


   下面是框架函数：
bool Setup()
{
     D3DXVECTOR3 lightDirection(0.0f, -1.0f, 0.0f);
     TheTerrain = new Terrain(Device, "coastMountain256.raw",
                              256, 256, 10, 1.0f);
     TheTerrain->genTexture();
     TheTerrain->lightTerrain(&directionToLight);
     ...

     return true;
}

void Cleanup()
{
     d3d::Delete<Terrain*>(TheTerrain);
     d3d::Delete<FPSCounter*>(FPS);
}

bool Display(float timeDelta)
{
     if( Device )
     {
          // Update the scene:
          ...[snipped input checking]

          // Walking on the terrain: Adjust camera's height so we
          // are standing 5 units above the cell point we are
          // standing on.
          D3DXVECTOR3 pos;
          TheCamera.getPosition(&pos);

          float height = TheTerrain->getHeight( pos.x, pos.z );

          pos.y = height + 5.0f;

          TheCamera.setPosition(&pos);

          D3DXMATRIX V;
          TheCamera.getViewMatrix(&V);
          Device->SetTransform(D3DTS VIEW, &V);

          // Draw the scene:
          Device->Clear(0, 0, D3DCLEAR TARGET | D3DCLEAR ZBUFFER,
                        0xff000000, 1.0f, 0);
          Device->BeginScene();

          D3DXMATRIX I;
          D3DXMatrixIdentity(&I);

          if( TheTerrain )
              TheTerrain->draw(&I, false);

          if( FPS )
              FPS->render(0xffffffff, timeDelta);

          Device->EndScene();
          Device->Present(0, 0, 0, 0);
     }
     return true;
}

   
13.7 一些改进
   Terrain读取顶点数据到一个很大的缓存，在多重的顶点缓存中划分地形结构，在速度和可测量性方面都十分有利。为我们提出一个问题：顶点缓存最大支持多大？回答是，这依赖于你的硬件。所以你必须先检测。
   将地图划分为许多小的顶点缓存是重要的练习，然后将类似矩阵的数据结构编入索引，并且管理数据，这不需要引入新的概念。我们不必详细讨论它。简单的说，你基本上站在地形中一个我们叫做“blocks”的矩阵上,每个block是地形的一个矩形区域。另外，每个block区域（在它自己的顶点索引缓存中）的下方包含地形中的几何信息，为了画它在地形中的位置。
   另外，你可以读取地形到一个很大的ID3DXMesh接口。使用D3D函数D3DXSplitMesh划分地形为许多小的Mesh, 以下是D3DXSplitMesh函数原型：
void D3DXSplitMesh(
    const LPD3DXMESH pMeshIn,
    const DWORD *pAdjacencyIn,
    const DWORD MaxSize,
    const DWORD Options,
    DWORD *pMeshesOut,
    LPD3DXBUFFER *ppMeshArrayOut,
    LPD3DXBUFFER *ppAdjacencyArrayOut,
    LPD3DXBUFFER *ppFaceRemapArrayOut,
    LPD3DXBUFFER *ppVertRemapArrayOut
);
       这个函数将一个源Mesh划分多个小的Mesh,，pMeshIn参数是一个指针，指向想划分的Mesh，pAdjacencyIn指向一个邻接数组，MaxSize参数指定作为结果返回的最大顶点数，为返回的Meshe使用指定的创建标记，pMeshesOut参数返回ppMeshArrayOut数组中的Mesh数量，最后3个参数是可选的（可以指定为null），返回邻接信息的数组。
13.8 摘要
* 我们能用三角形网格和不同的高度值来模拟地形，创建山丘、河流。
* Heightmap数据包含地形顶点的高度值。
* 我们能通过程序使用磁盘上的图像文件生成地形上的纹理。
* 我们能照亮地形，通过计算阴影系数来使每个格子变亮或变暗，阴影系数是由光照在格子上的角度决定的。
* 使照相机在地形上走动，我们需要找到我们站立的三角形。我们计算三角形上的邻边和对边这两个向量，高度是通过…（线性插值在这些向量中每个使用x、z对应的单位向量，以左高顶点为原点为参数。）找到的。 
（The height is then found by linearly interpolating on each of these vectors using the x- and z-coordinates in a normalized cell with an upper-left vertex at the origin as parameters.）

