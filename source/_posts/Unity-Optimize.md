---
title: Unity优化
date: 2018-04-12 14:51:15
tags:
  - Unity
---

1. 更新不透明贴图的压缩格式为ETC 4bit，因为android市场的手机中的GPU有多种，每家的GPU支持不同的压缩格式，但他们都兼容ETC格式。
2. 对于透明贴图，我们只能选择RGBA 16bit 或者RGBA 32bit。
3. 减少FPS，在ProjectSetting-> Quality中的VSync Count 参数会影响你的FPS，EveryVBlank相当于FPS=60，EverySecondVBlank = 30；
这两种情况都不符合游戏的FPS的话，我们需要手动调整FPS，首先关闭垂直同步这个功能，然后在代码的Awake方法里手动设置FPS（Application.targetFrameRate = 45;）
降低FPS的好处：
1）省电，减少手机发热的情况；
2）能都稳定游戏FPS，减少出现卡顿的情况。
4. 当我们设置了FPS后，再调整下Fixed timestep这个参数，这个参数在ProjectSetting->Time中，目的是减少物理计算的次数，来提高游戏性能。
5. 尽量少使用Update LateUpdate FixedUpdate，这样也可以提升性能和节省电量。多使用事件（不是SendMessage，使用自己写的，或者C#中的事件委托）。
6. 待机时，调整游戏的FPS为1，节省电量。
7. 图集大小最好不要高于1024，否则游戏安装之后、低端机直接崩溃、原因是手机系统版本低于2.2、超过1000的图集无法读取、导致。2.2 以上没有遇见这个情况。注意手机的RAM 与 ROM、小于 512M的手机、直接放弃机型适配。

## VSCount 垂直同步

Unity3D中新建一个空场景的时候，帧速率（FPS总是很低），大概在60~70之间。在Unity3D中当运行场景打开Profiler的时候，我们会看到VSync 这一项占了很大的比重。
这个是什么呢，这个就是垂直同步。
我们可以关闭VSync来提高帧速率，选择edit->project settings->Quality。
在右侧面板中可以找到VSync Count,把它选成Don't Sync。
这就关闭了VSync(垂直同步)，现在在运行场景看看，帧速率是不是提高很多。
现在来说说什么是垂直同步，要知道什么是垂直同步，必须要先明白显示器的工作原理，
显示器上的所有图像都是一线一线的扫描上去的，无论是隔行扫描还是逐行扫描，
显示器都有两种同步参数——水平同步和垂直同步。

什么叫水平同步？什么叫垂直同步？
垂直和水平是CRT中两个基本的同步信号，水平同步信号决定了CRT画出一条横越屏幕线的时间，
垂直同步信号决定了CRT从屏幕顶部画到底部，再返回原始位置的时间，
而恰恰是垂直同步代表着CRT显示器的刷新率水平。

为什么关闭垂直同步信号会影响游戏中的FPS数值？
如果我们选择等待垂直同步信号（也就是我们平时所说的垂直同步打开），
那么在游戏中或许强劲的显卡迅速的绘制完一屏的图像，但是没有垂直同步信号的到达，
显卡无法绘制下一屏，只有等85单位的信号到达，才可以绘制。
这样FPS自然要受到操作系统刷新率运行值的制约。

而如果我们选择不等待垂直同步信号（也就是我们平时所说的关闭垂直同步），那么游戏中作完一屏画面，
显卡和显示器无需等待垂直同步信号就可以开始下一屏图像的绘制，自然可以完全发挥显卡的实力。
但是不要忘记，正是因为垂直同步的存在，才能使得游戏进程和显示器刷新率同步，使得画面更加平滑和稳定。
取消了垂直同步信号，固然可以换来更快的速度，但是在图像的连续性上势必打折扣。
这也正是很多朋友抱怨关闭垂直后发现画面不连续的理论原因。

合并材质球Unity3D中每导入一次模型就多一个材质球，可我的这些模型都是共用一张贴图的就想共用一个材质球，所以每次都要删除再附上，很麻烦。怎么才能合并这些材质球？
采用TexturePacking吧

1. 遍历gameobject，取出material，并根据shader来将material分类；
2. 调用Unity自带的PackTextures函数来合并每个shader分类中的material所对应的textures（PackTextures函数有缺陷，不过可以将就用）；
3. 根据合并的大的texture来更新原有模型的texture、material已经uv坐标值。

需要注意的是：需要合并的纹理应该是物体在场景中距离相近的，如果物体在场景中的距离较远，
则不建议合并纹理，因为这样做很有可能非但起不到优化的作用，反而降低了运行效率。 

mesh合并 
分为2种方式合并

1. 自带的合并必须勾选静态。

   所有被勾选了“Static”的GameObject，其中的Mesh Filter中的mesh都会被合并到 "Combined Mesha (root: scene)" 中

2. 也可以用脚本来合并mesh 。
```C#
using UnityEngine;
using System.Collections; 

public class  MyClass : MonoBehaviour
{

    void Start ()
    {

        MeshFilter [] meshFilters = GetComponentsInChildren<MeshFilter> ();
        CombineInstance[] combine = new CombineInstance[meshFilters.Length];
        for(int i = 0; i < meshFilters.Length; i++) 
        {
            combine[i].mesh = meshFilters[i].sharedMesh;
            combine[i].transform = meshFilters[i].transform.localToWorldMatrix;
            meshFilters[i].gameObject.active = false;
        }
        transform.GetComponent<MeshFilter>().mesh = new Mesh ();
        transform.GetComponent<MeshFilter>().mesh.CombineMeshes (combine);
        transform.gameObject.active = true;
    }
}
```
1. 先在 Unity 中建立空物件 ( Empty ) 
2. 再创建2个 Cube 方块，并放入空物件底下 (可以改成你自己的模型)
3. 把 MyClass 代码丟进空物件上 。
4. (可选) 建立一个 Material 材质，并且丢进空物件上
5. 执行
 
1. 角色Material数量2-3个，骨骼数量小于30个，面片数量300-1500，一般角色应该没有IK结点
这是因为角色的动作大多数都是事先设定好的，并不需要经过IK操作来进行实时计算（Rogdoll除外），所以在模型导入时，不要将IK结点一起导入。
2. 静态实体不要附加Animation Component
在静态实体上附加Animation部件虽然对结果没有影响，但却会增加一定的CPU开销来调用这一组件，所以尽量去掉该组件。
网格顶点数小于500；UV值范围尽量不要超过（0, 1）区间；尽量保证UV值不越界，这对于将来的纹理拼合优化很有帮助。
3. 地形
地形的分辨率大小
长宽均尽量小于257。这是因为地形太大，会造成大量顶点数据，给你的内存带宽造成一定的影响，在目前的ios设备中，内存带宽是非常有限的，需要尽量节省。同时，如果用Unity自带的地形，一定也要使用Occlusion Culling，因为Unity的刷地形工具虽然方便，但却是framekiller，刷过之后，你会发现drawcall增加的非常多。
混合纹理数量
不要超过4。地形的混合操作是很耗时的，应该尽量避免。能合并的纹理尽量合并。
4. 纹理
- 纹理格式
建议png或tga。不用转成ios硬件支持的PVRTC格式，因为Unity在发布时会帮你自动转的。
- 纹理尺寸
长宽小于1024。同时应该尽可能地小，够用就好，以保证纹理对内存带宽的影响达到最小。
- 支持Mipmap（UI不需要转，不然会增大内存）
建议生成Mipmap。虽然这种做法会增加一些应用程序的大小，但在游戏运行时，系统会根据需求应用Mipmap来渲染，从而减少内存带宽。
- 检查Alpha值
如果纹理的alpha通道均为1，则用RGB的24位纹理来代替RGBA的32位纹理。（据说Unity内部会进行自动检测）
5. 光源
光源“Important”个数建议1个，一般为方向光。“Important”个数应该越小越少。个数越多，drawcall越多。
Pixel Light数目1-2个。
6. 粒子特效
屏幕上的最大粒子数建议小于200个粒子。
每个粒子发射器发射的最大粒子数建议不超过50个。
粒子大小如果可以的话，粒子的size应该尽可能地小。因为Unity的粒子系统的shader无论是alpha test还是alpha blending都是一笔不小的开销。同时，对于非常小的粒子，建议粒子纹理去掉alpha通道。
尽量不要开启粒子的碰撞功能。
非常耗时。
7. 音频
游戏中播放时间较长的音乐（如背景音乐）使用.ogg或.mp3的压缩格式。
较短音乐（如枪声）使用.wav和.aif的未压缩音频格式。
8. 相机
裁剪平面
将远平面设置成合适的距离。远平面过大会将一些不必要的物体加入渲染，降低效率。
根据不同的物体设置不同的远裁剪平面
Unity提供了可以根据不同的layer来设置不同的view distance，所以我们可以实现将物体进行分层，大物体层设置的可视距离大些，而小物体层可以设置地小些，另外，一些开销比较大的实体（如粒子系统）可以设置得更小些等等。
9. 碰撞
尽量不用MeshCollider
如果可以的话，尽量不用MeshCollider，以节省不必要的开销。如果不能避免的话，尽量用减少Mesh的面片数，或用较少面片的代理体来代替。
10. 其他
Drawcall
尽可能地减少Drawcall的数量。IOS设备上建议不超过100。减少的方法主要有如下几种：Frustum Culling，Occlusion Culling，Texture Packing。Frustum Culling是Unity内建的，我们需要做的就是寻求一个合适的远裁剪平面；Occlusion Culling，遮挡剔除，Unity内嵌了Umbra，一个非常好OC库。但Occlusion Culling也并不是放之四海而皆准的，有时候进行OC反而比不进行还要慢，建议在OC之前先确定自己的场景是否适合利用OC来优化；Texture Packing，或者叫Texture Atlasing，是将同种shader的纹理进行拼合，根据Unity的static batching的特性来减少draw call。建议使用，但也有弊端，那就是一定要将场景中距离相近的实体纹理进行拼合，否则，拼合后很可能会增加每帧渲染所需的纹理大小，加大内存带宽的负担。这也就是为什么会出现“DrawCall降了，渲染速度也变慢了”的原因。

非运动物体尽量打上Static标签
Unity在运行时会对static物体进行自动优化处理，所以应该尽可能将非运行实体勾上static标签。

场景中尽可能地使用prefab
尽可能地使用prefab的实例化物体，以降低内存带宽的负担。检查实体的PrefabType，尽量将其变成PrefabInstance，而不是ModelPrefabInstance。

移动平台相对于PC机，具有体积小，计算弱，带宽少的特点。

因此做手机游戏的开发，优化的方向，与力度对比PC游戏都有所区别。

必须要做到优化流程，合理利用资源。
目前在手机上面，还不能够像PC游戏那样追求高质量渲染效果，为了让手机不那么容易发烫，还要控制cpu，gpu，不能让他们全速运算。

材质方面：
纹理方面，建议使用压缩纹理，

Android上面使用ETC1，苹果上面使用PVRTC。
UV坐标控制在0到1之间，人物模型面数控制在1500内，骨骼控制在30个以内。
场景中使用一个主光（不能再多了）。
尽量减少alphaTest和alphaBlend材质的使用。在手机上，这是很杀效率的。
骨骼动画方面：
在动画方面可以考虑不使用插值，固定的帧率的动画。

如果要做插值，考虑使用四元数（表示旋转）和向量（表示位移）来做插值。

四元数做插值速度比矩阵来的快，Slerp提供了平滑插值。

优化的常规技巧
剖析你的游戏。
不要花费时间来优化那些晦涩的代码或者缩减图形文件的大小，除非这是你游戏的瓶颈。
第一次剖析你的游戏将会使你发现你游戏的瓶颈。Apple's Shark是一个很好的用来剖析基于OpenGL的程序的工具。
再次剖析你的游戏。
优化之后不要忘记再剖析一次你的游戏，这样可以检查你所做的优化是否达到了预期的效果。
当然，这样做也可能会使你发现更多的瓶颈。
流程第一、性能第二。花费时间来使你游戏的创建尽可能地流畅。
尽可能快地修正游戏中的错误将会使你后期更容易优化你的游戏。
在Scene View中测试场景。
这样做将会使你清楚了解这个场景中的物体或者附加在物体上的脚本是否降低了游戏性能。
如果Scene View反应迟钝，那么有可能是图形方面的原因，如果Scene View反应不迟钝，那么瓶颈可能出在脚本或者物理系统上。
禁用指定游戏物体。
在play模式下，尝试禁用并启用游戏物体来排查出游戏慢的原因。

网格
如果可能的话，把相邻的物体（网格）合并为一个只有一个材质的物体（网格）。比如，你的游戏中包含一个桌子，上面有一堆东西，你完全可以在3D程序中将它们合并在一起（这可能也需要你将这些物体的纹理合并为一个大的纹理集）。减少需要渲染的物体的数量可以极大地提高游戏性能。

不要有不必要的网格。
如果你的游戏场景中有一个人物，那么他应该是一个网格。如果你有一个船，那么它也应该只是一个网格。
每一个网格只用一种材质。
使用极少的面数的网格（比如500个多边形以下）。
最好把你人物的三角面数量控制在1500-2000个之间。
这个数量可以说是游戏质量和性能之间一个均衡值。如果你的模型有四边形，那么在导入模型的时候，引擎将会把每个四边形变为两个三角形。

光照
像素光。
像素光可以让你的游戏看起来效果很牛逼，但是不要使用过多的像素光。
在你的游戏中可以使用质量管理器来调节像素光的数量来取得一个性能和质量的均衡点.

性能占用顺序：聚光灯>点光源>平行光。
一个好的点亮场景的方法就是先得到你想要的效果，然后看看哪些光更重要；
在保持光效的前提下看看哪些光可以去掉。

点光源和聚光灯只影响它们范围内的网格。
如果一个网格处于点光源或者聚光灯的照射范围之外，并且光源的attenuate开关是打开的，那么这个网格将不会被光源所影响，这样就可以节省性能开销。
这样做理论上来讲可以使用很多小的点光源而且依然能有一个好的性能，因为这些光源只影响一小部分物体。
一个网格在有8个以上光源影响的时候，只响应前8个最亮的光源。

贴图
在外观不变的前提下，贴图大小越小越好。
如果你的显卡的显存不够大的话，你游戏中的贴图将会被转存到系统内存中，在显卡调用它们的时候再传到显卡中。
对于比较新的电脑来说，内存和显卡之间有足够的带宽来达到一个很好的性能；
如果你很无耻地用了巨多的大图片的话，在低显存的电脑上运行你的游戏的时候，你的游戏必然会挂掉。
倒是没有必要在图形编辑软件中调整贴图的大小。你可以在unity导入贴图的时候进行调整。

不要使用低质量的图片。
在小播放界面的游戏中使用低质量的jpeg图片或者低色彩的png图片亦或是gif图片没什么问题。
在发布游戏的时候，引擎会自动压缩这些图片，多重压缩和解压将会降低图片的质量，所以最好保持贴图文件的分辨率为原始分辨率。
这样就会减少多重压缩和解压所导致的图片失真现象。

Shaders
多重效果的shader就比看起来样式很单一的shader要更耗费资源。
同样在一个拥有贴图和光反射的物体上，使用VertexLit Diffuse shader无疑是最省资源的。

在美术制作场景的过程中，会使用到大量的粒子系统。
比如场景中的火把。在我们的一个地下城场景中，美术们放置了大量的火把。整个场景中的各个地方，有100来个火把。

unity中，在摄像机范围外的粒子系统虽然不会被绘制。
但是update是一直持续的。这也就意味着，这100多个火把，不论是否可见都在更新。

这个设计应该是很不合理的，在我看过的其他引擎中，都会有一个开关，来控制不可见的粒子系统是否需要update。
有的粒子系统在不可见的时候需要更新,比如爆炸。有的不需要更新，比如火堆火把。

为了避免不必要的update开销，尤其是最后游戏是要发布到页游平台（web player只能使用一个cpu的核）。
于是写了一个脚本，控制不可见的粒子系统就不更新。

该脚本主要是用到了2个MonoBehaviour的函数。
OnBecameInvisible() 当变为不可见   和   OnBecameVisible() 当变成可见。 

要这2个函数起作用的前提是，该GameObject绑定了MeshRender组件。
所以，我们要在粒子系统的GameObject放置在一个GameObject  下，且给该GameObject绑定一个MeshRender 与 MeshFilter。
MeshFilter中的mesh可以随便找个cube。

在Start（） 的时候，把最GameObject的scale设置为很小，以保证该cube不被看见。
其实遍历所有的child，把active设置为false。

在OnBecameVisible 中 遍历所有child，把active设置为true。
在OnBecameInvisible中 遍历所有child，把active设置为false。

Unity 性能优化 Draw Call 

Unity（或者说基本所有图形引擎）生成一帧画面的处理过程大致可以这样简化描述：引擎首先经过简单的可见性测试，确定摄像机可以看到的物体，然后把这些物体的顶点（包括本地位置、法线、UV等），索引（顶点如何组成三角形），变换（就是物体的位置、旋转、缩放、以及摄像机位置等），相关光源，纹理，渲染方式（由材质/Shader决定）等数据准备好，然后通知图形API——或者就简单地看作是通知GPU——开始绘制，GPU基于这些数据，经过一系列运算，在屏幕上画出成千上万的三角形，最终构成一幅图像。

在Unity中，每次引擎准备数据并通知GPU的过程称为一次Draw Call。这一过程是逐个物体进行的，对于每个物体，不只GPU的渲染，引擎重新设置材质Shader也是一项非常耗时的操作。因此每帧的Draw Call次数是一项非常重要的性能指标，对于iOS来说应尽量控制在20次以内，这个值可以在编辑器的Statistic窗口看到。

Unity内置了Draw Call Batching技术，从名字就可以看出，它的主要目标就是在一次Draw Call中批量处理多个物体。只要物体的变换和材质相同，GPU就可以按完全相同的方式进行处理，即可以把它们放在一个Draw Call中。Draw Call Batching技术的核心就是在可见性测试之后，检查所有要绘制的物体的材质，把相同材质的分为一组（一个Batch），然后把它们组合成一个物体（统一变换），这样就可以在一个Draw Call中处理多个物体了（实际上是组合后的一个物体）。

但Draw Call Batching存在一个缺陷，就是它需要把一个Batch中的所有物体组合到一起，相当于创建了一个与这些物体加起来一样大的物体，与此同时就需要分配相应大小的内存。这不仅会消耗更多内存，还需要消耗CPU时间。特别是对于移动的物体，每一帧都得重新进行组合，这就需要进行一些权衡，否则得不偿失。但对于静止不动的物体来说，只需要进行一次组合，之后就可以一直使用，效率要高得多。

Unity提供了Dynamic Batching和Static Batching两种方式。Dynamic Batching是完全自动进行的，不需要也无法进行任何干预，对于顶点数在300以内的可移动物体，只要使用相同的材质，就会组成Batch。Static Batching则需要把静止的物体标记为Static，然后无论大小，都会组成Batch。如前文所说，Static Batching显然比Dynamic Batching要高效得多

要有效利用Draw Call Batching，首先是尽量减少场景中使用的材质数量，即尽量共享材质，对于仅纹理不同的材质可以把纹理组合到一张更大的纹理中（称为Texture Atlasing）。然后是把不会移动的物体标记为Static。此外还可以通过CombineChildren脚本（Standard Assets/Scripts/Unity Scripts/CombineChildren）手动把物体组合在一起，但这个脚本会影响可见性测试，因为组合在一起的物体始终会被看作一个物体，从而会增加GPU要处理的几何体数量，因此要小心使用。

对于复杂的静态场景，还可以考虑自行设计遮挡剔除算法，减少可见的物体数量同时也可以减少Draw Call。

总之，理解Draw Call和Draw Call Batching原理，根据场景特点设计相应的方案来尽量减少Draw Call次数才是王道，其它方面亦然。

Draw Call Batching （绘制调用批处理）

To draw an object on the screen, the engine has to issue a draw call to the graphics API (OpenGL ES in the case of iOS). Every single draw call requires a significant amount of work on the part of the graphics API, causing significant performance overhead on the CPU side.
在屏幕上渲染物体，引擎需要发出一个绘制调用来访问图形API（iOS系统中为OpenGL ES）。
每个绘制调用需要进行大量的工作来访问图形API，从而导致了CPU方面显著的性能开销。

Unity combines a number of objects at runtime and draws them together with a single draw call. This operation is called "batching". The more objects Unity can batch together, the better rendering performance you will get.
Unity在运行时可以将一些物体进行合并，从而用一个绘制调用来渲染他们。这一操作，我们称之为“批处理”。
一般来说，Unity批处理的物体越多，你就会得到越好的渲染性能。

Built-in batching support in Unity has significant benefit over simply combining geometry in the modeling tool (or using theCombineChildren script from the Standard Assets package). Batching in Unity happensafter visibility determination step. The engine does culling on each object individually, and the amount of rendered geometry is going to be the same as without batching. Combining geometry in the modeling tool, on the other hand, prevents effecient culling and results in much higher amount of geometry being rendered.
Unity中内建的批处理机制所达到的效果要明显强于使用几何建模工具（或使用Standard Assets包中的CombineChildren脚本）的批处理效果。
这是因为，Unity引擎的批处理操作是在物体的可视裁剪操作之后进行的。
Unity先对每个物体进行裁剪，然后再进行批处理，这样可以使渲染的几何总量在批处理前后保持不变。
但是，使用几何建模工具来拼合物体，会妨碍引擎对其进行有效的裁剪操作，从而导致引擎需要渲染更多的几何面片。

Materials
材质
Only objects sharing the same material can be batched together. Therefore, if you want to achieve good batching, you need to share as many materials among different objects as possible.
只有拥有相同材质的物体才可以进行批处理。
因此，如果你想要得到良好的批处理效果，你需要在程序中尽可能地复用材质和物体。

If you have two identical materials which differ only in textures, you can combine those textures into a single big texture - a process often calledtexture atlasing. Once textures are in the same atlas, you can use single material instead.
如果你的两个材质仅仅是纹理不同，那么你可以通过纹理拼合操作来将这两张纹理拼合成一张大的纹理。
一旦纹理拼合在一起，你就可以使用这个单一材质来替代之前的两个材质了。

If you need to access shared material properties from the scripts, then it is important to note that modifyingRenderer.material will create a copy of the material. Instead, you should useRenderer.sharedMaterial to keep material shared.
如果你需要通过脚本来访问复用材质属性，那么值得注意的是改变Renderer.material将会造成一份材质的拷贝。
因此，你应该使用Renderer.sharedMaterial来保证材质的共享状态。

Dynamic Batching
动态批处理
Unity can automatically batch moving objects into the same draw call if they share the same material.
如果动态物体共用着相同的材质，那么Unity会自动对这些物体进行批处理。

Dynamic batching is done automatically and does not require any additional effort on your side.
动态批处理操作是自动完成的，并不需要你进行额外的操作。

Tips:
提醒：
1、Batching dynamic objects has certain overheadper vertex, so batching is applied only to meshes containing less than900 vertex attributes in total.
批处理动态物体需要在每个顶点上进行一定的开销，所以动态批处理仅支持小于900顶点的网格物体。

2、If your shader is using Vertex Position, Normal and single UV, then you can batch up to 300 verts and if your shader is using Vertex Position, Normal, UV0, UV1 and Tangent, then only 180 verts.
Please note: attribute count limit might be changed in future
如果你的着色器使用顶点位置，法线和UV值三种属性，那么你只能批处理300顶点以下的物体；
如果你的着色器需要使用顶点位置，法线，UV0，UV1和切向量，那你只能批处理180顶点以下的物体。
请注意：属性数量的限制可能会在将来进行改变。

4、Don't use scale. Objects with scale (1,1,1) and (2,2,2) won't batch.
不要使用缩放尺度（scale）。分别拥有缩放尺度(1,1,1)和(2,2,2)的两个物体将不会进行批处理。

5、Uniformly scaled objects won't be batched with non-uniformly scaled ones.
统一缩放尺度的物体不会与非统一缩放尺度的物体进行批处理。
Objects with scale (1,1,1) and (1,2,1) won't be batched. On the other hand (1,2,1) and (1,3,1) will be.
使用缩放尺度(1,1,1)和 (1,2,1)的两个物体将不会进行批处理，但是使用缩放尺度(1,2,1)和(1,3,1)的两个物体将可以进行批处理。

6、Using different material instances will cause batching to fail.
使用不同材质的实例化物体（instance）将会导致批处理失败。

7、Objects with lightmaps have additional (hidden) material parameter: offset/scale in lightmap, so lightmapped objects won't be batched (unless they point to same portions of lightmap)
拥有lightmap的物体含有额外（隐藏）的材质属性，比如：lightmap的偏移和缩放系数等。所以，拥有lightmap的物体将不会进行批处理（除非他们指向lightmap的同一部分）。

8、Multi-pass shaders will break batching. E.g. Almost all unity shaders supports several lights in forward rendering, effectively doing additional pass for them
多通道的shader会妨碍批处理操作。比如，几乎unity中所有的着色器在前向渲染中都支持多个光源，并为它们有效地开辟多个通道。

9、Using instances of a prefab automatically are using the same mesh and material.
预设体的实例会自动地使用相同的网格模型和材质。

Static Batching
静态批处理

Static batching, on the other hand, allows the engine to reduce draw calls for geometry of any size (provided it does not move and shares the same material). Static batching is significantly more efficient than dynamic batching. You should choose static batching as it will require less CPU power.
相对而言，静态批处理操作允许引擎对任意大小的几何物体进行批处理操作来降低绘制调用（只要这些物体不移动，并且拥有相同的材质）。因此，静态批处理比动态批处理更加有效，你应该尽量低使用它，因为它需要更少的CPU开销。

In order to take advantage of static batching, you need explicitly specify that certain objects are static and willnot move, rotate or scale in the game. To do so, you can mark objects as static using the Static checkbox in the Inspector:
为了更好地使用静态批处理，你需要明确指出哪些物体是静止的，并且在游戏中永远不会移动、旋转和缩放。想完成这一步，你只需要在检测器（Inspector）中将Static复选框打勾即可；
Using static batching will require additional memory for storing the combined geometry. If several objects shared the same geometry before static batching, then a copy of geometry will be created for each object, either in the Editor or at runtime. This might not always be a good idea - sometimes you will have to sacrifice rendering performance by avoiding static batching for some objects to keep a smaller memory footprint. For example, marking trees as static in a dense forest level can have serious memory impact.
使用静态批处理操作需要额外的内存开销来储存合并后的几何数据。在静态批处理之前，如果一些物体共用了同样的几何数据，那么引擎会在编辑以及运行状态对每个物体创建一个几何数据的备份。这并不总是一个好的想法，因为有时候，你将不得不牺牲一点渲染性能来防止一些物体的静态批处理，从而保持较少的内存开销。比如，将浓密森里中树设为Static，会导致严重的内存开销。

Static batching is only available in Unity iOS Advanced.
静态批处理目前只支持Unity iOS Advanced。

前两天，MadFinger，就是当今iOS与Android上画质最牛逼闪闪的游戏之一——ShadowGun的开发商，令人惊异地放出了一个ShadowGun的样例关卡以及若干可免费使用的Shader，国外同行们的分享精神真的是令人赞叹不已。原文在这里，以下是我的一些摘录和笔记。

首先是一些优化常识。针对图形方面的优化主要包括三角形数量，纹理所占内存，以及Shader，前两项基本没什么好讲的，针对设备机能的限制制定相应的指标即可，所以Shader就成为了图形性能优化的关键。

Alpha blending

在Unity官方文档中讲，由于硬件原因，在iOS设备上使用alpha-test会造成很大的性能开销，应尽量使用alpha-blend代替。这里提到，在同屏使用alpha-blend的面数，尤其是这些面所占屏幕面积的大小，对性能也会造成很大影响。原因是使用alpha-blend的面会造成overdraw的增加，这尤其对低性能设备的影响很大。不过没有购买Pro版，没有Occlusion Culling功能的话，就不必顾虑这一问题了，反正overdraw是必然的。

复杂的Per-pixel shader

Per-pixel shader即Fragment shader，顾名思义是要对每个渲染到屏幕上的像素做处理的shader，如果per-pixel shader比较复杂且需要处理的像素很多时，也就是使用该shader的面占屏幕面积很大时，对性能的影响甚至要超过alpha blending。因此复杂的per-pixel shader只适用于小物体。

下面是对几个Shader的逐一讲解：

Environment specular maps(Shader Virtual Gloss Per Vertex Additive)
Specular map通常都是利用贴图的alpha通道来定义物体表面的光滑程度（反光度），这个shader的特点是per-vertex计算反光度的，有着相当不错的效果的同时比per-pixel的shader性能要高得多。这个shader很适用于关卡环境等占很大区域的模型。

经过优化的动态角色光照和阴影(Light probes和BRDF Shader)
传统的Lightmaps无法支持动态物体，对此Unity提供了Light probes技术，预先把动态物体的光照信息保存在代理对象(即Light probes)中，运行时动态物体从距离最近的Probe中获取光照信息。

Unity本身还提供了一个效果非常棒的专为移动设备优化过的角色Shader，支持Diffuse、Specular和Normal maps，并通过一个特殊的脚本生成贴图用于模仿BRDF光照效果。最终产生的效果堪比次时代大作中的角色光影效果。

雾和体积光(Shader Blinking Godrays)
目前在移动设备上要开启真正的雾效基本不可行，ShadowGun的方案是通过简单的网格＋透明贴图(称为雾面)来模拟雾效。在玩家靠近时，雾面逐渐变淡，同时fog plane的顶点也会移开(即使完全透明的alpha面也会消耗很多渲染时间)。

使用这个Shader的网格需要经过处理：

顶点的alpha值用于决定顶点是否可以移动(在例子中0为不可动，1为可动)。
顶点法线决定移动的方向
然后Shader通过计算与观察者的距离来控制雾面的淡入/淡出。
这个Shader还可以用来做体积光和其它一些alpha效果。

飞机坠毁的浓烟效果(Shader Scroll 2 Layers Sine Alpha-blended)
通过粒子产生浓烟的代价太高，所以ShadowGun中使用了网格＋贴图动画来制作这个效果。通过混合两层贴图并让它们交错移动来产生动画效果。其中顶点alpha值用于让网格的边缘看起来比较柔和，同时使用顶点颜色来模拟从火焰到烟雾的过渡效果。

带动态效果的天空盒(Shader Scroll 2 Layers Multiplicative)
通过两张贴图的混合和移动产生云的动态效果。

旗帜和衣服的飘动效果(Shader Lightmap + Wind)
同样利用顶点alpha值决定哪些顶点可以移动，然后shader的参数用于调整摆动的方向和速度。

一、程序方面
　　1. 务必删除脚本中为空或不需要的默认方法；
　　2. 只在一个脚本中使用OnGUI方法；（最好不要加）
　 3. 避免在OnGUI中对变量、方法进行更新、赋值，输出变量建议在Update内；
　　4. 同一脚本中频繁使用的变量建议声明其为全局变量，脚本之间频繁调用的变量或方法建议声明为全局静态变量或方法；
　　5. 不要去频繁获取组件，将其声明为全局变量；
　　6. 数组、集合类元素优先使用Array，其次是List；
　　7. 脚本在不使用时脚本禁用之，需要时再启用；
　　8. 可以使用Ray来代替OnMouseXXX类方法；
　　9. 需要隐藏/显示或实例化来回切换的对象，尽量不要使用SetActiveRecursively或active，而使用将对象远远移出相机范围和移回原位的做法；
　　10. 尽量少用模运算和除法运算，比如a/5f，一定要写成a \* 0.2f。
　　11. 对于不经常调用或更改的变量或方法建议使用Coroutines & Yield；
　　12. 尽量直接声明脚本变量，而不使用GetComponent来获取脚本；
iPhone
　　13. 尽量使用整数数字，因为iPhone的浮点数计算能力很差；
　　14. 不要使用原生的GUI方法；
　　15. 不要实例化（Instantiate）对象，事先建好对象池，并使用Translate“生成”对象；
二、模型方面
　　1. 合并使用同贴图的材质球，合并使用相同材质球的Mesh；
　　2. 角色的贴图和材质球只要一个，若必须多个则将模型离分离为多个部分；
　　2. 骨骼系统不要使用太多；
　　3. 当使用多角色时，将动画单独分离出来；
　　4. 使用层距离来控制模型的显示距离；
　　5. 阴影其实包含两方面阴暗和影子，建议使用实时影子时把阴暗效果烘焙出来，不要使用灯光来调节光线阴暗。
　　6. 少用像素灯和使用像素灯的Shader；
　　7. 如果硬阴影可以解决问题就不要用软阴影，并且使用不影响效果的低分辨率阴影；
　　08、实时阴影很耗性能，尽量减小产生阴影的距离；
　　09、允许的话在大场景中使用线性雾，这样可以使远距离对象或阴影不易察觉，因此可以通过减小相机和阴影距离来提高性能；
　　10、使用圆滑组来尽量减少模型的面数；
　　11、项目中如果没有灯光或对象在移动那么就不要使用实时灯光；
　　12、水面、镜子等实时反射/折射的效果单独放在Water图层中，并且根据其实时反射/折射的范围来调整；
　　13、碰撞对效率的影响很小，但碰撞还是建议使用Box、Sphere碰撞体；
　　14、建材质球时尽量考虑使用Substance；
　　15、尽量将所有的实时反射/折射（如水面、镜子、地板等等）都集合成一个面；
　　16、假反射/折射没有必要使用过大分辨率，一般64*64就可以，不建议超过256*256；
　　17、需要更改的材质球，建议实例化一个，而不是使用公共的材质球；
　　18、将不须射线或碰撞事件的对象置于IgnoreRaycast图层；
　　19、将水面或类似效果置于Water图层
　　20、将透明通道的对象置于TransparentFX图层；
　　21、养成良好的标签（Tags）、层次（Hieratchy）和图层（Layer）的条理化习惯，将不同的对象置于不同的标签或图层，三者有效的结合将很方便的按名称、类别和属性来查找；
　　22、通过Stats和Profile查看对效率影响最大的方面或对象，或者使用禁用部分模型的方式查看问题到底在哪儿；
　　23、使用遮挡剔除（Occlusion Culling）处理大场景，一种较原生的类LOD技术，并且能够“分割”作为整体的一个模型。

三、其它
　　场景中如果没有使用灯光和像素灯，就不要使用法线贴图，因为法线效果只有在有光源（Direct Light/Point Light/Angle Light/Pixel Light）的情况下才有效果。



