---
title: UGUI vs NGUI
date: 2017-09-24 10:56:00
tags:
    Unity
---
| Tables   |      NGUI      |  UGUI |
|----------|----------------|------|
| 锚点         | Anchor     | RectTransform Anchor |
| 图片         | Sprite     | Image     |
| 文字         | Label      | Text      |
| 根节点       | UIRoot     | Canvas    |
| UI面板       | Panel      | Canvas    |
| UI容器       | Lets you essentially lower the heightmap resolution used for rendering. |
| heightmapPixelError | An approximation of how many pixels the terrain will pop in the worst case when switching lod. |
| lightmapindex | The index of the lightmap applied to this renderer. |
| terrainData 				| The Terrain Data that stres htightmaps,terrain textures,detail meshes and trees. |
| treeBillboardDistance | Distance from the camera where trees will be rendered as billboards only. |
| terrCrossFadeLength | Total distance delta that trees will use to transition from billboard orientation to to mesh orientation. |
| treeDistance 				| The maximum distance at which trees are rendered. |
| treeMaximumFullLODCount 	| Maximum number of trees rendered at full LOD. |
# NGUI
{% asset_img 1.jpg  %}
上图是UIWidget，UIGeometry和UIDrawCall的关系图
UIPanel用来收集和管理它下面所有的widget的组件。通过widget的geometry创建实际的draw call。没有panel所有东西都不能被渲染出来。可以把UIPanel当作Renderer。

每一个UIWidget都有一个UIGeometry，但是并不都有一个UIDrawCall，而是要通过Batch合并达到减少DrawCall的数量。
所有Panel都有一个Depth值，会影响所有它包含的widget。Panel的depth权重要高于widget的depth权重，但是低于render queue。

当勾选static的时候，该panel下面所有的widget都不会被移动，这样可以提高性能。此时，NGUI会忽略所有的position/rotation/scale改变。所以在运行时移动widget不会有效。

### UIDrawCall

1.成员变量
a) List<UIDrawCall> mActiveList和mInactiveList，mActiveList保持当前激活的UIDrawCall，mInactiveList主要是用于回收UIDrawCall.Destroy()的UIDrawCall，以达到循环利用避免内存的反复申请和释放，减少GC的次数。
b) Material mMaterial和mDynamicMat，mMaterial是图集的Material，mDynamicMat是实际采用的Material，因为UIPanel的Clipping有AlphaClip和SoftClip这两个是要通过切换Shader实现的，所以需要对应动态创建一个Material，这个就是mDynamicMat的存在。
c) bool mRebuildMat和isDirty，这两者表示UIDrawCall所处的状态，当改变UIDrawCall的Material和Shader，mRebuildMat就变成true，就会引起RebuildMaterial()的调用。isDirty若为true，表示UIDrawCall要进行重写“填充”，调用Set函数。

负责将uv、顶点、color等信息输入到mesh中，继而绘制图形。

```C#
void UpdateGeometry()
{
    if (mFilter == null)
    {
        mFilter = gameObject.AddComponent<MeshFilter>();
        mMesh = new Mesh();
        mTriangles = (verts.size >> 2);
        mMesh.verticess = verts.buffer;
        mMesh.uv = uvs.buffer;
        mFilter.mesh = mMesh;
    }
    if (mRenderer == null)
    {
        mRenderer = gameObject.AddComponent<MeshRenderer>();
    }
    UpdateMaterial();
}
```

以上所有的buffer都是用BetterList来存储的。

```C#
void UpdateMaterial()
{
    if (mDynamicMat == null || mRebuildMat || mClipCount != panel.clipCount)
    {
        RebuildMaterial();
        mRebuildMat = false;
    }
    else if (mSharedMat != mDynamicMat)
    {
        mSharedMaterials = new Material[]{mDynamicMat};
    }
}

void RebuildMaterial()
{
    CreateMaterial();
}
```
优化DrawCall
使用相同material的连续UIWidget(UILbale,UISprite)共用一个UIDrawCall。对UIWidget.list进行排序，使得相同的material的UIWidget在UIWidget.list相连，而UIWidget.list是根据UIWidget的depth进行排序。（但是这样不会改变渲染顺序吗？）
1) 修改UIWidget(UILabel,UISprite)的depth，限定好UIWidget.list的排序
2) 重写UIWidget的CompareFunc方法。
夹层问题

### UIRoot
UIRoot的作用是缩放UI

PixelPerfect        保持原分辨率
FiexedSize          根据比例缩放
FixedSizeOnMobile   PC保持原分辨率，Mobile缩放

### UICamera

 带有这个组件渲染出来的物体可以接受NGUI事件

### UIPanel

Panel的集合，一个Panel中包含多个Widget，负责更新会决定何时绘制Widget。

```C#
static public List<UIPanel> list = new List<UIPanel>();
public RenderQueue renderQueue = RenderQueue.Automatic; //渲染次序类型
public int startingRenderQueue = 3000;
public List<UIWidget> widgets = new List<UIWidget>();
public List<UIDrawCall> drawcalls = new List<UIDrawCall>();
int mDepth = 0; // 深度
int mSortingOrder; // 队列排序值
bool mRebuild = false;//如果为true，需要重构所有的Drawcall，Panel中的OnEnable、RemoveWidget、AddWidget等和改变depth会将该值设置为true。
void LateUpdate()
{
    if (mUpdateFrame != Time.frameCount)
    {
        
    }
}
```

{% asset_img 2.jpg LateUpdate调用图 %}