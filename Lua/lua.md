---
title: Lua
date: 2017-10-20 14:37:59
categories:
- Lua
---

# 让lua编译时计算
由于lua的编译速度相当快，而且这种迭代编译的过程仅仅在程序加载的时候进行一次，故而可以带来性能的提高；一些在系统初始化可以决定的参数（比如从配置文件中读出来的数据直接编译为常量置入程序中。

```lua
	local select = select
	local setmetatable = setmetatable
	local getfenv = getfenv
	local setfenc = setfenv
	local loadstring = loadstring
	local type = type
	local tostring = tostring
	local next = next
	local unpack = unpack
	local assert = assert
	local string = string
	local table = table
	local io = io

	local function result(...)
		return select("#", ...), select(1, ...) -- 参数的个数，第一个参数
	end
```

# Lua的upvalue和闭包
Lua函数可以被当成参数传递，也可以被当成结果返回，在函数体中仍然可以定义内嵌函数，Lua闭包是Lua函数生成的数据对象。每个闭包可以有一个upvalue值，或者多个闭包共享一个upvalue值。


# 什么是JIT
JIT=Just In Time即时编译，是动态编译的一种形式，是一种优化虚拟机运行的技术。
程序运行通常有两种方式，一种是静态编译，一种是动态编译，即使编译混合了这二者。Java和.Net/mono中都使用了这种技术。
然而IOS中禁止使用（不是针对JIT，而是所有的动态编译都不支持）

# 为什么要使用JIT
解释执行：
- 效率低
- 代码暴露

静态编译：
- 不够灵活，无法热更新
- 平台兼容性差。

JIT：
- 效率：高于解释执行，低于静态编译。
- 安全性：一般都会先转换成字节码。
- 热更新：无论源码还是字节码本质都是资源文件。
- 兼容性：虚拟机会处理平台差异，对用户透明。

# JIT是如何实现的
这里讲的实际上是JIT的一个变种：自适应动态编译(adaptive dynamic compilation)。它分为两种：Method JIT和Trace JIT。
简单来讲：
1. 跟踪热点函数或trace，编译成机器码执行，并缓存起来供以后使用。
2. 非热点函数解释执行。

为什么只编译热点函数？
对只执行一次的代码而言，解释执行其实总是比JIT编译要快。对这些代码做JIT编译在执行，可以说是得补偿式。而对于只执行少量次数的代码，JIT编译带来的执行速度的提升也未必必能抵消掉最初编译带来的开销。只有对频繁执行的代码，JIT编译才能保证有正面的收益。

# Lua元素(Metatable)

1.定义算术操作符和关系操作符的行为
	
	+ __add
	* __mul
	- __sub
	/ __div
	- __unm(for negation) 自减
	% __mod
	^ __pow
	
关系操作符
	
	== __eq
	<  __lt
	<= __le

当我们访问某个不存在的字段的时候，就会调用`__index`元方法。

### 源文件划分

1.虚拟机运转核心功能

|源文件名		|功能							|前缀|
|--------------|------------------------------|---|
|lapi.c 		|C语言接口						|luaC|
|lctype.c 		|C标准库中ctype相关实现			||
|ldebug.c 		|Debug 接口						||
|ldo.c 			|函数调用以及栈管理				|luaD|
|lfunc.c 		|函数原型及闭包管理				||
|lgc.c 			|垃圾回收						||
|lmem.c 		|内存管理接口					||
|lobject.c 		|对象操作的一些函数				||
|lopcodes.c 	|虚拟机的字节码定义				||
|lstate.c 		|全局状态机						||
|lstring.c 		|字符串池						||
|ltable.c 		|表类型的相关操作				|luaH|
|ltm.c 			|元方法							||
|lvm.c 			|虚拟机							||
|lzio.c 		|输入流接口						|luaZ|

2.源代码解析以及预编译字节码

|源文件名		|功能							|前缀|
|--------------:|------------------------------:|---:|
|lcode.c 	|代码生成器||
|ldump.c 	|序列化预编译的Lua字节码||
|llex.c 		|词法分析器||
|lparser.c 	|解析器||
|lundump.c 	|还原预编译的字节码||

3.内嵌库

|源文件名		|功能							|前缀|
|--------------:|------------------------------:|---:|
|lauxlib.c 	|库编写用到的辅助函数库||
|lbaselib.c 	|基础库||
|lbitlib.c 	|位操作库||
|ldblib.c 	|Debug库||
|lini.c 	|	内嵌库的初始化||
|liolib.c 	|IO库||
|lstrlib.c 	|字符串库||
|ltablib.c 	|表处理库||

4.可执行的解析器，字节码编译器

|源文件名		|功能							|前缀|
|--------------:|------------------------------:|---:|
|lua.c 		|解释器||
|luac.c 		|字节码编译器||

### Lua核心

	Lua核心部分仅包括Lua虚拟机的运转。Lua虚拟机的行为是由一组组opcode控制的。这些opcode定义在lopcodes.h及lopcodes.c中。而虚拟机对opcode的解析和运作在lvm.c中，其API以luaV为前缀。
	Lua虚拟机的外在数据形式是一个lua_State结构体，取名State大概意为Lua虚拟机的当前状态。全局State引用了整个虚拟机的所有数据。这个全局State的相关代码放在lstate.c中，API使用luaE为前缀。
	函数的运行流程：函数调用及返回则放在ldo.c中，相关API以luaD为前缀。
	Lua中最复杂和重要的三种数据类型function、table、string的实现分属在lfunc.c、ltable.c、lstring.c中。这三组内部API分别以luaF、luaH、luaS为前缀命名。不同的数据类型最终呗统一定义为LuaObject，相关操作在lobject.c中，API以luaO为前缀。
	Lua从第五版后增加了元表，元表的处理在ltm.c中，API以luaT为前缀。
	另外，核心系统还用到两个基础设施：内存管理lmem.c，API以luaM为前缀；带缓存的流处理lzio.c，API以luaZ为前缀。
	最后是核心系统里最为复杂的部分，垃圾回收部分，在lgc.c中实现，API以luaC为前缀。
	Lua设计的初衷之一就为了最好的和宿主系统相结合。它是一门嵌入式语言，所以必须提供和宿主系统交互API。这些API以C函数的形式提供，大多数实现在lapi.c中。API直接以lua为前缀，可供C编写的程序库直接调用。

### 代码翻译及预编译字节码

	光有核心代码和一个虚拟机还无法让Lua程序运行起来。因为必须从外部输入将Lua运行起来。Lua的脚本需要经过解析得到内部的数据结构（常量和opcode的集合）。这个是用parser:lparser.c （luaY:语法解析)及词法分析llex.c(luaX)
	解析完脚本代码，还需要最终生成code码，在lcode.c中实现。luaK
	为了满足某些需求，加快代码翻译的流程。还可以采用预编译的过程。把运行时编译的结果，生成为字节码。这个过程以及逆过程由ldump.c和lundump.c实现。luaU

### 内嵌库

	作为嵌入式语言，其实完全可以不提供任何库及函数。全部由宿主系统注入到State中即可。也的确有许多系统是这么用的。但Lua的官方版本还是提供了少量必要的库。尤其是一些基础函数paris、error、setmetatable、type等等，完成了语言的一些基本特性，几乎很难不使用。
	而coroutine、string、table、math等等库，也很常用。Lua提供了一套简洁的方案，允许你自由加载你需要的部分，以控制最终执行文件的体积和内存的占用量。主动加载这些内建库进入lua_State，是由在lualib.h中的API实现的。
	
```C
static TValue *index2addr (lua_State *L, int idx) {
	CallInfo *ci = L->ci;
	if (idx > 0) {
		TValue *o = ci->func + idx;
		api_check(L, idx <= ci->top - (ci->func + 1), "unacceptable index");
		if (o >= L->top) return NONVALIDVALUE;
		else return o;
	}
	else if (!ispseudo(idx)) { /* nagative index */
		api_check(L, idx != 0 && -idx <= L->top - (ci->func + 1), "invalid index");
		return L->top + idx;
	}
	...
}
```

```C#
public class LuaMgr
{
	public static Module GetModule(string name)
	{
		Module m = m_modules.PG_TryGetValue(name);
		if (m == null)
		{
			...
		}
		// repeat
		if (m.m_state != Module.State.None)
		{
			if (m.m_state == Module.State.Failed) return null;
			return m;
		}

		var L = m_L;//lua_State
		string[] names = name.Split(m_dotSep); // mulit file
		LuaDLL.lua_pushvalue(L, LuaIndexes.LUA_GLOBALSINDEX); // 0
		for (int i = 0; i < names.Length; ++ i)
		{
			LuaDLL.lua_pushstring(L, names[i]);
			LuaDLL.lua_rawget(L, -2); //

			var t = LuaDLL.lua_type(L, -1); // 1
			switch (t)
			{
				case LuaTypes.LUA_TNIL:
					LuaDLL.lua_pop(L, 1);

					luaDLL.lua_newtable(L);
					luaDLL.lua_pushstring(L, names[i]);
					luaDLL.lua_pushvalue(L, -2);
					LuaDLL.lua_rawset(L, -4); // _G.name = module

					LuaDLL.lua_rawgeti(L, LuaIndexes.LUA_REGISTRYINDEX, m_ref_globalMeta);
					LuaDLL.lua_setmetatable(L, -2);
					LuaDLL.lua_pushvalue(L, -1);
					LuaDLL.lua_setfield(L, -2, "_M"); // module._M = module
					break;
				case LuaTypes.LUA_TTABLE:
					break;
				default:
					m.m_state = Module.State.Failed;
					LuaDLL.lua_pop(L, 2);
					return null;
			}

			LuaDLL.lua_remove(L, -2);
		}

		m.m_index = ++m_moduleIndex;
		m.m_ref_module = LuaDLL.luaL_ref(L, luaIndexes.LUA_REGISTRYINDEX);
		ReloadModule(m);
		return m;
	}
}
public static int LoadModule()
{
	var L = LuaMgr.m_L;
	string name = LuaDLL.lua_tosting(L, 1);
	var m = LuaMgr.GetModule(name);
	LuaDLL.lua_pushboolean(L, m != null);
	return 1;
}
```

### 阅读源代码的次序

首先、阅读外围的库是如何实现功能扩展的，这样可以熟悉Lua公开API，不必陷入功能细节。
然后、阅读API的具体实现。Lua对外暴露的API可以说是一个对内部模块的一层封装，这个层次尚未触及核心，但可以对核心代码有个初步的了解。
之后、可以开始了解Lua VM的实现。
接下来就是分别理解函数调用、返回，string,table,metatable等如何实现。
debug模块是一个额外的设施，但可以帮助你理解Lua内部细节。
最后是parser等编译相关的部分。
垃圾回收是最难的部分，可能会花掉最多的时间去理解细节。

# Lua模块与包
模块类似于一个封装库，从Lua5.1开始，Lua加入了标准的模块管理机制，可以把一些公用的代码放在一个文件里，以API接口的形式在其他地方调用，有利于代码的重用和降低代码耦合度。
Lua的模块是由变量、函数等已知元素组成的table，因此创建一个模块很简单，就是创建一个table，然后把需要导出的常量、函数放入其中，最后返回这个table就行。以下为创建自定义模块module.lua，文件代码格式如下：
```lua
-- 文件名module.lua
-- 定义一个名为module的模块
module = {}
-- 定义一个常量
module.constant = "这是一个常量"
-- 定义一个函数
function module.func1()
	io.write("这是一个公有函数！\n")
end

local function func2()
	print("这是一个私有函数！")
end
function module.func3()
	func2()
end
return module
```
由上可知，模块的结构就是一个table的结构，因此可以像操作table里的元素那样来操作调用模块里的常量或函数。
上面的func声明为程序块的局部变量，即表示一个私有函数，因此是不能从外部访问模块里的这个私有函数，必须通过模块的公有函数来调用。
# require 函数
Lua提供了一个名为require的函数用来加载模块。要加载一个模块，只需要简单地调用就可以了。例如：

	require("<模块名>")

或者

	require "<模块名>"

执行require后会返回一个由模块常量或函数组成的table，并且还会定义一个包含该table的全局变量。
## 加载机制
对于自定义的模块，模块文件不是放在哪个目录都行，函数require有它自己的文件路劲加载策略，它会尝试从Lua文件或C程序库中加载模块。
require用于搜索Lua文件的路径是存放在全局变量package.path中，当Lua启动后，会以环境变量LUA_PATH的值来初始这个环境变量。如果没有找到该环境变量，则使用一个编译时定义的默认路径来初始化。
当前目录也会加载

