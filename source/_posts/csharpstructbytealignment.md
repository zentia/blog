---
title: C#结构体字节对齐方式
mathjax: true
date: 2019-05-10 19:57:56
tags:
    - CSharp
categories: CSharp
---
```CSharp
[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Ansi)]
internal struct DllInvoices
{
    [MarshalAs(UnmanagedType.ByValArray, SizeConst=8)]
    public byte[] serial;
    public byte Type;
    public uint Sum;
}
```
调用Marshal.SizeOf(typeof(DllInvoices))得到的大小为16，为什么不是13？
这其实是体系结构和汇编的问题。
在内存里特定类型数据的起始地址通常有一定的对齐要求，比如说32位机器上的int起始地址必须是4的整数倍，结构通常也是如此。
如果一个结构包含一个char和一个int，那么char作为结构体的第一个字段，需要在4的整数倍地址开始，而int有同样的要求，所以char之后必须空出3个字节来，才能使int的地址满足对齐的要求。
.NET的CLR是基于32位的，所以也是4字节对齐的。在byte之后，必须空出3个字节。所以结果就是16字节了。
可以通过StructLayout的Pack属性改变这种对齐设置。

    [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Ansi, Pack=1)]

MarshalAs属性指示如何在托管代码和非托管代码之间封送数据。

当MarshalAsAttribute.Value设置为ByValArray时，必须设置SizeConst以指示数组中的元素数。当需要区分字符串类型时，ArraySubType字段可以选择包含数组元素的UnmanagedType。此UnmanagedType只可用于作为结构中的字段的数组。
而SizeConst则是指数组的元素个数。
