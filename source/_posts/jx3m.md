---
title: 剑网3口袋版笔记
mathjax: true
date: 2019-05-23 10:40:02
tags:
    - JX3M
categories: JX3M
---
Unity版本2018.3
2D场景，3D模型，确实有点惊喜，场景里面的东西看到的基本都是用片做的。那么定位就应该偏向格斗了，可能是帧同步吧。

以下基本是静态分析结果。
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

lua使用的slua。首先我不是引战，我只是想大家有自己清晰的认识slua到底好不好。好了，这里是ulua的官网，大家可以去看一下。http://www.ulua.org/cstolua.html
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
然后slua对于用户感觉是透明的，然后每个注册之后都要返回两个参数，大部分来说。生成的cs文件完全是调用另一个文件的函数。
这种加一层的包装真好的好吗，难道用户不需要知道里面的东西吗？
（PS：个人感受，slua这种warp的方式真的好难受），好吧，自动化的注册确实跟不要钱一样。
## 模块
这里的模块是自定义的，而不是根据配置读取的，其实也无可厚非，只不过会增加了一些代码量，这里的代码量仅仅指模块名称。比如UILoadHelper.SpawdPrefabFromPool，在这里UILoadHelper被我们习惯称为模块。
# AssetBundle
好吧，这块真的是异常痛苦的，以至于我F12了好半天，才看到一个UnityEngine.AssetBundle.LoadFromFile(path);在ResourcesHook.cs中有两个接口
```CSharp
public static UnityEngine.AssetBundle LoadFromFile(string path)
{
    ResourceLoadProfiler.Begin();
    UnityEngine.AssetBundle ret = UnityEngine.AssetBundle.LoadFromFile(path);
    ResourceLoadProfiler.End("AssetBundle.LoadFromFile", path, null);

    return ret;
}
public static AssetBundleCreateRequest LoadFromFileAsync(string path)
{
    AssetBundleCreateRequest ret = UnityEngine.AssetBundle.LoadFromFileAsync(path);
    ResourceLoadProfiler.AddAsyncRequest(ret, "AssetBundle.LoadFromAsync", path, null);
    return ret;
}
```
# UI
开发模式属于MV模式。
## 目录结构
NGUI + slua
UI脚本目录 Assets/StreamingAssets/Lua
数据控制逻辑在LogicData目录下
### Common
#### Timer.lua

### Helper
#### UIHelper.lua
##### AddTimer
```lua
function UIHelper.AddTimer(script, nTime, func)
    local nid = Timer:Add(nTime, func)
    script._tbUnscaledTime = script._tbUnscaledTimer or {}
    script._tbUnscaledTime[nId] = func
    return nId
end
```
```lua
-- 按钮注册事件
function UIHelper.BindUIEvent(go, szEvent, func, tbParam, nIntervals)

    if not go or not szEvent or not func then return end
    local listenter = UIEventLister.Get(go.gameObject)
    if tbParam ~= nil and type(tbParam) == "table" then
        listener.paramter = tbParam
    end
    if nInterval ~= nil then
        UIClickIntervalsController.Instance:RegisterInterval(go.gameObject, nIntervals)
    end
    if szEvent == UI_EVENT.onClick then
        listener.onClick = func
    elseif then

    elseif then
    elseif then
    else
        LOG.ERROR("未知的按钮事件类型{0}", szEvent)
    end
end
```
#### UILoadHelper.lua
##### LoadPrefabHandleAsync
```lua

-- 异步加载预制体接口
function UILoadHelper.LoadPrefabHandleAsync(script, nID, parent, szRename, nPostNameID, ...)
    if not script or not nID or not parent then
        LOG.ERROR("UILoadHelper.LoadPrefabHandleAsync param exception")
        return
    end

    nPostNameID = nPostNameID or -1

    local handle = Lua2CS.LoadPrefabHandleAsync(nID, parent, transform, szRename, nPostNameID, ...)
    if not handle then
        LOG.ERROR("UILoadHelper.LoadPrefabHandleAsync Don't Get handle")
        return
    end

    if not script.tbPrefabHandle then
        script.tbPrefabHandle = {}
    end

    script.tbPrefabHandle[handle.nHandleId] = handle

    return handle
end
```
##### LoadImg(img, nID, bMakePerfect)--- 加载图片(UISprite, UITexture)
```lua
function UILoadHelper.LoadImg(img, nID, bMakePerfect)
    if not img or not nID then LOG.ERROR("UILoadHelper.LoadImg 传入空参数") return end
    bMakePerfect = bMakePerfect or false
    Lua2CS.LoadImg(img, nID, UIImgUsageType.MainTexture, true, bMakePerfect)
end
```

#### UINavigateHelper.lua
```lua
--- 打开新界面（异步）
function UINavigateHelper.NavigateViewAsync(nViewID,...)
    if UIViewOpenSequenceManager.AddView(nViewID, false, true, ...) then
        return
    end
    local shell = UINavigationManager.Instance:NavigateView(nViewID, false, true, ...)

    -- 注册异步消息管理
    if shell and not shell.IsViewLoad then
        local config = UIAsyncMsgTab[nViewID]
        if config then
            local msgAsyncHolder = UIAsyncMsgHelper.RegisterMsgAysnc(config)
            shell.AsyncMsg = msgAsyncHolder
        end
    end
    return shell
end
```
## 表格文件
表格路径：Assets/JX3Game/Source/File/UI
转换成lua文件方法：
```CSharp
[MenuItem("Tools/Excel to Lua/Export")]
public static void ExcelToLua()
{
    Lua.ExcelToLua();
}
public static void ExcelToLua()
{
    try
    {
        System.Diagnostics.Process pro = new System.Diagnostics.Process();
        pro.StartInfo.WorkingDirectory = Application.dataPath + "/JX3Game/Source/File/ToLuaTool/";
        pro.StartInfo.FileName = Application.dataPath + "/JX3Game/Source/File/ToLuaTool/run.bat";
        pro.StartInfo.UseShellExecute = true;
        pro.Start();
        pro.WaitForExit();
    }
    catch(System.Exception)
    {

    }
}
```
注意：如果你看源码的时候发现生成的文件工程中的文件不一致，比如我看UICardTab.lua的时候，就发现缺少第一句注释，然后我尝试生成了以下，注释就加上了。
原理：其实是把表格文件转成lua文件了，寻仙的时候在在线转的，这里是离线转的。
对比：比如 UIAchieveLabelTab.xls（其实是一个txt文件，这块让人刚开始有点雾水）大小是12KB，而生成的UIAchieveLabelTab.lua却占105KB。缺点是会让包体增大，那么带来一系列的坏处就是加载这块的问题。而如果在线转的问题是在转的这块要做好优化。个人更喜欢在线转。事实上常见的也是在线转。
## UIPrefab绑定Lua
{%asset_img 2.jpg 反人类设计的选择器%}
这个东西很坑的，一般来说我们的选择器都是把所有节点罗列出来，然后供你选择，而这里的是要你输入文件名然后进行匹配。。。是节省性能吗？
交互做好的UI，然后我们这边绑上LuaBind脚本，好吧，要注意，序列化这件事情，即没有用的变量就不要绑定了。Unity序列化给人感觉还是不太友好，尤其是Prefab的序列化。
绑定完之后点击生成头部检测函数，如下图：

{%asset_img 1.jpg%}
点击生成头部检测可以将绑定的变量输出到控制台上，然后我们将输入的日志自己拷贝粘贴到lua文件中。
好吧，这里我可以说好坑的吗，我们做了那么多自动化的工具，为什么这里却是要自己把日志手动粘贴到Lua文件中的呢？
然后还有刷新按钮，当我们点击刷新的时候，会重新检测文件中绑定数据，这个时候才可以进行删除操作。好吧，这样的目的是为了防止安全，但是这些反人类设计真的要吐槽。
继续吐槽，绑定变量数组不能在数组元素中插入吗？

Lua脚本是如何生成的呢？
>我们注意到在Editor目录下有一个LuaTemplate.lua模板文件。
该文件通过LuaFileCreator.cs中的
```CSharp
[MenuItem("Assets/Create/Lua Script", false, 80)]
public static void CreateNewLua()
{
    ProjectWindowUtil.StartNameEditingIfProjectWindowExists(0, ScriptableObject.CreateInstance<CreateScriptAsset>(), GetSelectedPathOrFallback() + "/New Lua.lua", null, "Assets/Editor/LuaTemplate.lua");
}
```
这里开发的同学应该要注意，生成的模板文件如果我们不需要的时候，需要把它干掉！类似的做法在C#的MonoBehaviour脚本也适用。
## 字体
字体使用的动态字体。表现效果好，但是CPU，内存占用大。不过这个也在情理之中，而且也是很常见的做法。
## 图集
图集可能不是使用Texturepacker，但是都是2的幂次方，应该是要根据不同设备进行压缩的。在编辑器下观察，应该都是带alpha通道的，那么这块其实是可以优化的。比如将rgb和r通道分离。之前潘多拉就是这样的做法，这样其实和etc2的差别不是很大，但是和RGB32的差别还是肉眼可见的。
{%asset_img LockOpen.png%}
好吧，浪费还是蛮严重的。
## 艺术字
看着像是按照静态字体的方式制作的。
## 相机
相机有一个UIMainCamera和一个FrontCamera，这里有个坑的地方就是在编辑器和模拟器上当前视图是使用的FrontCamera渲染，后置视图使用UIMainCamera并添加高斯模糊。
所以要注意在任何时候的相机操作。
# 工具
## 调试器
这里有很多调试器，都值得一试，它可能会帮助你修复难以解决的问题。
在Tools下面有RemoteDebugger.HierarchyPanel，打开面板，点击Query，然后所有的对象可以陈列在面板上，而且你可以对他进行修改，除了name和transform。
name是禁止修改的，但是transform怀疑是bug，导致无法修改。