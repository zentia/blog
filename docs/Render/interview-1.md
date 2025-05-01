- 程序的优化处理，这个是面试考察的重点，内存的管理也是Unity开发的重中之重。内存管理无非就是对内存的使用，针对UI的内存管理使用的图集方式，Unity自身的图集功能浪费内存，比如2048 * 2048的图集大小，它占的内存是4M，也就是说不管图集是否填满，它都需要在内存中开辟4M的空间，但是如果使用Texture Packer工具就可以节省没有填满的空间，针对大量UI的加载可以采用分类打包图集的方法处理。当然内存池的使用也是可以应用到UI的管理，另外就是3D场景资源的管理，需要场景图片有自己的图库，这样图片可以重复利用，另外模型的面熟，骨骼数量，材质数量都是需要去关注。
- 内存优化还需要涉及到图片的格式，不带Alpha通道的图片采用jpg格式，带有Alpha通过的采用png格式，这是一种处理方式，另外一种处理方式就是将Alpha通道单独拿出来，所有的图片都是jpg格式，然后用带有Alpha通道的图片跟jpg格式文件进行结合使用。
- 在结构涉及方面，就要注意了，代码的编写，比如常用的设计模式，工厂模式，单例模式，MVC模式，FSM模式，观察者模式等，这些模式作为开发者必须要掌握的。
- 接下来就是Shader的编写了，可以使用Shader Forge编辑器，对于一些材质渲染非常好用，这个可以作为读者学习Shader的工具。
- 九、另外对于C#中定义的String，StringBuilder或者List,ArrayList等它们之间的区分要搞清楚。
- 热更新技术的实现，市面上应用比较广的ulua作为脚本语言架构设计。
- 作为客户端开发者也需要了解网络方面的知识，比如tcp,udp,http这些协议的处理方式。
- 场景、导航、寻路
- 作为渲染场景的后处理方式，比如Bloom,Blur,HDR,SSAO,PSSM等等。
- C#中的事件监听，委托的使用也要熟练掌握。
- 协程与多线程的区别。
- 文本文件的加载，比如json,xml,csv,二进制等等。

GC是Mono运行时的机智，而非Unity3D引擎的机制，所以GC也主要是针对Mono对象来说的。

```CSharp
	String s = new String("lyf");
```
在运行时涉及了几个String Object
两个，一个是字符串字面量"lyf"所对应的，驻留(intern)在一个全局共享的字符串常量池中的实例，i另一个是通过new String(String)创建并初始化的、内容于"lyf"相同的实例

# OpenGL中要用到哪几种buffer

帧缓冲(Frame Buffer)
颜色缓冲(Color Buffer)
模版缓冲(Stencil Buffer)
顶点缓冲(Vertice Buffer)
深度缓冲(Depth Buffer)

# 冰川网络

1.啥子是泛型？给你一个CBase基类，写一个继承CBase的泛型。
百度答：所谓泛型，即通过参数化类型来实现在同一份代码操作多种数据类型。
我答：类的数据成员不固定，成员函数的返回值和传参类型不固定的，这样的方式叫做泛型。本想说这样的类叫做泛型类，但是人家问的是泛型，只能说这样的方式了。

我不知道写一个继承CBase的泛型考点是啥子，然后我就开始瞎写
```CSharp
public class CDrive:public CBase // 为毛还要加public，难道还有公有继承，保护继承，私有继承？？？
{
    public void Do<T>(T t) // 这他妈写的什么玩意。
    {}
}
```

3.啥子是反射？写一段加载程序集，然后获取类型Type，然后通过方法名调用
网上答案：
```CSharp
using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;

public class InvokeReflection : MonoBehaviour
{
    void Start()
    {
        Learn00();
    }
    public void Learn00()
    {
        /*
         * System.Reflection.Assembly类有两个静态方法：Assembly.Load(string assemblyname)和Assembly.LoadFrom(string filename) 。
         * 通常用这两个方法把程序集加载到应用程序域中。 如果你希望加载的程序集超出了CLR的预定探查范围，你可以用Assembly.LoadFrom直接从一个文件位置加载程序集。
         * Assembly.LoadFrom()和Assembly.LoadFile()，两者的主要区别有两点：
         * 一：Assembly.LoadFile()只载入指定的dll文件，而不会自动加载相关的dll文件。如果下文例子使用Assembly.LoadFile()加载SayHello.dll，那么程序就只会加载
         * SayHello.dll而不会去加载SayHello.dll引用的BaseClass.dll文件。
         * 二：Assembly.LoadFrom()载入一个Assembly时，会先检查前面是否已经载入过相同名字的Assembly；Assembly.LoadFile()则不会做类似的检查。
         */
        //C:\Users\Administrator\Desktop\New Unity Project\Assets\DLL
        Assembly assemly2 = Assembly.LoadFrom(@"C:\Users\Administrator\Desktop\New Unity Project\Assets\Scripts\TestReflection.dll");
        Assembly assemly1 = Assembly.LoadFile(@"C:\Users\Administrator\Desktop\New Unity Project\Assets\Scripts\TestReflection.dll");
        Assembly assemly = Assembly.Load("TestReflection");//1填加DLL
        Type testType = assemly.GetType("HaiLan.TestReflection.TestReflectionDLL");//2获取对应的类信息，需要填加对应的命名空间
        object oTestReflectionDLL = Activator.CreateInstance(testType);//3 创建对象
        IReflectionDLL iTestReflectionDLL = oTestReflectionDLL as IReflectionDLL;//4 类型转换
        iTestReflectionDLL.TestShow1();//5 方法调用

        foreach (var item in assemly.GetModules())
        {
            Debug.Log(item.Name);//打印结果 TestReflection.dll
        }
        foreach (var type in assemly.GetTypes())
        {
            Debug.Log(type.Name);
            foreach (var item in type.GetMethods())
            {
                Debug.Log(item.Name);
            }
        }
    }
}
```
4.avatar如何换装？
5.啥子是Prefab，如何获取一个Prefab实例化的GameObject的类型？PrefabInstance和PrefabModelInstance有啥子区别（我都不知道他在问啥子）
6.获取一个Render的材质，然后设置流光shader的uniform/
7.地震了，狗跑了，房屋塌了，人慌了，这是哪个设计模式？写出伪代码或者画出流程图；要求可扩展
8.给你一个新项目，内存贼高，FPS贼低，咋优化，是Xlua和C#写的。
9. AssetBundle的打包和加载
10. 如何判断线段和多边形是否相交？

# 深圳易凡互动

1. NGUI的uv和顶点是怎么生成的？
2. 客户端网络是否有用多线程？为什么要用？
3. TCP IPV6是怎么实现的？
4. Unity5.6和Unity2018的光照图有什么区别，流程是怎样的？

# 深圳奇妙能力

1. TCP粘包可以描述一下吗？
2. Shader Bloom泛光是怎么实现的？
3. B树，B+树是啥子？
4. 从一堆无序的数字中找到不重复的数字，重复的数字是都是双数个。

# 牛蛙互动

1. 点和多边形相交（曲线先交）？
2. 输出下列的结果？
```CSharp
public class Program
{
    readonly int b = a + 10;
    readonly int a = 10;
    static int d = c + 10;
    static int c = 10;
    static void Main(string[] args)
    {
        Program p = new Program();
        Console.WriteLine("a = {0}, b = {1}, c = {2}, d = {3}", p.a, p.b, c, d);
    }
}
```
3. 设计一种存储结构实现字典查找。
4. 600个一M的纹理，给一个UI使用，每次只会加载一个，是打成一个AB包好，还是多个好？

# IGG

1. TCP粘包和三次握手的联系?
2. 动静分离？
3. Overdraw的优化？
4. 半透明和不透明物体的渲染流程？

# 珠海西山居

1. 工作中遇到过什么困难，如何解决，获得什么成就？
2. Array和List的区别？
3. 闭包是什么？
4. Lua Table如何实现的，Hash是什么，如何解决Hash冲突？
5. new Struct存放在哪里？堆和栈的区别？
6. 什么是元表？有什么应用？
7. 堆和栈在C++中的运用以及实现方式以及优缺点？
8. UI的overdraw如何解决，异步加载造成数据不同步如何解决？
9. 剧情编辑器如何不根据地形制作剧情？
10. 协程的原理？

# 乐易网络

1. NGUI如何优化？
2. NGUI是如何绘制的？
3. UIWidget如何提交渲染的？
4. 一个已经排好序的链表构成一个数组，如何合成一个新的链表？（机试题）

# 四维时代

1. 进程间是如何交互的？各自有啥好处还坏处？
2. 漫反射原理？
3. lua的加载流程是怎样的？
4. TCP/UDP/HTTP运用场景，优缺点？
5. 线程和协程的区别？

# 乐逗

1. TTF字体
2. 传输层解释一下为什么UDP在手机上要优于TCP
3. Lua的Hash是如何实现的
4. 红黑树的时间复杂度
5. 渲染流程管线，相机抖动是如何出现的
6. new free malloc delete 区别在哪里？

# 广州百田

1. 介绍过往工作、项目经历
2. unity的基本框架有哪些？请描述或者简单画一画项目核心的架构分层图，以及核心模块间的交互时序（Unity框架范围问题比较泛，最好能结合业务说下某个业务的框架），如果没用过unity，那就画一下目前项目的业务框架。
3. 是否做过发布打包相关的工作？用unity是怎么管理和加载资源的？如何热更新资源，对热更资源和放置路径有什么要求？（可以说下热更新分哪些内容，以及对AssetBundle的理解）
4. 是否完成过大型模块，如战斗，场景，副本等；（说设计思路）
5. 是否做过游戏相关优化，如GPU，CPU，Shader等；（优化工作的具体方面：1.资源包大小的优化，2.内存占用，内存分配和碎片的优化，3.cpu热点和毛刺优化，4.gpu性能开销优化）
6. 同步、异步相关问题；（执行时序的概念，资源加载，网络通信，逻辑调用的各种时序流程）
7. AI框架有哪些；（这个问题，成熟的AI解决方案就是行为树，面试官会问，对行为树的了解情况，是怎么组合完成复杂AI的，这个是加分题，大部分面试人其实说的不太透）
8. 底层网络用哪些协议（补充：tcp，udp（以及可靠版的udp）的区别，分别适用于哪些业务场景）
9. 用过哪些开源库；（单纯考察你对业内相关开源库的敏感度，知识广度）
10. 了解哪些比较好的AI插件；
11. NGUI相关问题（补充问题：NGUI绘制开销如何优化，减少drawcall需要在制作UI和图集的时候注意什么）
12. 怎么做内存、显存优化；
13. 图片的处理格式（补充：怎么选择图片的格式，安卓和IOS图片压缩分别是什么格式，有什么要求，不同业务场景下图片尺寸位数，mipmap又分别有什么要求和区别）
14. 服务器的战斗同步方案，给出一个优质的方案；（你需要针对不通的游戏类型和玩法做出不同的针对性优化，优化策略和网络同步方案都不同，典型代表2类，传统CS同步和帧同步）
15. 渲染管线流程是由哪些部分组成，分别有什么作用，能讲透彻是加分题；
16. vertexshader和pixel shader作用分别是什么，如何协作的；
17. 后处理是什么？你所知道的后处理效果有哪些，大概说说这些后处理实现的原理
18. 延迟渲染是什么？
19. 延迟渲染和延迟光照的区别？
20. shadow map的原理
21. 简单描述1，2个你认为最具有挑战性的功能，你是怎么思考，设计，优化的，是否还有不足，还能否继续改进。
22. 结构体和类的区别
23. ArrayList和List的区别
24. 如果想将一个class作为字典表的Key需要实现哪些函数
25. 字典表的赋值与key的遍历需要注意的地方。
26. interface与抽象类的区别
27. Unity monobehaviour的生命周期中OnEnable() Awake() Start() Update()的顺序，以及哪些会重复调用
28. MipMap是什么，以及它的作用
29. Renderer是什么，MeshRenderer与SkinnedMeshRenderer的区别
30. AplhaBlend与AlphaTest的区别
31. Unity自带的Diffuse着色器，里面的灯光运算是顶点灯光还是像素灯光，这两者有什么区别
32. 在一个固定视角的场景中有一个固定方向的方向光，怎样给渲染物体时做优化
33. 介绍项目客户端资源分包策略
34. 介绍客户端代码热更新的实现方式
35. 介绍ZTEST ZWRITE的作用
36. 渲染实体对象和半透明对象，有什么优化手段
37. AlphaTest为什么在部分移动设备存在性能问题，如何规避
38. 介绍行为树的各类节点，各类节点返回值对于父节点的影响。
39. 如何实现镜面效果
40. 是否使用过NGUI，NGUI如何管理Widget和组织drawcall
41. 后处理中，如何根据深度缓存还原出世界坐标？
42. 简述Shadow Map的原理，Shadow bias的作用是什么？
43. 热更新断点续传？

## 图形相关面试题

1. 软蒙皮和硬蒙皮的区别，硬蒙皮的限制
2. 阴影的实现方式有哪些，优缺点各是什么，方向光，点光源，聚光灯的阴影实现又有什么区别
3. 自阴影如何实现
4. 延迟渲染的原理
5. HDR，bloom，径向模糊这几个后处理的实现原理
6. Unity里Shader怎么做LOD
7. OpenGL ES3.0相比2.0增加了哪些特性

## 终面技巧

### 技术终面
1. 回顾自己的简历，对简历上所有提到的技术要点一定要非常熟悉，因为面试官会针对简历上的技术问题进行扩展；
2. 重点准备项目技术框架，包括如何搭建，技术选型，为何选用这个框架，优劣势，项目开发过程中，上线过程中，维护过程中所出现的问题，针对不通的问题有哪些解决方案，为何选用这种解决方案，优劣势对比等；
3. 重点阐述自己在项目开发中承担的角色，对项目开发的共享，自己在工作中解决的问题；
4. 时刻显示出自己对工作，对技术的热情，对新技术的关注（特别关注近期新技术，新闻等）
5. 可以重点阐述自己工作之余喜爱研究的底层技术
6. 何回答对待加班的看法：强调自己是效率性员工，自己开发所用时间段，bug少（此处可举例说明），但项目紧急会资源加班，和团队一起完成紧急任务。

### 态度

面带微笑，不卑不亢，无论是待会HR还是技术面试都给予足够尊重，当不懂技术的HR问到技术问题时，一定要当作对方是懂技术的人员去认真回答这个问题。
越是简单的问题，越考察基础，遇到简单问题要认真回答。

### 离职意愿

- 家里和自己更喜欢深圳这个城市，也是家人强烈要求到深圳长期发展，广州深圳的游戏环境也更好
- 从个人职业发展上来说，很向往XXX这样的平台，自己也私下了解了一下相关的信息，觉得XXX将来会发展的很好，希望能过来学习更多的东西；
- 本身XXX是老牌的研发公司，研发实力很强，但是就现阶段来说，自己上升及发展到达了一个瓶颈期，期望出来寻求给多的技术。

# 9130
## 客户端
1. Lua的Hash是怎么实现的？
2. Hash是怎么实现的？
3. 介绍一下项目中导航？
4. 介绍一下Mono
5. Google的Probuffer介绍
6. Lua的module是如何实现的？
7. C++和Lua是如何交互的？

## 服务器
1. 描述服务器架构？
2. 全球服中如何管理玩家连接，当有一个逻辑服宕机之后如何处理？
3. 对称加密和非对称加密？
4. TCP/IP的滑动窗口

# 搜狐二面
1. 针对项目情况问一些技术问题吧，不过问了好几次有遇到什么技术问题或者技术难点之类的。
2. 问了一下AssetBundle管理
3. 还有资源管理
4. 还有问了一下内存占用，面数，drawcall占用，drawcall如何合理分配。模型划分。
15.2K的三角形数，14.6k的顶点数，Batches：83，Draw Calls：85

# 好游科技
1. 角色遮挡显示
2. 渲染管线
3. Unity是左手坐标系还是右手坐标系
4. 屏幕坐标如何转换到世界坐标
```CSharp
Camera.main.ScreenToWorldPoint(new vector3(x,y,0))
```
5. 播放动作到一定时间的时候如何播放粒子
6. 如何将3DMAX的模型导入UNITY中，如何播放3DMAX动作文件
7. 2D场景，3D人物如何实现
8. 如何实现热更新
9. 如何读取StreamingAsset下面的资源
10. GameObject.Find和transform.Find的区别
第一个返回GameObject类型并且未激活的对象不会被查找到，第二个返回Transform类型。
11. 结构体和Class的区别
12. Foreach遍历类里面所有的成员
13. 什么是协程，为什么协程可以分布执行
14. 类内初始化变量和构造函数初始化变量的区别

# 网易TA
1. 判断点在多边形内部 
2. 不用分支语句推导三角波函数图像的函数
3. GPU instance中，渲染大量草用什么方式存储草的颜色位置比较好
4. 双线性过滤 4倍各向异性过滤 邻近点过滤和三次插值，双三次插值，线性插值优化过后的采样次数
5. 给你场景物体的顺序和混合因子让你计算最后的颜色的rgba
6. rgb转hsv的公式
7. 美术给模型做细分和GPU曲面细分的适用场合和优缺点
8. 正交投影下计算良好效果边缘光的方法
9. 描述一下沙漠的地貌特征和形成原因
10. 色相差的表现和实现方法