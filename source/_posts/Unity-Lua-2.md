---
title: Unity手游开发-使用Lua来开发大型游戏（下）
date: 2018-03-19 21:09:20
tags:
    - Lua
---

# 基本原理

Hotfix的基本原理依然是基于动态语言的Reload功能，更加准确的说是Function Reload。下图简单描述了整个Hotfix的流程：
<img src="http://oxcvfpext.bkt.clouddn.com/Unity-Lua-2-1.png">
更加具体地可以描述为：
1.程序发现要修复的bug，编写特殊的Hotfix代码进行修复，测试通过后上传svn服务器；
2.通过发布指令，将svn上更新后的Hotfix代码同步到服务器上；
3.服务器发现Hotfix代码有更新，则将其压缩序列化后通过socket发送给所有在线的客户端，同时带上字符串的MD5值供客户端验证；
4.客户端收到Hotfix消息之后，首先反序列化数据得到代码内容，校验MD5值之后，如果和本地已经执行过的Hotfix的MD5值，则执行替换逻辑，并记录当前已经执行过的Hotfix的MD5值，如果相同则不再执行；
5.客户端连接服务器的时候会主动请求一次Hotfix。

# 实现方式(项目通过打一个更新包来实现，如果是表格的话可以走Hotfix)

执行Hotfix执行的代码非常简单，基于loadstring函数即可：
```Lua
local f = loadstring(GameContent.HotfixData)
if f then
	ClientUtils.trycall(f)
end
```
这里的实现就没有reload那么复杂，但是也是有一定的限制，比如local的函数或者在闭包的函数依然很难做正确的hotfix,需要编写特殊的Hotfix代码。

## 向lua中注册c函数的过程是通过lua_pushcclosure(luaState, fn, n)函数实现的

