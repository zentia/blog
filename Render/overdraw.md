# Overdraw
在某个背景上有个按钮，要将按钮绘制在背景上，这个就是Overdraw，Overdraw无法避免，只能优化降低

# 性能参数
- 总填充数峰值：单帧总填充像素数量最大值
- 填充倍数峰值：单帧最大填充倍数（10.0X就是该帧刷新10遍）
- 单帧填充倍数：该帧总填充数/该帧渲染相机分辨率

# 优化方案
- 控制绘制顺序：PC上资源无限，一般都是从后往前绘制，但在移动上，尽量从前往后绘制。在Unity中，那些Shader中被设置为“Geometry”队列的对象总是从前往后绘制的，而其他固定队列（如"Transparent"，"Overlay"）的物体，则都是从后往前绘制的。这意味着，我们可以尽量把物体的队列设置为"Geometry"。
- 尽量减少过度绘制区域：实在需要多层绘制的地方，要尽量减少各部分过度绘制区域，使重合区小，绘制的像素点也就少一点。
- 过大的不必要的绘制尽量代码实现：例如点击屏幕空白区域返回功能，加透明image会增加很多
- UI设计上尽可能减少重叠

## 文字部分

主要原因是使用了Outline，Outline实现方式是将Text的四个顶点传过去复制4份，设置4份偏移量实现效果，将偏移量设置很大之后，可以看到一个Text周围有4个相同的Text

```CSharp
public class Outline : Shadow
{
    protected Outline()
    {}

    public override void ModifyVertices(List<UIVertex> verts)
    {
        if (!IsActive())
            return;

        var start = 0;
        var end = verts.Count;
        ApplyShadow(verts, effectColor, start, verts.Count, effectDistance.x, effectDistance.y);// 右上

        start = end;
        end = verts.Count;
        ApplyShadow(verts, effectColor, start, verts.Count, effectDistance.x, -effectDistance.y); // 右下

        start = end;
        end = verts.Count;
        ApplyShadow(verts, effectColor, start, verts.Count, -effectDistance.x, effectDistance.y); // 左上

        start = end;
        end = verts.Count;
        ApplyShadow(vertx, effectColor, start, verts.Count, -effectDistance.x, -effectDistance.y); // 左下
    }

    protected void ApplyShadow(List<UIVertex> verts, Color32 color, int start, int end, float x, float y)
    {
        UIVertex vt;

        var neededCpacity = verts.Count * 2;
        if (verts.Capacity < neededCapacity)
            verts.Capacity = neededCapacity;

        for (int i = start; i < end; ++i)
        {
            vt = verts[i];
            verts.Add(vt);

            Vector3 v = vt.position;
            v.x += x;
            v.y += y;
            vt.position = v;
            var newColor = color;
            if (m_UseGrahpicAlpha)
                newColor.a = (byte)((newColor.a * verts[i].color.a) / 255);
            vt.color = newColor;
            verts[i] = vt;
        }
    }
}
```

解决方法
1. 不使用或者使用Shadow(Shadow通过为图像或者文字的mesh添加顶点实现阴影效果，Outline继承Shadow，在对象四个角个添加一个Shadow)
2. 使用Textmesh Pro(Unity5.5)需要制作相应的字体文件，对于动态生成的文字效果不好，固定字体很好。
3. 修改Mesh的UV坐标，提取文字原始UV坐标，扩大文字绘图区域，对文字纹理周围像素点采样，新旧颜色融合

## Mask组件

Unity的Mask组件会增加一层Overdraw，还会增加4个DrawCall
解决方法：
1. 使用RectMask2D代替，缺点是只能用于矩形
2. 对于多边形，用MeshMask，红色为UnityMask，蓝色是MeshMask，UnityMask消耗15个DrawCall，Overdraw2次，MeshMask消耗1个DrawCall，1层OverDraw
   
## Image的slide属性
对于slide九宫格图片，可以看情况取消fill center属性，那样中心区域不会渲染，中心区域也就镂空，重合面积也会小。

## 无用的Image

