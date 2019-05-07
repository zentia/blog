title: C/C++可变参数，"##__VA_ARGS__"宏参数的介绍和使用
tags: []
categories: []
date: 2017-12-24 10:56:00
---
在一个宏定义语句中，发现了一个之前未见过的编译器预定义宏："__VA_ARGS__"。
在GNU C中，宏可以接受可变数目的参数，就像参数一样，例如：
```C
#define pr_debug(fmt,arg...)\
printk(KERN_DEBUG fmt, ##arg)
```
你可能很熟悉在函数中使用可变参数表，如：
	
	void printf(const char* format, ...);

直到最近，可变参数还是只能应用在真正的函数中，不能使用在宏中。
C99编译器标准终于改变了这种局面，它允许你可以定义可变参数宏(variadic macros)，这样你就可以使用拥有可以变化的参数表的宏。可变参数就像下面这个样子：

	#define debug(...) printf(__VA_ARGS__)

缺省号代表一个可以变化的参数表，使用保留名__VA_ARGS__把参数传递给宏。当宏的调用展开时，实际的参数就传递给printf()了，翌日：

	debug("Y = %d\n", y);

而处理器会把宏的调用替换成：

	printf("Y = %d\n", y);

因为debug()是一个可变参数宏