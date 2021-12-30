组件复制

https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/Networking/Actors/Components/

调用`AActorComponent::SetIsReplicated(true)`
会有一个`NetGUID`4字节

https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/Networking/Actors/RPCs/

# RPC同步机制
游戏世界对每个player都是隔离的概念，即使玩家在同一场游戏，玩家看到的所有变化都是服务端的一个拷贝（有玩家A玩家B服务端C，每个玩家控制一个character，则共有6个actor，A/B/C分别有两个actor：character A/character B）。也就是说，玩家在自己的世界中的操作（如行走、跳跃、开火等）并不能被其它玩家和服务器看到（只是修改自己世界中的内容），需要有一个管理员（服务器，或者某一个客户端）处理这种改变世界状态的行为并且判断这种行为是否合法，然后将这种变化更新到其它所有客户端。好在UE4提供了完整的RPC同步机制，非常方便的实现同步（暂先不考虑网络延迟、丢包等其它因素）。

按照个人理解，RPC同步可以分为：1.变量同步2.事件同步3对象同步。对象是UE世界中的一种实体，包括Actor、Character、Pawn等（类似于C++的类对象）。场景中的每一个物体每一块底板都是一个对象，玩家控制的角色也是一个对象。

# UE4属性同步（一）服务器同步属性

https://zhuanlan.zhihu.com/p/412517987

```C++
UENUM()
enum ENetRole
{
    ROLE_None,
    // 这个Actor是其它客户端在本机客户端的一个模拟代理
    ROLE_SimulatedProxy,
    // 这个Actor是本机客户端自己控制的角色
    ROLE_AutonomousProxy,
    // 表示这个Actor在服务器上
    ROLE_Authority,
    ROLE_MAX,
};
```