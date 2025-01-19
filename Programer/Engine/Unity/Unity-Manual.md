---
title: Unity Manual
date: 2018-04-16 19:01:26
categories:
- Engine
- Unity
---

|||
|--|--|
|public static float Mathf.Round(float f)|四舍五入|

DisallowMultipleComponent 禁止一个脚本被加载多次

### YieldInstruction 中断指令

所有yield指令的基类
参考WaitForSeconds, WaitForFixedUpdate, Coroutine and MonoBehaviour,StartCoroutine

### WaitForSeconds 等待几秒

Namespace: UnityEngine
Inherits from: YieldInstruction

在给定的秒数内，暂停协同程序的执行。
WaitForSeconds只能用在协同程序中的yield声明。

### RequireComponent

当你添加了一个用了RequireComponent组件的脚本，需要的组件将会自动被添加到GameObject上。这个可以有效避免组装错误。在新建类的前面加

	[RequireComponent(typeof(LineRenderer))]

### AssetDatabase.ImportAsset

	static function ImportAsset(path : string, options : ImportAssetOptions = ImportAssetOptions.Default):void

导入指定路径的资源
所有路径都是相对于工程目录文件，例如"Assets/MyTexture/hello.png"

### AssetImporter.GetAtPath

	static function GetAtPath(path: string):AssetImporter

通过指定路径来导入资源

### TextureWrapMode

在进行纹理贴图时，图像会出现在物体表面(u,v)位置上，而这些值在[0.0,1.0]范围内。但是，如果超出这个范围，会发生什么情况呢，这由纹理的映射函数来决定.常见有下面几种：

1. 重复(REPEAT):图像在表面上重复出现。在算法上，忽略纹理坐标的整数部分，并将纹理图的拷贝粘贴在物体表面上。对于大多数复制纹理的使用，在纹理顶部的纹理单元应于底部的纹理单元相匹配，在纹理左侧的纹理单元也应于右侧的纹理单元相匹配。
2. 截取(CLAMP): 将大于1.0的数值设置为1.0，将小于0.0的数值设置为0.0，即将超出[0.0,1.0]范围的数值截取到[0.0,1.0]范围内，这样会导致纹理边缘的重复。
3. 镜像重复(MIRRORED_REPEAT_ARB)
4. 边界截取(CLAMP_TO_BORDER_ARB)
5. 边缘截取(CLAMP_TO_EDGE)

### CameraClearFlag

```C#
public enum CameraClearFlags
{
	Skybox = 1,			// 这是默认设置，在屏幕上空的部分显示当前相机的天空盒。如果当前相机没有设置天空盒，它会默认使用Edit->Render Settings里)中选择天空盒。然后它将退回使用背景颜色，另外天空盒组件可以添加到相机上。
	SolidColor = 2, // 屏幕上的任何空的部分显示当前相机的背景颜色
	Color = 2,
	Depth = 3,			// 该深度的东西永远都不会被裁剪
	Nothing = 4,		// 什么事情都不做
}
```

### Projector

    nearClipPlane       近裁剪面的距离
    farClipPlane        远裁剪面的距离
    fieldOfView         该投影的视野，以度为单位
    aspectRatio         投影的宽高比
    orthographic        投射是正交还是透视
    orthographicSize    在正交模式下投射的一半尺寸
    ignoreLayers        哪个物体层被这个投射器忽略
    material            要投射到每个物体的材质

## Mesh

### Mesh.RecalculateBounds

在修改完顶点后你应该用这个函数以确保包围体是恰当的。赋值三角形将自动重新计算包围体。

### Mesh.RecalculateNormals

重新计算网格的法线
在修改完定点后，通常会更新发现来反映新的变化。发现是根据共享的顶点计算出来的。导入到网格有时不共享所有顶点。例如：一个顶点在一个纹理坐标的接缝处将会被分为两个顶点。因此这个RecalculateNormals函数将会在纹理坐标接缝处创建一个不光滑的法线。RecalculateNormals不会自动产生切线，因此bumpmap着色器在调用RecalculateNormals之后不会工作。然而你可以提取你自己的切线。

## 惯性坐标系

因为在进行物体到世界坐标系的转换时候，即需要平移同时也需要进行旋转，为了简化这种变化，在其中假如惯性坐标系，其中惯性坐标系的原点与本地坐标系的原点重合其轴和世界坐标系平英。那么本地坐标系到惯性坐标系只需要旋转操作（之前也得缩放）再到世界坐标系只需要进行平移。

# 构建AssetBundles

在AssetBundle工作流的文档中，我们有一个代码示例，它将三个参数传递给BuildPipeline.BuildAssetBundles函数。让我们更深入实际的了解。
Assets/AssetBundles:这是AssetBundles输出的目录。您可以将其更改为您想要的任何输出目录，只需确保在尝试构建之前文件夹实际存在。
## BuildAssetBundleOptions
BuildAssetBundleOptions您可以指定几种具有各种效果的不同。

![](Unity-UI.svg)