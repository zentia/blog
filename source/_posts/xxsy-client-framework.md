---
title: 寻仙手游客户端分析
date: 2018-04-20 18:55:07
tags:
  - Unity
  - MMORPG
---

寻仙手游客户端是采用Unity5.3.8+uLua开发的一款MMORPG手游。

# 流程顺序

应用程序启动
Handheld.PlayFullScreenMovie通过该方法播放视频文件
进入更新模块
SDK初始化安卓通过AndroidJavaClass访问Java实例

# 资源模块

# UI模块

# 更新模块

# 网络模块
这里是在多线程中做的，使用的是NetworkStream这个。

# Lua模块

## Lua 注册

```C#
[MonoInvokeCallbackAttribute(typeof(LuaCSFunction))]
static int call(System.IntPtr L)
{
    int upidx = LuaDLL.lua_upvalueindex(1);
    int fnidx = (int)LuaDLL.lua_touserdata(L, upidx);
    if (fnidx <= || fnidx > m_count)
    {
        LuaDLL.luaL_error(L, "SafeCallApi : bad function index " + fnidx);
        return 0;
    }
    var info = m_infos[fnidx - 1];
    try
    {
        if (L != LuaMgr.m_L)
        {
            using (new GuardL { L = LuaMgr.m_L})
            {
                LuaMgr.m_L = L;
                int v = info.function();
                return v;
            }
        }
        int r = info.function();
        return r;
    }
    catch (System.Exception err)
    {
        LuaDLL.lua_pushstring(L, "SafeCallApi : exception \n" + err.ToString());
    }
    LuaDll.lua_error(L);
    return 0;
}
```

# 技能模块

# 角色模块

# 动画模块

# 状态模块

# CD模块

# 状态模块

# 场景模块

# 子物体模块

# 同屏系统

每个场景里一般都有很多对象，对其中一个对象而言，它一般只关注自己周围一定范围内的其它对象。同屏系统的主要工作就是将对象周围一定范围内的其它对象精确且高效的统计，为客户端的表现以及其它系统提供服务。

我们采取的思想是每个对象维护了一个同屏信息，同屏信息主要包括同屏对象列表和同屏玩家列表。

导致同屏信息表改变的原因主要有三个：新对象进入场景，对象位置改变，对象离开场景。
针对以上三种原因我们采取了两种实现机制：定时更新机制，立刻更新机制。
立刻更新机制：
    主要针对对象离开场景的情况。
    实现步骤主要是根据离开对象记录的同屏信息，再相关联对象和自己的同屏信息内相互去掉。
定时更新机制：
    主要针对对象进入场景和对象位置改变两个原因。
    针对对象进入场景原因实现主要是对象进入场景时，设置新对象标记。然后通过定时更新实现同屏信息变化。
    针对对象位置改变原因实现步骤主要是对象位置改变，设置位置变更标记。然后通过定时更新实现同屏信息变化。
    定时更新机制是每个tick对场景内所有本tick进行过位置变化或者新进入的对象进行同屏信息更新。更新的主要逻辑是再同屏信息内的对象若离开同屏则进行清除和未在同屏信息内的玩家若满足进入同屏则加入。
    由于定时更新使用频繁且实现复杂，因此采取了搞笑的实现方法来实现这个功能。由于定时更新需要检测哪些对象进入同屏，如果整个检测整个场景内的对象，比较费，由此提出了单元格概念(CellSpace)，就是把场景划分N * N的格子，每个各自作为一个单元格，单元格内维护对象列表。然后再根据玩家的同屏距离，首先获得所在的单元戈，再判断对应单元格的对象是否进入对象同屏，减少判断数量。

游戏专用子系统
武器、道具、
游戏专用渲染
地形渲染、水体模拟及渲染
玩家机制
状态机及动画、相对摄像机的操控（HID）、碰撞流行、移动
游戏摄像机
固定设想、脚本/动画摄像机、跟踪玩家摄像机、调试用飞行摄像机
人工智能
目标及决策、动作（引擎接口）、视线追踪及感知、路径搜寻（A* ）
前端
平视显示器（HUD）、全动视频（FMV）、游戏内置电影（IGC）、游戏内置GUI、游戏内置菜单、包裹/吸引模式
视觉效果
光照贴图及动态阴影、高动态光照、PRT光照此表面散射、粒子及贴花系统、后处理效果、环境贴图
场景图/剔除优化
空间部分（BSP树）、遮挡及潜在可见集、纹理及表面管理、调试绘图（直线等）、图形设备接口
性能剖析及调试
录影及回放、内存及性能统计、游戏内菜单或控制台
游戏性基础
高层次的游戏流程系统/FSM
脚本系统
静态世界元素、动态游戏对象模型、实时基于代理人模拟、时间/信息系统、世界载入/串流
骨骼动画
动画状态树及蹭、反向动力学（IK）层阶式物体依附、游戏专用的后期处理、线性插值、动画播放、子骨骼播放、动画解压、骨骼网格渲染、布娃娃物理
在线多人
安排比赛及游戏管理、对象管辖权策略、游戏状态复制
音频
DSP/效果、三维音频模型、音频播放/管理
低阶渲染器
材质及着色器、静态及动态光源、摄像机、文本及字体、几何图元提交、视区及虚拟屏幕、纹理及表面管理、调试绘图（直线等）、图形设备接口
碰撞及物理
力及约束、光线/形状投影（查询）、刚体、Phantom、形状/可碰撞体、物理/碰撞世界
人体学接口设备（HID）
游戏专用接口、物理设备I/O
资源（游戏资产）
三维模型资源、纹理资源、材质资源、字体资源、骨骼资源、碰撞资源、物理参数、游戏世界/地图、其他
核心系统
模块启动及终止、断言、单元测试、内存分配、数学库、字符串及散列字符串标识符、调试用打印及日志、本地化服务、影片播放器、语法分析器（CSV、XML等）、性能剖析/统计采集、引擎配置（INI文件等）、随机数生成器、曲线及曲圆库、RITI/反射/序列化、对象句柄/唯一标识符、异步文件I/O
平台独立层
平台检测、原子数据类型、集合及迭代器、文件系统、网络传播层（UDP/TCP）、高分辨率时钟、线程库、图形包裹类、物理/碰撞包裹类
第三方软件开发包
DirectX\OpenGL、Boost、STL/STLPort