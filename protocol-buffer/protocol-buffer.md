# 定义一个消息类型

```c
message SearchRequest
{
    required string query = 1;
    optional int32 page_number = 2;
    optional int32 result_per_page = 3;
}
```
该消息定义了三个字段，两个int32类型和一个string类型的字段，每个字段由字段限制，字段类型，字段名和Tag四部分组成，对于C++，每一个.proto文件经过编译之后都会对应的生成一个.h和一个.cc文件

# 字段限制

字段限制共有3类
**required**:必须赋值的字段
**optional**:可有可无的字段
**repeated**:可重复字段（变长字段），类似于数组
由于一些历史原因，repeated字段并没有想象中那么高效，新版本中允许使用特殊的选项来获得更高效的编码：

    repeated int32 samples = 4 [packet=true];

# Tags
消息中的每一个字段都由一个独一无二的数值类型的Tag.1到15使用一个字节编码，16到2047使用2个字节编码，所以应该将Tags 1到15留个频繁使用的字段。
可以指定的最小的Tag为1，最大位$2^29$-1或536,870,911，但不能使用19000到19999之间的值，这些值是预留给protocol buffer的。

# Google Protocol Buffer的Encoding

Protobuf序列化所生成的二进制消息非常紧凑，这得益于Protobuf采用的非常巧妙的Encoding方法。
考擦消息结构之前，让我首先要介绍一个叫Varint的术语。
Varint是一种紧凑的表示数字的方法。它用一个或这个字节来表示一个数字，值越小的数字使用越小的字节数。这能减少用来表示数字的字节数。
比如对于int32类型的数字，一般需要4个byte来表示。但是采用Varint，对于很小的int32类型的数字，则可以用1个byte来表示。大的数字则需要5个byte表示。从统计的角度来说，一般不会所有的消息中的数字都是大数，因此大多数情况下，采用Varint后，可以用更少的字节数来吧iaoshi数字信息。下面就详细介绍一下Varint。
Varint中的每个byte的最高位bit有特殊的含义，如果该位为1，表示后续的byte也是该数字的一部分，如果该位为0，则结束。其他的7个bit都用来表示数字。因此小于128的数字都可以用一个byte表示。大于128的数字，比如300，会用两个字节来表示：1010 1100 0000 0010
下图演示了Google Protocol Buffer如何解析两个bytes。注意到最终计算前将两个byte的位置相互交换过一次，这是因为Google Protocol Buffer字节序采用little-endian的方式。

{% asset_img 1.jpg %}
消息经过序列化会成为一个二进制数据流，该流中的数据为一系列的Key-Value对。如下图所示：
{% asset_img 7.jpg %}
采用这种Key-Pair结构无需使用分隔符来分割不通的Field。对于可选的Field，如果消息中不存在该Field，那么在最终的Message Buffer中就没有该field，这些特性都有助于节约消息本身的大小。
假如我们生成如下的一个消息Test1：

    Test1.id = 10;
    Test1.str = "hello";

则最终的Message Buffer中有两个Key-Value对，一个对应消息中的id；另一个对应str。
Key用来标识具体的field，在解包的时候，Protocol Buffer根据Key就可以知道相应的Value应该对应于消息中那一个Field。
Key的定义如下：

    (field_number<<3)|wire_type

可以看到Key由两部分组成。第一部分是field_number，比如消息Im.helloworld中的field id的field_number为1.第二部分为wire_type。表示Value的传输类型。

Wire Type可能的类型如下表所示：

Type|Meaning|Used For
---|-----|------
0|Varint|int32,int64,uint32,uint64,sint32,sint64,bool,enum
1|64-bit|fixed64,sfixed64,double

在我们的例子中，field id所采用的数据类型为int32，因此对应的wire type为0。可以看到Type 0所能表示的数据类型中有int32和sint32这两个非常类似的数据类型。Google Protocol Buffer区别它们的主要意图也是为了减少encoding后的字节数。
在计算内，一个负数一般会被表示为一个很大的整数，因为计算机定义负数的符号位为数字的最高位。如果采用Varint表示一个负数，那么一定需要5个byte。为此Google Protocol Buffer定义了sint32这种类型，采用zigzag编码。
Zigzag编码用无符号数来表示有符号数字，正数和负数交错，这就是zigzag这个词的含义了。
如图所示：
{%asset_img 8.jpg %}

使用zigzag编码，绝对值小的数字，无论正负都可以采用较少的byte来表示，充分利用了Varint这种技术。
其他的数据类型，比如字符串则采用类似数据库中的varchar的表示方法，即用一个varint表示长度，然后将其余部分紧跟在这个长度部分之后即可。
通过以上对protobuf Encoding方法的介绍，想必已经发现protobuf消息的内容效，适合网络传输。
对于消息Test1，用Protobuf序列化后的字节序为：
08 65 12 06 48 65 6C 6C 6F 77
而如果用XML，则类似这样
31 30 31 3C 2F 69 64 3E 3C 6E 61 6D 65 3E 68 65 
6C 6C 6F 3C 2F 6E 61 6D 65 3E 3C 2F 68 65 6C 6C 
6F 77 6F 72 6C 64 3E 
一共55个字节，这些奇怪的数字需要稍微解释一下，其含义用ASCII表示如下：
<helloworld>
    <id>101</id>
    <name>hello</name>
</helloworld>
# 封解包的速度

首先我们来了解一下XML的封解包过程。XML需要从文件中读取出字符串，在转换为XML文档对象结构模型。之后，在从XML文档对象结构模型中读取指定节点的字符串，最后再将这个字符串转换成指定类型的变量。这个过程非常复杂，其中将XML文件转换为文档对象结构模型的过程通常需要完成词法文法分等大量消耗CPU的复杂计算。
反观Protobuf，它只需要简单地将一个二进制序列，按照指定的格式读取到C++对应的结构类型中就可以了。
{% asset_img 9.jpg %}