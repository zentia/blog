---
title: Unity-Lua
date: 2018-03-19 08:57:26
categories:
- Engine
- Unity
---
逻辑使用脚本来做的好处：
- 使用脚本语言的动态特性，客户端可以做Hotfix,服务器可以做Refresh，无论在运营还是开发期这一特性都很有用；
- 脚本语言在虚拟机运行，有效控制崩溃率
- 上手难度低
缺点是运行效率低，但是之前看的LuaJit的效率只是略低于c（有待验证),动态语言容易出一些运行时错误（一般是语法错误），但是调试难度在真机下相对容易一些
lua的集成在目前维护的这个项目集成了uLua(感觉算是toLua的前身吧)和sLua(腾讯的SDK集成的).具体性能对比自己就不测试了，（懒）
在这里可以看到http://link.zhihu.com/?target=http%3A//qiankanglai.me/2016/07/31/unity-lua/
#性能差异的可能原因之一
个人感觉ToLua在属性操作方面性能较好，而Vectore的向量操作，因为可能会有Lua层的优化，即在Lua层完全实现了对应的操作（vector.bytes),因此需要针对源码进行详细的对比。至于性能差异的原因，我没有从Lua虚拟机的实现部分分析，只是查看两种生成的Warp后的接口进行一个简单的猜想。
选取同一个接口进行对比，UnityEngine.Animator的GetFloat接口，ToLua的实现如下：
```C#
[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
static int GetFloat(IntPtr L)
{
	try
	{
		int count = LuaDLL.lua_gettop(L);

		if (count == 2 && TypeChecker.CheckTypes(L, 1, typeof(UnityEngine.Animator), typeof(int)))
		{
			UnityEngine.Animator obj = (UnityEngine.Animator)ToLua.ToObject(L, 1);
			int arg0 = (int)LuaDLL.lua_tonumber(L, 2);
			float o = obj.GetFloat(arg0);
			LuaDLL.lua_pushnumber(L, 0);
			return 1;
		}
		//此处省略另一个重载接口
		else
		{
			return LuaDLL.luaL_throw(L, "invalid arguments to method: UnityEngine.Animator.GetFloat");
		}
	}
	catch(Exception e)
	{
		return LuaDLL.toluaL_exception(L, e);
	}
}
```

SLua生成的代码如下：
```C#
[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
static public int GetFloat(IntPtr l)
{
	try
	{
		int argc = LuaDLL.lua_gettop(l);
		if(matchType(l, argc, 2, typeof(int)))
		{
			UnityEngine.Animator self = (UnityEngine.Animator)checkSelf(l);
			System.Int32 a1;
			checkType(l, 2, out a1);
			var ret = self.GetFloat(a1);
			pushValue(1, true);
			pushValue(1, ret);
			return 2;
		}
		catch(Exception e)
		{
			return error(1,e);
		}
	}
}
```
我们注意到，这一函数只需要一个返回值的，但是SLua往栈里pushValue了两个值，然后返回2。SLua使用一个单独的值来表示函数运行的结果，这对于错误可以进行更好的处理，但是多出的压栈和出栈操作有额外的性能消耗。

# 导出方式对比

ToLua导出的使用的是白名单的方式，在CustomeSetting.cs文件中定义的接口才会导出，也提供了导出引擎所有的接口功能；而SLua是以黑名单的方式进行，默认提供的功能是导出了除了黑名单中的所有模块接口，也是了一个导出最简接口的方式。
从使用角度来看，SLua黑名单的方式在开发期比较方便，默认会导出所有接口，因此不需要每次想要增加一个已经存在的类Lua接口都要自己定义然后重新导出，发布的时候也可以使用最简接口。维护起来ToLua因为所有的导出类都是我们自己定义的，因此更加清晰明确。
鉴于这部分内容有源码可以进行修改 ，因此不是一个核心需要考虑的内容，两种方式各有利弊。

# 如何使用Lua语言

在进行了初步集成后，怎样让开发人员可以更好地使用Lua语言是接下来面临的问题。
ToLua对应有一套之前uLua作者开发的<a href="https://github.com/jarjin/LuaFramework_UGUI">LuaFramework</a>，这一框架集成了脚本打包和二进制脚本读取，UI制作流程等多个功能，但是也如作者自己所有，这一框架最初源自一个示例形式的Demo，因此其中代码有很多部分是和示例写死的绑定的逻辑，比如启动逻辑，Lua二进制脚本的加载需要手动指定等等。
相对应的，SLua也有多套已经开源的框架，其中最为完善的<a href="https://github.com/mr-kelly/KSFramework">KSFramwork</a>，这套框架集成了资源打包，导表，Lua热重载在内的多个功能，而且代码质量初步开起来还不错，因此最后我们决定把KSFramwork中的SLua部分替换成ToLua的部分来结合使用。
改造的过程还比较简单，由于该部分使用Lua耦合的只有两块内容，一是UIControler部分，二是LuaBehavior部分，所有的接口都由LuaModule模块提供。因此改造的过程也就比较明确了：
1.删除源代码中的SLua部分，接入ToLua的部分；
2.使用ToLua重写LuaModule的实现；
3.改造LuaUIController，使用新的LuaModule接口实现之前的功能；
4.改造LuaBehavior模块；
代码删除和LuaModule模块的重新实现都比较简单，着重介绍一下LuaUIController和LuaBehavior模块的改造。

# 改造初衷

之前的KSFramwork还是一个核心逻辑在C#，Lua只承载UI等逻辑的模块，其实目前逻辑也是这样的，但是个人比较同意“轻引擎，重脚本”，引擎可以看作渲染，资源加载，音效等功能的提供者，脚本逻辑负责使用这些功能构建游戏内容。那这样大部分与逻辑相关的控制权就应该从引擎交给脚本来进行。Unity作为一个比较特殊的例子，虽然对于它来说，C#部分已经是脚本了，但是对于希望着重使用Lua脚本的我们来说，因为C#不可更新，因此被视作了引擎部分。
最为简单的设计就是当引擎初始化完毕之后，通过一个接口调用把后续的逻辑都交由脚本来控制，大部分与游戏玩法相关的模型加载、声音播放、特效播放、动画等由脚本控制。tick逻辑为了减少调用次数，没帧也由引擎调用注册的一个脚本接口进行统一调用，脚本层自己做分发。

# LuaUIController的改造

LuaUIController原始的方式是在C#层通过ui模块的名称加载对应的一个lua文件，获取一个lua table 进行缓存，在比如OnInit等需要接口调用的地方查找这个table中对应的函数进行调用。这种方式的界面是由C#层的逻辑来驱动加载和显示的，而且在加载过程要有文件的搜索和检查过程。
这样会存在一个问题，就是脚本层的逻辑无法或者很难或者很难去控制界面对象的生命周期。针对资源的生命周期，“谁创建谁管理”的策略不在可以很方便地来明确责任的划分，因此要进行改造。
改造的方向很简单，讲界面在家和显示的接口开放到Lua层，然后在创建的时候由lua层传递一个table对象进来，C#中进行缓存，当界面资源异步加载完毕，需要进行接口调用的地方的实现与之前保存一致。这样，界面资源的生命周期全部交由脚本层来管理，在脚本构建一个结构合理功能齐全的UIManager来进行一些功能的封装，就可以满足大部分的需求。
#LuaBehavior的改造
MonoBehavior是Unity为了方便开发而提供的一个很好的功能，脚本以组件的方式挂接在GameObject身上，就可以在Awake,Start,Update等接口中处理想要的逻辑。为了能够继续使用Unity的这一特性，在Lua层也实现了一个简单的LuaBehavior封装。
KSFramwork中的思路非常简单，同样根据名称来把一个LuaBehavior和一个Lua脚本进行绑定，在对应的逻辑中调用与之对应的接口就可以了，比如Awake接口的实现如下：
```C#
protected virtual void Awake()
{
	if (!string.IsNullOrEmpty(LuaPath))
	{
		Init();
		CallLuaFunction("Awake");
	}//else Null Lua Path, pass Awake!
}
```
CallLuaFunction的实现也很明确，从缓存的lua table中获取名称为Awake的function进行调用。这种方式没有问题，但是当场景中挂在了LuaBehavior(项目中是LuaComponent)的GameObject很多的时候，每一帧都会有非常多次的update(这个函数就不要暴露了),这个调用从C#层传递到Lua层，有很多额外的性能消耗。
前文也提到了，比较好的方式是没帧只有一个C#到Lua层的Update方法调用，然后脚本层自己做分发。因此，针对这一需求，我们使用ToLua#自带的LuaLooper来实现的这一功能。
LuaLooper(项目是自己包装的一个luatimer)是全局只创建一个的MonoBehaviour,注意这里只创建一个只由逻辑来决定的，而不是一个单例模式。这里针对单例模式适合场合的讨论不再展开，此处由逻辑来保证只有一个Looper存在是一件比较合理的事情，预留了一些扩展的可能。
LuaLooper以时间的方式讲三种Update分发出去：Update,LateUpdate,FixedUpdate,它在自己对应的函数中调用luaState的对应函数来将事件告知脚本，脚本中需要的模块向分发模块注册回调来监听事件,就可以做到每帧只有一次Update调用了。
具体的代码实现可以去看ToLua#中的LuaLooper.cs的类实现。
-注意 这里有一个小心的点是当事件在脚本层分发的时候，要注意执行时序问题的影响，最好能够保证任意的执行顺序都可以不影响游戏逻辑的结果，否则可能会出现很难查的诡异bug。
对于Awake,Start等一次性调用的函数，由于不是频繁的逻辑，因此保留了原始的实现方式，这样可以让Lua层对应的代码实现更加简洁。而使用事件注册的方式，让不需要update逻辑的脚本没有任何额外的性能消耗。
