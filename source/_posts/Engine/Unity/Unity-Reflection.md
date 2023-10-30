---
title: Unity Reflection
date: 2019-03-06 12:11:36
categories:
- Engine
- Unity
tags:
  - Unity
---

# 概念

反射是.NET中的重要机制，通过反射，可以在运行时获得程序或者程序集中每一个类型（包括类、结构、委托、接口和枚举等）的成员和成员的信息。另外还可以直接创建对象，即使这个对象的类型在编译时还不知道。

## 用途

- 使用Assembly定义和加载程序集，加载在程序集清单中列出模块，从此程序集中查找类型并创建并创建该类型的实例。
- 使用Module了解包含模块的程序集以及模块中的类等，还可以获取在模块上定义的所有全局方法或其他特定的非全局方法。
- 使用ConstructorInfo了解构造函数的名称，参数，访问修饰符（如public或private）和实现详细信息（如abstract或virtual）等。
- 使用MethodInfo了解方法的名称，返回类型，参数，访问修饰符（如public或private）和实现详细信息（如abstract或virtual）等。
- 使用FieldInfo了解字段的名称、访问修饰符（如public或private）和实现相信信息（如static）等，并获取或设置字段值。
- 使用EventInfo了解事件的名称、事件处理程序数据类型、自定义属性、声明类型和反射类型等，添加或移除事件处理程序。
- 使用PropertyInfo了解属性的名称、数据类型、声明类型、反射类型和只读或可写状态等，获取或设置属性值。
- 使用ParameterInfo了解参数的名称、数据类型、是输入参数还是输出参数，以及参数在方法签名中的位置等。

## 需要的namespace

    System.Reflection;
    System.Type;
    System.Reflection.Assembly;

## 主要类

    System.Type // 通过这个类可以访问任何给定数据类型的信息
    System.Reflection.Assembly; // 访问或者加载程序集的信息

### System.Type

System.Type类对于反射起着核心的作用。它是一个抽象的基类，Type有与每种数据类型对应的派生类。
```CSharp
// 属性
string  Name {get;}           // 数据类型名
string  FullName {get;}       // 数据类型的完全限定名（包括命名空间）
string  Namespace;      // 命名空间名
bool    IsAbstract;     // 是否抽象
bool    IsArray;        // 是否数组
bool    IsClass;        // 是否类
bool    IsEnum;         // 是否枚举
bool    IsInterface;    // 是否接口
bool    IsPublic;       // 是否是公有的类型
bool    IsSealed;       // 是否是密封类
bool    IsValueType;    // 是否是指类型
// 方法
// 用于取得该类的构造函数的信息
public ConstructorInfo GetConstructor();
// 同上
public ConstructorInfo[] GetConstructors();
// 取得该类的事件信息
public EventInfo GetEvent();
public EventInfo[] GetEvents();
public FieldInfo GetField();
public FieldInfo[] GetFields();
public InterfaceInfo GetInterFace();
public InterfaceInfo[] GetInterFaces();
public MemberInfo GetMember();
public MemberInfos GetMembers();
public PropertyInfo GetProperty();
public PropertyInfo[] GetProperties();
// 调用上述成员，方式是调用Type的InvokeMember()方法，或者调用MethodInfo，PropertyInfo的Invoke方法
```

```CSharp
// 查看类的构造方法
NewClass nc = new NewClass();
Type t = nc.GetType();
ConstructorInfo[] ci = t.GetConstructors(); // 获取类的所有构造函数
foreach (ConstructorInfo c in ci)
{
    ParamterInfo[] ps = c.GetParameters();
    foreach (ParamterInfo pi in ps)
    {
        Debug.Log("{0}\t{1}", pi.ParamterType.ToString(), pi.Name);
    }
}
```

```CSharp
// 用构造函数动态生成对象
Type t = typeof(NewClass);
Type[] pt = new Type[2];
pt[0] = typeof(string);
pt[1] = typeof(string);
// 根据参数类型获取构造函数
ConstructorInfo ci = t.GetConstructor(pt);
// 构造Object数组，作为构造函数的输入参数
object[] obj = new object[2]{"Liyanfeng", "Hello World"};
// 调用构造函数生成对象
object o = ci.Invoke(obj);
```

```CSharp
// 用Activator生成对象
Type t = typeof(NewClass);
// 用Activator的CreateInstance静态方法，生成新对象
object o = Activator.CreateInstance(t, "Liyanfeng", "Hello World");
```

### 获取给定类型的Type引用有3中常用方式

```CSharp
// 使用typeof运算啊
Type t = typeof(string);
// 使用对象GetType()方法
string s = "Liyanfeng";
Type t = s.GetType();
// 调用Type类的静态方法GetType()
Type t = Type.GetType("System.String");

foreach(MemberInfo mi in t.GetMembers())
{
    Debug.Log("{0}/t{1}", mi.MemberType, mi.Name);
}
```