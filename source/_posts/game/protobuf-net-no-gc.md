---
title: Unity3D游戏GC优化总结---protobuf-net无GC版本优化实践
date: 2019-04-24 10:44:29
tags:
    - Unity
---
# protobuf-net优化效果图
protobuf-net是Unity3D游戏开发中被广泛使用的Google Protocol Buffer库的c#版本，之所以c#版本被广泛使用，是因为c++版本的源代码不支持Unity3D游戏在各个平台上的动态库构建。它是一个网络传输层协议，对应的lua版本有两个可用的库：一个是proto-gen-lua，由tolua作者开发，另外一个是protoc，由云风开发。protobuf-net在GC上有很大的问题，在一个高频率网络通讯的状态同步游戏中使用发现GC过高，所以对它进行了一次比较彻底的GC优化。下面是优化前后的对比图：
{% asset_img 1.jpg protobuf-net优化前GC和性能效果图 %}
{% asset_img 2.jpg protobuf-net优化后GC和性能效果图 %}

# Unity3D游戏GC优化概述

有关Unity3D垃圾回收的基本概念和优化策略Unity官网有发布过文章：<a href="https://unity3d.com/de/learn/tutorials/topics/performance-optimization/optimizing-garbage-collection-unity-games?playlist=44069">Optimizing garbage collection in Unity games</a>>。这篇文章讲述了Unity3D垃圾回收机制，和一些简单的优化策略，讨论的不是特别深入，但是广度基本上算是够了。

1. C#变量分为两种类型：值类型和引用类型，值类型分配在栈区，引用类型分配在堆区，GC关注引用类型
2. GC卡顿原因：堆内存垃圾回收，向系统申请新的堆内存
3. GC触发条件：堆内存分配而当内存不足时、按频率自动触发、手动强行触发（一般用在场景切换）
4. GC负面效果：内存碎片（导致内存变大，GC触发更加频繁）、游戏顿卡
5. GC优化方向：减少GC次数、降低单次GC运行时间、场景切换时主动GC
6. GC优化策略：减少对内存分配次数和引用次数、降低堆内存分配和回收频率
7. 善用缓存：对有堆内存分配的函数，缓存其调用结果，不要反复去调用
8. 清除列表：而不要每次都去new一个新的列表
9. 用对象池：必用
10. 慎用串拼接：缓存、Text组件拆分、使用StringBuild、Debug.Log接口封装（打Conditional标签）
11. 警惕Unity函数调用：GameObject.name、GameObject.tag、FindObjectsOfType<T>()等众多函数都有堆内存分配，实测为准
12. 避免装箱：慎用object形参、多用泛型版本（如List<T>）等，这里的细节问题很多，实测为准
13. 警惕协程：StartCoroutine有GC、yield return带返回值有GC、yield return new xxx有GC（最好自己做一套协程管理）
14. foreach：unity5.5之前版本有GC，使用for循环或者获取迭代器
15. 减少引用：建立管理类统一管理，使用ID作为访问token
16. 慎用LINQ：这东西最好不用，GC很高
17. 结构体数组：如果结构体中含有引用类型变量，对结构体数组进行拆分，避免GC时遍历所有结构体成员
18. 在游戏空闲（如场景切换时）强制执行GC

# protobuf-net GC分析

## protobuf-net序列化

先分析下序列化GC，deep profile如下：
![](3.jpg)

打开PropertyDecorator.cs脚本，找到Write函数如下：

```CSharp
public override void Write(object value, ProtoWriter dest)
{
    Helpers.DebugAssert(value != null);
    value = property.GetValue(value, null);
    if(value != null) Tail.Write(value, dest);
}
```
可以看到这里MonoProperty.GetValue产生GC的原因是因为反射的使用；而ListDecorator.Write对应于代码Tail.Write，继续往下看：
{% asset_img 4.jpg %}
找到对应源代码：
```CSharp
public override void Write(object value, ProtoWriter dest)
{
    SubItemToken token;
    bool writePacked = WritePacked;
    if (writePacked)
    {
        ProtoWriter.WriteFieldHeader(fieldNumber, WireType.String, dest);
        token = ProtoWriter.StartSubItem(value, dest);
        ProtoWriter.SetPackedField(fieldNumber, dest);
    }
    else
    {
        token = new SubItemToken(); // default
    }
    bool checkForNull = !SupportNull;
    foreach (object subItem in (IEnumerable)value)
    {
        if (checkForNull && subItem == null) { throw new NullReferenceException(); }
        Tail.Write(subItem, dest);
    }
    if (writePacked)
    {
        ProtoWriter.EndSubItem(token, dest);
    }
}
```

可以看到这里的GC是由list遍历的foreach引起的。继续往内展开，产生GC的点全部是这两个原因上。

## protobuf-net反序列化

找到第一个产生GC的分支：
{% asset_img 5.jpg %}

同上述分析，MonoProperty.GetValue、MonoProperty.SetValue产生GC原因是反射。而Int32Serializer.Read()代码如下：
```CSharp
public object Read(object value, ProtoReader source)
{
    Helpers.DebugAssert(value == null); // since replaces
    return source.ReadInt32();
}
```
可见产生GC的原因是因为装箱。继续往下展开ListDecorateor.Read函数：
{% asset_img 6.jpg %}
由Activator.CreateInstance得出这里产生GC的原因是实例的创建。继续往下展开：
{% asset_img 7.jpg %}
反射和装箱产生GC上面已经提到，看ProtoReader.AppendBytes代码：
```CSharp
public static byte[] AppendBytes(byte[] value, ProtoReader reader)
{
    if (reader == null) throw new ArgumentNullException("reader");
    switch (reader.wireType)
    {
        case WireType.String:
            int len = (int)reader.ReadUInt32Variant(false);
            reader.wireType = WireType.None;
            if (len == 0) return value == null ? EmptyBlob : value;
            int offset;
            if (value == null || value.Length == 0)
            {
                offset = 0;
                value = new byte[len];
            }
            else
            {
                offset = value.Length;
                byte[] tmp = new byte[value.Length + len];
                Helpers.BlockCopy(value, 0, tmp, 0, value.Length);
                value = tmp;
            }
            // value is now sized with the final length, and (if necessary)
            // contains the old data up to "offset"
            reader.position += len; // assume success
            while (len > reader.available)
            {
                if (reader.available > 0)
                {
                    // copy what we *do* have
                    Helpers.BlockCopy(reader.ioBuffer, reader.ioIndex, value, offset, reader.available);
                    len -= reader.available;
                    offset += reader.available;
                    reader.ioIndex = reader.available = 0; // we've drained the buffer
                }
                //  now refill the buffer (without overflowing it)
                int count = len > reader.ioBuffer.Length ? reader.ioBuffer.Length : len;
                if (count > 0) reader.Ensure(count, true);
            }
            // at this point, we know that len <= available
            if (len > 0)
            {   // still need data, but we have enough buffered
                Helpers.BlockCopy(reader.ioBuffer, reader.ioIndex, value, offset, len);
                reader.ioIndex += len;
                reader.available -= len;
            }
            return value;
        default:
            throw reader.CreateWireTypeException();
    }
}
```
可见，这里产生GC的原因是因为new byte[]操作。
## Protobuf-net GC优化方案
protobuf-net在本次协议测试中GC产生的原因总结如下：
1. 反射
2. forearch
3. 装箱
4. 创建新的pb对象
5. 创建新的字节数组

### 去反射
用过lua的人都知道，不管是tolua还是xlua，去反射的方式是生成wrap文件，这里去反射可以借鉴同样的思想。
```CSharp
using CustomDataStruct;
using ProtoBuf.Serializers;

namespace battle
{
    public sealed class NtfBattleFrameDataDecorator : ICustomProtoSerializer
    {
        public void SetValue(object target, object value, int fieldNumber)
        {
            ntf_battle_frame_data data = target as ntf_battle_frame_data;
            if (data == null)
            {
                return;
            }

            switch (fieldNumber)
            {
                case 1:
                    data.time = ValueObject.Value<int>(value);
                    break;
                case 3:
                    data.slot_list.Add((ntf_battle_frame_data.one_slot)value);
                    break;
                case 5:
                    data.server_from_slot = ValueObject.Value<int>(value);
                    break;
                case 6:
                    data.server_to_slot = ValueObject.Value<int>(value);
                    break;
                case 7:
                    data.server_curr_frame = ValueObject.Value<int>(value);
                    break;
                case 8:
                    data.is_check_frame = ValueObject.Value<int>(value);
                    break;
                default:
                    break;
            }
        }

        public object GetValue(object target, int fieldNumber)
        {
            ntf_battle_frame_data data = target as ntf_battle_frame_data;
            if (data == null)
            {
                return null;
            }

            switch (fieldNumber)
            {
                case 1:
                    return ValueObject.Get(data.time);
                case 3:
                    return data.slot_list;
                case 5:
                    return ValueObject.Get(data.server_from_slot);
                case 6:
                    return ValueObject.Get(data.server_to_slot);
                case 7:
                    return ValueObject.Get(data.server_curr_frame);
            }

            return null;
        }
    }
}
```
反射产生的地方在protobuf-net的装饰类中，具体是PropertyDecorator，我这里并没有去写工具自动生成Wrap文件，而是对指定的协议进行了Hook。

### foreach

foreach对列表来说改写遍历方式就好了，我这里没有对它进行优化，因为Unity5.5以后版本这个问题就不存在了。篇首优化后的效果图中还有一点残留就是因为这里捣鬼。
### 无GC装箱
要消除这里的装箱操作，需要重构代码，而protobuf-net内部大量使用了object进行参数传递，这使得用泛型编程来消除GC变得不太现实。我这里是自己实现了一个无GC版本的装箱拆箱类ValueObject，使用方式十分简单，类似：

```CSharp
public object Read(object value, ProtoReader source)
{
    Helpers.DebugAssert(value == null); // since replaces
    return ValueObject.Get(source.ReadInt32());
}
public void Write(object value, ProtoWriter dest)
{
    ProtoWriter.WriteInt32(ValueObject.Value<int>(value), dest);
}
```

其中ValueObject.Get是装箱，而ValueObject.Value<T>是拆箱，装箱和拆箱的步骤必须一一对应。

### 使用对象池

对于protobuf-net反序列化的时候会创建pb对象这一点，最合理的方式是使用对象池，Hook住protobuf-net创建对象的地方，从对象池中取对象，而不是新建对象，用完以后再执行回收。池接口如下：
```CSharp
/// <summary>
/// 说明：proto网络数据缓存池需要实现的接口
/// 
/// @by wsh 2017-07-01
/// </summary>

public interface IProtoPool
{
    // 获取数据
    object Get();

    // 回收数据
    void Recycle(object data);

    // 清除指定数据
    void ClearData(object data);
    
    // 深拷贝指定数据
    object DeepCopy(object data);
    
    // 释放缓存池
    void Dispose();
}
```
### 使用字节缓存池
对于new byte[]操作的GC优化也是一样的，只不过这里使用的缓存池是针对字节数组而非pb对象，我这里是自己实现了一套通用的字节流与字节buffer缓存池StreamBufferPool，每次需要字节buffer时从中取，用完以后放回。
# protobuf-net GC优化实践
以上关键的优化方案都已经有了，具体怎么部署到protobuf-net的细节问题这里不再多说，有兴趣的朋友自己去看下源代码。这里就优化以后的protobuf-net使用方式做下介绍，首先是目录结构：
{% asset_img 8.jpg protobuf-net-gc-optimization工程结构 %}

1. CustomDatastruct：自定义的数据结构
2. Protobuf-extension/Protocol：测试协议
3. Protobuf-extension/ProtoFactory：包含两个部分，其中ProtoPool是pb对象池，而ProtoSerializer是对protobuf-net装饰器的扩展，用于特定协议的去反射
4. ProtoBufSerializer：Protobuf-net对外接口的封装。

主要看下ProtoBufSerializer脚本：

```CSharp
using battle;
using CustomDataStruct;
using ProtoBuf.Serializers;
using System.IO;

/// <summary>
/// 说明：ProtoBuf初始化、缓存等管理；序列化、反序列化等封装
/// 
/// @by wsh 2017-07-01
/// </summary>

public class ProtoBufSerializer : Singleton<ProtoBufSerializer>
{
    ProtoBuf.Meta.RuntimeTypeModel model;

    public override void Init()
    {
        base.Init();

        model = ProtoBuf.Meta.RuntimeTypeModel.Default;
        AddCustomSerializer();
        AddProtoPool();
        model.netDataPoolDelegate = ProtoFactory.Get;
        model.bufferPoolDelegate = StreamBufferPool.GetBuffer;
    }

    public override void Dispose()
    {
        model = null;
        ClearCustomSerializer();
        ClearProtoPool();
    }

    static public void Serialize(Stream dest, object instance)
    {
        ProtoBufSerializer.instance.model.Serialize(dest, instance);
    }

    static public object Deserialize(Stream source, System.Type type, int length = -1)
    {
        return ProtoBufSerializer.instance.model.Deserialize(source, null, type, length, null);
    }

    void AddCustomSerializer()
    {
        // 自定义Serializer以避免ProtoBuf反射
        CustomSetting.AddCustomSerializer(typeof(ntf_battle_frame_data), new NtfBattleFrameDataDecorator());
        CustomSetting.AddCustomSerializer(typeof(ntf_battle_frame_data.one_slot), new OneSlotDecorator());
        CustomSetting.AddCustomSerializer(typeof(ntf_battle_frame_data.cmd_with_frame), new CmdWithFrameDecorator());
        CustomSetting.AddCustomSerializer(typeof(one_cmd), new OneCmdDecorator());
    }

    void ClearCustomSerializer()
    {
        CustomSetting.CrearCustomSerializer();
    }


    void AddProtoPool()
    {
        // 自定义缓存池以避免ProtoBuf创建实例
        ProtoFactory.AddProtoPool(typeof(ntf_battle_frame_data), new NtfBattleFrameDataPool());
        ProtoFactory.AddProtoPool(typeof(ntf_battle_frame_data.one_slot), new OneSlotPool());
        ProtoFactory.AddProtoPool(typeof(ntf_battle_frame_data.cmd_with_frame), new CmdWithFramePool());
        ProtoFactory.AddProtoPool(typeof(one_cmd), new OneCmdPool());
    }

    void ClearProtoPool()
    {
        ProtoFactory.ClearProtoPool();
    }
}
```
其中：
1. AddCustomSerializer：用于添加自定义的装饰器到protobuf-net
2. AddProtoPool：用于添加自定义对象池到protobuf-net
3. Serialize：提供给逻辑层使用的序列化接口
4. Deserialize：提供给逻辑层使用的反序列化接口

使用示例：
```CSharp
const int SENF_BUFFER_LEN = 64 * 1024;
const int REVIVE_BUFFER_LEN = 128 * 1024;
MemoryStream msSend = new MemoryStream(sendBuffer, 0, SENF_BUFFER_LEN, true, true);;
MemoryStream msRecive = new MemoryStream(reciveBuffer, 0, REVIVE_BUFFER_LEN, true, true);;

msSend.SetLength(SENF_BUFFER_LEN);
msSend.Seek(0, SeekOrigin.Begin);

ntf_battle_frame_data dataTmp = ProtoFactory.Get<ntf_battle_frame_data>();
ntf_battle_frame_data.one_slot oneSlot = ProtoFactory.Get<ntf_battle_frame_data.one_slot>();
ntf_battle_frame_data.cmd_with_frame cmdWithFrame = ProtoFactory.Get<ntf_battle_frame_data.cmd_with_frame>();
one_cmd oneCmd = ProtoFactory.Get<one_cmd>();
cmdWithFrame.cmd = oneCmd;
oneSlot.cmd_list.Add(cmdWithFrame);
dataTmp.slot_list.Add(oneSlot);
DeepCopyData(data, dataTmp);
ProtoBufSerializer.Serialize(msSend, dataTmp);
ProtoFactory.Recycle(dataTmp);//*************回收，很重要

msSend.SetLength(msSend.Position);//长度一定要设置对
msSend.Seek(0, SeekOrigin.Begin);//指针一定要复位
//msRecive.SetLength(msSend.Length);//同理，但是如果Deserialize指定长度，则不需要设置流长度
msRecive.Seek(0, SeekOrigin.Begin);//同理

Buffer.BlockCopy(msSend.GetBuffer(), 0, msRecive.GetBuffer(), 0, (int)msSend.Length);

dataTmp = ProtoBufSerializer.Deserialize(msRecive, typeof(ntf_battle_frame_data), (int)msSend.Length) as ntf_battle_frame_data;

PrintData(dataTmp);
ProtoFactory.Recycle(dataTmp);//*************回收，很重要
```
# Unity3D游戏GC优化实践

protobuf-net的GC优化实践要说的就这么多，其实做GC优化的大概步骤就是这些：GC分析，优化方案，最后再重构代码。这里再补充一些其它的内容，CustomDatastruct中包含了：

1. BetterDelegate：泛型委托包装类，针对深层函数调用树中使用泛型委托作为函数参数进行传递时代码编写困难的问题。
2. BetterLinkedList：无GC链表
3. BetterStringBuilder：无GC版StrigBuilder
4. StreamBufferPool：字节流与字节buffer缓存池
5. ValueObject：无GC装箱拆箱
6. ObjPool：通用对象池

　　其中protobuf-net的无GC优化用到了StreamBufferPool、ValueObject与ObjPool，主要是对象池和免GC装箱，其它的在源代码中有详细注释。TestScenes下包含了各种测试场景：
{% asset_img 9.jpg %}
这里对其中关键的几个结论给下说明：

1. LinkedList当自定义结构做链表节点，必须实现IEquatable<T>、IComparable<T>接口，否则Roemove、Cotains、Find、FindLast每次都有GC产生。

```CSharp
// 重要：对于自定义结构一定要继承IEquatable<T>接口并实现它
// 此外：对于Sort，实现IComparable<T>接口，则在传入委托的时候可以和系统简单值类型一样
public struct CustomStruct : IEquatable<CustomStruct>, IComparable<CustomStruct>
{
    public int a;
    public string b;

    public CustomStruct(int a, string b)
    {
        this.a = a;
        this.b = b;
    }

    public bool Equals(CustomStruct other)
    {
        return a == other.a && b == other.b;
    }

    public int CompareTo(CustomStruct other)
    {
        if (a != other.a)
        {
            return a.CompareTo(other.a);
        }

        if (b != other.b)
        {
            return b.CompareTo(other.b);
        }

        return 0;
    }

    // 说明：测试正确性用的，不是必须
    public override string ToString()
    {
        return string.Format("<a = {0}, b = {1}>", a, b);
    }
}
```
2. 所有委托必须缓存，产生GC的测试一律是因为每次调用都生成了一个新的委托

```CSharp
public class TestDelegateGC : MonoBehaviour
{
    public delegate void TestDelegate(GameObject go, string str, int num);
    public delegate void TestTDelegate<T,U,V>(T go, U str, V num);

    Delegate mDelegate1;
    Delegate mDelegate2;
    TestDelegate mDelegate3;
    TestTDelegate<GameObject, string, int> mDelegate4;
    TestDelegate mDelegate5;
    Comparison<int> mDelegate6;
    Comparison<int> mDelegate7;

    int mTestPriviteData = 100; 
    List<int> mTestList = new List<int>();

    // Use this for initialization
    void Start () {
        mDelegate1 = (TestDelegate)DelegateFun;
        mDelegate2 = Delegate.CreateDelegate(typeof(TestDelegate), this, "DelegateFun");
        mDelegate3 = DelegateFun;
        mDelegate4 = TDelegateFun;

        //static
        mDelegate5 = new TestDelegate(StaticDelegateFun);
        mDelegate6 = SortByXXX;
        mDelegate7 = TSortByXXX<int>;

        mTestList.Add(1);
        mTestList.Add(2);
        mTestList.Add(3);
    }
    
    // Update is called once per frame
    void Update () {
        // 不使用泛型
        TestFun(DelegateFun); 
        TestFun(mDelegate1 as TestDelegate); //无GC
        TestFun(mDelegate2 as TestDelegate); //无GC
        TestFun(mDelegate3); //无GC，推荐
        TestFun(mDelegate5); //无GC
        // 使用泛型，更加通用
        TestTFun(TDelegateFun, gameObject, "test", 1000);//每次调用产生104B垃圾
        TestTFun(mDelegate4, gameObject, "test", 1000);// 无GC，更通用，极力推荐***********
        // Sort测试
        mTestList.Sort();//无GC
        TestSort(SortByXXX);//每次调用产生104B垃圾
        TestSort(mDelegate6);//无GC
        TestSort(TSortByXXX);//每次调用产生104B垃圾
        TestSort(TSortByXXX);//每次调用产生104B垃圾
        TestSort(mDelegate7);//无GC
    }

    private void TestFun(TestDelegate de)
    {
        de(gameObject, "test", 1000);
    }

    private void TestTFun<T, U, V>(TestTDelegate<T, U, V> de, T arg0, U arg1, V arg2)
    {
        de(arg0, arg1, arg2);
    }

    private void TestSort<T>(List<T> list, Comparison<T> sortFunc)
    {
        list.Sort(sortFunc);
    }

    private void TestSort(Comparison<int> sortFunc)
    {
        mTestList.Sort(sortFunc);
    }

    private void DelegateFun(GameObject go, string str, int num)
    {
    }

    private void TDelegateFun<T, U, V>(T go, U str, V num)
    {
    }

    private static void StaticDelegateFun(GameObject go, string str, int num)
    {
    }

    private int SortByXXX(int x, int y)
    {
        return x.CompareTo(y);
    }

    private int TSortByXXX<T>(T x, T y) where T : IComparable<T>
    {
        return x.CompareTo(y);
    }
}
```

3. List<T>对于自定义结构做列表项，必须实现IEquatable<T>、IComparable<T>接口，否则Roemove、Cotains、IndexOf、sort每次都有GC产生；对于Sort，需要传递一个委托。这两点的实践上面都已经说明。