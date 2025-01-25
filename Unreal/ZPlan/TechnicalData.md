---
title: ZPlan 制作规范
date: 2022-01-05 10:39:00
password: zplan
categories:
- Engine
- Unreal
- ZPlan
tags:
- Engine
- Unreal
- private
---
# 模型
## LOD
1. 模型需要拆分三档LOD，分别是60000面（用于角色界面），30000（用于安卓局内），8000 (用于IOS局内)
2. 面部LOD2不能超过1000面。

其中的面数是指身体和面部的面数总和，而不是单独的面数。这个未来会严格限制。
## 骨骼
1. 目前身体已经定死了。如无必要，禁止在添加附属骨骼，如需添加，需要找程序或者QA测试性能。


## 动画
1. 除了头发和飘带可以用物理骨骼，其它地方不能使用。
2. LevelSequencer Blend Animation会很耗，非必要的情况，不能使用。
![Blend](1.png)
3. /Game/ArtContent/LevelSequence/Songs 这个目录下，禁止有相机相关操作。


## 材质
1. 角色材质目前不能使用半透明，如需开启，需要程序去测试性能。

## 阴影
只有高画质开启，中低画质关闭。
### IOS
一个角色不能超过5个材质，在LOD2
### Android
一个角色不能超过9个材质
# 特效
## LOD
每个特效需要制作LOD
# 场景
## InstanceStaticMesh 用MergeActor代替
https://answers.unrealengine.com/questions/892103/view.html
## 材质
1. 能用无光照的材质就用无光照的材质
2. 有光照的材质不得已为了支持阴影而存在，一般情况下最好不要使用。
3. 上帝之光很耗，会画两边，只能在高画质中开启，在中低画质需要使用StaticMeshGroup中关闭，这个会用自动化工具去检查。
4. SSR也很耗，目前只在Android中开启，每个需要用到的场景需要程序或者自动化测试工具测试该场景在不开启SSR的时候的GPU的耗时低于6ms才可以使用。

`Alt+8`预览材质复杂度，只有绿色才是达标的。其他颜色均不达标，需要找程序或者TA去优化材质。
![Blend](2.png)
## StaticMeshGroup
切换到Shader复杂度视图，对于红色以上的材质，必须在StaticMeshGroup中关掉。
## 面数
1. 场景面数理论不能超过60000面。
2. 场景的所有Static Mesh会有LOD，分别是1，0.5，0.25，未来在自动导入Static Mesh的时候会自动设置上。现在需要手动设置一下。
## 裁剪
我们目前没有开启视距裁剪。未来场景大的情况下需要开启。这个需要和美术讨论一下。
## 后处理
### DepthOfField(景深)
现在景深是没有控制的，因为认为景深不会一直开启的。
### Bloom(泛光)
泛光的开启和关闭是表格中配置的。
### FXAA(快速抗锯齿)
同样也是在表格中配置的。
### MobileHDR(Tonemapper)
一直在开启的，不开启效果很拉闸
### AO
美术需要手动关闭。
## 天光
只有高画质会开，中低画质会关闭。
# UI
1. 大的面片(Image会很耗，尽量避免，看不到的东西就删掉吧，不要去绘制)
2. Mask那个组件很耗，不要一直开着。
3. 看不到的界面就不要显示了。未来通过overdraw的方式去检查。

# 自动化
1. 自动将InstanceStaticMesh转换成MergeActor
2. 自动对StaticMesh进行LOD化
3. 自动扫描光照图大小
4. 自动扫描角色模型，贴图大小，LOD规范。

# Zplan&手Q蒙皮规范

{% pdf Zplan&手Q蒙皮规范.pdf %}

# zplan制作文档-角色

{% pdf zplan制作文档-角色.pdf %}