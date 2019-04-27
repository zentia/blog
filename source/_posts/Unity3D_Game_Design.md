---
title: Unity
date: 2018-07-01 08:47:00
tags:
    Unity
---

## 命名

1. 模型命名 Max文件中角色模型命名为skin@sk_001武器模型命名为skin@wq_001
2. 颜色贴图skin@st_001.png 高光贴图skin@st_001_ctr.png

## 导入导出

### 能够从3DsMax导入Unity3D的资源

- 所有节点的位移、旋转、锁方、轴心和命名都将导入
- 网格模型包含顶点颜色、法线，一到两个UV通道
- 材质包含贴图以及漫反射颜色，以及单个网格的多维材质
- 动画（物体移动、旋转、缩放）
- 骨骼动画（角色动画）

## 角色模型制作规范

### 模型

1. 选人界面模型面数约4500-5000左右
2. 游戏内主角模型面数2500-3000左右
3. Boss模型面数根据体型大小3000-5000左右
4. Npc模型面数900-1200造型简单面数没下限
5. 武器模型面数250-500 造型简单面数没下限
6. 模型在不是非常废面的情况下，转折的地方做出厚度，厚度适当夸张。
7. 地面部分制作成双面或是封闭，小布条类模型做成双面模型
8. 角色结构整体均衡 比如眼睛部分不需要细致刻画

### 贴图

主角 512\*512 png
Boss 512\*512 png
Npc 256\*256 png

武器体积较大的使用256\*256 体积较小 128\*128 png

#### 尺寸设定

制作模型前必须先确保模型尺寸，统一将显示单位与系统单位改为公尺。

### 注意正反面关系

每个模型都包含正面和反面，一般来说引擎只会显示模型的正面，如要反面也一起显示需透过shader来实现，但增加反面显示时会降低性能，因此尽量让模型单面显示就好，而在制作模型时必须确保要显示的模型面是正面即可。

### 不要存在破面和迭面

制作模型时注意顶点是否焊接好，未焊接的顶点可能会导致模型有破面的情形，另外模型面与面之间最好保持距离，不然会发生迭面的情况。

### 不要存在多余的顶点和T点

删除一些没用的顶点，并且避免出现T点。

### 删除看不到的面

制作模型时有些面可能在低下或是在模型里面，都是在场景进行中看不到的面，这些面可以删除，以减少模型面数提高效能。

### 物体的轴心

每个物体都有一个坐标轴心，宜据物体属性设计好物体轴心位置，一般来说轴心都会放置在中心的位置，其他情况如门的轴心可能会在低下角落的位置。
### 使用简单几何体来制作复杂对象

当遇到比较复杂的对象，且对象比较小，左乘模型反而增加了模型的面数，因此可以使用简单的平面模型结合透空贴图来实现复杂的造型，可以将图片存成PSD、PNG、TGA这种带有透明通道属性的贴图档。

### 重置模型（ResetXForm）

制作模型时可能使用了缩放功能或镜像功能，这些指令会使得模型比例错乱以及出现负数轴向的情形，因此在模型制作完毕时最好进行ResetXForm指令，让模型回归最基本的状态。

### 制作虚拟碰撞体

可以为模型制作简单的几何体来当作物体的碰撞体，可以依据物体的重要性与精确性来设计几何体外观。

## 材质贴图和UV

### Unity3D支持的Max材质

Unity3D支持3dsmax的Standard(标准材质)与Muti/Sub-Object(多维/子物体材质)。

### 模型汇入Unity后没有贴图？

当模型汇入Unity前，可以先创建一个Texture文件夹，并事先将贴图导入，这样模型汇入后就会自动读取到贴图。

### 模型塌陷

制作模型时，建议将模型以一栋建筑进行塌陷，接着对模型进行UV拆解，这样可以将一个模型使用一张贴图来完成。

### 避免拉伸UV

调整UV前先将材质球使用棋格 (checkter) 贴图来观看UV比例，调整UV时尽量让棋格维持正方形的形状。

### 贴图尺寸

贴图必须是2的N词放（8、16、32、64、128、256），建议贴图不要超过1024\*1024大小。

### 绘制UV出血

绘制材质时绘制出血边缘，可避免出现明显接缝线。

### 透空贴图在Unity里的使用

### 透空贴图毛边处理技巧

### 透空贴图毛边处理技巧（进阶）

### 材质与贴图命名

## 烘培贴图教学

### 3dsMax LightMap 烘培贴图

### 3dsMax CompleteMap 烘培贴图

## Unity烘培贴图

## Unity3D地形制作

### 地形编辑器基本功能介绍

### 水系统介绍

### 天空盒系统介绍

### 第一人称导览

## 3dsMax角色动画导入与基本控制

### 单个角色动画导入

### 多个角色动画导入

### 角色基本控制

## 第三人称角色控制

### 角色阴影

## 树木与粒子系统

### 树木制作

## 粒子系统-繁星粒子效果

## 粒子系统-雪花粒子效果

## 粒子系统-火焰粒子效果

## 编辑界面介绍

### 创建工程

### 打开工程

### Project视图

### Hierarchy视图

### Inspector视图

### Scene视图

#### 视图介绍

#### 移动视图

#### 移动模型

#### 旋转模型

#### 缩放模型

#### 场景工具

#### Sence视图控制条

#### Game视图

#### Game视图控制条

## 工程的打包

### 导出与导入

### 游戏案例1

## Android和iOS版本发布

## Unity3D脚本概述

### Unity3D脚本概述

### Unity3D的基本语法

#### 访问其他组件

#### 访问其他游戏对象

#### 向量

#### 成员变量与全局变量

#### 实例化

#### 协同程序与中断

#### 创建游戏对象

#### 获取游戏对象

#### 添加组件与修改组件

#### 脚本组件

#### 克隆游戏对象

#### 时间

```C#
void OnGUI()
{
	GUILayout.Label("当前游戏时间：" + Time.time);
	GUILayout.Label("上一帧所消耗的时间：" + Time.deltaTime);
	GUILayout.Label("固定增量时间：" + Time.fixedTime);
	GUILayout.Label("上一帧所消耗的固定时间：" + Time.fixedDeltaTime);
}
```

#### 随机数

#### 四元数

## Unity3D GUI界面设计（GUI&NGUI&UGUI）

### UGUI的网格重建、动静分离
动静分离：也就是说同一个界面下的UI，可活动的元素放在一个Canvas下，不可活动的元素放在另一个Canvas下。虽然两个Canvas打断了合批，但是却减少了网格重建时间，总体上是有优化的。

究其原因，是因为在同一个Canvas下的某个元素发生变化时，同一Canvas下的所有元素都会进行网格重建(ReBatch)。井静态的元素在逻辑上是不需要重建的，因为他们都没变过，所以需要分开。

#### 

### GUI常用控件

#### GUI Text

#### GUI Texture (LazyTexture)

#### Label控件

#### Button控件

#### TextField

#### ToolBar

#### Slider

#### ScrollView控件

#### 群组视图

#### 窗口

#### NGUI Panel & UGUI Canvas

#### 图集制作(TexturePacket)

#### UITween动画

#### 顶点动画

### GUI Skin

#### 自定义风格组件

## Unity3D输入与控制

### 键盘事件

#### 键盘按下事件

#### 键盘抬起事件

#### 键盘长按事件

### 触摸事件

#### 触摸按下事件

#### 触摸抬起事件

#### 触摸长按事件

### 鼠标事件

#### 鼠标按下事件

#### 鼠标抬起事件

#### 鼠标长按事件

### 自定义事件

#### 输入管理器（InputHelper）

#### 自定义按键事件

#### 自定义按键轴

#### 实例练习

### 角色模型与动画（Animation&Animator）

#### 模型的载入

#### 设置角色动画

#### 播放3D动画

#### 动画裁切

#### 动画的帧

#### Tween

### 多媒体

#### 音频

#### 视频

