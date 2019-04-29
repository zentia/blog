---
title: 面试
date: 2018-04-11 13:30:35
tags:
  - 面试
---
接口(Interface)是一种规划，为你定义了一系列的规则和任务，但不去实现它
类和结构体可以继承多个接口
抽象类表示该类中可能已经有一些的具体定义
生成的库在这里：

    Temp\StagingArea\libs\armeabi-v7a\libil2cpp.so

符号表在这里，需要把扩展名弄成.so，才能被bugly的符号工具转换。

    Temp\StagingArea\libs\armeabi-v7a\libil2cpp.so.debug

C++代码生成到这里，最好保存这个目录，以便对应看调用栈：

    Temp\StagingArea\Il2Cpp\il2cppOutput

# ArrayList 
会把所有插入其中的数据都当成了object类型来处理。
弊端：1、不是类型安全的；2、存在装箱拆箱操作
```CSharp
Add() //向数组中添加一个元素
Remove() //删除数组中的一个元素
RemoveAt(int i);//
//不是骨骼动画,可以用animator,/
//骨骼动画 animator只能用一个,
Reverse()//反转数组的元素
Sort()//从小到大排序数组的元素
Clone()//复制一个数组
//List
//可通过索引访问
```
在决定使用List还是使用ArrayList 类(两者具有类似的功能)时,记住List类在大多数情况下执行的更好并且是类型安全的,
如果对List类型T使用值类型,则需要考虑实现和装箱问题
如果对类型T实用值类型,则编译器将特别针对该值类型生成List类的实现,这意味着不必对
再提醒一下，模板容器里不要使用枚举。
比如Dictionary之类的，枚举会boxing，产生无用的GC
就用int替代吧

1. 请描述游戏动画有哪几种，以及其原理。
答:主要有关节动画,单一网络模型动画(关键帧动画),骨骼动画.
关节动画把角色分为若干独立部分,一个部分对应一个网络模型,部分的动画连接成一个整体的动画,角色比较灵活Quake2中实用了这种动画.
单一网络模型动画由一个完整的网络模型构成,在动画序列的关键帧里记录各个顶点的原位置及其改变量,然后插值运算实现动画效果,角色动画较真实.
骨骼动画,广泛应用的动画方式,集成了以上两个方式的优点,骨骼按角色特点组成一定的层次结构,由关节相连,可做相对运动,皮肤作为单一网络蒙在骨骼之外,决定角色的外观,皮肤网络每一个顶点都会收到骨骼的影响,从而实现完美的动画.

# alpha blend 工作原理

alpha blend用于做半透明效果.Color=(源颜色 * 源系数)OP(目标颜色 * 目标系数);其中OP(混合方式)有加,减,反减,取最小,取最大
## OpenGL Alpha混合
假设一种不透明的东西的颜色是A，另一种透明的东西颜色是B，那么透过B看A，看上去的颜色C就是B和A的混合颜色，可以用以下公式来模拟，设B物体的透明度为alpha（取值为0-1，0为完全透明，1为完全不透明）

    R(C)=alpha*R(B)+(1-alpha)R(A)
    G(C)=alpha*G(B)+(1-alpha)G(A)
    B(C)=alpha*B(B)+(1-alpha)G(A)

3. 写光照计算中的diffuse的计算公式
   答:漫反射光(diffuse)计算公式为:ldiffuse = Dintensity*Dcolor*N.L;(Dintensity)表示漫反射强度,Dcolor表示漫反射光颜色,N为该点的法向量,L为光源向量
其他,3D渲染中,物体表面的光照计算公式为:
I = 环境关(lambient) + 漫反射光(ldiffuse) + 镜面高光(lspecular);
其中,环境光(ambient)计算公式为:
	
	lambient = Aintensity*Acolor;(Aintensity)表示环境光强度,Acolor表示环境光颜色

镜面高光(specular)计算公式为:
lspecular = Sintensity*Scolor*(R.V)n;(Sintensity)表示镜面光照强度,Scolor表示镜面光颜色,R为光的反射向量,V为观察者向量

6. MipMap是什么？作用？
答:在三维计算机图形的贴图渲染中有一个常用的技术成为Mipmapping,为了加快渲染速度和减少图像锯齿,贴图被处理成由一系列被预先计算和优化过的图片组成的文件,这样的贴图被称为mipmap
7. 用u3d实现2d游戏，有几种方式？
答：一种用UI实现(GUI,NGUI...)，一种是采用3d实体对象（plane），绘制在3d对象上，调节摄像机，采用平行投影模式或则固定视角。
8. u3d中碰撞器和触发器的区别？
答：触发器只是碰撞器身上的一个属性，碰撞器是触发器的载体。
9. 物体发生碰撞的必要条件
答：需要检测碰撞的物体身上存在刚体组件（或被检测物体），也要碰撞器collider
10. CharacterController和Rigidbody的区别
11. 物体发生碰撞时，有几个阶段，分别对应的函数
答：排除触发，有3种，OnCollisionEnter(),OnCollisionStay(); OnCollisionExit()
12. u3d中，几种施加力的方式，描述出来。
答：a)爆炸力（AddExplosionForce(force : float, forcePos : Vector3，radius : float, upwards : float, mode : ForceMode)），应用一个力到刚体来模拟爆炸效果,就是在爆炸力中心坐标position,搜索在radius范围内的刚体，对其释放力作用，超出radius范围的刚体不受力作用，爆炸力将随着刚体的距离线性减弱。
b)力AddForce(force : Vector3, mode : ForceMode),主要施力给一个刚，使其移动。
c)位置力AddForceAtPosition(force : Vector3, position : Vector3, mode : ForceMode), 在position施加一个力，施力的主体将会受到一个力和力矩。
d)相对力AddRelativeForce(force : Vector3, mode : ForceMode),类似于AddForce；
13. 什么叫做链条关节
答:Hinge Joint ,他可以模拟两个物体见
14. 物体自旋转使用的函数叫什么
答：transform.Rotate(eulerAngles : Vector3, relativeTo : Space = Space.self);
15. 物体绕某点旋转使用函数叫什么
答：transform.RotateAround(point : Vector3, axis : Vector3, angles : float)
16. u3d提供了一个用于保存读取数据的类，（playerPrefs），请列出保存读取整形数据的函数
答：PlayerPrefs.GetInt(key : string, defaultValue : int = 0);
17. unity3d提供了几种光源，分别是什么
答：平行光,点光源，聚光灯，环境光
18. unity3d从唤醒到销毁有一段生命周期，请列出系统自己调用的几个重要方法。
答：void Awake(),void Start(), void Update(), void FixedUpdate(),void LateUpdate(), void OnGUI() ，void Reset(), OnDisable(), void OnDestroy()
19. 物理更新一般在哪个系统函数里？
答：void FixedUpdate()
20. 移动相机动作在哪个函数里，为什么在这个函数里。
答：void LateUpdate(),因为这个函数是在Update执行完毕才执行的，不然的话就有可能出现摄像机里面什么都看到的情况。
21. 当游戏中需要频繁创建一个物体对象时，我们需要怎么做来节省内存。
Unity里有两种动态加载机制:一个是Resources.Load,另外一个通过AssetBundle,其实两者区别不大,Resources.Load就是从一个缺省打进程序包里的AssetBundle里加载资源,而一般AssetBundle文件需要你自己创建,运行时动态加载
AssetBundle运行时加载
AssetBundle.Unload(flase)是释放AssetBundle文件
22. 一个场景放置多个camera并同时处于活动状态，会发生什么
23.简述prefab的用处和环境
24.如何销毁一个UnityEngine.Object以及其子类
25.为什么u3d会出现组件上数据丢失的情况
26.u3d下如何安全的在不同工程迁移asset数据
DrawCall优化
一 Mesh Renderer
二 Skinned Mesh Renderer
三 合并要求对比
四 总结
五 场景制作建议
DrawCall优化合并,即DrawCall Batching.通过减少Draw call数和对显卡性能的消耗来提高性能
一 Mesh Renderer
分为Dynamic Batching 和 Static Batching
Dynamic Batching
不需要任何操作,只要共享材质(即使是不同的Mesh模型也可以)
Unity中的内存种类实际上Unity游戏使用的内存一共有三种:程序代码,托管堆(Managed Heap)以及本机堆(Native Heap)
程序代码包括了所有的Unity引擎,使用的库,以及你所写的所有的游戏代码,在编译后,得到的运行文件将会被加载到设备中执行,并占用一定内存.
这部分内存实际上是没有办法去”管理的”

法线贴图
一: 法线贴图的原理
光照效果很大程度上是由垂直于物体表面的法线决定的,因为法线影响反射光的方向,均匀垂直的法线是镜面贴图但是有时候我们会给一个平面使用砖墙贴图,砖墙应该是凹凸不平的,而如果让砖墙使用该平面的法线的话,画面就很假,
而如果按真实砖墙去做模型的话,即做高精度模型,一方面制作麻烦,另一方面运行时对性能的损耗很大.
法线贴图就是来解决这个问题的.法线贴图就是把法线信息存储在一张图里.使用发现贴图时,通常顶点数和三角形面数只有高精度模型的十分之一不到.
二: 法线贴图的实现
将材质贴图对应的法线绘制在一张贴图上,将贴图对应点的单位法线向量信息float3(x,y,z)存储在途中对应的颜色里
压缩法线贴图的好处:
压缩后的法线贴图,大小只有原来的1/4左右,故可以使用更大或者更多的贴图来提升画面品质.
Unity3D的法线贴图
Unity3D使用的压缩法线贴图是DXT5nm格式的,有A和G两个通道,对于法线(x,y,z) A对应x,G对应y
对压缩法线贴图的采样依然是如下函数
	
	float4 packedNormal = tex2D(_NormalMap, IN.uv_MainTex);

packedNormal.w对应A通道,既法线的x
packedNormal.y对应G通道,既法线的y
Unity3D热更新全书-加载(一)从AssetBundle说起
我们试全面分析一下Unity3D提供的资源加载机制
1. Resourecs//内嵌资源,使用方法Resources.Load,可以加载任意种类的资源,不能动态修改
2. StreamingAssets//随包资源,使用方法IO或WWW.Load,WWW.Load可以加载任意种类资源,IO仅限bytes和text
3. WWW从网络下载并加载
4. WWW从网络加载AssetBundle
一和二显然不具有热更新的效果,这里就不做讨论
3 4都是从网络加载的,他们有什么区别呢
首先说3,这是没有缓冲的,我们显然不想让用户重复的浪费流量,不可取
然后是4,assetbundle提供了一个版本号来做缓冲对比,可以比较好的起到更新的目标
assetbundle原来就是Unity3D为我们准备的方案,难怪每本书都会大篇介绍AssetBundle,后文简称AB
每本书都告诉你,AB很强大,AB帮你解决了跨平台问题,帮你解决了依赖关系.
首先AB的确很好很强大,他能收纳Unity自己的所有资源种类,贴图,材质,shader预设
然后可以每平台支持,这就是第一个陷进,注意是每平台都支持,不是多平台支持
每个平台都要单独导出,而每个平台到底差了些什么呢?nothing
而最大的质量差异,源自贴图
有些平台贴图不压缩,有些平台贴图要压缩,而且根据不通的平台特性,套用不通的压缩算法,先压过再存到包里
这就是AB帮你干的最主要的事情
听起来很贴心呢,等等,你是不是忘了一个特定的命题,这个命题叫做UI
压缩的图片会有质量损失,UI贴图我们通常是不压缩的
然后UI还会触及到AB的另一个问题
以NGUI为例,NGUI的资源关系比较复杂,有贴图->图集->布局
不同的布局经常交叉应用贴图
如果用AB想把每个界面分开打包,给用户最小的资源更新量,这个任务可以用来灾难来形容
最终热更新推送给用户的东西是以文件为单位的,而AB在小粒度文件并且之间有较为复杂的应用关系这种需求的下的使用是一场灾难
AB对每个平台的差异编译不是可选的,而是强制的,假如你有web,ios,android,三个平台,无论如何你都要导出三次
没了AssetBundle,我们怎么办
把碎片文件下载回来,并且组装
对于UI这个情景,是完全可行的,把布局和图集保存为文本形式,把文本和贴图下载回来,然后组装
对于其他场景片段,在有动画和贴图需要压缩的情况下,AB依然是唯一的选择
unity没有提供在运行时压缩压缩贴图的手段,动画也不容易存储,只能运行时压缩DXT,只有pc和wp8支持
对于各种各样的资源加载回特别凌乱么,不会
其实本质上可以统一成bytes的处理
texture可以从bytes加载
字符串可以从bytes加载
AB可以从bytes加载
自定义二进制存储,本身就是bytes
Unity3D热更新全书-加载(二)如何在不用AssetBundle的前提下动态加载预设
Unity3D的主要构成大家都知道,首先是场景图,场景图上的节点构成一棵树
每个节点对应一个GameObject对象
然后每个GameObject有若干个组件
有一些组件会与资源产生关系,比如MeshRenderer会关联材质,材质回关联shader和贴图
场景图的一部分回保存为一个预设,prefab
有时候我们会需要预设去复用,而预设的加载似乎只能通过AB去打包,其实不然,这里我们有一个开源的库就可以解决这个问题
为什么不使用AB,可以见上一篇,加载(一),不使用AB一份资源全平台同意,也没有痛苦的打包时间,资源依赖也很容易处置

物理更新一般在FixedUpdate里,每固定帧绘制时执行一次,和update不同的是FixedUpdate是渲染帧执行,如果你的渲染效率低下的时候FixedUpdate调用次数就会跟着下降,FixedUpdate比较适用于物理引擎的计算,因为是跟每帧渲染有关,Update就比较适合做控制.
移动相机动作在LateUpdate函数,当所有Update结束才调用,比较适合用于命令脚本的执行,
当游戏中需要频繁创建一个物体对象时,做一个pool,游戏开始时预先实例化足够的数量,用的时候取,不用的时候回收.
1. 什么是渲染管道?
渲染管道中有很多步骤,都要将几何体从一个坐标系中变换到另一个坐标系中.
本地坐标->视图坐标->背面裁剪->光照->裁剪->投影->视图变换->光栅化
优化内存:
1. 自带压缩类库
2. 将暂时不用的以后还需要实用的物体隐藏起来而不是直接Destroy掉
3. 释放AssetBundle占用的资源
4. 降低模型的片面数,降低模型的骨骼数量,降低贴图的大小
5. 使用光照贴图,使用多层次细节(LOD),使用着色器(Shader),使用预设(Prefab)
资源加载:
1. Resources.Load();
2. AssetBundle
localPosition:自身位置,相对于父级的变换位置,Position:在世界坐标transform的位置
静态构造函数不允许访问修饰符,也不接受任何参数
碰撞器(Collider)有碰撞效果,IsTrigger=false,可以调用OnCollisionEnter/Stay/Exit函数
触发器(Trigger)没有碰撞效果,IsTrigger=true,可以调用OnTriggerEnter/Stay/Exit函数
2. 物体发生碰撞的必要条件?
必须带有collider碰撞起和rigibody刚体属性或者人物控制器其实人物控制器就包含了前两者,另外一个也要带有Collider,Collider分类:网格碰撞器,盒子碰撞器,胶囊碰撞器,球型碰撞器,地形碰撞器
3. CharacterController和Rigidbody的区别?
Rigidbody具有完全真是物理的特性,而CharacterController可以说是受限的Rigidbody,具有一定的物理效果但不是完全真实的.
4.物体发生碰撞时,几种施加力的方式,描述出来?
rigidbody.AddForce/AddForceAtPosition,都是rigidbody的成员函数
5. 物体发生碰撞时,有几个阶段,分别对应的函数
三个阶段,OnCollsionEnter/Stay/Exit函数
6. Unity3d提供了几种光源,分别是什么
共4中,DirectrionaLight,PointLight,SpotLight,AreaLight(用于烘培)
8. 使用Unity3d实现2d游戏,有几种方式?
(1).使用本身GUI
(2).把相机的Projection(投影)值调位Orthographic(正交投影),不考虑z轴;
(3).使用2d插件,如:2DTollKit
9.下列代码在运行中回产生几个临时对象?
string a = new string(“abc”);
a = (a.ToUpper() + “123”).SubString(0, 2);
答:其实在C#第一行是会出错的(Java中倒是可行),应该这样初始化
string b = new string(new char[]{‘a’, ‘b’, ‘c’}}
10. 请简述关键字Sealed用在类声明和函数声明时的作用
答:类声明时可防止其他类继承此类,在方法中声明则可防止派生类重写此方法
11. 简述四元数的作用,四元数对欧拉角的优点?
答:四元数用于表示旋转
相对于欧拉角的优点:
(1).能进行增量旋转
(2).避免方向锁
(3)给定方位的表达方式有两种,互为负(欧拉角有无数种表达方式)
12.如何安全的在不用工程间安全的迁移asset数据,三种方法
答
(1).将Assets目录和Library目录一起迁移
(2)导出包
(3)用unity自带的assert server功能
13.当一个细小的高速物体碰撞另一个较大的物体时,会出现什么情况?如何避免?
穿透(碰撞检测失败)
14.MipMap是什么?作用?
在三维计算机图形的贴图渲染中有一个常用的技术被成为Mipmapping,为了加快渲染速度和减少图像锯齿,贴图被处理成为

# 反射

实现原理：在运行时根据程序集和其中的类型得到元数据
实现步骤:
1. 导入 using System.Reflection
2. Assembly.Load("程序集")加载一个程序集，返回类型是一个Assembly
3. 得到程序集中所有类的名称
```CSharp
foreach(Type type in assembly.GetType())
{
    stromh t = type.name;
}
```
4. Type type = assembly.GetType("程序集.类名"); //获取当前类的类型
5. Activator.CreateInstance(type); // 创建此类型实例
6. MethodInfo mInfo = type.GetMethod("方法名");//获取当前方法
7. m.Info.Invok(null, 方法参数);