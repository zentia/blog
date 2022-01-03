---
title: DS
date: 2022-01-03 23:12:00
tags:
    - Unreal
---
三方：玩家本地、服务器、其他客户端。对于你的游戏内角色，在三方都有一份。
可复制
在构造函数内加上bReplicates=true;
加上之后，客户端生成该类的对象时，会自动的在所有客户端生成一个相同的对象。同时也是属性同步的必备条件。
Role、RemoteRole
Role是本地的角色，RemoteRole是远端的角色。
对于客户端来说，RemoteRole处于服务端。反之亦然。
Replicate/属性同步
属性同步/复制，是一种状态同步的架构，既有一些属性在第三方的Actor内保持一致，Role和RemoteRole保持一直。本地获取玩家输入，通过DedicatedServer，同步至其他服务器。
# 客户端服务器模式

https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/Networking/Server/

```C++
/**
 * 当前游戏运行的网络模式
 * @see https://docs.unrealengine.com/latest/INT/Gameplay/Networking/Overview/
 */
enum ENetMode
{
    /** 独立：游戏作为服务器运行，不接受远程客户端连接。参与游戏的玩家必须为本地玩家。此模式用于单人游戏和本地多人游戏。其将运行本地玩家使用的服务器逻辑和客户端逻辑。多个手柄 */
    NM_Standalone,
    /** DS模式，没有本地玩家，在服务器上运行 */
    NM_DedicatedServer,
    /** 服务器运行。接受远程客户端连接，且直接在服务器上拥有本地玩家。此模式通常用于临时合作和竞技多人游戏，局域网游戏，红警，War3那种 */
    NM_ListenServer,
    /** 网络多人游戏绘画中与服务器连接的客户端运行。其不会运行服务器逻辑 */
    NM_Client,
    NM_MAX,
}
```
# 编译时确定的网络模式
虚幻中有一堆宏来控制编译，和网络模式相关有如下几个
- `UE_SERVER`: 开了这个宏的肯定是服务器了，编译时就会把客户端相关的模块给剔除掉，比如渲染相关的
- `WITH_SERVER_CODE`: 这个宏来控制网络逻辑代码的开关，如果是`!WITH_SERVER_CODE`，那就是纯客户端了，在`PlatformProperties.h`中，`FWindowsPlatformProperties/FMacPlatformProperties/FLinuxPlatformProperties`，这三个模板类的`IS_CLIENT_ONLY`参数传的就是`!WITH_SERVER_CODE`

`FPlatformProperties`中封装了几个函数来判断编译是一什么模式来编译的
- `IsServerOnly`:如果是true则表示编译的时候是以`DedicatedServer`的模式来编译代码，会剔除客户端相关的代码，比如渲染模块，也不包含`Editor`相关的代码
- `IsGameOnly`:如果是true则表示编译时剔除`Editor`相关的代码
- `IsClientOnly`:如果是true则表示编译时剔除了服务器和`Editor`相关的代码，这里需要注意的时，在以`xxx client`模式来出包，如果出的是`Windows/Mac/Linux`，则这个函数返回的是true，如果出的移动端的包，则返回false，因为`FIOSPlatformProperties/FAndroidPlatfromProperties`，没有覆盖掉父类中的默认`IsClientOnly`实现，父类的默认实现返回的false，不明白这里为啥行为不一致

# IsRunningDedicatedServer/IsRunningGame/IsRunningClientOnly
这几个函数和上面的三个函数类似，在非`Editor`模式下的返回值和上面三个函数一致，区别在于当运行`Editor`时，可以通过命令行来切换运行模式，具体如下
- 命令行中包含`SERVER`或者`RUN=SERVER`时，以`DedicatedServer`模式模拟运行
- 命令行中包含`GAME`时，以无`Editor`模式模拟运行
- 命令行中包含`GAME`和`ClientOnly`时，以纯客户端模式模拟运行

# 编译配置和GetNetMode的关系
编译配置联机模式（主机）GetNetMode返回值联机模式（客户端）GetNetMode返回值单机模式GetNetMode返回值xxx Client无此模式`NM_ClientNM_Standalone`xxx
Server`NM_DedicatedServer`无此模式
Debug/DebugGame/Development/Shipping `NM_ListenServerNM_ClientNM_Standalone`
# 搭建DS服务器

Actor的所有权-ROLE
UE将Actor的控制权分成了三类，分别是：
- ROLE_None：这个None就是我们平常理解的None，不属于下面三种的都是None
- ROLE_Authority: 服务器拥有所有 Actor的控制权，即所有的Actor的服务端的控制权都是ROLE_Authority
- ROLE_AutonomousProxy:客户端对本地Actor拥有这个控制权
- ROLE_SimulatedProxy:客户端对网络Actor,即其他端的Actor，拥有这个控制权

这三个属性是UE设计Actor时就为Actor设计好的固有属性，可以用于判断一个Actor所在位置，因为UE的服务器代码和客户端代码是一体的，所以Actor设计这个属性是十分必要的。