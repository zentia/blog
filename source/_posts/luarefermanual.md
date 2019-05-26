---
title: Lua参考手册下半部分
mathjax: true
date: 2019-05-24 14:28:10
tags:
    - Lua
categories: Lua
---
当前大部分来源是5.1的标准
### lua_Reader

    typedef const char * (*lua_Reader) (lua_State *L, void *data, size_t *size);

lua_load 用到的读取器函数， 每次它需要一块新的 chunk 的时候， lua_load 就调用读取器， 每次都会传入一个参数 data 。 读取器需要返回含有新的 chunk 的一块内存的指针， 并把 size 设为这块内存的大小。 内存块必须在下一次函数被调用之前一直存在。 读取器可以通过返回一个 NULL 来指示 chunk 结束。 读取器可能返回多个块，每个块可以有任意的大于零的尺寸。

### lua_register

    void lua_register (lua_State *L, const char *name, lua_CFunction f);

把 C 函数 f 设到全局变量 name 中。 它通过一个宏定义：

     #define lua_register(L,n,f) \
            (lua_pushcfunction(L, f), lua_setglobal(L, n))

### lua_remove

    void lua_remove (lua_State *L, int index);

从给定有效索引处移除一个元素， 把这个索引之上的所有元素移下来填补上这个空隙。 不能用伪索引来调用这个函数， 因为伪索引并不指向真实的栈上的位置。

### lua_replace

    void lua_replace (lua_State *L, int index);

把栈顶元素移动到给定位置（并且把这个栈顶元素弹出）， 不移动任何元素（因此在那个位置处的值被覆盖掉）。

### lua_resume

    int lua_resume (lua_State *L, int narg);

在给定线程中启动或继续一个 coroutine 。
要启动一个 coroutine 的话，首先你要创建一个新线程 （参见 lua_newthread ）； 然后把主函数和若干参数压到新线程的堆栈上； 最后调用 lua_resume ， 把 narg 设为参数的个数。 这次调用会在 coroutine 挂起时或是结束运行后返回。 当函数返回时，堆栈中会有传给 lua_yield 的所有值， 或是主函数的所有返回值。 如果 coroutine 切换时，lua_resume 返回 LUA_YIELD ， 而当 coroutine 结束运行且没有任何错误时，返回 0 。 如果有错则返回错误代码（参见 lua_pcall）。 在发生错误的情况下， 堆栈没有展开， 因此你可以使用 debug API 来处理它。 出错信息放在栈顶。 要继续运行一个 coroutine 的话，你把需要传给 yield 作结果的返回值压入堆栈，然后调用 lua_resume 。

### lua_setallocf

    void lua_setallocf (lua_State *L, lua_Alloc f, void *ud);

把指定状态机的分配器函数换成带上指针 ud 的 f 。

### lua_setfenv

    int lua_setfenv (lua_State *L, int index);

从堆栈上弹出一个 table 并把它设为指定索引处值的新环境。 如果指定索引处的值即不是函数又不是线程或是 userdata ， lua_setfenv 会返回 0 ， 否则返回 1 。

### lua_setfield

    void lua_setfield (lua_State *L, int index, const char *k);

等价于t[k] = v的操作，这里t是给出的有效索引index处的值，而v是栈顶的那个值。这个函数将把这个值弹出堆栈。 跟在Lua中一样，这个函数可能触发一个 "newindex" 事件的元方法。

### lua_setglobal

    void lua_setglobal (lua_State *L, const char *name);

从堆栈上弹出一个值，并将其设到全局变量 name 中。 它由一个宏定义出来：

    #define lua_setglobal(L,s)   lua_setfield(L, LUA_GLOBALSINDEX, s)

### lua_setmetatable

    int lua_setmetatable (lua_State *L, int index);

把一个 table 弹出堆栈，并将其设为给定索引处的值的 metatable 。

### lua_settable

    void lua_settable(lua_State *L, int index);

作一个等价于 t[k] = v的操作，这里t是一个给定有效索引 index 处的值， v 指栈顶的值， 而 k 是栈顶之下的那个值。这个函数会把键和值都从堆栈中弹出。 和在 Lua 中一样，这个函数可能触发 "newindex" 事件的元方法。

### lua_settop

    void lua_settop(lua_State *L, int index);

参数允许传入任何可接受的索引以及0。它将把堆栈的栈顶设为这个索引。如果新的栈顶比原来的大，超出部分的新元素将被填为nil。如果index为0，把栈上所有元素移除。

lua_State
typedef struct lua_State lua_State;
一个不透明的结构，它保存了整个 Lua 解释器的状态。 Lua 库是完全可重入的： 它没有任何全局变量。 （译注：从 C 语法上来说，也不尽然。例如，在 table 的实现中 用了一个静态全局变量 dummynode_ ，但这在正确使用时并不影响可重入性。 只是万一你错误链接了 lua 库，不小心在同一进程空间中存在两份 lua 库实现的代码的话， 多份 dummynode_ 不同的地址会导致一些问题。） 所有的信息都保存在这个结构中。

这个状态机的指针必须作为第一个参数传递给每一个库函数。 lua_newstate 是一个例外， 这个函数会从头创建一个 Lua 状态机。

### lua_status

    int lua_status (lua_State *L);

返回线程 L 的状态。
正常的线程状态是 0 。 当线程执行完毕或发生一个错误时，状态值是错误码。 如果线程被挂起，状态为 LUA_YIELD 。

### lua_toboolean

    int lua_toboolean (lua_State *L, int index);

把指定的索引处的的Lua值转换为一个C中的boolean值（ 0 或是 1 ）。 和 Lua 中做的所有测试一样， lua_toboolean 会把任何 不同于 false 和 nil 的值当作 1 返回； 否则就返回 0 。 如果用一个无效索引去调用也会返回 0 。 （如果你想只接收真正的 boolean 值，就需要使用 lua_isboolean 来测试值的类型。）

### lua_tocfunction
    
    lua_CFunction lua_tocfunction (lua_State *L, int index);

把给定索引处的 Lua 值转换为一个 C 函数。 这个值必须是一个 C 函数；如果不是就返回 NULL 。

### lua_tointeger

    lua_Integer lua_tointeger (lua_State *L, int idx);

把给定索引处的 Lua 值转换为 lua_Integer 这样一个有符号整数类型。 这个 Lua 值必须是一个数字或是一个可以转换为数字的字符串 （参见 §2.2.1）； 否则，lua_tointeger 返回 0 。
如果数字不是一个整数， 截断小数部分的方式没有被明确定义。

### lua_tolstring

    const char *lua_tolstring (lua_State *L, int index, size_t *len);

把给定索引处的 Lua 值转换为一个 C 字符串。 如果 len 不为 NULL ， 它还把字符串长度设到 *len 中。 这个 Lua 值必须是一个字符串或是一个数字； 否则返回返回 NULL 。 如果值是一个数字，lua_tolstring 还会把堆栈中的那个值的实际类型转换为一个字符串。 （当遍历一个表的时候，把 lua_tolstring 作用在键上，这个转换有可能导致 lua_next 弄错。）

lua_tolstring 返回 Lua 状态机中 字符串的以对齐指针。 这个字符串总能保证 （ C 要求的）最后一个字符为零 ('\0') ， 而且它允许在字符串内包含多个这样的零。 因为 Lua 中可能发生垃圾收集， 所以不保证 lua_tolstring 返回的指针， 在对应的值从堆栈中移除后依然有效。

### lua_tonumber

    lua_Number lua_tonumber (lua_State *L, int index);

把给定索引处的 Lua 值转换为 lua_Number 这样一个 C 类型（参见 lua_Number ）。 这个 Lua 值必须是一个数字或是一个可转换为数字的字符串 （参见 §2.2.1 ）； 否则，lua_tonumber 返回 0 。

### lua_topointer

    const void *lua_topointer (lua_State *L, int index);

把给定索引处的值转换为一般的 C 指针 (void*) 。 这个值可以是一个 userdata ，table ，thread 或是一个 function ； 否则，lua_topointer 返回 NULL 。 不同的对象有不同的指针。 不存在把指针再转回原有类型的方法。
这个函数通常只为产生 debug 信息用。

### lua_tostring

    const char *lua_tostring (lua_State *L, int index);

等价于 lua_tolstring ，而参数 len 设为 NULL 。

### lua_tothread

    lua_State *lua_tothread (lua_State *L, int index);

把给定索引处的值转换为一个 Lua 线程（由 lua_State* 代表）。 这个值必须是一个线程；否则函数返回 NULL 。

### lua_touserdata

    void *lua_touserdata (lua_State *L, int index);

如果给定索引处的值是一个完整的userdata ，函数返回内存块的地址。 如果值是一个 light userdata ，那么就返回它表示的指针。 否则，返回 NULL 。

### lua_type

    int lua_type (lua_State *L, int index);

返回给定索引处的值的类型， 当索引无效时则返回 LUA_TNONE （那是指一个指向堆栈上的空位置的索引）。 lua_type 返回的类型是一些个在 lua.h 中定义的常量： LUA_TNIL ， LUA_TNUMBER ， LUA_TBOOLEAN ， LUA_TSTRING ， LUA_TTABLE ， LUA_TFUNCTION ， LUA_TUSERDATA ， LUA_TTHREAD ， LUA_TLIGHTUSERDATA 。

### lua_typename

    const char *lua_typename  (lua_State *L, int tp);

返回 tp 表示的类型名， 这个 tp 必须是 lua_type 可能返回的值中之一。

### lua_Writer

typedef int (*lua_Writer) (lua_State *L, const void* p, size_t sz, void* ud);

由 lua_dump 用到的写入器函数。 每次 lua_dump 产生了一块新的 chunk ，它都会调用写入器。 传入要写入的缓存 (p) 和它的尺寸 (sz) ， 还有 lua_dump 的参数 data 。
写入器会返回一个错误码： 0 表示没有错误； 别的值均表示一个错误，并且会让 lua_dump 停止再次调用写入器。

### lua_xmove

    void lua_xmove (lua_State *from, lua_State *to, int n);

传递 同一个 全局状态机下不同线程中的值。
这个函数会从 from 的堆栈中弹出 n 个值， 然后把它们压入 to 的堆栈中。

### lua_yield

    int lua_yield  (lua_State *L, int nresults);

切出一个 coroutine 。
这个函数只能在一个 C 函数的返回表达式中调用。如下：

     return lua_yield (L, nresults);
当一个 C 函数这样调用 lua_yield ， 正在运行中的 coroutine 将从运行中挂起， 然后启动这个 coroutine 用的那次对 lua_resume 的调用就返回了。 参数 nresults 指的是堆栈中需要返回的结果个数，这些返回值将被传递给 lua_resume 。

3.8 - 调试接口
Lua 没有内建的调试设施。 取而代之的是提供了一些函数接口和钩子。 利用这些接口，可以做出一些不同类型的调试器， 性能分析器，或是其它一些需要从解释器中取到“内部信息”的工具。

### lua_Debug

    typedef struct lua_Debug 
    { 
        int event;
        const char *name;           /* (n) */
        const char *namewhat;       /* (n) */
        const char *what;           /* (S) */
        const char *source;         /* (S) */
        int currentline;            /* (l) */
        int nups;                   /* (u) upvalue 个数 */
        int linedefined;            /* (S) */
        int lastlinedefined;        /* (S) */
        char short_src[LUA_IDSIZE]; /* (S) */
        /* 私有部分 */
        其它域
    } lua_Debug;

一个用来携带活动中函数的各种信息的结构。 lua_getstack 仅填写这个结构中的私有部分， 这些部分以后会用到。 调用 lua_getinfo 则可以填上 lua_Debug 中有用信息的那些域。

lua_Debug 中的各个域有下列含义：

source: 如果函数是定义在一个字符串中，source 就是这个字符串。 如果函数定义在一个文件中， source 是一个以 '@' 开头的文件名。
short_src: 一个“可打印版本”的 source，用于出错信息。
linedefined: 函数定义开始处的行号。
lastlinedefined: 函数定义结束处的行号。
what: 如果函数是一个 Lua 函数，则为一个字符串 "Lua" ； 如果是一个 C 函数，则为 "C"； 如果它是一个 chunk 的主体部分，则为 "main"； 如果是一个作了尾调用的函数，则为 "tail" 。 别的情况下，Lua 没有关于函数的别的信息。
currentline: 给定函数正在执行的那一行。 当提供不了行号信息的时候，currentline 被设为 -1 。
name: 给定函数的一个合理的名字。 因为 Lua 中的函数也是一个值， 所以它们没有固定的名字： 一些函数可能是全局复合变量的值， 另一些可能仅仅只是被保存在一个 table 中。 lua_getinfo 函数会检查函数是这样被调用的，以此来找到一个适合的名字。 如果它找不到名字，name 就被设置为 NULL 。
namewhat: 结实 name 域。 namewhat 的值可以是 "global", "local", "method", "field", "upvalue", 或是 "" （空串）。 这取决于函数怎样被调用。 （Lua 用空串表示其它选项都不符合）
nups: 函数的 upvalue 的个数。

### lua_gethook

    lua_Hook lua_gethook (lua_State *L);

返回当前的钩子函数。

### lua_gethookcount

    int lua_gethookcount (lua_State *L);

返回当前钩子记数。

### lua_gethookmask

    int lua_gethookmask (lua_State *L);

返回当前的钩子掩码 (mask) 。

### lua_getinfo

    int lua_getinfo (lua_State *L, const char *what, lua_Debug *ar);

返回一个指定的函数或函数调用的信息。
当用于取得一次函数调用的信息时， 参数 ar 必须是一个有效的活动的记录。 这条记录可以是前一次调用 lua_getstack 得到的， 或是一个钩子 （参见 lua_Hook）得到的参数。
用于获取一个函数的信息时，可以把这个函数压入堆栈， 然后把 what 字符串以字符 '>' 起头。 （这个情况下，lua_getinfo 从栈顶上弹出函数。） 例如，想知道函数 f 在哪一行定义的， 你可以下下列代码：

    lua_Debug ar;
    lua_getfield(L, LUA_GLOBALSINDEX, "f");  /* 取到全局变量 'f' */
    lua_getinfo(L, ">S", &ar);
    printf("%d\n", ar.linedefined);

what 字符串中的每个字符都筛选出结构 ar 结构中一些域用于填充，或是把一个值压入堆栈：

'n': 填充 name 及 namewhat 域；
'S': 填充 source， short_src， linedefined， lastlinedefined，以及 what 域；
'l': 填充 currentline 域；
'u': 填充 nups 域；
'f': 把正在运行中指定级别处函数压入堆栈； （译注：一般用于获取函数调用中的信息， 级别是由 ar 中的私有部分来提供。 如果用于获取静态函数，那么就直接把指定函数重新压回堆栈， 但这样做通常无甚意义。）
'L': 压一个 table 入栈，这个 table 中的整数索引用于描述函数中哪些行是有效行。 （有效行指有实际代码的行， 即你可以置入断点的行。 无效行包括空行和只有注释的行。）
这个函数出错会返回 0 （例如，what 中有一个无效选项）。

### lua_getlocal

    const char *lua_getlocal (lua_State *L, lua_Debug *ar, int n);

从给定活动记录中获取一个局部变量的信息。 参数 ar 必须是一个有效的活动的记录。 这条记录可以是前一次调用 lua_getstack 得到的， 或是一个钩子 （参见 lua_Hook）得到的参数。 索引 n 用于选择要检阅哪个局部变量 （ 1 表示第一个参数或是激活的第一个局部变量，以此类推，直到最后一个局部变量）。 lua_getlocal 把变量的值压入堆栈并返回它的名字。
以 '(' （正小括号）开始的变量指内部变量 （循环控制变量，临时变量，C 函数局部变量）。
当索引大于局部变量的个数时，返回 NULL （什么也不压入）。

### lua_getstack

    int lua_getstack (lua_State *L, int level, lua_Debug *ar);

获取解释器的运行时栈的信息。
这个函数用正在运行中的给定级别处的函数的活动记录来填写 lua_Debug 结构的一部分。 0 级表示当前运行的函数， 而 n+1 级处的函数就是调用第 n 级函数的那一个。 如果没有错误，lua_getstack 返回 1 ； 当调用传入的级别大于堆栈深度的时候，返回 0 。

### lua_getupvalue

    const char *lua_getupvalue (lua_State *L, int funcindex, int n);

获取一个 closure 的 upvalue 信息。 （对于 Lua 函数，upvalue 是函数需要使用的外部局部变量， 因此这些变量被包含在 closure 中。） lua_getupvalue 获取第 n 个 upvalue ， 把这个 upvalue 的值压入堆栈，并且返回它的名字。 funcindex 指向堆栈上 closure 的位置。 （ 因为 upvalue 在整个函数中都有效，所以它们没有特别的次序。 因此，它们以字母次序来编号。）

当索引号比 upvalue 数量大的时候，返回 NULL （而且不会压入任何东西） 对于 C 函数，这个函数用空串 "" 表示所有 upvalue 的名字。

### lua_Hook

    typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);

用于调试的钩子函数类型。
无论何时钩子被调用，它的参数 ar 中的 event 域 都被设为触发钩子的事件。 Lua 把这些事件定义为以下常量： LUA_HOOKCALL， LUA_HOOKRET, LUA_HOOKTAILRET， LUA_HOOKLINE， and LUA_HOOKCOUNT。 除此之外，对于 line 事件，currentline 域也被设置。 要想获得 ar 中的其他域， 钩子必须调用 lua_getinfo。 对于返回事件，event 的正常值可能是 LUA_HOOKRET， 或者是 LUA_HOOKTAILRET 。 对于后一种情况，Lua 会对一个函数做的尾调用也模拟出一个返回事件出来； 对于这个模拟的返回事件，调用 lua_getinfo 没有什么作用。
当 Lua 运行在一个钩子内部时，它将屏蔽掉其它对钩子的调用。 也就是说，如果一个钩子函数内再调回 Lua 来执行一个函数或是一个 chunk ， 这个执行操作不会触发任何的钩子。

### lua_sethook

    int lua_sethook (lua_State *L, lua_Hook f, int mask, int count);

设置一个调试用钩子函数。

参数 f 是钩子函数。 mask 指定在哪些事件时会调用： 它由下列一组位常量构成 LUA_MASKCALL， LUA_MASKRET， LUA_MASKLINE， 以及 LUA_MASKCOUNT。 参数 count 只在 mask 中包含有 LUA_MASKCOUNT 才有意义。 对于每个事件，钩子被调用的情况解释如下：
call hook: 在解释器调用一个函数时被调用。 钩子将于 Lua 进入一个新函数后，函数获取参数前被调用。
return hook: 在解释器从一个函数中返回时调用。 钩子将于 Lua 离开函数之前的那一刻被调用。 你无权访问被函数返回出去的那些值。 （译注：原文 (You have no access to the values to be returned by the function) 如此。 但“无权访问”一词值得商榷。 某些情况下你可以访问到一些被命名为 (\*temporary) 的局部变量， 那些索引被排在最后的 (*temporary) 变量指的就是返回值。 但是由于 Lua 对特殊情况做了一些优化，比如直接返回一个被命名的局部变量， 那么就找不到对应的 (*temporary) 变量了。本质上，返回值一定存在于此刻的局部变量中， 并且可以访问它，只是无法确定是哪些罢了。至于这个时候函数体内的其它局部变量， 是不保证有效的。进入 return hook 的那一刻起，实际已经退出函数内部的运行环节， 返回值占用的局部变量空间以后的部分，都有可能因 hook 本身复用它们而改变。）
line hook: 在解释器准备开始执行新的一行代码时， 或是跳转到这行代码中时（即使在同一行内跳转）被调用。 （这个事件仅仅在 Lua 执行一个 Lua 函数时发生。）
count hook: 在解释器每执行 count 条指令后被调用。 （这个事件仅仅在 Lua 执行一个 Lua 函数时发生。）
钩子可以通过设置 mask 为零屏蔽。

### lua_setlocal

    const char *lua_setlocal (lua_State *L, lua_Debug *ar, int n);

设置给定活动记录中的局部变量的值。 参数 ar 与 n 和 lua_getlocal 中的一样 （参见 lua_getlocal）。 lua_setlocal 把栈顶的值赋给变量然后返回变量的名字。 它会将值从栈顶弹出。
当索引大于局部变量的个数时，返回 NULL （什么也不弹出）。

### lua_setupvalue

    const char *lua_setupvalue (lua_State *L, int funcindex, int n);

设置 closure 的 upvalue 的值。 它把栈顶的值弹出并赋于 upvalue 并返回 upvalue 的名字。 参数 funcindex 与 n 和 lua_getupvalue 中的一样 （参见 lua_getupvalue）。
当索引大于 upvalue 的个数时，返回 NULL （什么也不弹出）。

4 - The Auxiliary Library
The auxiliary library provides several convenient functions to interface C with Lua. While the basic API provides the primitive functions for all interactions between C and Lua, the auxiliary library provides higher-level functions for some common tasks.

All functions from the auxiliary library are defined in header file lauxlib.h and have a prefix luaL_.

All functions in the auxiliary library are built on top of the basic API, and so they provide nothing that cannot be done with this API.

Several functions in the auxiliary library are used to check C function arguments. Their names are always luaL_check* or luaL_opt\*. All of these functions raise an error if the check is not satisfied. Because the error message is formatted for arguments (e.g., "bad argument #1"), you should not use these functions for other stack values.

4.1 - Functions and Types
Here we list all functions and types from the auxiliary library in alphabetical order.

### luaL_addchar

    void luaL_addchar (luaL_Buffer *B, char c);

Adds the character c to the buffer B (see luaL_Buffer).

### luaL_addlstring

    void luaL_addlstring (luaL_Buffer *B, const char *s, size_t l);

Adds the string pointed to by s with length l to the buffer B (see luaL_Buffer). The string may contain embedded zeros.

### luaL_addsize

    void luaL_addsize (luaL_Buffer *B, size_t n);

Adds to the buffer B (see luaL_Buffer) a string of length n previously copied to the buffer area (see luaL_prepbuffer).

### luaL_addstring

    void luaL_addstring (luaL_Buffer *B, const char *s);

Adds the zero-terminated string pointed to by s to the buffer B (see luaL_Buffer). The string may not contain embedded zeros.

### luaL_addvalue

    void luaL_addvalue (luaL_Buffer *B);

Adds the value at the top of the stack to the buffer B (see luaL_Buffer). Pops the value.
This is the only function on string buffers that can (and must) be called with an extra element on the stack, which is the value to be added to the buffer.

### luaL_argcheck

    void luaL_argcheck (lua_State *L, int cond, int narg, const char *extramsg);

Checks whether cond is true. If not, raises an error with the following message, where func is retrieved from the call stack:

     bad argument #<narg> to <func> (<extramsg>)

### luaL_argerror

    int luaL_argerror (lua_State *L, int narg, const char *extramsg);

Raises an error with the following message, where func is retrieved from the call stack:

     bad argument #<narg> to <func> (<extramsg>)

This function never returns, but it is an idiom to use it in C functions as return luaL_argerror(args).

### luaL_Buffer

typedef struct luaL_Buffer luaL_Buffer;
Type for a string buffer.
A string buffer allows C code to build Lua strings piecemeal. Its pattern of use is as follows:
First you declare a variable b of type luaL_Buffer.
Then you initialize it with a call luaL_buffinit(L, &b).
Then you add string pieces to the buffer calling any of the luaL_add* functions.
You finish by calling luaL_pushresult(&b). This call leaves the final string on the top of the stack.
During its normal operation, a string buffer uses a variable number of stack slots. So, while using a buffer, you cannot assume that you know where the top of the stack is. You can use the stack between successive calls to buffer operations as long as that use is balanced; that is, when you call a buffer operation, the stack is at the same level it was immediately after the previous buffer operation. (The only exception to this rule is luaL_addvalue.) After calling luaL_pushresult the stack is back to its level when the buffer was initialized, plus the final string on its top.

### luaL_buffinit

    void luaL_buffinit (lua_State *L, luaL_Buffer *B);

Initializes a buffer B. This function does not allocate any space; the buffer must be declared as a variable (see luaL_Buffer).

### luaL_callmeta

    int luaL_callmeta (lua_State *L, int obj, const char *e);

Calls a metamethod.
If the object at index obj has a metatable and this metatable has a field e, this function calls this field and passes the object as its only argument. In this case this function returns 1 and pushes onto the stack the value returned by the call. If there is no metatable or no metamethod, this function returns 0 (without pushing any value on the stack).

### luaL_checkany

    void luaL_checkany (lua_State *L, int narg);

Checks whether the function has an argument of any type (including nil) at position narg.

### luaL_checkint

    int luaL_checkint (lua_State *L, int narg);

Checks whether the function argument narg is a number and returns this number cast to an int.

### luaL_checkinteger

    lua_Integer luaL_checkinteger (lua_State *L, int narg);

Checks whether the function argument narg is a number and returns this number cast to a lua_Integer.

### luaL_checklong

    long luaL_checklong (lua_State *L, int narg);

Checks whether the function argument narg is a number and returns this number cast to a long.

### luaL_checklstring

    const char *luaL_checklstring (lua_State *L, int narg, size_t *l);

Checks whether the function argument narg is a string and returns this string; if l is not NULL fills \*l with the string's length.

luaL_checknumber
lua_Number luaL_checknumber (lua_State *L, int narg);
Checks whether the function argument narg is a number and returns this number.

### luaL_checkoption

    int luaL_checkoption (lua_State *L, int narg, const char *def, const char *const lst[]);

Checks whether the function argument narg is a string and searches for this string in the array lst (which must be NULL-terminated). Returns the index in the array where the string was found. Raises an error if the argument is not a string or if the string cannot be found.
If def is not NULL, the function uses def as a default value when there is no argument narg or if this argument is nil.
This is a useful function for mapping strings to C enums. (The usual convention in Lua libraries is to use strings instead of numbers to select options.)

luaL_checkstack
void luaL_checkstack (lua_State *L, int sz, const char *msg);
Grows the stack size to top + sz elements, raising an error if the stack cannot grow to that size. msg is an additional text to go into the error message.

luaL_checkstring
const char *luaL_checkstring (lua_State *L, int narg);
Checks whether the function argument narg is a string and returns this string.

luaL_checktype
void luaL_checktype (lua_State *L, int narg, int t);
Checks whether the function argument narg has type t.

luaL_checkudata
void *luaL_checkudata (lua_State *L, int narg, const char *tname);
Checks whether the function argument narg is a userdata of the type tname (see luaL_newmetatable).

luaL_dofile
int luaL_dofile (lua_State *L, const char *filename);
Loads and runs the given file. It is defined as the following macro:

     (luaL_loadfile(L, filename) || lua_pcall(L, 0, LUA_MULTRET, 0))
It returns 0 if there are no errors or 1 in case of errors.

luaL_dostring
int luaL_dostring (lua_State *L, const char *str);
Loads and runs the given string. It is defined as the following macro:

     (luaL_loadstring(L, str) || lua_pcall(L, 0, LUA_MULTRET, 0))

It returns 0 if there are no errors or 1 in case of errors.

### luaL_error

    int luaL_error (lua_State *L, const char *fmt, ...);

Raises an error. The error message format is given by fmt plus any extra arguments, following the same rules of lua_pushfstring. It also adds at the beginning of the message the file name and the line number where the error occurred, if this information is available.
This function never returns, but it is an idiom to use it in C functions as return luaL_error(args).

### luaL_getmetafield

    int luaL_getmetafield (lua_State *L, int obj, const char *e);

Pushes onto the stack the field e from the metatable of the object at index obj. If the object does not have a metatable, or if the metatable does not have this field, returns 0 and pushes nothing.

### luaL_getmetatable

    void luaL_getmetatable (lua_State *L, const char *tname);

Pushes onto the stack the metatable associated with name tname in the registry (see luaL_newmetatable).

### luaL_gsub

    const char *luaL_gsub (lua_State *L, const char *s, const char *p, const char *r);

Creates a copy of string s by replacing any occurrence of the string p with the string r. Pushes the resulting string on the stack and returns it.

### luaL_loadbuffer

    int luaL_loadbuffer (lua_State *L, const char *buff, size_t sz, const char *name);

Loads a buffer as a Lua chunk. This function uses lua_load to load the chunk in the buffer pointed to by buff with size sz.
This function returns the same results as lua_load. name is the chunk name, used for debug information and error messages.

### luaL_loadfile

    int luaL_loadfile (lua_State *L, const char *filename);

Loads a file as a Lua chunk. This function uses lua_load to load the chunk in the file named filename. If filename is NULL, then it loads from the standard input. The first line in the file is ignored if it starts with a #.
This function returns the same results as lua_load, but it has an extra error code LUA_ERRFILE if it cannot open/read the file.
As lua_load, this function only loads the chunk; it does not run it.

### luaL_loadstring

    int luaL_loadstring (lua_State *L, const char *s);

Loads a string as a Lua chunk. This function uses lua_load to load the chunk in the zero-terminated string s.
This function returns the same results as lua_load.
Also as lua_load, this function only loads the chunk; it does not run it.

### luaL_newmetatable

    int luaL_newmetatable (lua_State *L, const char *tname);

If the registry already has the key tname, returns 0. Otherwise, creates a new table to be used as a metatable for userdata, adds it to the registry with key tname, and returns 1.
In both cases pushes onto the stack the final value associated with tname in the registry.

### luaL_newstate

    lua_State *luaL_newstate (void);

Creates a new Lua state. It calls lua_newstate with an allocator based on the standard C realloc function and then sets a panic function (see lua_atpanic) that prints an error message to the standard error output in case of fatal errors.
Returns the new state, or NULL if there is a memory allocation error.

### luaL_openlibs

    void luaL_openlibs (lua_State *L);

Opens all standard Lua libraries into the given state.

luaL_optint
int luaL_optint (lua_State *L, int narg, int d);
If the function argument narg is a number, returns this number cast to an int. If this argument is absent or is nil, returns d. Otherwise, raises an error.

### luaL_optinteger
    lua_Integer luaL_optinteger (lua_State *L,
                             int narg,
                             lua_Integer d);
If the function argument narg is a number, returns this number cast to a lua_Integer. If this argument is absent or is nil, returns d. Otherwise, raises an error.

### luaL_optlong
    long luaL_optlong (lua_State *L, int narg, long d);
If the function argument narg is a number, returns this number cast to a long. If this argument is absent or is nil, returns d. Otherwise, raises an error.

### luaL_optlstring

    const char *luaL_optlstring (lua_State *L, int narg, const char *d, size_t *l);

If the function argument narg is a string, returns this string. If this argument is absent or is nil, returns d. Otherwise, raises an error.
If l is not NULL, fills the position *l with the results's length.

### luaL_optnumber

    lua_Number luaL_optnumber (lua_State *L, int narg, lua_Number d);

If the function argument narg is a number, returns this number. If this argument is absent or is nil, returns d. Otherwise, raises an error.

### luaL_optstring

    const char *luaL_optstring (lua_State *L, int narg, const char *d);

If the function argument narg is a string, returns this string. If this argument is absent or is nil, returns d. Otherwise, raises an error.

### luaL_prepbuffer

    char *luaL_prepbuffer (luaL_Buffer *B);

Returns an address to a space of size LUAL_BUFFERSIZE where you can copy a string to be added to buffer B (see luaL_Buffer). After copying the string into this space you must call luaL_addsize with the size of the string to actually add it to the buffer.

### luaL_pushresult

    void luaL_pushresult (luaL_Buffer *B);

Finishes the use of buffer B leaving the final string on the top of the stack.

### luaL_ref

    int luaL_ref (lua_State *L, int t);

Creates and returns a reference, in the table at index t, for the object at the top of the stack (and pops the object).
A reference is a unique integer key. As long as you do not manually add integer keys into table t, luaL_ref ensures the uniqueness of the key it returns. You can retrieve an object referred by reference r by calling lua_rawgeti(L, t, r). Function luaL_unref frees a reference and its associated object.

If the object at the top of the stack is nil, luaL_ref returns the constant LUA_REFNIL. The constant LUA_NOREF is guaranteed to be different from any reference returned by luaL_ref.

### luaL_Reg

    typedef struct luaL_Reg {  const char *name;  lua_CFunction func;} luaL_Reg;

Type for arrays of functions to be registered by luaL_register. name is the function name and func is a pointer to the function. Any array of luaL_Reg must end with an sentinel entry in which both name and func are NULL.

### luaL_register

    void luaL_register (lua_State *L, const char *libname, const luaL_Reg *l);

Opens a library.

When called with libname equal to NULL, it simply registers all functions in the list l (see luaL_Reg) into the table on the top of the stack.
When called with a non-null libname, luaL_register creates a new table t, sets it as the value of the global variable libname, sets it as the value of package.loaded[libname], and registers on it all functions in the list l. If there is a table in package.loaded[libname] or in variable libname, reuses this table instead of creating a new one.
In any case the function leaves the table on the top of the stack.

```lua
static int foo(lua_State *L)
{
    lua_pushnumber(L, 1);
    return 1;
}

static const struct lua_Reg modulename[] = {
    {"foo", foo},
    {NULL, NULL},
};

int luaopen_modulename(lua_State *L)
{
    luaL_newlib(L, modulename);// 5.2之前使用的Lua_register(L, "modulename", modulename);
    return 1;
}
```
工程目录 https://github.com/metanqy/binary_world.git
### luaL_typename

const char *luaL_typename (lua_State *L, int idx);
Returns the name of the type of the value at index idx.

luaL_typerror
int luaL_typerror (lua_State *L, int narg, const char *tname);
Generates an error with a message like the following:

     location: bad argument narg to 'func' (tname expected, got rt)
where location is produced by luaL_where, func is the name of the current function, and rt is the type name of the actual argument.

### luaL_unref

    void luaL_unref (lua_State *L, int t, int ref);

Releases reference ref from the table at index t (see luaL_ref). The entry is removed from the table, so that the referred object can be collected. The reference ref is also freed to be used again.

If ref is LUA_NOREF or LUA_REFNIL, luaL_unref does nothing.

### luaL_where

    void luaL_where (lua_State *L, int lvl);

Pushes onto the stack a string identifying the current position of the control at level lvl in the call stack. Typically this string has the following format:

     chunkname:currentline:
Level 0 is the running function, level 1 is the function that called the running function, etc.
This function is used to build a prefix for error messages.

Standard Libraries
The standard Lua libraries provide useful functions that are implemented directly through the C API. Some of these functions provide essential services to the language (e.g., type and getmetatable); others provide access to "outside" services (e.g., I/O); and others could be implemented in Lua itself, but are quite useful or have critical performance requirements that deserve an implementation in C (e.g., sort).

All libraries are implemented through the official C API and are provided as separate C modules. Currently, Lua has the following standard libraries:

basic library;
package library;
string manipulation;
table manipulation;
mathematical functions (sin, log, etc.);
input and output;
operating system facilities;
debug facilities.
Except for the basic and package libraries, each library provides all its functions as fields of a global table or as methods of its objects.

To have access to these libraries, the C host program should call the luaL_openlibs function, which opens all standard libraries. Alternatively, it can open them individually by calling luaopen_base (for the basic library), luaopen_package (for the package library), luaopen_string (for the string library), luaopen_table (for the table library), luaopen_math (for the mathematical library), luaopen_io (for the I/O and the Operating System libraries), and luaopen_debug (for the debug library). These functions are declared in lualib.h and should not be called directly: you must call them like any other Lua C function, e.g., by using lua_call.

The basic library provides some core functions to Lua. If you do not include this library in your application, you should check carefully whether you need to provide implementations for some of its facilities.

### assert (v [, message])

Issues an error when the value of its argument v is false (i.e., nil or false); otherwise, returns all its arguments. message is an error message; when absent, it defaults to "assertion failed!"
collectgarbage (opt [, arg])
This function is a generic interface to the garbage collector. It performs different functions according to its first argument, opt:

"stop": stops the garbage collector.
"restart": restarts the garbage collector.
"collect": performs a full garbage-collection cycle.
"count": returns the total memory in use by Lua (in Kbytes).
"step": performs a garbage-collection step. The step "size" is controlled by arg (larger values mean more steps) in a non-specified way. If you want to control the step size you must experimentally tune the value of arg. Returns true if the step finished a collection cycle.
"setpause": sets arg/100 as the new value for the pause of the collector (see §2.10).
"setstepmul": sets arg/100 as the new value for the step multiplier of the collector (see §2.10).
dofile (filename)
Opens the named file and executes its contents as a Lua chunk. When called without arguments, dofile executes the contents of the standard input (stdin). Returns all values returned by the chunk. In case of errors, dofile propagates the error to its caller (that is, dofile does not run in protected mode).
error (message [, level])
Terminates the last protected function called and returns message as the error message. Function error never returns.
Usually, error adds some information about the error position at the beginning of the message. The level argument specifies how to get the error position. With level 1 (the default), the error position is where the error function was called. Level 2 points the error to where the function that called error was called; and so on. Passing a level 0 avoids the addition of error position information to the message.

_G
A global variable (not a function) that holds the global environment (that is, _G._G = _G). Lua itself does not use this variable; changing its value does not affect any environment, nor vice-versa. (Use setfenv to change environments.)
getfenv (f)
Returns the current environment in use by the function. f can be a Lua function or a number that specifies the function at that stack level: Level 1 is the function calling getfenv. If the given function is not a Lua function, or if f is 0, getfenv returns the global environment. The default for f is 1.
getmetatable (object)
If object does not have a metatable, returns nil. Otherwise, if the object's metatable has a "__metatable" field, returns the associated value. Otherwise, returns the metatable of the given object.

ipairs (t)
Returns three values: an iterator function, the table t, and 0, so that the construction

     for i,v in ipairs(t) do body end
will iterate over the pairs (1,t[1]), (2,t[2]), ···, up to the first integer key absent from the table.

load (func [, chunkname])
Loads a chunk using function func to get its pieces. Each call to func must return a string that concatenates with previous results. A return of nil (or no value) signals the end of the chunk.

If there are no errors, returns the compiled chunk as a function; otherwise, returns nil plus the error message. The environment of the returned function is the global environment.

chunkname is used as the chunk name for error messages and debug information.

loadfile ([filename])
Similar to load, but gets the chunk from file filename or from the standard input, if no file name is given.

loadstring (string [, chunkname])
Similar to load, but gets the chunk from the given string.

To load and run a given string, use the idiom

     assert(loadstring(s))()
next (table [, index])
Allows a program to traverse all fields of a table. Its first argument is a table and its second argument is an index in this table. next returns the next index of the table and its associated value. When called with nil as its second argument, next returns an initial index and its associated value. When called with the last index, or with nil in an empty table, next returns nil. If the second argument is absent, then it is interpreted as nil. In particular, you can use next(t) to check whether a table is empty.

The order in which the indices are enumerated is not specified, even for numeric indices. (To traverse a table in numeric order, use a numerical for or the ipairs function.)

The behavior of next is undefined if, during the traversal, you assign any value to a non-existent field in the table. You may however modify existing fields. In particular, you may clear existing fields.

### pairs (t)

Returns three values: the next function, the table t, and nil, so that the construction

     for k,v in pairs(t) do body end

will iterate over all key–value pairs of table t.

See function next for the caveats of modifying the table during its traversal.

### pcall (f, arg1, ···)

Calls function f with the given arguments in protected mode. This means that any error inside f is not propagated; instead, pcall catches the error and returns a status code. Its first result is the status code (a boolean), which is true if the call succeeds without errors. In such case, pcall also returns all results from the call, after this first result. In case of any error, pcall returns false plus the error message.

### print (···)

Receives any number of arguments, and prints their values to stdout, using the tostring function to convert them to strings. print is not intended for formatted output, but only as a quick way to show a value, typically for debugging. For formatted output, use string.format.
rawequal (v1, v2)
Checks whether v1 is equal to v2, without invoking any metamethod. Returns a boolean.
rawget (table, index)
Gets the real value of table[index], without invoking any metamethod. table must be a table; index may be any value.
rawset (table, index, value)
Sets the real value of table[index] to value, without invoking any metamethod. table must be a table, index any value different from nil, and value any Lua value.
This function returns table.

select (index, ···)
If index is a number, returns all arguments after argument number index. Otherwise, index must be the string "#", and select returns the total number of extra arguments it received.

setfenv (f, table)
Sets the environment to be used by the given function. f can be a Lua function or a number that specifies the function at that stack level: Level 1 is the function calling setfenv. setfenv returns the given function.

As a special case, when f is 0 setfenv changes the environment of the running thread. In this case, setfenv returns no values.

setmetatable (table, metatable)
Sets the metatable for the given table. (You cannot change the metatable of other types from Lua, only from C.) If metatable is nil, removes the metatable of the given table. If the original metatable has a "__metatable" field, raises an error.

This function returns table.

tonumber (e [, base])
Tries to convert its argument to a number. If the argument is already a number or a string convertible to a number, then tonumber returns this number; otherwise, it returns nil.
An optional argument specifies the base to interpret the numeral. The base may be any integer between 2 and 36, inclusive. In bases above 10, the letter 'A' (in either upper or lower case) represents 10, 'B' represents 11, and so forth, with 'Z' representing 35. In base 10 (the default), the number may have a decimal part, as well as an optional exponent part (see §2.1). In other bases, only unsigned integers are accepted.

tostring (e)
Receives an argument of any type and converts it to a string in a reasonable format. For complete control of how numbers are converted, use string.format.
If the metatable of e has a "__tostring" field, then tostring calls the corresponding value with e as argument, and uses the result of the call as its result.

type (v)
Returns the type of its only argument, coded as a string. The possible results of this function are "nil" (a string, not the value nil), "number", "string", "boolean", "table", "function", "thread", and "userdata".
unpack (list [, i [, j]])
Returns the elements from the given table. This function is equivalent to
     return list[i], list[i+1], ···, list[j]
except that the above code can be written only for a fixed number of elements. By default, i is 1 and j is the length of the list, as defined by the length operator (see §2.5.5).

_VERSION
A global variable (not a function) that holds a string containing the current interpreter version. The current contents of this variable is "Lua 5.1".
xpcall (f, err)
This function is similar to pcall, except that you can set a new error handler.

xpcall calls function f in protected mode, using err as the error handler. Any error inside f is not propagated; instead, xpcall catches the error, calls the err function with the original error object, and returns a status code. Its first result is the status code (a boolean), which is true if the call succeeds without errors. In this case, xpcall also returns all results from the call, after this first result. In case of any error, xpcall returns false plus the result from err.

### Coroutine Manipulation

The operations related to coroutines comprise a sub-library of the basic library and come inside the table coroutine. 

coroutine.create (f)
Creates a new coroutine, with body f. f must be a Lua function. Returns this new coroutine, an object with type "thread".

coroutine.resume (co [, val1, ···])
Starts or continues the execution of coroutine co. The first time you resume a coroutine, it starts running its body. The values val1, ··· are passed as the arguments to the body function. If the coroutine has yielded, resume restarts it; the values val1, ··· are passed as the results from the yield.

If the coroutine runs without any errors, resume returns true plus any values passed to yield (if the coroutine yields) or any values returned by the body function (if the coroutine terminates). If there is any error, resume returns false plus the error message.

coroutine.running ()
Returns the running coroutine, or nil when called by the main thread.

coroutine.status (co)
Returns the status of coroutine co, as a string: "running", if the coroutine is running (that is, it called status); "suspended", if the coroutine is suspended in a call to yield, or if it has not started running yet; "normal" if the coroutine is active but not running (that is, it has resumed another coroutine); and "dead" if the coroutine has finished its body function, or if it has stopped with an error.

coroutine.wrap (f)
Creates a new coroutine, with body f. f must be a Lua function. Returns a function that resumes the coroutine each time it is called. Any arguments passed to the function behave as the extra arguments to resume. Returns the same values returned by resume, except the first boolean. In case of error, propagates the error.

coroutine.yield (···)
Suspends the execution of the calling coroutine. The coroutine cannot be running a C function, a metamethod, or an iterator. Any arguments to yield are passed as extra results to resume.

### Modules

The package library provides basic facilities for loading and building modules in Lua. It exports two of its functions directly in the global environment: require and module. Everything else is exported in a table package.
module (name [, ···])
Creates a module. If there is a table in package.loaded[name], this table is the module. Otherwise, if there is a global table t with the given name, this table is the module. Otherwise creates a new table t and sets it as the value of the global name and the value of package.loaded[name]. This function also initializes t._NAME with the given name, t._M with the module (t itself), and t._PACKAGE with the package name (the full module name minus last component; see below). Finally, module sets t as the new environment of the current function and the new value of package.loaded[name], so that require returns t.

If name is a compound name (that is, one with components separated by dots), module creates (or reuses, if they already exist) tables for each component. For instance, if name is a.b.c, then module stores the module table in field c of field b of global a.

This function may receive optional options after the module name, where each option is a function to be applied over the module.

### require (modname)

Loads the given module. The function starts by looking into the package.loaded table to determine whether modname is already loaded. If it is, then require returns the value stored at package.loaded[modname]. Otherwise, it tries to find a loader for the module.

To find a loader, first require queries package.preload[modname]. If it has a value, this value (which should be a function) is the loader. Otherwise require searches for a Lua loader using the path stored in package.path. If that also fails, it searches for a C loader using the path stored in package.cpath. If that also fails, it tries an all-in-one loader (see below).

When loading a C library, require first uses a dynamic link facility to link the application with the library. Then it tries to find a C function inside this library to be used as the loader. The name of this C function is the string "luaopen_" concatenated with a copy of the module name where each dot is replaced by an underscore. Moreover, if the module name has a hyphen, its prefix up to (and including) the first hyphen is removed. For instance, if the module name is a.v1-b.c, the function name will be luaopen_b_c.

If require finds neither a Lua library nor a C library for a module, it calls the all-in-one loader. This loader searches the C path for a library for the root name of the given module. For instance, when requiring a.b.c, it will search for a C library for a. If found, it looks into it for an open function for the submodule; in our example, that would be luaopen_a_b_c. With this facility, a package can pack several C submodules into one single library, with each submodule keeping its original open function.

Once a loader is found, require calls the loader with a single argument, modname. If the loader returns any value, require assigns the returned value to package.loaded[modname]. If the loader returns no value and has not assigned any value to package.loaded[modname], then require assigns true to this entry. In any case, require returns the final value of package.loaded[modname].

If there is any error loading or running the module, or if it cannot find any loader for the module, then require signals an error.

package.cpath
The path used by require to search for a C loader.

Lua initializes the C path package.cpath in the same way it initializes the Lua path package.path, using the environment variable LUA_CPATH (plus another default path defined in luaconf.h).

package.loaded
A table used by require to control which modules are already loaded. When you require a module modname and package.loaded[modname] is not false, require simply returns the value stored there.

package.loadlib (libname, funcname)
Dynamically links the host program with the C library libname. Inside this library, looks for a function funcname and returns this function as a C function. (So, funcname must follow the protocol (see lua_CFunction)).

This is a low-level function. It completely bypasses the package and module system. Unlike require, it does not perform any path searching and does not automatically adds extensions. libname must be the complete file name of the C library, including if necessary a path and extension. funcname must be the exact name exported by the C library (which may depend on the C compiler and linker used).

This function is not supported by ANSI C. As such, it is only available on some platforms (Windows, Linux, Mac OS X, Solaris, BSD, plus other Unix systems that support the dlfcn standard).

package.path
The path used by require to search for a Lua loader.

At start-up, Lua initializes this variable with the value of the environment variable LUA_PATH or with a default path defined in luaconf.h, if the environment variable is not defined. Any ";;" in the value of the environment variable is replaced by the default path.

A path is a sequence of templates separated by semicolons. For each template, require will change each interrogation mark in the template by filename, which is modname with each dot replaced by a "directory separator" (such as "/" in Unix); then it will try to load the resulting file name. So, for instance, if the Lua path is

     "./?.lua;./?.lc;/usr/local/?/init.lua"
the search for a Lua loader for module foo will try to load the files ./foo.lua, ./foo.lc, and /usr/local/foo/init.lua, in that order.

package.preload
A table to store loaders for specific modules (see require).

package.seeall (module)
Sets a metatable for module with its __index field referring to the global environment, so that this module inherits values from the global environment. To be used as an option to function module.

5.4 - String Manipulation
This library provides generic functions for string manipulation, such as finding and extracting substrings, and pattern matching. When indexing a string in Lua, the first character is at position 1 (not at 0, as in C). Indices are allowed to be negative and are interpreted as indexing backwards, from the end of the string. Thus, the last character is at position -1, and so on.

The string library provides all its functions inside the table string. It also sets a metatable for strings where the __index field points to the string table. Therefore, you can use the string functions in object-oriented style. For instance, string.byte(s, i) can be written as s:byte(i).

string.byte (s [, i [, j]])
Returns the internal numerical codes of the characters s[i], s[i+1], ···, s[j]. The default value for i is 1; the default value for j is i.
Note that numerical codes are not necessarily portable across platforms.

string.char (···)
Receives zero or more integers. Returns a string with length equal to the number of arguments, in which each character has the internal numerical code equal to its corresponding argument.
Note that numerical codes are not necessarily portable across platforms.

string.dump (function)
Returns a string containing a binary representation of the given function, so that a later loadstring on this string returns a copy of the function. function must be a Lua function without upvalues.

string.find (s, pattern [, init [, plain]])
Looks for the first match of pattern in the string s. If it finds a match, then find returns the indices of s where this occurrence starts and ends; otherwise, it returns nil. A third, optional numerical argument init specifies where to start the search; its default value is 1 and may be negative. A value of true as a fourth, optional argument plain turns off the pattern matching facilities, so the function does a plain "find substring" operation, with no characters in pattern being considered "magic". Note that if plain is given, then init must be given as well.
If the pattern has captures, then in a successful match the captured values are also returned, after the two indices.

string.format (formatstring, ···)
Returns a formatted version of its variable number of arguments following the description given in its first argument (which must be a string). The format string follows the same rules as the printf family of standard C functions. The only differences are that the options/modifiers *, l, L, n, p, and h are not supported and that there is an extra option, q. The q option formats a string in a form suitable to be safely read back by the Lua interpreter: the string is written between double quotes, and all double quotes, newlines, embedded zeros, and backslashes in the string are correctly escaped when written. For instance, the call
     string.format('%q', 'a string with "quotes" and \n new line')
will produce the string:

     "a string with \"quotes\" and \
      new line"
The options c, d, E, e, f, g, G, i, o, u, X, and x all expect a number as argument, whereas q and s expect a string.

This function does not accept string values containing embedded zeros.

string.gmatch (s, pattern)
Returns an iterator function that, each time it is called, returns the next captures from pattern over string s.
If pattern specifies no captures, then the whole match is produced in each call.

As an example, the following loop

     s = "hello world from Lua"
     for w in string.gmatch(s, "%a+") do
       print(w)
     end
will iterate over all the words from string s, printing one per line. The next example collects all pairs key=value from the given string into a table:

     t = {}
     s = "from=world, to=Lua"
     for k, v in string.gmatch(s, "(%w+)=(%w+)") do
       t[k] = v
     end
string.gsub (s, pattern, repl [, n])
Returns a copy of s in which all occurrences of the pattern have been replaced by a replacement string specified by repl, which may be a string, a table, or a function. gsub also returns, as its second value, the total number of substitutions made.
If repl is a string, then its value is used for replacement. The character % works as an escape character: any sequence in repl of the form %n, with n between 1 and 9, stands for the value of the n-th captured substring (see below). The sequence %0 stands for the whole match. The sequence %% stands for a single %.

If repl is a table, then the table is queried for every match, using the first capture as the key; if the pattern specifies no captures, then the whole match is used as the key.

If repl is a function, then this function is called every time a match occurs, with all captured substrings passed as arguments, in order; if the pattern specifies no captures, then the whole match is passed as a sole argument.

If the value returned by the table query or by the function call is a string or a number, then it is used as the replacement string; otherwise, if it is false or nil, then there is no replacement (that is, the original match is kept in the string).

The optional last parameter n limits the maximum number of substitutions to occur. For instance, when n is 1 only the first occurrence of pattern is replaced.

Here are some examples:

     x = string.gsub("hello world", "(%w+)", "%1 %1")
     --> x="hello hello world world"
     
     x = string.gsub("hello world", "%w+", "%0 %0", 1)
     --> x="hello hello world"
     
     x = string.gsub("hello world from Lua", "(%w+)%s*(%w+)", "%2 %1")
     --> x="world hello Lua from"
     
     x = string.gsub("home = $HOME, user = $USER", "%$(%w+)", os.getenv)
     --> x="home = /home/roberto, user = roberto"
     
     x = string.gsub("4+5 = $return 4+5$", "%$(.-)%$", function (s)
           return loadstring(s)()
         end)
     --> x="4+5 = 9"
     
     local t = {name="lua", version="5.1"}
     x = string.gsub("$name%-$version.tar.gz", "%$(%w+)", t)
     --> x="lua-5.1.tar.gz"
string.len (s)
Receives a string and returns its length. The empty string "" has length 0. Embedded zeros are counted, so "a\000bc\000" has length 5.
string.lower (s)
Receives a string and returns a copy of this string with all uppercase letters changed to lowercase. All other characters are left unchanged. The definition of what an uppercase letter is depends on the current locale.
string.match (s, pattern [, init])
Looks for the first match of pattern in the string s. If it finds one, then match returns the captures from the pattern; otherwise it returns nil. If pattern specifies no captures, then the whole match is returned. A third, optional numerical argument init specifies where to start the search; its default value is 1 and may be negative.
string.rep (s, n)
Returns a string that is the concatenation of n copies of the string s.
string.reverse (s)
Returns a string that is the string s reversed.
string.sub (s, i [, j])
Returns the substring of s that starts at i and continues until j; i and j may be negative. If j is absent, then it is assumed to be equal to -1 (which is the same as the string length). In particular, the call string.sub(s,1,j) returns a prefix of s with length j, and string.sub(s, -i) returns a suffix of s with length i.
string.upper (s)
Receives a string and returns a copy of this string with all lowercase letters changed to uppercase. All other characters are left unchanged. The definition of what a lowercase letter is depends on the current locale.
5.4.1 - Patterns
Character Class:
A character class is used to represent a set of characters. The following combinations are allowed in describing a character class:

x: (where x is not one of the magic characters ^$()%.[]*+-?) represents the character x itself.
.: (a dot) represents all characters.
%a: represents all letters.
%c: represents all control characters.
%d: represents all digits.
%l: represents all lowercase letters.
%p: represents all punctuation characters.
%s: represents all space characters.
%u: represents all uppercase letters.
%w: represents all alphanumeric characters.
%x: represents all hexadecimal digits.
%z: represents the character with representation 0.
%x: (where x is any non-alphanumeric character) represents the character x. This is the standard way to escape the magic characters. Any punctuation character (even the non magic) can be preceded by a '%' when used to represent itself in a pattern.
[set]: represents the class which is the union of all characters in set. A range of characters may be specified by separating the end characters of the range with a '-'. All classes %x described above may also be used as components in set. All other characters in set represent themselves. For example, [%w_] (or [_%w]) represents all alphanumeric characters plus the underscore, [0-7] represents the octal digits, and [0-7%l%-] represents the octal digits plus the lowercase letters plus the '-' character.
The interaction between ranges and classes is not defined. Therefore, patterns like [%a-z] or [a-%%] have no meaning.

[^set]: represents the complement of set, where set is interpreted as above.
For all classes represented by single letters (%a, %c, etc.), the corresponding uppercase letter represents the complement of the class. For instance, %S represents all non-space characters.

The definitions of letter, space, and other character groups depend on the current locale. In particular, the class [a-z] may not be equivalent to %l.

Pattern Item:
A pattern item may be

a single character class, which matches any single character in the class;
a single character class followed by '*', which matches 0 or more repetitions of characters in the class. These repetition items will always match the longest possible sequence;
a single character class followed by '+', which matches 1 or more repetitions of characters in the class. These repetition items will always match the longest possible sequence;
a single character class followed by '-', which also matches 0 or more repetitions of characters in the class. Unlike '*', these repetition items will always match the shortest possible sequence;
a single character class followed by '?', which matches 0 or 1 occurrence of a character in the class;
%n, for n between 1 and 9; such item matches a substring equal to the n-th captured string (see below);
%bxy, where x and y are two distinct characters; such item matches strings that start with x, end with y, and where the x and y are balanced. This means that, if one reads the string from left to right, counting +1 for an x and -1 for a y, the ending y is the first y where the count reaches 0. For instance, the item %b() matches expressions with balanced parentheses.
Pattern:
A pattern is a sequence of pattern items. A '^' at the beginning of a pattern anchors the match at the beginning of the subject string. A '$' at the end of a pattern anchors the match at the end of the subject string. At other positions, '^' and '$' have no special meaning and represent themselves.

Captures:
A pattern may contain sub-patterns enclosed in parentheses; they describe captures. When a match succeeds, the substrings of the subject string that match captures are stored (captured) for future use. Captures are numbered according to their left parentheses. For instance, in the pattern "(a*(.)%w(%s*))", the part of the string matching "a*(.)%w(%s*)" is stored as the first capture (and therefore has number 1); the character matching "." is captured with number 2, and the part matching "%s*" has number 3.

As a special case, the empty capture () captures the current string position (a number). For instance, if we apply the pattern "()aa()" on the string "flaaap", there will be two captures: 3 and 5.

A pattern cannot contain embedded zeros. Use %z instead.

5.5 - Table Manipulation
This library provides generic functions for table manipulation. It provides all its functions inside the table table.

Most functions in the table library assume that the table represents an array or a list. For these functions, when we talk about the "length" of a table we mean the result of the length operator.

table.concat (table [, sep [, i [, j]]])
Given an array where all elements are strings or numbers, returns table[i]..sep..table[i+1] ··· sep..table[j]. The default value for sep is the empty string, the default for i is 1, and the default for j is the length of the table. If i is greater than j, returns the empty string.
table.insert (table, [pos,] value)
Inserts element value at position pos in table, shifting up other elements to open space, if necessary. The default value for pos is n+1, where n is the length of the table (see §2.5.5), so that a call table.insert(t,x) inserts x at the end of table t.

table.maxn (table)
Returns the largest positive numerical index of the given table, or zero if the table has no positive numerical indices. (To do its job this function does a linear traversal of the whole table.)

table.remove (table [, pos])
Removes from table the element at position pos, shifting down other elements to close the space, if necessary. Returns the value of the removed element. The default value for pos is n, where n is the length of the table, so that a call table.remove(t) removes the last element of table t.

table.sort (table [, comp])
Sorts table elements in a given order, in-place, from table[1] to table[n], where n is the length of the table. If comp is given, then it must be a function that receives two table elements, and returns true when the first is less than the second (so that not comp(a[i+1],a[i]) will be true after the sort). If comp is not given, then the standard Lua operator < is used instead.
The sort algorithm is not stable; that is, elements considered equal by the given order may have their relative positions changed by the sort.

5.6 - Mathematical Functions
This library is an interface to the standard C math library. It provides all its functions inside the table math.

math.abs (x)
Returns the absolute value of x.

math.acos (x)
Returns the arc cosine of x (in radians).

math.asin (x)
Returns the arc sine of x (in radians).

math.atan (x)
Returns the arc tangent of x (in radians).

math.atan2 (x, y)
Returns the arc tangent of x/y (in radians), but uses the signs of both parameters to find the quadrant of the result. (It also handles correctly the case of y being zero.)

math.ceil (x)
Returns the smallest integer larger than or equal to x.

math.cos (x)
Returns the cosine of x (assumed to be in radians).

math.cosh (x)
Returns the hyperbolic cosine of x.

math.deg (x)
Returns the angle x (given in radians) in degrees.

math.exp (x)
Returns the the value ex.

### math.floor (x)
Returns the largest integer smaller than or equal to x.

math.fmod (x, y)
Returns the remainder of the division of x by y.

math.frexp (x)
Returns m and e such that x = m2e, e is an integer and the absolute value of m is in the range [0.5, 1) (or zero when x is zero).

math.huge
The value HUGE_VAL, a value larger than or equal to any other numerical value.

math.ldexp (m, e)
Returns m2e (e should be an integer).

math.log (x)
Returns the natural logarithm of x.

math.log10 (x)
Returns the base-10 logarithm of x.

math.max (x, ···)
Returns the maximum value among its arguments.

math.min (x, ···)
Returns the minimum value among its arguments.

math.modf (x)
Returns two numbers, the integral part of x and the fractional part of x.

math.pi
The value of pi.

math.pow (x, y)
Returns xy. (You can also use the expression x^y to compute this value.)

math.rad (x)
Returns the angle x (given in degrees) in radians.

math.random ([m [, n]])
This function is an interface to the simple pseudo-random generator function rand provided by ANSI C. (No guarantees can be given for its statistical properties.)

When called without arguments, returns a pseudo-random real number in the range [0,1). When called with a number m, math.random returns a pseudo-random integer in the range [1, m]. When called with two numbers m and n, math.random returns a pseudo-random integer in the range [m, n].

math.randomseed (x)
Sets x as the "seed" for the pseudo-random generator: equal seeds produce equal sequences of numbers.

math.sin (x)
Returns the sine of x (assumed to be in radians).

math.sinh (x)
Returns the hyperbolic sine of x.

math.sqrt (x)
Returns the square root of x. (You can also use the expression x^0.5 to compute this value.)

math.tan (x)
Returns the tangent of x (assumed to be in radians).

math.tanh (x)
Returns the hyperbolic tangent of x.

5.7 - Input and Output Facilities
The I/O library provides two different styles for file manipulation. The first one uses implicit file descriptors; that is, there are operations to set a default input file and a default output file, and all input/output operations are over these default files. The second style uses explicit file descriptors.

When using implicit file descriptors, all operations are supplied by table io. When using explicit file descriptors, the operation io.open returns a file descriptor and then all operations are supplied as methods of the file descriptor.

The table io also provides three predefined file descriptors with their usual meanings from C: io.stdin, io.stdout, and io.stderr.

Unless otherwise stated, all I/O functions return nil on failure (plus an error message as a second result) and some value different from nil on success.

io.close ([file])
Equivalent to file:close(). Without a file, closes the default output file.

io.flush ()
Equivalent to file:flush over the default output file.

io.input ([file])
When called with a file name, it opens the named file (in text mode), and sets its handle as the default input file. When called with a file handle, it simply sets this file handle as the default input file. When called without parameters, it returns the current default input file.

In case of errors this function raises the error, instead of returning an error code.

io.lines ([filename])
Opens the given file name in read mode and returns an iterator function that, each time it is called, returns a new line from the file. Therefore, the construction

     for line in io.lines(filename) do body end
will iterate over all lines of the file. When the iterator function detects the end of file, it returns nil (to finish the loop) and automatically closes the file.

The call io.lines() (with no file name) is equivalent to io.input():lines(); that is, it iterates over the lines of the default input file. In this case it does not close the file when the loop ends.

io.open (filename [, mode])
This function opens a file, in the mode specified in the string mode. It returns a new file handle, or, in case of errors, nil plus an error message.

The mode string can be any of the following:

"r": read mode (the default);
"w": write mode;
"a": append mode;
"r+": update mode, all previous data is preserved;
"w+": update mode, all previous data is erased;
"a+": append update mode, previous data is preserved, writing is only allowed at the end of file.
The mode string may also have a 'b' at the end, which is needed in some systems to open the file in binary mode. This string is exactly what is used in the standard C function fopen.

io.output ([file])
Similar to io.input, but operates over the default output file.

io.popen (prog [, mode])
Starts program prog in a separated process and returns a file handle that you can use to read data from this program (if mode is "r", the default) or to write data to this program (if mode is "w").

This function is system dependent and is not available on all platforms.

io.read (···)
Equivalent to io.input():read.

io.tmpfile ()
Returns a handle for a temporary file. This file is opened in update mode and it is automatically removed when the program ends.

io.type (obj)
Checks whether obj is a valid file handle. Returns the string "file" if obj is an open file handle, "closed file" if obj is a closed file handle, or nil if obj is not a file handle.

io.write (···)
Equivalent to io.output():write.

file:close ()
Closes file. Note that files are automatically closed when their handles are garbage collected, but that takes an unpredictable amount of time to happen.

file:flush ()
Saves any written data to file.

file:lines ()
Returns an iterator function that, each time it is called, returns a new line from the file. Therefore, the construction

     for line in file:lines() do body end
will iterate over all lines of the file. (Unlike io.lines, this function does not close the file when the loop ends.)

file:read (···)
Reads the file file, according to the given formats, which specify what to read. For each format, the function returns a string (or a number) with the characters read, or nil if it cannot read data with the specified format. When called without formats, it uses a default format that reads the entire next line (see below).

The available formats are

"*n": reads a number; this is the only format that returns a number instead of a string.
"*a": reads the whole file, starting at the current position. On end of file, it returns the empty string.
"*l": reads the next line (skipping the end of line), returning nil on end of file. This is the default format.
number: reads a string with up to this number of characters, returning nil on end of file. If number is zero, it reads nothing and returns an empty string, or nil on end of file.
file:seek ([whence] [, offset])
Sets and gets the file position, measured from the beginning of the file, to the position given by offset plus a base specified by the string whence, as follows:

"set": base is position 0 (beginning of the file);
"cur": base is current position;
"end": base is end of file;
In case of success, function seek returns the final file position, measured in bytes from the beginning of the file. If this function fails, it returns nil, plus a string describing the error.

The default value for whence is "cur", and for offset is 0. Therefore, the call file:seek() returns the current file position, without changing it; the call file:seek("set") sets the position to the beginning of the file (and returns 0); and the call file:seek("end") sets the position to the end of the file, and returns its size.

file:setvbuf (mode [, size])
Sets the buffering mode for an output file. There are three available modes:

"no": no buffering; the result of any output operation appears immediately.
"full": full buffering; output operation is performed only when the buffer is full (or when you explicitly flush the file (see io.flush)).
"line": line buffering; output is buffered until a newline is output or there is any input from some special files (such as a terminal device).
For the last two cases, sizes specifies the size of the buffer, in bytes. The default is an appropriate size.

file:write (···)
Writes the value of each of its arguments to the file. The arguments must be strings or numbers. To write other values, use tostring or string.format before write.

5.8 - Operating System Facilities
This library is implemented through table os.

os.clock ()
Returns an approximation of the amount in seconds of CPU time used by the program.

os.date ([format [, time]])
Returns a string or a table containing date and time, formatted according to the given string format.

If the time argument is present, this is the time to be formatted (see the os.time function for a description of this value). Otherwise, date formats the current time.

If format starts with '!', then the date is formatted in Coordinated Universal Time. After this optional character, if format is the string "*t", then date returns a table with the following fields: year (four digits), month (1--12), day (1--31), hour (0--23), min (0--59), sec (0--61), wday (weekday, Sunday is 1), yday (day of the year), and isdst (daylight saving flag, a boolean).

If format is not "*t", then date returns the date as a string, formatted according to the same rules as the C function strftime.

When called without arguments, date returns a reasonable date and time representation that depends on the host system and on the current locale (that is, os.date() is equivalent to os.date("%c")).

os.difftime (t2, t1)
Returns the number of seconds from time t1 to time t2. In POSIX, Windows, and some other systems, this value is exactly t2-t1.

os.execute ([command])
This function is equivalent to the C function system. It passes command to be executed by an operating system shell. It returns a status code, which is system-dependent. If command is absent, then it returns nonzero if a shell is available and zero otherwise.

os.exit ([code])
Calls the C function exit, with an optional code, to terminate the host program. The default value for code is the success code.

os.getenv (varname)
Returns the value of the process environment variable varname, or nil if the variable is not defined.

os.remove (filename)
Deletes the file or directory with the given name. Directories must be empty to be removed. If this function fails, it returns nil, plus a string describing the error.

os.rename (oldname, newname)
Renames file or directory named oldname to newname. If this function fails, it returns nil, plus a string describing the error.

os.setlocale (locale [, category])
Sets the current locale of the program. locale is a string specifying a locale; category is an optional string describing which category to change: "all", "collate", "ctype", "monetary", "numeric", or "time"; the default category is "all". The function returns the name of the new locale, or nil if the request cannot be honored.

When called with nil as the first argument, this function only returns the name of the current locale for the given category.

os.time ([table])
Returns the current time when called without arguments, or a time representing the date and time specified by the given table. This table must have fields year, month, and day, and may have fields hour, min, sec, and isdst (for a description of these fields, see the os.date function).

The returned value is a number, whose meaning depends on your system. In POSIX, Windows, and some other systems, this number counts the number of seconds since some given start time (the "epoch"). In other systems, the meaning is not specified, and the number returned by time can be used only as an argument to date and difftime.

os.tmpname ()
Returns a string with a file name that can be used for a temporary file. The file must be explicitly opened before its use and explicitly removed when no longer needed.

5.9 - The Debug Library
This library provides the functionality of the debug interface to Lua programs. You should exert care when using this library. The functions provided here should be used exclusively for debugging and similar tasks, such as profiling. Please resist the temptation to use them as a usual programming tool: they can be very slow. Moreover, several of its functions violate some assumptions about Lua code (e.g., that variables local to a function cannot be accessed from outside or that userdata metatables cannot be changed by Lua code) and therefore can compromise otherwise secure code.

All functions in this library are provided inside the debug table. All functions that operate over a thread have an optional first argument which is the thread to operate over. The default is always the current thread.

debug.debug ()
Enters an interactive mode with the user, running each string that the user enters. Using simple commands and other debug facilities, the user can inspect global and local variables, change their values, evaluate expressions, and so on. A line containing only the word cont finishes this function, so that the caller continues its execution.

Note that commands for debug.debug are not lexically nested within any function, and so have no direct access to local variables.

debug.getfenv (o)
Returns the environment of object o.
debug.gethook ([thread])
Returns the current hook settings of the thread, as three values: the current hook function, the current hook mask, and the current hook count (as set by the debug.sethook function).

debug.getinfo ([thread,] function [, what])
Returns a table with information about a function. You can give the function directly, or you can give a number as the value of function, which means the function running at level function of the call stack of the given thread: level 0 is the current function (getinfo itself); level 1 is the function that called getinfo; and so on. If function is a number larger than the number of active functions, then getinfo returns nil.

The returned table may contain all the fields returned by lua_getinfo, with the string what describing which fields to fill in. The default for what is to get all information available, except the table of valid lines. If present, the option 'f' adds a field named func with the function itself. If present, the option 'L' adds a field named activelines with the table of valid lines.

For instance, the expression debug.getinfo(1,"n").name returns a name of the current function, if a reasonable name can be found, and the expression debug.getinfo(print) returns a table with all available information about the print function.

debug.getlocal ([thread,] level, local)
This function returns the name and the value of the local variable with index local of the function at level level of the stack. (The first parameter or local variable has index 1, and so on, until the last active local variable.) The function returns nil if there is no local variable with the given index, and raises an error when called with a level out of range. (You can call debug.getinfo to check whether the level is valid.)

Variable names starting with '(' (open parentheses) represent internal variables (loop control variables, temporaries, and C function locals).

debug.getmetatable (object)
Returns the metatable of the given object or nil if it does not have a metatable.

debug.getregistry ()
Returns the registry table (see §3.5).

debug.getupvalue (func, up)
This function returns the name and the value of the upvalue with index up of the function func. The function returns nil if there is no upvalue with the given index.

debug.setfenv (object, table)
Sets the environment of the given object to the given table. Returns object.

debug.sethook ([thread,] hook, mask [, count])
Sets the given function as a hook. The string mask and the number count describe when the hook will be called. The string mask may have the following characters, with the given meaning:

"c": The hook is called every time Lua calls a function;
"r": The hook is called every time Lua returns from a function;
"l": The hook is called every time Lua enters a new line of code.
With a count different from zero, the hook is called after every count instructions.

When called without arguments, debug.sethook turns off the hook.

When the hook is called, its first parameter is a string describing the event that has triggered its call: "call", "return" (or "tail return"), "line", and "count". For line events, the hook also gets the new line number as its second parameter. Inside a hook, you can call getinfo with level 2 to get more information about the running function (level 0 is the getinfo function, and level 1 is the hook function), unless the event is "tail return". In this case, Lua is only simulating the return, and a call to getinfo will return invalid data.

debug.setlocal ([thread,] level, local, value)
This function assigns the value value to the local variable with index local of the function at level level of the stack. The function returns nil if there is no local variable with the given index, and raises an error when called with a level out of range. (You can call getinfo to check whether the level is valid.) Otherwise, it returns the name of the local variable.

debug.setmetatable (object, table)
Sets the metatable for the given object to the given table (which can be nil).

debug.setupvalue (func, up, value)
This function assigns the value value to the upvalue with index up of the function func. The function returns nil if there is no upvalue with the given index. Otherwise, it returns the name of the upvalue.

debug.traceback ([thread,] [message] [, level])
Returns a string with a traceback of the call stack. An optional message string is appended at the beginning of the traceback. An optional level number tells at which level to start the traceback (default is 1, the function calling traceback).

6 - Lua Stand-alone
Although Lua has been designed as an extension language, to be embedded in a host C program, it is also frequently used as a stand-alone language. An interpreter for Lua as a stand-alone language, called simply lua, is provided with the standard distribution. The stand-alone interpreter includes all standard libraries, including the debug library. Its usage is:

     lua [options] [script [args]]
The options are:

-e stat: executes string stat;
-l mod: "requires" mod;
-i: enters interactive mode after running script;
-v: prints version information;
--: stops handling options;
-: executes stdin as a file and stops handling options.
After handling its options, lua runs the given script, passing to it the given args as string arguments. When called without arguments, lua behaves as lua -v -i when the standard input (stdin) is a terminal, and as lua - otherwise.

Before running any argument, the interpreter checks for an environment variable LUA_INIT. If its format is @filename, then lua executes the file. Otherwise, lua executes the string itself.

All options are handled in order, except -i. For instance, an invocation like

     $ lua -e'a=1' -e 'print(a)' script.lua
will first set a to 1, then print the value of a (which is '1'), and finally run the file script.lua with no arguments. (Here $ is the shell prompt. Your prompt may be different.)

Before starting to run the script, lua collects all arguments in the command line in a global table called arg. The script name is stored at index 0, the first argument after the script name goes to index 1, and so on. Any arguments before the script name (that is, the interpreter name plus the options) go to negative indices. For instance, in the call

     $ lua -la b.lua t1 t2
the interpreter first runs the file a.lua, then creates a table

     arg = { [-2] = "lua", [-1] = "-la",
             [0] = "b.lua",
             [1] = "t1", [2] = "t2" }
and finally runs the file b.lua. The script is called with arg[1], arg[2], ··· as arguments; it can also access these arguments with the vararg expression '...'.

In interactive mode, if you write an incomplete statement, the interpreter waits for its completion by issuing a different prompt.

If the global variable _PROMPT contains a string, then its value is used as the prompt. Similarly, if the global variable _PROMPT2 contains a string, its value is used as the secondary prompt (issued during incomplete statements). Therefore, both prompts can be changed directly on the command line. For instance,

     $ lua -e"_PROMPT='myprompt> '" -i
(the outer pair of quotes is for the shell, the inner pair is for Lua), or in any Lua programs by assigning to _PROMPT. Note the use of -i to enter interactive mode; otherwise, the program would just end silently right after the assignment to _PROMPT.

To allow the use of Lua as a script interpreter in Unix systems, the stand-alone interpreter skips the first line of a chunk if it starts with #. Therefore, Lua scripts can be made into executable programs by using chmod +x and the #! form, as in

     #!/usr/local/bin/lua
(Of course, the location of the Lua interpreter may be different in your machine. If lua is in your PATH, then

     #!/usr/bin/env lua
is a more portable solution.)

7 - Incompatibilities with the Previous Version
Here we list the incompatibilities that you may found when moving a program from Lua 5.0 to Lua 5.1. You can avoid most of the incompatibilities compiling Lua with appropriate options (see file luaconf.h). However, all these compatibility options will be removed in the next version of Lua.

7.1 - Changes in the Language
The vararg system changed from the pseudo-argument arg with a table with the extra arguments to the vararg expression. (See compile-time option LUA_COMPAT_VARARG in luaconf.h.)
There was a subtle change in the scope of the implicit variables of the for statement and for the repeat statement.
The long string/long comment syntax ([[string]]) does not allow nesting. You can use the new syntax ([=[string]=]) in these cases. (See compile-time option LUA_COMPAT_LSTR in luaconf.h.)
7.2 - Changes in the Libraries
Function string.gfind was renamed string.gmatch. (See compile-time option LUA_COMPAT_GFIND in luaconf.h.)
When string.gsub is called with a function as its third argument, whenever this function returns nil or false the replacement string is the whole match, instead of the empty string.
Function table.setn was deprecated. Function table.getn corresponds to the new length operator (#); use the operator instead of the function. (See compile-time option LUA_COMPAT_GETN in luaconf.h.)
Function loadlib was renamed package.loadlib. (See compile-time option LUA_COMPAT_LOADLIB in luaconf.h.)
Function math.mod was renamed math.fmod. (See compile-time option LUA_COMPAT_MOD in luaconf.h.)
Functions table.foreach and table.foreachi are deprecated. You can use a for loop with pairs or ipairs instead.
There were substantial changes in function require due to the new module system. However, the new behavior is mostly compatible with the old, but require gets the path from package.path instead of from LUA_PATH.
Function collectgarbage has different arguments. Function gcinfo is deprecated; use collectgarbage("count") instead.
7.3 - Changes in the API
The luaopen_* functions (to open libraries) cannot be called directly, like a regular C function. They must be called through Lua, like a Lua function.
Function lua_open was replaced by lua_newstate to allow the user to set a memory-allocation function. You can use luaL_newstate from the standard library to create a state with a standard allocation function (based on realloc).
Functions luaL_getn and luaL_setn (from the auxiliary library) are deprecated. Use lua_objlen instead of luaL_getn and nothing instead of luaL_setn.
Function luaL_openlib was replaced by luaL_register.
Function luaL_checkudata now throws an error when the given value is not a userdata of the expected type. (In Lua 5.0 it returned NULL.)
8 - The Complete Syntax of Lua
Here is the complete syntax of Lua in extended BNF. (It does not describe operator precedences.)


    chunk ::= {stat [`;´]} [laststat [`;´]]

    block ::= chunk

    stat ::=  varlist1 `=´ explist1 | 
         functioncall | 
         do block end | 
         while exp do block end | 
         repeat block until exp | 
         if exp then block {elseif exp then block} [else block] end | 
         for Name `=´ exp `,´ exp [`,´ exp] do block end | 
         for namelist in explist1 do block end | 
         function funcname funcbody | 
         local function Name funcbody | 
         local namelist [`=´ explist1] 

    laststat ::= return [explist1] | break

    funcname ::= Name {`.´ Name} [`:´ Name]

    varlist1 ::= var {`,´ var}

    var ::=  Name | prefixexp `[´ exp `]´ | prefixexp `.´ Name 

    namelist ::= Name {`,´ Name}

    explist1 ::= {exp `,´} exp

    exp ::=  nil | false | true | Number | String | `...´ | function | 
         prefixexp | tableconstructor | exp binop exp | unop exp 

    prefixexp ::= var | functioncall | `(´ exp `)´

    functioncall ::=  prefixexp args | prefixexp `:´ Name args 

    args ::=  `(´ [explist1] `)´ | tableconstructor | String 

    function ::= function funcbody

    funcbody ::= `(´ [parlist1] `)´ block end

    parlist1 ::= namelist [`,´ `...´] | `...´

    tableconstructor ::= `{´ [fieldlist] `}´

    fieldlist ::= field {fieldsep field} [fieldsep]

    field ::= `[´ exp `]´ `=´ exp | Name `=´ exp | exp

    fieldsep ::= `,´ | `;´

    binop ::= `+´ | `-´ | `*´ | `/´ | `^´ | `%´ | `..´ | 
         `<´ | `<=´ | `>´ | `>=´ | `==´ | `~=´ | 
         and | or

    unop ::= `-´ | not | `#´