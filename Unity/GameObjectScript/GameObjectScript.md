原文：http://catlikecoding.com/unity/tutorials/basics/game-objects-and-scripts/

- 用简单的脚本构建一个时钟
- 用C#写脚本
- 让时钟转动起来
- 完成

这边文章通过创建简单的组件脚本来实现一个时钟。您仅需几分钟来熟悉Unity编辑器，如果您已经熟悉此部分，可以跳过本篇文章。
![](tutorial-image.jpg)
#构建一个简单的时钟
打开Unity创建一个3D对象，你不需要任何额外的资源包.
![](default-layout.png)
##1.1 创建一个游戏对象
默认的场景包含两个游戏对象，他们在层次窗口中而且你能看到他们的图标在场景窗口中，第一个是主相机，它是用来渲染的，第二个是方向光，照明场景的。
使用GameObject->Create Empty 选项，也可以选择层级窗口添加一个新的对象在scene上。

<img src="hierarchy.png" alt="层次窗口的时钟对象">

监视窗口(Inspector window)展示游戏对象的细节，当选择时钟对象的时候，Inspector会展示他的name和一些额外的属性,默认为可用，非静态，没有标签属于一个默认的层。并且展示了所有的组件，transform组件是一定有的。

<img src="inspector.png" alt="clock对象的Inspector">
transform组件包括位置，旋转和缩放在3D空间，确保position和rotation是0，scale是1(这里指的都是本地坐标)

## 1.2 创建一个时钟

虽然我们有了clock object,但是我们什么也做不了,我们还得创建3D模型来渲染，Unity包含一些原始的对象，用这些我们能构建一个简单的时钟，s首先添加一个cylinder(圆柱Gameobject->3D Object->Cylinder)
<img src="cylinder.png" alt="">
<img src="cylinder-inspector.png" alt="">

圆柱体有一个Mesh Filter包含一个圆柱体的mesh，第二个是Capsule Collider(胶囊碰撞体)用来搞物理的，第三是Mesh Renderer确保可以得到渲染的，也用来控制用那个材质来渲染，最后一个就是材质(Material).

虽然对象是一个圆柱体，但是它用了capsule collider,因为unity没有原生的cylinder collider,我们不需要它，干掉！如果你不需要物理的话，你可以选择Mesh Collider 组件,组件能被干掉。

为了搞一个时钟的脸，我们必须把这玩意搞平，缩减scale的y值，

<img src="cylinder-scaled-scene.png" alt="">

将圆柱体的名字改为Face,表示clock的脸,
<img src="child-object.png" alt="">

## 1.3 创建时钟刻度

时钟有一圈刻度来告诉你现在几点了。
添加一个立方体对象通过Gameobject->3D Object->Cube,改变缩放(0.5,0.2,1)
刻度是难以看到的，因为他和表盘的颜色相同，让我们通过材质来区分吧，Assets->Create->Material，或者Proect Window都可以的，这个材质是复制默认的材质的，