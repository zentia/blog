---
title: Protocol Buffer
date: 2018-07-21 14:18:00
tags:
---

高效的二进制方式存储
Varint是一种紧凑的表示数字的方法。它用一个或多个字节来表示一个数字，值越少的数字使用越少的字节数。
采用这种Key-Pair结构无需使用分隔符来分割不同的Field。

|Type|Meaning|Used For|
|:---|:------|:-------|
|0|Varint|int32,int64,uint32,uint64,sint32,sint64,bool,enum|
|1|64-bit|fixed64|sfixed64,double|
|2|Length-delimi|string,bytes,embedded messages,packed repeated fields|
|3|Start group|Groups(deprecated)|
|4|End group|Groups(deprecated)|
|5|32-bit|fixed32,sfixed32,float|

在计算机内，一个负数一般会被表示一个很大的整数，因为计算机定义负数的符号位为数字的最高位。如果采用Varint表示一个负数，那么一定需要5个byte。为此Google Protocol Buffer定义了sint32这种类型，采用zigzag编码。

Zigzag编码用无符号来表示有符号数字，正数和负数交错，这就是zigzag这个词的含义了。


<img src="http://oxcvfpext.bkt.clouddn.com/Protocol-1.jpg">

使用zigzag编码，绝对值小的数字，无论正负都可以采用较少的byte来表示，充分利用了Varint这种技术。

其他的数据类型，比如字符串则采用类似数据库中的varchar的表示方法，即用一个varint表示长度，然后其余部分紧跟在这个长度部分之后即可。

### 封解包的速度

首先来了解一下XML的封解包过程。XML需要从文件中读取字符串，再转换为XML文档对象结构模型。之后，再从XML文档对象结构模型中读取指定节点的字符串，最后再将这个字符串转换成指定类型的变量。这个过程非常复杂，其中将XML文件转换为文档对象结构模型的过程通常需要完成词法文法分析等大量消耗CPU的复杂计算。

反观Protobuf，它只需要简单地将一个二进制序列，按照指的格式读取到C++对应的结构类型中就可以了。从上一节的描述可以看到消息的decoding过程也可以通过几个位移操作组成的表达式计算即可完成。速度非常快。

### T-L-V的数据存储方式

Tag-Lenght-Value,标识-长度-字段值存储方式

标识-长度-字段值表示单个数据，最终将所有数据拼接成一个字节流，从而实现数据存储的功能

- 其中Length可选存储，如存储Varint编码数据就不需要存储Length

