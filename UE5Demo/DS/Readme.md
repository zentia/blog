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