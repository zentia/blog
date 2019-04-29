---
title: Unity Mesh
date: 2018-04-21 14:39:23
tags:
    Unity
---
Mesh是Unity内的一个组件，成为网格组件。

- Mesh网格
- MeshFilter网格过滤器
- Mesh Renderer 网格渲染器

Mesh: 是指模型的网格，建模就是建网格。Mesh的主要属性内容包括顶点坐标，法线，纹理坐标，三角形绘制序列等其他有用属性和功能。因此建网格，就是画三角形；画三角形就是定位三个点。
Mesh Filter: 内包含一个Mesh组件，可以根据MeshFilter获得网格的组件，也可以为MeshFilter设置Mesh内容。
Mesh Renderer: 是用于把网格渲染出来的组件。MeshFilter的作用就是把Mesh扔给MeshRender将模型或者是几何体绘制显示出来。

它们之间的关系大概就是GameObject挂MeshFilter组件，该组件有Mesh属性，该属性存顶点坐标，法线等属性，然后用Mesh Renderer(SkinMeshRenderer)才能将此网格渲染出来。

### Mesh属性

- 顶点坐标(vertex) 顶点坐标数组存放Mesh的每个顶点空间坐标，假设某mesh有n个顶点，则vertex的size为n
- 法线(normal) 法线数组存放mesh每个顶点的法线，大小与顶点坐标对应，normal[i]对应vector[i]的法线
- 纹理坐标(uv) 它定义了图片上每个点的位置的信息，这些点与3D模型是互相联系的，以决定表面纹理贴图的位置，UV就是将图像上每个点精确对应到模型物体的表面uv[i]对应vectex[i]
- 三角形序列(triangle) 每个mesh都由若干个三角形组成，而三角形的三个点就是顶点坐标里的点，三角形的数组的size - 三角形个数 * 3

例如：某mesh有四个顶点0,1,2,3
v0( 1, 1, 0)
v1(-1, 1, 0)
V2( 1,-1, 0)
V3(-1,-1, 0)
那么它们可以组成这样的一个网格
tri[0] = v0,v3,v1, tri[1] = v0, v2, v3

注意：三角形的顶点顺序必须是顺时针，顺时针表示正面，逆时针表示背面，而Unity3D在渲染默认只渲染正面，背面是看不见的。(双面渲染一般用于头发等材质的处理)
那么该三角形可以表示:
	
	tri = new int[2 * 3]{0,3,1, 0, 2, 3};
如何要获取第N个三角形对应的三个顶点坐标，则:V1 = tri[N * 3+0],v2 = tri[N * 3 + 1], v3 = tri[N * 3 + 2]

#### 实例
```C#
using UnityEngine;
using System.Collections;
//注意Unity是左手坐标系！！！，Unity默认是不渲染背面的
public class CreateMesh : MonoBehaviour 
{
    private MeshFilter filter;
    private Mesh mesh;

	void Start () 
    {
        filter = GetComponent<MeshFilter>();
        mesh = new Mesh();
        filter.mesh = mesh;
        InitMesh();
	}
	
    void InitMesh()
    {
        mesh.name = "MyMesh";
        Vector3[] vertics = new Vector3[8]{
            new Vector3(1,1,0),
            new Vector3(-1, 1, 0),
            new Vector3(1,-1,0),
            new Vector3(-1,-1,0),
            new Vector3(1,1,1),
            new Vector3(-1, 1, 1),
            new Vector3(1,-1,1),
            new Vector3(-1,-1,1)
        };
        mesh.vertices = vertics;
        int[] triangles = new int[12 * 3]{
            0, 3, 1, 0, 2, 3,
            4, 7, 5, 4, 6, 7,
            0, 1, 5, 0, 5, 4,
            2, 3, 7, 2, 7, 6,
            0, 4, 6, 0, 6, 2,
            1, 3, 7, 1, 7, 5,
        };
        mesh.triangles = triangles;
        /*Vector2[] uv = new Vector2[4]{
            new Vector2(1,1),
            new Vector2(0,1),
            new Vector2(1,0),
            new Vector2(0,0)
        };
        mesh.uv = uv;*/
    }
}

```
效果如下(一定要加Mesh Renderer因为这玩意是负责渲染的,Unity这货材质缺失就是显示粉色的)
<img src="http://oxcvfpext.bkt.clouddn.com/Unity-Mesh-2.png">

网格已经生成了，接下来就是给网格贴图了，uv属性会直接控制贴显示贴图的哪一部分，以及如何显示贴图uv中的每一项和vertices的每一项都是一一对应的，unity在贴图的时候，会把uv中的每一个点和vertices中对应的索引的顶点一一关联起来，这样可以实现贴图任意形状实现
最终效果
<img src="http://oxcvfpext.bkt.clouddn.com/Unity-Mesh-3.png">

### shared

在源对象上修改属性，非shared每次修改属性会从新生成一个对象。
在编辑器下非运行期如果使用shared会修改本地文件，如果使用非shared会提示内存泄漏。