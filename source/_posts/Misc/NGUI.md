---
title: NGUI
mathjax: true
date: 2019-03-28 21:08:47
tags:
    - NGUI
categories: NGUI
---
# UIDrawCall
UpdateGeometry()通过顶点，UV，颜色，贴图等信息绘制UI图形。
# 填充顶点
UIBasicSprite
```CSharp
protected void Fill(BetterList<Vector3) verts, BetterList<Vector2> uvs, BetterList<Color32> cols, Rect outer, Rect inner)
{
    switch(type)
    {
        case Type.Simple:
            SimpleFill(verts, uvs, cols, outer, inner);
            break;
        case Type.Sliced:
            SlicedFitt(verts, uvs, cols, outer, inner);
            break;
        case Type.Filled:
            FilledFill(verts, uvs, cols, outer, inner);
            break;
        case Type.Tiled:
            TiledFill(verts, uvs, cols, outer, inner);
            break;
        case Type.Advanced:
            AdvancedFill(verts, uvs, cols, outer, inner);
            break;
    }
}
void SimpleFill(BetterList<Vector3> verts, BetterList<Vector2> uvs, BetterList<Color32> cols, Rect mOuterUV, Rect mInnerUV)
{
    Vector4 v = drawingDimensions;
    Vector4 u = MakeDrawingUVs(mOuterUV, mInnerUV);

    verts.Add(new Vector3(v.x, v.y));
    verts.Add(new Vector3(v.x, v.w));
    verts.Add(new Vector3(v.z, v.w));
    verts.Add(new Vector3(v.z, v.y));

    uvs.Add(new Vector2(u.x, u.y));
    uvs.Add(new Vector2(u.x, u.w));
    uvs.Add(new Vector2(u.z, u.w));
    uvs.Add(new Vector2(u.z, u.y));

    if (!mApplyGradient)
    {
        Color32 c = drawingColor;
        cols.Add(c);
        cols.Add(c);
        cols.Add(c);
        cols.Add(c);
    }
    else
    {
        AddVertexColours(cols, 1, 1);
        AddVertexColours(cols, 1, 2);
        AddVertexColours(cols, 2, 2);
        AddVertexColours(cols, 2, 1);
    }
}
```
UIWidget排序函数，大于0的要交换
```CSharp
static public int PanelCompareFunc(UIWidget left, UIWidget right)
{
    if (left.mDepth < right.mDepth) return -1;
    if (left.mDepth > right.mDepth) return 1;

    Material leftMat = left.material;
    Material rightMat = right.material;

    if (leftMat == rightMat) return 0;
    if (leftMat == null) return 1;
    if (rightMat == null) return -1;

    return (leftMat.GetInstanceID() < rightMat.GetInstanceID()) ? -1 : 1;
}
```