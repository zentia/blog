---
title: C++
date: 2019-02-25 18:00:00
tag:
    - C++
categories: C++
---

# 虚函数

- 对于一个class，产生一堆指向virtual functions的指针，虚函数表指针通常放在对象实例的最前面的位置。编译报错的时候没有vtable，表示该纯虚函数没有实现。
- 每一个对象添加一个指针，指向相关的virtual table。这个指针被称作虚函数表指针。

# C/C++中static关键字作用总结

## 先来介绍它的第一条也是最重要的一条：隐藏。（static函数，static变量均可）
当同事编译多个文件时，所有未加static前缀的全局变量和函数都具有全局可见性。
举例来说明。同时编译两个源文件，一个是a.c，另一个是main.c。
```C++
//a.c
char a = 'A'; // global variable
void msg()
{
    printf("Hello\n");
}

// main.c
int main()
{
    extern char a; // extern variable must be declared before use
    printf("%c", a);
    (void)msg();
    return 0;
}
```

程序运行结果是：
A Hello
为什么在a.c中定义的全局变量a和函数msg能在main.c中使用？前面说过，所有未加static前缀的全局变量和函数都具有全局可见性，其它的源文件也能访问。此例中，a是全局变量，msg是函数，并且都没有加static前缀，因此对于另外的源文件main.c是可见的。
如果加了static，就会对其它源文件隐藏。例如在a和msg的定义前加上了static，main.c就看不到它们了。利用这一特性可以再不同的文件中定义同名函数和同名变量，而不必担心命名冲突。static可以用作函数和变量的前缀，对于函数来讲，static的作用仅限于隐藏。

## static的第二个作用是保持内容的持久。（static变量中的记忆功能和全局生存期）
存储在静态数据区的变量会在程序刚开始运行时就完成初始化，也是唯一的一次初始化。共有两种变量存储在静态存储区：全局变量和static变量，只不过和全局变量比起来，static可以控制变量的可见范围，说到底static还是用来隐藏的，虽然这种用法不常见
PS：如果作为static局部变量在函数内定义，它的生存期为整个源程序，但是其作用域仍与自动变量相同，只能定义该变量的函数内使用该变量。退出该函数后，尽管该变量还继续存在，但不能使用它。

程序举例：
```C++
#include <stdio.h>
int fun()
{
    static int count = 10; //在第一次进入这个函数的时候，变量a被初始化为10！并接着自减1，以后每次进入该函数，a就不会再次被初始化了，仅进行自减1的操作；在static发明前，要达到同样的功能，则只能使用全局变量；
    return count--;
}
int count = 1;
int main(void)
{
    printf("global\t\tlocal static\n");
    for(;count<=10;++count)
    {
        printf("%d\t\t%d\n",count,fun());
    }
    return 0;
}
```
基于以上两点可以得出一个结论：把局部变量改变为静态变量后是改变了它的存储方式即改变了它的生存期。把全局变量改变为静态变量后是改变了它的作用域，限制了它的使用范围。因此static这个说明符在不同的地方所起的作用是不同的。

## static的第三个作用是默认初始化为0（static变量）

其实全局变量也具备这一属性，因为全局变量也存储在静态数据区。在静态数据区，内存中所有的字节默认值都是0x00，某些时候这一特点可以减少程序员的工作量。比如初始化一个稀疏举证，我们可以一个一个地把所有元素都置0，然后把不是0的几个元素赋值。如果定义成静态的，就省去了一开始置0的操作。再比如把一个字符数组当字符串来用，但又觉得每次在字符数组末尾加'\0'；太麻烦。如果把字符串定义成静态的，就省去了这个麻烦，因为那里本来就是'\0';

最后对static的三条作用做一句话总结。首先static的最主要功能是隐藏，其次因为static变量存放在静态存储区，所以它具备持久性和默认值0。

## static的第四个作用：C++中的类成员声明static（有些地方与以上作用重叠）
在类中声明static变量或者函数时，初始化时使用作用域运算符来标明它所属类，因此，静态数据成员是类的成员，而不是对象的成员，这样就出现以下作用：
（1）类的静态成员函数是属于整个类而非类的对象，所以它没有this指针，这就导致了它仅能访问类的静态数据和静态成员函数。
（2）不能将静态成员函数定义为虚函数。
（3）由于静态成员声明于类中，操作于其外，所以对其取地址操作，就多少有些特殊，变量地址是指向其数据类型的指针，函数地址类型是一个"nonmember函数指针"。
（4）由于静态成员函数没有this指针，所以就差不多等同于nonmember函数，结果就产生了一个意想不到的好处：成为一个callback函数，使得我们得以将C++和C-based XWindow系统结合，同时也成功的应用于线程函数身上。（这条没遇见过）
（5）static并没有增加程序的时空开销，相反它还缩短了子类对父类静态成员的访问时间，节省了子类的内存空间。
（6）静态数据成员在<定义或说明>时前面加关键字static。
（7）静态数据成员是静态存储的，所以必须对它进行初始化。（程序员手动初始化，否则编译时一般不会报错，但是Link时会报错误）
（8）静态成员初始化与一般数据成员初始化不同：
初始化在类体外进行，而前面不加static，以免与一般静态变量或对象相混淆；
初始化时不加该成员的访问权限控制符private,public等；
初始化时使用作用域来标明它所属类；
所以我们得出静态数据成员初始化的格式：
<数据类型> <类名>::<静态数据成员名>=<值>
（9）为了防止父类的影响，可以在子类定一个与父类相同的静态变量，以屏蔽父类的影响。这里有一点需要注意：我们说静态成员为父类和子类共享，但我们有重复定义了静态成员，这会不会引起错误呢？不会，我们的编译器采用了一种绝妙的手法：name-mangling用以生成唯一的标志。

# C++智能指针

## shared_ptr
shared_ptr使用引用计数，每一个shared_ptr的拷贝都指向相同的内存。

## weak_ptr
weak_ptr没有共享资源，它的构造不会引起指针引用计数的增加
## unique_ptr
"唯一"拥有其所指对象，同一时刻只能有一个unique_ptr指向给定对象

# 虚函数

通过一张虚函数表（Virtual Table）来实现
sizeof不统计虚函数表

# C/C++内存对齐

## 什么是内存对齐

32位系统下，int占4byte，char占1byte，那么将它们放到一个结构体中实际上占8byte，这就是内存对齐导致的。

# inline
## 什么是内联函数？
内联函数是指那些定义在类体内部的成员函数，即该函数的函数体放在类体内。
一般来说，inline适用的函数有两种，一种是类内定义的成员函数，另一种是类内声明，类外定义的成员函数，对于这两种情况inline的使用有一些不同；
## 内联函数的优缺点：
优点：
1.inline定义的类的内联函数，函数的代码被放入符号表中，在使用时直接进行替换，（像宏一样展开），没有了调用的开销，效率也很高。
2.inline也是真正的函数，编辑器在调用一个内联函数时，会首先检查它的参数的类型，保证调用正确。然后进行一系列的相关检查，就像对待任何一个正在的函数一样。这样就消除了它的隐患和局限性。（宏替换不会检查参数类型，安全隐患较大）
缺点：
1.内联函数具有一定的局限性，inline函数的函数体一般来说不能太大，如果太大，一般的编译器会放弃内联方式，而采用普通的方式调用函数。（换句话说，你使用内联函数，只不过是向编译器提出一个申请，编译器可以拒绝你的申请），这样，内联和普通函数执行效率一样了。
2.inline对编译器来说只是一种简易，编译器可以选择忽略这个建议。
## 注意事项
1.内联函数不能宝库欧复杂的控制语句，如循环语句和switch语句；

# 接口（抽象类）

接口描述了类的行为和功能，而不需要完成类的特定实现。

# 视C++为一个语言联邦

1. C语言
2. 面向对象
3. C++模版
4. STL容器

# 尽量用const，enum，inline替换#define

1. const的好处：define直接常量替换，出现编译错误不易定位（不知道常量是哪个变量，这个其实还好，工程规范点的都不存在这个问题）；define没有作用域，const有作用域提供了封装性（这个同上，不过这两点都是人为的避开了）
2. enum的好处：提供了封装性，编译器肯定不会分配额外的内存空间（其实const也不会，难道define会？至于封装性，undef，所以这么多年了，虽然说避免，但是工程上用的还是比较多的）
3. inline的好处：define宏函数容易造成误用

宏实现工厂设计模式

1. 需要一个全局map用于存储类的信息以及创建实例的函数
2. 需要调用全局对象的构造函数用于注册

```
// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <map>

using namespace std;

typedef void *(*register_fun)();

class CCFactory
{
public:
    static void *NewInstance(string class_name)
    {
        auto it = map_.find(class_name);
        if (it == map_.end())
        {
            return NULL;
        }
        else
        {
            return it->second();
        }
    }
    static void Register(string class_name, register_fun func)
    {
        map_[class_name] = func;
    }
private:
    static map<string, register_fun> map_;
};

map<string, register_fun> CCFactory::map_; // 体外初始化

#define REGISTER_CLASS(class_name); \
    CCFactory::Register(#class_name, []()->void *{return (new class_name);});

class Product
{
public:
    virtual void Say() = 0;
};

class Register : public Product // 注册者，就是产品，虽然不喜欢设计模式这坨东西，但是研究起来还是挺好玩的
{
public:
    void Say()
    {
        std::cout << "Say What?\n";
    }
};

void Init()
{
    REGISTER_CLASS(Register);
}

void Release()
{

}

int main()
{
    Init();
    Register* a = (Register*)CCFactory::NewInstance("Register");
    a->Say();
    Release();
}
```
没写完，但是感觉做个池还是挺好玩的。

# 尽可能使用const

1. const定义接口，防止误用
2. const成员函数，代表这个成员函数承诺不会改变对象值（const成员只能调用const成员函数(加-fpermissive编译选项就可以了)，非const成员可以调用所有成员函数）

# 确定对象使用前被初始化

```
// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <map>

using namespace std;

typedef void *(*register_fun)();

class CCFactory
{
public:
    static void *NewInstance(string class_name)
    {
        auto it = map_.find(class_name);
        if (it == map_.end())
        {
            return NULL;
        }
        else
        {
            return it->second();
        }
    }
    static void Register(string class_name, register_fun func)
    {
        map_[class_name] = func;
    }
private:
    static map<string, register_fun> map_;
};

map<string, register_fun> CCFactory::map_; // 体外初始化

#define REGISTER_CLASS(class_name); \
    CCFactory::Register(#class_name, []()->void *{return (new class_name(#class_name));});

class Product
{
public:
    virtual void Say() = 0;
};

class Register : public Product // 注册者，就是产品，虽然不喜欢设计模式这坨东西，但是研究起来还是挺好玩的
{
public:
    void Say()
    {
        std::cout << name.c_str() << ": Say What?\n";

    }
    void Run() const // 表明这个函数不会对类对象的数据成员（准确地说是非静态数据成员）作任何改变
    {

    }
    int b;
    const string name; // 必须在初始化的时候赋值
    Register(string _y) : name(_y)
    {

    }
};

void Init()
{
    REGISTER_CLASS(Register);
}

void Release()
{

}

int main()
{
    Init();
    Register* a = (Register*)CCFactory::NewInstance("Register");
    a->Say();
    Release();
}
```

# 构造、析构、赋值运算

如果类中没有定义，程序却调用了，编译器会产生一些函数

1. 一个default的构造函数
2. 一个copy构造函数
3. 一个copy assignment 操作符
4. 一个析构函数(non virtual)

- 如果自己构造了带参数的构造函数，编译器不会产生default构造函数
- base class如果把拷贝构造函数或者复制操作符设置为private，不会产生这两个函数
- 含有引用成员变量或者const成员变量不产生复制操作符

# 若不想使用编译器自动生成的函数，就该明确拒绝

将默认生成的函数声明为private，或者C++ 11新特性"=delete"
```
class UncopyTable
{
private:
    UncopyTable(const UncopyTable&);
    UncopyTable& operator=(const UncopyTable&);
};
```

# 静态类型

上下文无关，在编译时就可以确定其类型。

# 动态类型

基类指针指向的对象在编译的时候无法确定

# 动态绑定与静态绑定

静态绑定：编译时绑定，通过对象调用
动态绑定：运行时绑定，通过地址实现

要触发动态绑定，需满足两个条件

1. 只有虚函数才能惊醒动态绑定，非虚函数不能进行动态绑定
2. 必须通过基类类型的引用或指针进行函数调用

# 为多态基类声明virtual析构函数

1. 给多态基类应该主动声明virtual析构函数
2. 非多态基类，没有virtual函数，不要声明virtual析构函数

# 复制对象时勿忘其每个成分

1. 实现拷贝构造函数和赋值操作符的时候，调用base的相关函数
2. 可以让拷贝构造函数和赋值操作符调用一个共同的函数，比如Init

# 资源管理

## 以对象管理资源

1. 为了防止资源泄漏，请使用RAII对象，在构造函数里面获得资源，在析构函数里面释放资源
2. shared_ptr，unique_lock都是RAII对象

## 在资源管理类小心copy行为

* 常见的RAII对象copy行为
   - 禁止copy
   - 引用计数
   - 深度赋值
   - 转移资源拥有权

# 头文件搜索目录

    #include "headfile.h"

搜索顺序为：
1. 当前目录
2. -I指定目录
3. gcc的环境变量CPLUS_INCLUDE_PATH（C程序使用的是C_INCLUDE_PATH）
4. gcc内定目录
/usr/include
/usr/local/include
/usr/lib/gcc/x86_64-redhat-linux/4.1.1/include

