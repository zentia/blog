---
title: Lua GC 的源码剖析（1）
mathjax: true
date: 2019-03-27 11:46:53
tags:
    Lua
categories: Lua
---
Lua采用一个简单标记清楚算法的GC系统。
在Lua中，一共只有9就数据类型，分别为nil、boolean、lightuserdata、number、string、table、function、userdata和thread。其中，只有string table function thread四种在vm中以引用方式共享，是需要被GC管理回收的对象。其它类型都以值形式存在。
但在Lua的实现中，还有两种类型的对象需要被GC管理。分别是proto（可以看作未绑定upvalue的函数），upvalue（多个upvalue会引用同一个值）。
Lua是以union+type的形式保存值。具体定义可见lobject.h的100-107行：
```C
/*
** Union of all Lua values
*/
typedef union Value
{
    GCObject *gc;       /* collectable objects */
    void *p;            /* light userdata */
    int b;              /* booleans */
    lua_CFunction f;    /* light C functions */
    lua_Integer i;      /* integer numbers */
    lua_Number n;       /* float numbers */
}Value;

#define TValuefields Value valua_; int tt_

typedef struct lua_TValue {
    TValuefields;
}TValue;
```
我们看到，Value以union方式定义。如果是需要被GC管理的对象，就以GCObject指针形式保存，否则直接保值。在代码的其它部分，并不直接使用Value类型，而是TValue类型。它比Value多了一个类型标识。用int tt_记录。通常的系统中，每个TValue长度为12个字节。
所有的GCObject都有一个相同的数据头，叫做CommonHeader，在lobject.h里79行以宏形式定义出来。使用宏是源于使用上的某种遍历。C语言不支持结构的继承。
```C
/*
** Common Header for all collectable objects (in maro form, to be included in other objecs)
*/
#define CommonHeader GCObject *next; lu_byte tt; lu_byte marked
```
从这里我们可以看到：所有的GCObject都用一个单向量表串了起来。每个对象都以tt来识别其类型。marked域用于标记清除工作。
标记清除算法是一种简单的GC算法。每次GC过程，先以若干根节点开始，逐个把直接以及间接和它们相关的节点都做上标记。对于Lua，这个过程很容易实现。因为所有的GObject都在同一个链表上，当标记完成后，遍历这个链表，把未被标记的节点--删除即可。
Lua在实际实现时，其实不只用一条链表维系所有的GCObject。这是因为string类型有其特殊性。所有的string放在一张大的hash表中。它需要保证系统中不会有值相同的string被创建两份。顾string是被单独管理的，而不串在GCObject的链表中。
回头来看看lua_State这个类型。这是写C和Lua交互时用的最多的数据类型。顾名思义，它表示lua vm的某种状态。从实现上说，更接近lua的一个thread以及其包含的相关数据（堆栈、环境等）。事实上一个lua_State也是一个类型的thread的GCObject。见lstate.h。
```C
sturct lua_State {
    CommonHeader;
    unsigned short nci; /* number of items in 'ci' list */
    lu_byte status;
    StkId top; /* first free slot in the stack */
    global_State *l_G;
    CallInfo *ci; /* call info for current function */
    const Instruction *oldpc; /* last pc traced */
    StkId stack_last; /* last free slot in the stack */
}
```