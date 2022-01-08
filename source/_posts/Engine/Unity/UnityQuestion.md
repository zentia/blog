---
title: Unity Question
date: 2017-10-23 12:07:11
categories:
- Unity
---

# AssetBundle lockpersistentmanager开销
观察性能曲线，发现某一帧AssetBundle加载中，lockpersistentmanager耗时比较大。请问这块是否能够优化？
这说明当前帧或前几帧中存在较大量的资源在通过LoadAsync来进行加载，其本质是所加载的资源过大所致，对自身资源进行合理优化可降低Loading.LockPersistentManager的开销。另外，将异步加载换成同步加载，LockPersistentManager就不会出现了，但其总加载耗时是没有变化的，因为总加载量没变。
关于主要资源的加载优化，可参考如下链接：
https://blog.uwa4d.com/archives/LoadingPerformance_Texture.html 
https://blog.uwa4d.com/archives/LoadingPerformance_Mesh.html 
https://blog.uwa4d.com/archives/LoadingPerformance_Shader.html 
https://blog.uwa4d.com/archives/Loading_AnimationClip.html 
https://blog.uwa4d.com/archives/livebroadcast6-8.html
	
	** IN DIR:3rd, make dep with Makefile_debug.mk dep **
   	make[1]: 进入目录“/root/work_code/3rd”
   	** IN DIR:cryptlib, make dep with Makefile_debug.mk dep **
   	make[2]: 进入目录“/root/work_code/3rd/cryptlib”
	g++ -DLINUX -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -march=core2 -m32 -MM -MT "../temp/debug/cryptlib/cpu.o_d" -MF ../temp/debug/cryptlib/cpu.d_d cpu.cpp -I/usr/include/mysql  -I/root/work_code/3rd -I/root/work_code/3rd/freetype221 -I/root/work_code/src/engine -I/root/work_code/src/svr -I/root/work_code/src/xxsy -I/root/work_code/src/svr/glogger/tlog/linux
   	<built-in>:0:0: 致命错误：打开依赖文件 ../temp/debug/cryptlib/cpu.d_d：没有那个文件或目录

	-DLINUX 				#define LINUX
	-D_FILE_OFFSET_BITS 	#define _FILE_OFFSET_BITS
-D_LARGFEFILE64_SOURCE	
march指定的是当前cpu的架构，而mtune是真正作用于某一型号cpu的选项。
	-M 生成文件关联的信息。
	-MM 忽略由#include造成的依赖关系
	-MF 指一个文件用于存放生成文件的关联信息，这些信息与-M或-MM是一样的，
	-MT 指定目标文件名

# Camera.Render里面Animation.RebuildInternalState耗时较多
之所以出现这一项，主要可以说以下两点：
（1）项目用的是Animation老版动画系统；
（2）对含有Animation组件的GameObject进行了大量频繁的Active或者Instantiate操作，一般这种情况比较常见于特效、UI HUD、角色/怪物等。
# 是否可以针对特定GameObject提升它的物理模拟次数
我们在做一个尽速项目，发现有时候物理的计算并不是非常准确，在运动速度较快时，依然会和墙体造成部分穿插，这给我们的游戏带来了很不好的体验，我知道可以通过Fixed TimeStep来提升物理系统的每帧的计算速度，目前该值为0.02，也就是1秒计算50次，Edit>Project Settings>Time>Fixed TimeStep=0.02。我们曾将其设置为0.005，穿插问题明显好转，但是看上去该值是对所有GameObject均使用的，所以我想问问能否仅针对某一个GameObject来进行提升它的物理计算频率？
这是不行的，因为Fixed TimeStep是Unity引擎物理模块的全局参数。建议题主可以考虑将碰撞检测的模式改为"Continuous Dynamic"，看看是否满足需求。但是，需要说明的是，如果物体的速度确实非常快，那么任何一种碰撞模式都不能100%达到不穿插的效果。建议题主可以考虑增大碰撞体的Size或者通过射线求交的方式来将出现穿插问题的概率降到最低。

# 请教AssetBundle Diff Patch 方案是否可行
我原先的更新方案是比对AssetBundle文件的hash值直接替换整个AssetBundle和manifest来达到更新效果的，有没有其它方案类似diff patch,可以减少更新包体，如果可以做diff patch，资源颗粒度是不是九可以忽略了
-----------------------------------------------------------------------------------------------------------------------------
基于AB的直接Diff更新目前没有成熟的方案，不过只把完整的Resources库中的部分资源更新的项目，已经做了2个了。
思路其实很简单：
1、出整包的时候，用ScriptableObject记录下AssetsDataBase中所有被发布资源的MD5码（不能用Unity自己的hash码）。
2、用工具调出哪些assets是代码动态Load的，分一个Res包中；找出这些资源的依赖资源，分入一个Share包中，并记录每个Asset所属的AB包。
3、出补丁时，对比当前AssetsDataBase中哪些代码中动态Load的资源出现了增加和改动（删除可以无视），包括他们依赖的资源。仍然把代码中动态Load的资源ResPatch中，依赖放入SharePatch中。对于没有变化的资源，仍然维持原来AB名字。
4、记录下次补丁之后，更新每个Asset所属的AB包。
5、下一次补丁时的Res和Share可能会依赖上一次的补丁的Share。
6、运行时，对所有补丁入的Res建立一个字典索引。Load的时候，有限判断这个Asset是否在补丁的Res包中，如果是则读补丁的AB包，并按套路处理依赖包。否则就用二进制版本内建的包

这种做法的好处是：
1、充分利用版本中的已经发布的资源，以减小补丁包的体积
2、补丁包的打包规则可以几乎无视完整包的分包规则，方便后续折腾


当然缺点也是很严重的：

- Reserved Total 				- Unity引擎在内存方面的总体分配量。Unity引擎的内存并不是随用随取，而是预先从操作系统中申请一块，然后再进行使用。
- Uesd Total 					- Unity引擎再内存方面的总体使用量。
- Reserved Unity & Used Unity 	- Unity引擎自身各个模块内部的内存分配，宝库偶各个Manager的内存占用、序列化信息的内存站哟个，WebStream/SerializedFile内存占用和部分资源的内存暂用等等。 

看知乎的时候发现一个好玩的东西，https://lab.uwa4d.com/lab/5bc42d5404617c5805d4d685 UGUI绘制图表工具，一直想做一个性能分析工具。
GDC2018中描述了Northlight引擎的内部实现和渲染相关的一些最新进展。
前育碧的两位资深老员工成立Second Order公司后，开发了第一款游戏《Claybook》，其中包括了许多创新技术，比如基于GPU渲染的粘土与流体模拟、完全可变性的世界与角色、光线追踪的视觉效果。

- 请问Unity的stats窗口上显示的Batches和SnapdragonProfiler转出来的DC差距比较大可能是因为什么？
{% asset_img 1.jpg %}
这个Batches的数量其实并不代表DrawCall的数量，他其实和Render面板Total Batches的数量匹配，你如果要看DrawCall的数量，那么可以查看Render面板上面的DrawCall统计，这个统计应该和你SnapdragonProfiler抓出来的差不多。然后再贴一下Rendering面板上几个参数代表啥意思，仅供参考。这是我本机的截图
{% asset_img 2.jpg %}
Render界面各个参数说明
SetPass Calls:shader状态切换次数
Draw Calls:绘制调用次数
Total Batches:总合批的数量=静态合批+动态合批+实例合批；这个数一定是比Draw Calls的数量小，小的越多说明合并的物体越多。
动态合批：Batched Draw calls:表示合批过的mesh数，比如上面显示的60个mesh合批。
动态合批：Batched：表示合批的次数，上面截图数据说明60个mesh通过47次合批才完成。其实如果情况好的时候可能几次合批就能够合并掉60个mesh，可惜可能是很多属性不符合动态合并要求。
静态合批与实例化与上面动态合批的解释相同。
- so热更新除了只更新il2cpp.so还是修改global-metadata.data，其他的so不需要更新。