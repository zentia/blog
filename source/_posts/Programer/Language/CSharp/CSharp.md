---
title: CSharp
date: 2018-04-02 09:46:52
tags:
  - CSharp
categories: CSharp
---
# internal、sealed
**internal**：本应用程序使用，其它进程无法使用

**sealed（密封）**：由它修饰的类、方法、属性将不能被继承或是重写。sealed 必须和override一起使用
sealed修饰符表示密封用于类时，表示该类<font color="red">**不能被继承**</font>，不能和abstract同时使用，<font color="red">**不能被实例化**</font>。密封类不能用作基类、也不能继承，而抽象类总是希望被继承的。

# fixed

fixed 语句可防止垃圾回收器重新定位可移动的变量。 fixed 语句仅允许存在于不安全的上下文中。 Fixed 还可用于创建固定大小的缓冲区。

fixed 语句将为托管变量设置一个指针，并在该语句的执行过程中“单边锁定”该变量。 如果没有 fixed，指向可移动的托管变量的指针将几乎没有什么用处，因为垃圾回收可能会不可预见地重新定位变量。 C# 编译器只允许将指针分配给 fixed 语句中的托管变量。

1. 不支持持多继承，C#类始终继承一个基类（如果未在声明中指定一个基类，则继承自System.Object)
2. 如果对类没有定义显示构造函数，则编译器将提供默认的构造函数，以初始化这些类的成员字段。但是，如果显式添加了构造函数，就可以有效控制整个构造过程。派生类通过使用:base()语法可以显式调用基类的构造函数
3. override 具有override关键字修饰符的方法是对基类中同名方法的新实现，基类中的同名方法必须声明为virtual或abstract类型。给基类中的方法添加virtual关键字表示可以重写它的实现。new static 和 virtual关键字不能与override访问修饰符同时使用。
4. virtual 支持多态，不能与static override同时使用
5. abstract 一个抽象类包含抽象方法和非抽象方法，抽象方法只存放函数原型，不涉及主体代码，override关键字可在派生类中抽象方法，经override声明重写的方法称为重写基类方法，其签名必须与override方法的签名相同。只是不能被实例化，除此以外具有类的其他特性，重要的是抽象类可以包含抽象方法，这是普通类所不能的。抽象方法只能声明于抽象方法中，且不包含任何实现，派生类必须覆盖它们。
6. interface 包含未实现的方法声明，派生类必须实现未实现的方法，抽象类是抽象方法，接口则是所有成员，接口包含属性，索引器事件

# 索引器

索引器允许类或者结构的实例按照于数组相同的方法进行索引取值，索引器与属性类似，不同的是索引器的访问时带参的
- 索引器的索引值(Index)类型不受限制
- 索引器允许重载
- 索引器不是一个变量

## 索引器和属性的不同点

- 属性以名称来标识，索引器以函数形式标识
- 索引器可以被重载，属性不可以
- 索引器不能声明为static，属性可以

# interface、abstract

||abstract|interface|
|--|--|--|
|能否实例化|不能|不能|

## 抽象类和接口的区别：

- 类是对对象的抽象，可以把抽象类理解为把类当作对象，抽象成的类叫抽象类，而接口只是一个行为的规范或规定，
- 接口基本上不具备类型的任何具体特点，它仅仅承诺了能够调用的方法
- 抽象类比较细，而接口比较简单

## 抽象类和接口的使用

- 如果预计要创建组件的多个版本，则创建抽象类。抽象类提供简单的方法来控制组件版本。
- 抽象类主要用于关系密切的对象；而接口适合为不相关的类提供通用功能

# Marshal

提供了一个方法集合，这些方法用于分配非托管内存、复制非托管内存块、将托管类型转换为非托管类型，此外还提供了在于非托管代码交互时使用的其他方法

命名空间：System.Runtime.InteropServices
程序集：mscorlib

AllocHGlobal(int32); // 通过使用指定的字节数，从进程的非托管内存中分配内存
SizeOf(Type)返回非托管类型的大小(以字节为单位)

# this

```CSharp
public static class PGExt
{
	public static T PG_GetComponent(this List<Component> components) where T : Component
	{
		int c = components.Count;
		for (int i = 0; i < c; ++i)
		{
			var p = components[i] as T;
			if ((object)p != null) return p;
		}
		return null;
	}
}
```
为什么这里会有一个this关键字，做什么用？其实这就是扩展方法！这个扩展方法在静态类中声明，定义一个静态方法，其中第一个参数定义可它的扩展类型，PG_GetComponent方法扩展了List类，因为它的第一个参数定义了List类型，为了区分扩展方法和一般的静态方法，扩展方法还需要给第一个参数使用this关键字。

现在就可以使用带List类型的PG_GetComponent方法了：

	var mf = cacheComponents.PG_GetComponent<MeshFilter>();

# volatile

volatile关键字指示一个字段可以由多个同时执行的线程修改。声明volatile的字段不受编译器优化的限制。这样可以确保该字段在任何时间呈现的都是最新的值。
volatile修饰符通常用于由多个线程访问但不使用lock语句对访问进行序列化的字段。
volatile关键字可应用于以下类型的字段：

- 引用类型
- 指针类型（在不安全的上下文中）。请注意，虽然指针本身可以是可变的，但是它指向的对象不能是可变的。换句话说，无法声明“指向可变对象的指针”。
- 类型，如sbyte、byte、short、ushort、int、uint、。
- 引用类型的泛型类型参数。
- IntPtr和UIntPtr

# 事件和委托
```CSharp
public delegate void Print();
class Test
{
    public event Print print; // 去掉event就是一个委托
    public void Start()
    {
        if (print == null)
            return;
        print();
    }
}
class Program
{
    static void Print1()
    {
        Console.WriteLine("Hello");
    }
    static void Print2()
    {
        Console.WriteLine("World");
    }
    static void Main(string[] args)
    {
        var obj = new Test();
        obj.print += Print1; // 事件无法使用=
        obj.print += Print2; // 委托会清掉之前的关联
        obj.Start();
    }
}
```
事件实际上是一个特殊的委托实例，不用事件也没有关系。实际上事件只是削弱了委托的功能，event在编辑器角度保护了程序的安全，因为你只能用+=、-=来注册事件了，而不能使用=为事件关联方法。（在委托中还可以使用=来绑定方法，不过=是一种破坏性代码，不管之前是否已经绑定方法，他都会将其清除）

extern:指示方法在外部实现

# 堆栈和堆 Heap & Stack

首先堆栈和堆（托管堆）都在进程的虚拟内存中。（在32位处理器上每个进程的虚拟内存为4GB）
栈是内存中完全用于存储局部变量或成员（值类型数据）的高效区域，但其大小有限制。
托管堆占内存比栈大得多，当访问速度较慢。托管堆只用于分配内存。
在创建值类型数据时，在栈上分配内存；
当创建引用型类型数据时，在托管堆上分配并返回对象的引用。注意这个对象的引用，像其他局部变量一样也是保存在栈中，该引用指向的值则位于托管堆中。
如果创建一个包含值类型的引用类型，比如数组，其元素的值也是存放在托管堆中的某个地方，由使用该实体的变量引用；而值类型存储在使用它们的地方，有几处在使用，就有几个副本存在。
对于引用类型，如果在声明变量的时候没有使用new运算符，运行时不会给它分配托管堆的内存空间，而是在栈上给它分配一个包含null的值的引用。对于值类型，运行时会给它分配栈上的空间，并且调用构造函数来初始化对象的状态。
## 堆栈stack
堆栈中存储值类型
堆栈实际上是向下填充，即由高内存地址指向低内存地址填充。
堆栈的工作方式是先分配内存的变量后后释放（先进后出原则）。
堆栈的变量是从下向上释放，这样就保证了堆栈中先进后出的规则不与变量的生命周期起冲突
堆栈的性能非常高，但是对于所有的变量来说不太灵活，而且变量的生命周期必须嵌套。
通常我们希望使用一种方法分配内存来存储数据，并且方法退出后很长一段时间内数据仍然可以使用。此时就要用到堆（托管堆）

## 堆（托管堆）heap
<font color="blue">堆（托管堆）存储引用类型。</font>
引用类型共有四种：类类型、接口类型、数组类型和委托类型。
所有引用类型变量所引用的对象，其内存都是托管堆中分配的。
严格地说，我们常说的“对象变量”其实是类类型的引用变量。但在实际中人们经常将引用类型的变量简称为“对象变量”，用它来指代所有四种类型的引用变量。
此堆非彼堆，.NET中的堆由垃圾收集器自动管理。
与堆栈不同，堆是从下往上分配，所以自动的空间都在已用空间的上面。
比如创建一个对象：
    
    Customer cus;
    cus = new Customer();

声明一个Customer的引用cus，在堆栈上给这个引用分配存储空间。这仅仅只是一个引用，不是实际的Customer对象！
cus占4个字节的空间，包含了存储Customer的引用地址。
接着分配堆上的内存以存储Customer对象的实例，假定Customer对象的实例是32字节，为了在堆上找到一个存储Customer对象的存储位置。
.NET运行库在堆中搜索第一个从未使用的，32字节的连续块存储Customer对象的实例。
然后把分配给Customer对象实例的地址赋给cus变量。

从这个例子中可以看出，建立对象引用的过程比建立值变量的过程复杂，且不能避免性能的降低！
实际上就是.NET运行库中保存堆的状态信息，在堆中添加新数据时，堆栈中的引用变量也要更新。性能上损失很多。
有种机制在分配变量内存的时候，不会受到堆栈的限制：把一个引用变量的值赋给一个相同类型的变量，那么这两个变量就引用同一个堆中对象。
当一个应用变量出作用域时，它会从堆栈中删除。但引用对象的数据仍然保留在堆中，一直到程序结束或者该数据不被任何变量应用时，垃圾收集器会删除它。

装箱转化
```
using System;
class Boxing
{
    public static void Main()
    {
        int i = 110;
        object obj = i;
        Console.WriteLine("i={0},obj = {1}",i,obj);
        obj = 330;
        Console.WriteLine("i = {0},obj={1}",i,obj);
    }
}
```

定义整数类型变量的时候，这个变量占用的内存是内存占中分配的，第二句是装箱操作将变量110存放到了内存堆中，而定义object对象类型的变量Obj则在内存栈中，并指向int类型的数值110，而该数值是赋给变量i的数值副本。
内存格局通常分为四个区
全局数据区：存放全局变量，静态数据，常量
代码区：存放所有的程序代码
栈区：存放为运行而分配的局部变量，参数，返回数据，返回地址等。

值类型变量和引用类型变量的内存分配模型也不一样。为了理解清楚这个问题，首先先区分两种不同类型的内存区域：线程堆栈（Thread Stack）和托管堆（Managed Heap）。每个正在运行的程序都对应着一个进程（process），在一个进程内部，可以有一个或多个线程（thread），每个线程都拥有一块“自留地”，称为“线程堆栈”，大小为1M，用于保存自身的一些数据，比如函数中定义的局部变量、函数调用的传参值，这部分内存区域的分配与回收不需要人为干涉。
<font color="blue">所有值类型的变量都是在线程堆栈中分配的。</font>
另一块内存区域成为“堆（heap）”，在.NET托管环境下，堆由CLR进行管理，所以又称为“托管堆（managed heap）”。
<font color="red">用new关键字创建的类的对象时，分配给对象的内存单元就位于托管堆中。</font>
在程序中我们可以随意地使用new 关键字创建多个对象，因此，托管堆中的内存资源是可以动态申请并使用的，当然使用完了必须归还。

对象变量的相互赋值不会导致对象自身被复制，其结果是两个对象变量指向同一对象。另外，由于对象变量是一个局部变量，因为，对象变量本身是位于线程堆栈中的。
严格区分**对象变量**和**对象变量所引用的对象**，是面向对象编程的关键之一。

## 托管堆

.NET框架包含一个托管堆，所有的.NET语言在分配引用类型对象时都要使用它。像值类型这样的轻量级对象始终分配在栈中，但是所有的**类实例**和**数组**都被生成在一个内存池中，这个内存池就是托管堆。

垃圾收集器算法：
- 将所有的托管内存标记为垃圾
- 寻找正被使用的内存块，并将他们标记为有效
- 释放所有没有被使用的内存块
- 整理堆以减少碎片

## 托管堆优化

看上去很简单，但是垃圾收集器实际采用的步骤和堆管理系统的其它部分并非微不足道，其中尝尝涉及为提高性能而作的优化设计。举例来说，垃圾收集遍历整个内存池具有很高的开销。然而，研究表明大部分的托管堆上分配的对象只有很短的生存期，因此堆被分为三个段，称作generations。新分配的对象被放在generation 0中。这个generation是最先被回收的-在这个generation中最有可能找到不再使用的内存，由于它的尺寸很小（小到足以放进处理器的L2 cache中），因此在它里面的回收将是最快和最高效的。
托管堆的另外一种优化操作与locality of reference规则有关。该规则表明，一起分配的对象经常被一起使用。如果对象们在堆中位置很紧凑的话，高速缓存的性能将会得到提高。由于托管堆的天性，对象们总是被分配在连续的地址上，托管堆总是保持紧凑，结果使得对象们始终彼此靠近，永远不会分得很远。这一点与标准提供的非托管代码形成了鲜明的对比，在标准堆中，堆很容易变成碎片，而且一起分配的对象经常分得很远。
还有一种优化是与大对象有关的。通常，大对象具有很长的生存期。当一个大对象在.NET托管堆中产生时，它被分配在堆的一个特殊部分中，这部分堆永远不会被整理。因此移动大对象所带来的开销超过了整理这部分堆所能提高的性能。
## 关于外部资源（External Resources）的问题

垃圾收集器能够有效地管理从托管堆中释放的资源，但是资源回收操作只有在内存紧张而触发一个回收动作时才执行。那么，累时怎样来管理像数据库连接或者窗口句柄这样有限的资源的呢？等待，直到垃圾回收被触发之后在清理数据库连接或者文件句柄并不是一个好方法，这会严重降低系统的性能。
所有拥有外部资源的类，在这些资源已经不再用到的时候，都应当执行Close或者Dispose方法。
需要清理外部资源的类还应当实现一个终止操作（finalizer）。在C#中，创建终止操作的首选方式是在析构函数中实现，而在Framework层，终止操作的实现则是通过重载System.Object.Finalize方法。以下两种实现终止操作的方法是等效的：
```
~OverdueBookLocator()
{
    Dispose(false);
}
public void Finalize()
{
    base.Finalize();
    Dispose(false);
}
```
在C#中，同时在Finalize方法和析构函数实现终止操作将会导致错误的产生。
除非你有足够的理由，否则你不应该创建析构函数或者Finalize方法。终止操作会降低系统的性能，并且增加执行期的内存开销。同时，由于终止操作被执行的方式，你并不能保证何时一个终止操作被执行。
## 内存分配和垃圾回收的细节

对GC有了一个总体印象之后，让我们来讨论关于托管堆中的分配与回收工作的细节。托管堆看起来与我们已经熟悉的C++编程中的传统的堆一点都不像。在传统的堆中，数据结构习惯于使用大块的空闲内存。在其中查找特定大小的内存块是一件很耗时的工作，尤其是当内存中充满碎片的时候。与此不同，在托管堆中，内存被组成连续的数组，指针总是巡着已经被使用的内存和未被使用的内存之间的边界移动。当内存被分配的时候，指针只是简单地递增--由此而来的一个好处是，分配操作的效率得到了很大的提升。
当对象被分配的时候，它们一开始被generation 0中。当generation 0的大小快要达到它的上限的时候，一个只在generation 0中执行的回收操作被触发。由于generation 0的大小很小，因此这将是一个非常快的GC过程。这个GC过程的结果是将generation 0彻底的刷新了一遍。不再使用的对象被释放，正在使用的对象被整理并移入generation 1中。
当generation 1的大小随着generation 0中移入的对象数量的增加而接近它的上限的时候，一个回收动作被触发在generation 0和generation 1中执行GC过程。如同在generation 0中一样，不再使用的对象被释放，正在被使用的对象被整理并移入下一个generation中。大部分GC过程的主要目标是generation 0，因为在generation 0中最优可能存在大量的已不再使用的临时对象。对generation 2的回收过程具有很高的开销，并且此过程只有在generation 0和generation 1的GC过程不能释放足够的内存时才会被触发。如果对generation 2的GC过程仍然不能释放足够的内存，那么系统就会抛出OutOfMemoryException异常。
带有终止操作的对象的垃圾收集过程要稍微复杂一些。当一个带有终止操作的对象被标记为垃圾时，它并不会被立即释放。相反，它会被放置一个终止队列（finalizetion queue）中，此队列为这个对象建立一个引用，来避免这个对象被回收。后台线程为队列中的每个对象执行它们各自的终止操作，并且将已经执行过终止操作的对象从终止队列中删除。只有那些已经执行过终止操作的对象才会在下一次垃圾回收过程中被内存中删除。这样做的一个后果是，等待被终止的对象有可能在它被清除之前，被移入更高一级的generation中，从而增加它被清除的延迟时间。
需要执行终止操作的对象应当事先IDisposable接口，以便程序通过此接口快速执行终止动作。IDisposable接口包含一个Dispose方法。这个方法被用来释放外部资源并抑制终止操作。
```
public class OverdueBookLocator:IDisposable
{
    ~OverdueBookLocator()
    {
        InternalDispose(false);
    }
    public void Dispose()
    {
        InternalDispose(true);
    }
    protected void InternalDispose(bool disposing)
    {
        if (disposing)
        {
            GC.SuppressFinalize(this);
        }
    }
}
```

## 栈和托管堆
通用类型系统（CTS）区分两种基本类型：值类型和引用类型。它们之间的根本区别在于它们在内存中的存储方式。.NET使用两种不同的物理内存来存储数据<font color="orange">栈和托管堆</font>
{% asset_img 1.png %}
值类型总是内存中占用一个预定义的字节数（例如，int类型占4个字节，而string类型占用的字节数会根据字符串的长度而不同）。当声明一个值类型变量时，会在栈中分配适当大小的内存（除了引用类型的值类型成员外，如类的int字段）。内存中的这个空间用来存储变量所包含的值。.NET维护一个栈指针，它包含栈中下一个可用的内存空间的地址。当一个变量离开作用域时，栈指针指向下移动被释放变量所占用的字节数。所以它仍指向下一个可用地址。
引用变量也利用栈，但这时候栈包含的只是对另一个内存位置的引用，而不是实际值。这个位置是托管堆中的一个地址，和栈一样，他也维护一个指针，包含堆中下一个可用的内存地址。但是，堆不是先入后出的，因为对对象的引用可在我们的程序中传递（例如，作为参数传递给方法调用）。堆中的对象不会在程序的一个预定点离开作用域。为了在不适用在堆中分配的内存时将它释放。.NET定期执行垃圾回收器，垃圾垃圾递归检查应用程序中所有对象的引用。引用不再有效的对象使用的内存无法从程序中访问，该内存就可以回收。

## 类型层次结构

CTS定义了一种类型层级结构，该结构不仅仅描述了不同预定义类型，还指出了用户定义类型的层次结构的位置。

{% asset_img 2.png%}

# 枚举
可以定义除Char以外的所有整数类型，默认类型是int
```CSharp
enum Color : byte
{
    Red,
    Green,
    Blue,
};
```

# readonly与const
readonly是运行时常量，程序运行时进行赋值，赋值完之后无法更改，也被称为只读变量。
const表示编译时常量，程序编译时将对常量值进行解析，并将所有常量值引用替换为相应值。

# using 语句
提供可确保正确使用IDisposable对象的方便语法。
## 示例
```CSharp
using (Font font1 = new Font("Arial", 10.0f))
{
    byte charset = font1.GdiCharSet;
}
```
# 备注
File和Font是访问非托管资源（本例中为文件句柄和设备上下文）的托管类型的示例。有许多其他类别的非托管资源和封装这些资源的类库类型。所有此类型都必须实现IDisposable接口。
IDisposable对象的生存期限于单个方法时，应在using语句中声明并实例化它。using语句按照正确的方式调用对象上的Dispose方法，并（在按照前面所示方式使用它时）会导致在调用Dispose时对象自身处于范围之外。在using块中，对象是只读的并且无法进行修改或重新分配。

using语句可确保调用Dispose，即使using块中发生异常也是如此。通过将对象放入try块中，然后调用finally块中的Dispose，可以实现相同的结果；实际上，这就是编译器转换using语句的方式。前面的代码示例在编译时将扩展到以下代码（请注意，使用额外的大括号为对象创建有限范围）；
```CSharp
{
    Font font1 = new Font("Arial", 10.0f);
    try
    {
        byte charset = font1.GdiCharSet;
    }
    finally
    {
        if (font1 != null)
            ((IDsiposable)font1).Dispose();
    }
}
```
可在using语句中声明一个类型的多个实例，如下面的示例中所示：
```CSharp
using (Font font3 = new Font("Arial", 10.0f), font4 = new Font("Arial", 10.0f))
{
    // Use font3 and font4.
}
```
可以在实例化资源对象，然后将变量传递到using语句，但这不是最佳做法。在这种情况下，控件退出using块以后，对象保留在作用域中，但是可能没有访问其未托管资源的劝降。换句话说，它不再是完全初始化的。如果尝试在using块外部使用该对象，则可能导致引发异常。因此，通常最好在using语句中实例化该对象并将其范围限制在using块中。
```CSharp
Font font2 = new Font("Arial", 10.0f);
using (font2) // not recommended
{
    // use font2
}
// font2 is still in scope
// but the method call throws an exception
float f = font2.GetHeight();
```
# HashTable HashSet和Dictionary的区别
## HashTable
哈希表(HashTable)表示键/值对的集合.在.NET Framework中,HashTable是System.Collections命名空间提供的一个容器,用于处理和表现类似key-value的键值对,其中key通常可用来快速查找,同时key是区分大小写;value用于存储对应于key的值.hashtable中key-value键值对均为object类型,所以hashtable可以支持任何类型的keyvalue键值对,任何非null对象都可以用作键值对.
在哈希表添加一个key/键值对:HashTableObject.Add(key,);
在哈希表中去除某个key/键值对:HashTableObject.Remove(key);
从哈希表中移除所有元素:HashtableObject.Clear();
判断哈希表是否包含特定键key:HashtableObject.Contains(key);
## HashSet
HashSet<T>类主要是设计用来做高性能集运算的,例如对两个集合求交集,并集,差集等.集合中包含一组不重复出现且无特性顺序的元素,HashSet拒绝接受重复的对象.
HashSet<T>的一些特性如下:
1. HashSet<T>中的值不能重复且没有顺序
2. HashSet<T>的容量会按需自动添加

## Dictionary
Dictionary表示键值的集合.
`Dictionary<string,string>`是一个泛型
他本身有集合的功能有时候可以把它堪称数组
它的结构是这样的:`Dictionary<[key],[value]>`
它的特点是存入对象是需要与[key]值一一对应的存入该泛型
通过某一个一定的`[key]`去找对应的值
## HashTable和Dictionary的区别
1. HashTable不支持泛型,而Dictionary支持泛型
2. HashTable的元素属于Object类型,所以在存储或检索值类型时通常发生装箱和拆箱的操作,所以你可能需要进行类型转换的操作,而且对于int,float这些值类型还需要进行装箱等操作,非常耗时.
3. 单线程程序中推荐使用Dictionary,有泛型优势,且读取速度较快,容量利用更充分.多线程程序中推荐使用Hashtable,默认的hashtable允许单线程写入,多线程读取,对hashtable进一步调用`synchronized()`方法可以获得完全线程安全的类型,而Dictionary非线程安全,必须人为使用lock语句进行保护,效率搭建.
4. 在通过代码测试的时候发现key是整数型Dictionary的效率比hashtable快,如果key是字符串型,Dictionary的效率没有hashtable快.
```CSharp
static int count = 1000000;
static void IntMethod()
{
    Dictionary<int, int> dictionary = new Dictionary<int, int>();
    Hashtable hashtable = new Hashtable();
    for (int i = 0; i < count; i++)
    {
        dictionary.Add(i,i);
        hashtable.Add(i,i);
    }
    Stopwatch stopwatch = Stopwatch.StartNew();
    for (int i = 0; i < count; i++)
    {
        int value = dictionary[i];
    }
    stopwatch.Stop();
    UnityEngine.Debug.Log(stopwatch.ElapsedMilliseconds);
    stopwatch = Stopwatch.StartNew();
    for (int i = 0; i < count; i++)
    {
        object value = hashtable[i];
    }
    stopwatch.Stop();
    UnityEngine.Debug.Log(stopwatch.ElapsedMilliseconds);

}

static void StringMethod()
{
    Dictionary<string,string> dictionary = new Dictionary<string, string>();
    Hashtable hashtable = new Hashtable();
    for (int i = 0; i < count; i++)
    {
        dictionary.Add(i.ToString(),"aaa");
        hashtable.Add(i.ToString(), "bbb");

    }
    Stopwatch stopwatch = Stopwatch.StartNew();
    for (int i = 0; i < count; i++)
    {
        string vale = dictionary[i.ToString()];
    }
    stopwatch.Stop();
    UnityEngine.Debug.Log(stopwatch.ElapsedMilliseconds);
    stopwatch = Stopwatch.StartNew();
    for (int i = 0; i < count; i++)
    {
        object value = hashtable[i.ToString()];
    }
    stopwatch.Stop();
    UnityEngine.Debug.Log(stopwatch.ElapsedMilliseconds);
}
```
{% asset_img 1.jpg %}