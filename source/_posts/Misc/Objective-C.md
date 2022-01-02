---
title: Objective-C
date: 2018-03-24 16:05:37
tags:
---

# Objective-C: C的超集

Objective-C是C语言的严格超集--任何C语言不经修改可以直接通过Objective-C编译器

# Objective-C代码文件的扩展名

.h  头文件
.m  源代码文件，包含Objective-C和C代码
.mm 源代码文件，包含Objective-C、C、C++代码，仅在你的Objective-C代码中确实需要使用C++类或者特性的时候采用这种扩展名

当你需要在源代码中包含头文件的时候，你可以使用标准的#include编译选项，但是Objective-C提供了更好的方法。#import选项和#include选项完全相同，只是它可以确保相同的文件只会被包含一次。Objective-C的例子和文档都倾向于使用#import。

# 语法

C++调用方法
	
	obj.method(argument);

Objective-C调用方法

	[obj method: argument];

类声明总是由@interface编译选项开始，由@end选项结束。
<img src="http://oxcvfpext.bkt.clouddn.com/Objective-C-1.png">

```Objective-C
@interface MyObject: NSObject{
	int memberVar1; // 实体变量
	id 
}
+(return_type) class_method; //类方法

-(return_type) instance_method1; //实例方法
-(void)fbSendInviteByID: (NSDictionary*)info
{

}
@end
```
方法前面+/-号代表函数的类型：加号(+)代表类方法(class method),不需要实例就可以调用，与C++的静态函数(static member function)相似。减号(-)即是一般的实例方法(instance method)
```Objective-C
-(void)setColorToRed: (float)red Green: ()
```

#pragma mark