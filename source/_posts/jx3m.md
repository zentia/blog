---
title: 剑网3口袋版刨析
mathjax: true
date: 2019-05-23 10:40:02
tags:
    - 剑网3口袋版
categories: 剑网3口袋版
---
Unity版本2018.3
# 启动流程
start场景挂了两个节点一个是Start在上面下挂了一个GameStart脚本，作为启动脚本，另一个是Camera没什么必要。
GameStart在Start函数调用RuntimeCoroutineTracker.InvokeStart(this, HelperUtil.Steper(30, Initialize(), Sleep));封装协程的目的是抓帧用的。HelperUtil.Steper间隔执行。
在Initialize中初始化逻辑，其中yield return Lifecycle.GameModuleMgr.InitCoroutine(0, RefreshProgress);加载游戏模块。
在到GameModule的InitCoroutine中执行Init。说实在的OOP这一套看代码看的确实很烦。然后看一下GameMoudle有哪些
- AccountModule
- AchievementManager
- AngleFilterDataModule
- AngleFilterManager
- ArenaModule
- BarrageManager
- BattlegroundModule
- BrokenManager
- CameraManagerController
- CameraMotionPathManager
- ChallengeModule
- CharacterVoceManager
- CharManager
- SceneLoad
- MidasManager
- PVEModule
- Lua

好了由于太多了就不一一列举了。虽然都是Module但是会涵盖具体玩法和一些公共组件。让人看起来很迷，甚至认为有的代码真的会用到的感觉吗。这里我们重点分析Lua。
在Lua.OnInit中，调用luasvr = new LuaSvr();这里面会生成一个LuaSvrProxy这个GameObject，然后在它的Update中处理time等的tick事件。加载了一个调用DoFile(UIDef.LuaScriptPath);其中UIDef.LuaScriptPath = "Lua/PreLoadList.lua";这块进行了预加载Lua脚本。

# Lua

lua使用的slua。
我们看一个addMember函数
```CSharp
public static void addMember(IntPtr l, string name, LuaCSFunction get, LuaCSFunction set, bool instance)
{
    checkMethodValid(get);
    checkMethodValid(set);

    int t = instance ? -2 : -3;

    LuaDll.lua_createtable(l, 2, 0);
    if (get == null)
        LuaDLL.lua_pushnil(l);
    else
        pushValue(l, get);
    LuaDll.lua_rawseti(l, -2, 1);

    if (set == null)
        LuaDLL.lua_pushnil(l);
    else
        pushValue(l, set);
    LuaDLL.lua_rawseti(l, -2, 2); // t[n]=v,n是2，v是-2出的值，lua是栈存储结构，栈索引值越小，说明越早压栈

    LuaDll.lua_setfield(l, t, name); // t[name]=v
}
```
其他注册方式类似。
## 模块
这里的模块是自定义的，而不是根据配置读取的，其实也无可厚非，只不过会增加了一些代码量，这里的代码量仅仅指模块名称。比如UILoadHelper.SpawdPrefabFromPool，在这里UILoadHelper被我们习惯称为模块。
# UI
NGUI + slua
UI脚本目录 Assets/StreamingAssets/Lua
## Common
### Timer.lua

## Helper
### UIHelper.lua
```lua
function UIHelper.AddTimer(script, nTime, func)
    local nid = Timer:Add(nTime, func)
    script._tbUnscaledTime = script._tbUnscaledTimer or {}
    script._tbUnscaledTime[nId] = func
    return nId
end
```
### UILoadHelper.lua
```lua
```