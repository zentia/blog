## 字节序

- Little-Endian         低位字节放在内存的低地址端，高位字节放在内存的高地址端。
- Big-Endian            高位字节放在内存的高地址端，低位字节放在内存的低地址端。
- Network byte order    TPC/IP各层协议将字节序定义为Big-Endian，因此TCP/IP协议中使用的字节序通常称之为网络字节序。

### 高/低地址端

首先要知道C程序中内存的空间布局情况：在《C专家编程》和《Unix环境高级编程》中关于内存空间布局情况的说明，大致如下图：

------------------------------ 最高内存地址 0xffffffff
栈底
栈
栈顶
NULL（空洞）
堆
未初始化的数据
正文段（代码段
------------------------------ 最高内存地址 0x00000000

在内存分布中，栈是向下增长的，而堆是向上增长的。

加入在栈分配unisgned char buf[4]，他在栈上分布如下
栈底（高地址）
buf[3]
buf[2]
buf[1]
buf[0]
栈顶（低地址）

### 高/低位字节

在十进制中靠左边的是高位，靠右边的地址，在其它进制也是如此

unisgned int value = 0x12345678;

Big-Endian:低地址存放高位
栈底（高地址）
buf[3] (0x78)
buf[2] (0x56)
buf[1] (0x34)
buf[0] (0x12)
栈顶（低地址）

Litter-Endian:高地址存高位
栈顶（高地址）
buf[0] (0x78)
buf[1] (0x56)
buf[2] (0x34)
buf[3] (0x12)
栈底（低地址）


<td bgcolor=orange> 主机序（Host Order）就是遵循Little-Endian规则</td>。所以当两台主机之间通过TCP/IP协议进行通信的时候就需要调用相应的函数进行主机序（Little-Endian）和网络（Big-Endian）的转换。
## Big-Endian和Little-Endian优缺点

Big-Endian 优点：
1. 靠首先提取高位字节，总是可以在偏移位置为0的字节来确定这个数字是正数还是负数。
2. 数值打印是按顺序存放的

Litter-Endian 优点：

1. 数学计算，精度处理。