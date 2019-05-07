---
title: Lua中元方法_newindex详解
date: 2017-10-23 09:46:45
tags:
	lua
---
\__index元方法用于处理调用table中不存在的字段和查询
\__newindex用于更新

```lua
local smartMan = {
	name = "lyf",
	money = 900,

	sayHello = function()
		print("hello,I’m lyf");
	end
}

local t1 = {}

local mt = {
	__index = smartMan,
}

setmetatable(t1, mt);

t1.sayHello = function()
	print("en");
end
t1.sayHello()	
```
这是一个模仿继承结构的例子
mt作为t1的元表,设置__index为smartMan
于是,当我们调用t1中不存在的字段时,就会自动去smartMan中查找
比如我们调用了t1.sayHello(),自然就能找到对应的函数

```lua

    local smartMan = {
        name = "none",
        money = 9000000,

        sayHello = function()
            print("Hello world!");
        end
    }
    local t1 = {};
    local mt = {
        __index = smartMan,

        __newindex = function(table, key, value)

            print(key .. "字段是不存在的，不要试图给它赋值！");

        end

    }

    setmetatable(t1, mt);

    t1.sayHello = function()
        print("en");

    end

    t1.sayHello()
```
留意mt元表,我们给他加了一个__newindex
导致sayHello字段赋值失败,因为给sayHello字段赋值的时候,调用了__index元方法,代替了赋值操作
和__index一样,__newindex元方法也可以赋值一个table
__newindex规则
1. 如果__newindex是一个函数,则在给table不存在的字段赋值时,会调用这个函数
2. 如果__newindex是一个table,则在给table不存在的字段赋值时,会直接给__newindex的table赋值

# Lua类和继承实现
Lua本身不能像C++那样直接实现继承，但是可以用table来实现
```Lua
Object = {class_id = 0}
funtion Object:New(o)
    o = o or {}
    setmetatable(o, self) -- 对象o调用不存在的成员时会去self中查找，而这里的self指的就是Object
    self.__index = self
    return o
end
```
