---
title: index newindex rawset rawget setmetatable
date: 2017-10-23 09:46:45
tags:
	Lua
---

1. `__index`可以当作get方法，获取一个value值，不在当前表的时候就会去元表查询，元表首先调用这个函数，如果这个函数没有重写，那么直接从元表中查找key对应的value，如果函数被重写了，任何从元表的get方法都只走这里，而不从元表查找，规则如下：
   - 如果`__index`是一个函数，则读取table不存在的字段时，会调用这个函数。
   - 如果`__index`是一个table，则读取table不存在的字段时，会直接读取`__index`的table值。
2. `__newindex`可以当作set方法，设置一个`key,value`值，如果当前表没有这个值，那么会调元表中的`__newindex`方法，规则如下：
   - 如果`__newindex`是一个函数，则在给table不存在的字段赋值时，会调用这个函数。
   - 如果`__newindex`是一个table，则在给table不存在的字段赋值时，会直接给__newindex的table赋值。
3. `rawset`使用这个函数，直接设置当前表的`key,value`值，不管是否有`__newindex`方法。
4. `rawget`使用这个函数，仅能从当前表获取值，即使当前表没有值，元表有值，也不会返回元表的值，而是返回`nil`。
5. `setmetatable`使用这个函数，可以给当前表设置一个元表。

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
这是一个模仿继承结构的例子mt作为t1的元表,设置`__index`为smartMan，于是，当我们调用t1中不存在的字段时，就会自动去smartMan中查找，比如我们调用了`t1.sayHello()`，自然就能找到对应的函数。
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
留意mt元表,我们给他加了一个`__newindex`，导致sayHello字段赋值失败，因为给sayHello字段赋值的时候，调用了`__index`元方法，代替了赋值操作，和`__index`一样，`__newindex`元方法也可以赋值一个table。


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