---
title: Unity-Lua-Manual
date: 2018-09-18 12:10:28
tags:
---
## GameObject

### GO_name

	gameObject.GO_name = "name"

设置或者获取游戏对象的名字

### GO_self

    gameObject.GO_self

绑定到自己身上的Lua模块指针

### GO_active

    gameObject.GO_active = true/false

设置游戏对象的显式或者隐藏

### GO_parent

    gameObject.GO_parent = parent

设置游戏对象的父节点


### GO_childCount

    gameObject.GO_childCount

获取游戏对象的子节点对象，不递归

### GO_AddComponent

    gameObject:GO_AddComponent(“UILabel")

给游戏对象添加组件

### GO_GetComponent

    gameObject:GO_GetComponent("UILabel")
    gameObject:GO_GetComponent("UILabel", "Sprite/Label")

方法一：获取游戏对象的组件
方法二：获取子节点上的组件，参数二表示相对于父节点的路径

### GO_Find

    gameObject:GO_Find("Sprite/Label")

获取子节点对象

### GO_AddChild

    gameObject:GO_AddChild("UI/ui_node")
    gameObject:GO_AddChild(ui_node)

方法一：通过相对于Resources下的路径来给游戏对象添加子节点
方法二：通过实例对象来给游戏对象添加子节点
如果是UI节点，请使用***UI_AddChild***

### UI_AddChild

    gameObject:UI_AddChild("UI/ui_node")
    gameObject:UI_AddChild(ui_node)    

方法一：通过相对于Resources下的路径来给游戏对象添加子节点
方法二：通过实例对象来给游戏对象添加子节点

### GO_DelChild

    gameObject:GO_DelChild(idx)

删除游戏对象的子节点，idx是下标从0开始的索引（顺序是检视面板显示的顺序）


### GO_DestroySelf

    gameObject:GO_DestroySelf()

销毁自己

### GO_RemoveAllChildren

    gameObject:GO_RemoveAllChildren()

销毁所有的子节点

### GO_ShowAllChildren

    gameObject:GO_ShowAllChildren(ture/false)

显示或者隐藏自己所有的子节点

### GO_GetLocalPosition

    local x,y,z = gameObject:GO_GetLocalPosition()

获取自己的localPosition

### GO_SetLocalPosition

    gameObject:GO_SetLocalPosition(x,y [,z])

设置自己的localPosition,\[\] 表示可选参数,当未指定时表示使用之前的z

### GO_GetLocalRotation

    local x,y,z = gameObject:GO_GetLocalRotation()

获取自己的localRotation

### GO_SetLocalRotation

    gameObject:GO_SetLocalRotation(x,y [,z])

设置自己的localRotation,\[\]表示可选参数,当未指定时表示使用之前的z

### GO_SetLocalScale

    gameObject:GO_SetLocalScale(x,y,z)

设置自己的localScale

### GO_UpdateAllAnchors

    gameObject:GO_UpdateAllAnchors()

手动刷新自身的锚点

### UI_active

    gameObject.UI_active = true/false

递归刷新自己和子节点的显示和隐藏，如无特殊要求请使用***GO_active***

### UI_onClick

    gameObject.UI_onClick = function()
    end

添加点击事件，游戏对象需要添加boxcollider2D

### UIToggle_onChange

    gameObject.UIToggle_onChange = function()
    end

添加Toggle状态改变的回调函数，游戏对象需要添加UIToggle

### UILabel_text

    gameObject.UILabel_text = "Hello, World!"

设置或者读取游戏对象的UILabel文本，游戏对象需要添加UILabel

### UILabel_color

    gameObject.UILabel_color = Color.FromHex("C4C4C4FF")

设置或者读取文本颜色，返回值是Color

### UISprite_spriteName

    gameObject.UISprite_spriteName = "btn_blue"

设置或者读取精灵的名字，游戏对象需要添加UISprite组件

### UISprite_color

    gameObject.UISprite_color = Color.FormHex("6f4f3fff")

设置或者读取精灵的颜色，游戏对象需要添加UISprite组件

### UIToggle_value

    gameObject.UIToggle_value = true/false

设置UIToggle的值

### UIWidget_color

    gameObject.UIWidget_color = Color.FormHex("6f4f3fff")

设置UIWidget或者继承他的子类（UILabel,UISprite)的颜色

### UIWidget_width

    gameObject.UIWidget_width = 100

设置UIWidget或者继承他的子类（UILabel,UISprite)的宽度

### UIWidget_height

    gameObject.UIWidget_height = 100

设置UIWidget或者继承他的子类的高度

### UIGrid_Reposition

    gameObject:UIGrid_Reposition()

使Grid重新排序，一般给Grid添加了子节点后使用

### Tween_Play

    gameObject:Tween_Play()

播放Tween动画

## MB

### MB.Log

    MB.Log("Hello,World!")

打印日志

### MB.PCall

    MB.PCall(function()
    end)

安全的调用函数，当函数内部发生错误的时候，不会中断当前执行环境

### MB.LoadModule

    MB.LoadModule("ui_task")

加载一个lua模块

### MB.ShowTheTip

    MB.ShowTheTip("Hello,World!", 2)

弹出Tips，第二个参数表示时长单位是秒



### String.CSFormat

    String.CSFormat("id={0},name={1}",10,"test")

调用C#的格式化函数

### MB.GetStringX

    MB.GetStringX(bytes[], 10, "test")

调用C#的格式化函数，第一个参数是byte数组，一般来说表格中的字段用它

### MB.GetExcelList

    MB.GetExcelList("NewTaskResConf")

读取表格

### MB.GO_GetResource

    self.u_testSprite.atlas = MB.GO_GetResource("UI/01Common")

获取资源 参数表示相对于Resources的路径，返回的是GameObject

## UITexture

### SetTexturePath

    texture:SetTexturePath("Texture/UI/Chapter1")

设置texture的资源路径，路径相对于Resources，texture是通过gameObject获取的组件

### SetMat

    texture:SetMat(path)

设置texture的材质，path表示相对Resources路径

## UILable

### text

    label.text = "Hello, World！"

设置或者读取Label的文本

### color

    label.color = Color.FromHex("6F4F3FFF")

设置或者读取Label的颜色


### effectColor

    label.effectColor = Color.FromHex("6F4F3FFF")

设置或者读取Label的效果颜色

### fontSize

    label.fontSize = 20

设置或者读取字体的大小

## UISprite

### spriteName

    sprite.spriteName = "btn_red"

设置或者读取Sprite的名字（图集中的）

### width

    sprite.width = 100

设置或者读取Sprite的宽度

### height

    sprite.height = 100

设置或者读取Sprite的高度

### atlas

    sprite.atlas = MB.GO_GetResource("UI/01Common")

设置或者读取图集

## LuaEvent

C#中

    LuaEvent.CallEvent((int)eLuaEvnet.eLuaEvent_PlayerTaskUpdate, param1, param2, param3);

lua中

    lua_event.InvokeEvent(lua_def.eLuaEvent_PlayerTaskUpdate, param1, param2, param3)

    lua_event.InsertEvent(lua_def.eLuaEvent_PlayerTaskUpdate, callback, name, 0)
    function callback(param1, param2, param3)

    end

CallEvent表示C#事件通知Lua
InvokeEvent表示Lua事件通知Lua
LuaEvent.CallEvent中的第一个参数表示类型在obj_def.cs中定义，后面是需要传递给lua的参数

## lua_net

### SendBin

    lua_net.SendBin(lua_def.CS_CMD_GET_TASK_LIST_REQ, {Page = 1, Type = 4}, callback)
    function callback(msg)
    end
发送二进制包，第一个参数是消息ID，第二个参数是相对于数据结构类型，callback是回调函数，可选类型
回调函数中的msg表示一个json的xml中结构。

### Send

    lua_net.Send(lua_def.eCTS_UI_Task, {op = "get"})

发送JSON包，第一个参数是cts_def.cs中定义的消息，第二个参数是lua table

### Register

    lua_net.Register(lua_def.eSTC_UI_Task, OnUITaskHandle, signal, -1)
    function OnUITaskHandler(msg)
    end
    lua_net.Register(lua_def.CS_CMD_GET_TASK_LIST_RES, OnGetTaskListRes, signal, -1)
    function OnGetTaskListRes(msg) -- msg是xml生成的数据结构，传到lua里面是一个json table
    end

注册一个消息回调，signal表示一个回调的唯一句柄，-1表示调用该回调函数的顺序，默认是0


## lua_timer

    lua_timer.Timer(self.gameObject, span, onTick, count, name)

-- 创建Timer对象
-- go : 指定挂到某个gameObject上
-- span : 时间间隔，毫秒
-- onTick : 回调函数(timer, dt)，回调的dt参数是超时的时间，一般也用不上
-- count : 重复次数，缺省是1，如果是-1则无限循环。
-- *return* : 返回一个lua table作为Timer

