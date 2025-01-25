---
title: HLSL着色器语言基础
mathjax: true
date: 2019-04-11 15:27:51
categories: 
- RHI
- DirectX
tags:
- RHI
- DirectX
---

# HLSL语言基础
HLSL(High-Level Shading Language, 高级着色语言)是由微软开发的一种着色器语言，D3D9及以上版本使用其作为着色语言（注：D3D8的Shader使用是类似汇编的语言来编写），拥有如下特点：
1. 基于C语言的语法（如：大小写敏感，每条语句必须以分号结尾），是一门面向过程的强类型语言（type sensitive language）
2. 除了bool、int、uint、half、float、double基础类型外，还支持数组类型，另外HLSL还内置了适合3D图形操作的向量和矩阵类型，以及采样器（纹理）类型
3. 基础类型的隐式转换规则与C语言一致
4. 变量没有赋初值时，都会被填充为false、0或0.0
5. if条件语句和switch条件语言与C语言一致
6. for循环语句和while循环语句与C语言一致
7. return、continue和break与C语言一致。另外引入了discard，该关键字只能在PS中使用，表示放弃当前像素，直接处理下一个像素。注：也可以使用clip内置函数来将当前像素裁剪掉。
8. 无指针、无字符和字符串类型
9. 无union、无enum
10. 向量、矩阵可通过构造函数进行初始化

# 通用着色器核心

所有的可编程着色器阶段使用通用着色器核心来实现相同的基础功能。此外，顶点着色阶段、几何着色阶段和像素着色阶段提供了独特的功能。
例如几何着色阶段可以生成新的图元或删减图元，像素着色阶段可以决定当前像素是否被抛弃等。
下图展示了数据是怎么流向一个着色阶段，以及通用着色器核心与着色器内存资源之间的关系：
![1](1.png)

Input Data:顶点着色器从输入装配阶段获取数据；几何着色器则从上一个着色器阶段的输出获取等等。通过给形参引入可以使用的系统值可以提供额外的输入
Output Data:着色器生成输出的结果然后传递给管线的下一个阶段。有些输出会被通用着色器核心解释成特定用途（如顶点位置、渲染目标对应位置的值），另外一些输出则由应用程序来解释。
Shader Code:着色器代码可以从内存读取，然后用于执行代码中所期望的内容。
Samplers:采样器决定了如何对纹理进行采样和滤波。
Textures:纹理可以使用采样器进行采样，也可以基于索引的方式按像素读取。
Buffers:缓冲区可以使用读取相关的内置函数，在内存中按元素直接读取。
Constant Buffers:常量缓冲区对常量值的读取有所优化。他们被设计用于CPU对这些数据的频繁更新，因此他们有额外的大小、布局和访问限制。

# 注释
单行注释
```C
// Hello HLSL.
```

# 多行注释
```C
/********************************
This is my first HLSL.
Let's take a look.
********************************/
```
# 预处理
#if #elif [defined(),!defined()] #else #ifdef #ifndef #endif // 条件编译
```C
#define TEST1 // 定义为空的宏

#ifdef TEST1 // 条件成立
#endif 

#define TEST1 1 // 定义TEST1宏为1 注：可以不用先undef TEST1宏 但会报warning X1519: 'TEST1' : macro redefinition

#undef TEST1 // 取消 TEST1宏

#if TEST1 // 条件不成立
#endif

#ifndef TEST1 // 条件成立
#endif 

#define TEST1 -1 // 定义为int的宏

#define TEST2 // 定义为空的宏

#if TEST1 && defined(TEST2) // 条件成
#endif

#define TEST3 true // 定义为bool宏

#if TEST3 // 条件不成立
#endif 

#if !TEST3 // 条件成立
#endif

#if TEST3==true // 条件成立
#endif

#if !defined(TEST0) && TEST1 && defined(TEST2) // 条件成立
#endif

#define TEST4 100 // 定义为int的宏

#if TEST4 // 条件成立
#endif

#if TEST4 > 150
#elif (TEST4 > 120) || TEST1 // 进入elif分支
#endif

#if TEST4 > 160
#else // 进入else分支
#define xx2 1
#endif

#if !TEST4
#elif (TEST4 > 110)
#else // 进入 else分支
#endif

#define TEST5 2.0 // 定义为float的宏

// #if TEST5 // float 不能进行条件判断 编译失败
// #endif

// #if TEST5 > 0.0 // float 不能进行条件判断 编译失败
```

#define #undef // 宏定义、宏取消
```C
#define TEST1 100 // 定义TEST1宏为100

#ifdef TEST1 //条件成立
#undef TEST1 // 取消TEST1宏的定义
#endif

#if !defined(TEST1) // 条件成立
#endif

#define SQUARE(a) ((a)*(a))

#define MAX(a,b) \
   (((a) > (b)) ? (a) : (b)) // 宏必须在一行写完，多行写时必须带上 \ 行连接符，弹药注意 \ 后不要有空格，否则会编译失败

#define MERGE(a, b) a##b // ## 字符拼接符
```

#line // 指示下一行的符号，及当前所在的文件；该命令会修改 __FILE__,__LINE__ 的值
该命令是提供给编译器使用的，程序员最好不要使用该命令
```C
#if __LINE__==10 //判断当前行号是否为10
#endif 

#if __FILE__=0 // 条件成立 __FILE__ 始终是0
#endif

#line 116 // 指定下一行的行号为116
#if __LINE__ == 116 // 条件成立
#endif

#line 200 "test.hlsl" // 指定下一行的行号为200，当前文件名为test.hlsl
#if __FILE__ = 0 // 条件成立   __FILE__始终为0

// test.hlsl(204,2):error:This is an error!
#error This is an error!
```

#error //error命令被执行，会导致当前文件编译失败
```C
// E:\ModenD3D\HLSL-Development-Cookbook\book_sample\Chpater 1 - Forward Light\Ambient Light\ForwardLight.hlsl(40,2): error: This is an error! 
#error This is an error!
```

#pragma // 用来控制编译器的一些行为
```C
#pragma warning(disable : 1519; once : 3205; error : 3206) // 忽略1519 warning, 只报一次320 warning, 3206 warning视为error

#define TEST1 //  定义为空的宏
#define TEST1 1 // 定义TEST1宏为1 注：可以不用先 undef TEST1宏 但是会报warning X1519: 'TEST1': macro redefinition

#paragma message("Hello HLSL.") // Hello HLSL

#paragma pack_matrix(column_major) // 将uniform参数的 matrix设置为列主序（缺省）注1：列主序生成的指令数更少、因此其效率比行主序的效率更高 注2：构造matrix时不受 #pragma pack_matrix影响，始终为行主序
#paragma pack_matrix(row_major) //  将uniform 参数的 matrix设置为行主序

#error This is an error! //  hlsl中出现error时，才会打印 pragma message和warning信息
```

注1：需要注意的时，hlsl中出现error时(#error或语法错误)，才会打印pragma message 和warning信息
注2：更多error、warning number说明，详见：https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/hlsl-errors-and-warnings
注3：在C++代码层中，DirectXMath数学库创建的矩阵都是行矩阵，但当矩阵从C++传递给HLSL时，HLSL默认是列矩阵的，因此传递前要进行一次转置。如果希望不发生转置操作的话，可以添加修饰关键字 row_major

#include // 引用其他hlsl文件 与C/C++语法用法一致
```C
#include "common.hlsl" // 引用其他的hlsl文件
```

# typedef
用于类型的别名，用法与C语言一致
```C
typedef vector<float, 3> POINT;
typedef const float CFLOAT;

POINT pt;
CFLOAT cfl;
```

# 运算符
除了没有指针相关的运算符外，其他的与C语言完全一致
注1：对于向量、矩阵类型，运算符会在各个分量上进行
注2：支持浮点数取模%
```C
int nl = 5 % 3; // 取模即求余数，结果为2
float f1 = 3.3 % 1.6; // 3.3-(int)(3.3/1.6)*1.6 = 3.3-2*1.6 = 0.1
float f2 = -3.3 % 1.6;
```
注3：二元运算中变量类型的提升规则：
1. 对于二元运算来说，如果运算符左右操作数的维度不同，那么维度较小的变量类型将会被隐式提升为维度较大的变量类型。但是这种提升仅限与标量到向量的提升，即x会变成为(x,x,x)。但是不支持像float2到float3的提升。
2. 对于二元运算来说，如果运算符左右的操作数类型不同，那么低精度变量的类型将被隐式提升为高精度变量的类型，这点和C/C++是类似的。

# 控制流
## 条件语句
HLSL也支持`if`，`else`， `continue`，`break`，`switch`关键字，此外`discard`关键字用于像素着色阶段抛弃该像素。
条件的判断使用一个布尔值进行，通常由各种逻辑运算符或者比较运算符操作得到。注意向量之间的比较或者逻辑操作是得到一个存有布尔值的向量，不能够直接用于条件判断，也不能用于`switch`语句。
## 判断与动态分支
基于值的条件分支只有在程序执行的时候被编译好的着色器汇编成两种方式：判断(predication)和动态分支(dynamic branching)。
如果使用的是判断的形式，编译器会提前计算两个不同分支下表达式的值。然后使用比较指令来基于比较结果来“选择”正确的值。
而动态分支使用的是跳转指令来避免一些非必要的计算和内存访问。
着色器程序在同时执行的时候应当选择相同的分支，以防止硬件在分支的两边执行。通常情况下，硬件会同时将一系列连续的顶点数据传入到顶点着色器并行计算，或者是一系列连续的像素单元传入到像素着色器同时运算等。
动态分支会由于执行分支指令所带来的开销而导致一定的性能损失，因此要权衡动态的开销和可以跳过的指令数目。
通常情况下编译器会自动选择使用判断还是动态发呢之，但我们可以通过重写某些属性来修改编译器的行为。我们可以在条件语句前可以选择添加下面两个属性之一：

|属性|描述|
|---|---|
|[branch]|缺省。根据条件值的结果，只计算其中一边的内容，会产生跳转指令。|
|[flatten]|两边的分支都会计算，然后根据条件值选择其中一遍。可以避免跳转指令的产生。|

## 循环语句
HLSL也支持`for`，`while`和 `do while`循环。和条件语句一样，它可能也会基于运行时的条件值判断而产生动态分支，从而影响程序性能。用法如下：

|属性|描述|
|---|---|
|[loop]|缺省。默认不加属性的循环语句为loop型。|
|[unroll]|如果循环次数较小，我们可以使用属性[unroll]来展开属性，代价是产生更多的汇编指令。|

```C
times = 4;
sum = times;
[unroll]
while (times--)
{
   sum += times;
}
```

# 寄存器（register）
用于在C++与HLSL之前传递数据，包括如下4中：

|寄存器 |说明 | 上限|
|---|---|---|
|b | 常量缓冲区视图（CBV），用于从C++传递只读数据给HLSL|15个常量缓冲区（共16个，系统内部保留1个）|
| t|着色器资源视图（SRV），用于从C++传递只读内存块或纹理数据给HLSL|128个|
|u|无序访问视图（UAV），用于可读写数据的传递||
|s|用于从C++传递采样器设置给HLSL|128个|

参见：[Resource binding in HLSL(HLSL中的资源绑定)](https://docs.microsoft.com/zh-cn/windows/win32/direct3d12/resource-binding-in-hlsl)

# 变量
变量名要符合以下规则：
1. 只能包含大小写字母、数字和下划线
2. 变量名不能以数字开头
3. 不能是关键字或预留的关键字

全局变量：定义在函数体外的变量。作用域规则与C语言全局变量一致。
局部变量：定义在函数体内的变量。作用域规则与C语言局部变量一致。

## const变量
该变量为一常量，需要被初始化，在运行时不能被修改，与C/C++用法一致。

## static变量
进一步可分为static局部变量和static全局变量，与c/c++用法一致。
static局部变量需要在HLSL中自己初始化，否则使用默认初始化，初始化操作仅执行一次（首次被访问时）
只在着色器内部可见

## extern变量
在全局变量上可用，非静态的全局变量默认是extern类型
可在着色器外被访问，比如被C++应用程序

## uniform变量
在D3D代码中初始化，然后在作为输入传给着色器
允许在C++应用层中修改，但在着色器执行的过程中，其值始终保持不变（运行前可变，运行时不变）。着色器程序中的全局变量默认为即uniform又extern。

`volatile` -- 表示该变量经常被修改，用于局部变量
`shared` -- 在全局变量上可用，提示效果框架该变量可在多个效果之间共享
`nointerpolation` -- 修饰的变量，在将顶点着色器的输出传递到像素着色器之前，请勿对其进行插值。
`groupshared` --  将一个变量标记为用于计算着色器的线程组共享内存
`precise` --  用于保证该变量在计算时，是严格精确的
`row_major` -- 标记一个可在单个行中存储4个成分的变量，以便可以将它们存储在单个常量寄存器中
`column_major` -- 标记一个可在单个列中存储4个称为的变量，以优化矩阵数学（缺省）

   在这一章里我们描述高级着色器语言（High-Level Shading Language ，简称HLSL），在下三章里我们用它去编写顶点和像素着色器。简单的说，在我们写的程序里顶点和像素是很小的对象，它们由GPU来执行，是固定功能管线的一部分。用我们自己写的着色器程序替换一部分固定功能管线，在绘制效果上我们获得很大的灵活性。我们不再局限于预定义的"固定"操作。
   为了编写着色器程序，我们需要一种语言。 在DirectX 8.x,中，着色器是用低级着色器汇编语言编写的。幸运的是，我们不必再用汇编语言来写着色器了，DirectX 9支持一种高级着色器语言来编写。用HLSL在汇编语言来写着色器程序与使用高级语言有同样的优势，像C++，它超越了汇编语言，即：
   
* 增加生产力—用高级语言比用低级语言写程序更快、更容易。 我们可以花费更多的时间关注于算法而不是代码。
* 增加可读性—用高级语言写的程序更易读，这意味着用高级语言编程更易于调试和维护。
* 大多数情况下，编译器产生的汇编代码比手写有效率。
* 使用HLSL 编译器，我们可以编译我们的代码到任何可用shader版本，使用汇编语言我们将不得不为每个需要的版本移植代码。

HLSL 同C和C++语法很类似, 所以缩短了学习曲线。
   最后，如果你的显卡不支持顶点和像素着色器的话，为了执行着色器的例子程序你将需要转换REF设备。使用REF设备意味着着色器例子运行的会很慢，但它至少能显示结果，让我们去检查是否代码可以被执行。
   
提示：顶点shaders可以用软件来模拟 ―― D3DCREATE_SOFTWARE_VERTEX-PROCESSING。16.1 

目标
* 学习如何定、编译一个HLSL 着色器程序。
* 学习如何将程序中的数据传送到着色器程序。
* 熟悉语法、类型，和HLSL的内建函数。
# 编写HLSL着色器
我们可以在程序源文件中用字符串直接编写HLSL着色器代码，然而更方便、更模块化的方法是把它与程序代码分离出来。因此，我们在记事本中编写着色器并保存成一般的ASCII文本文件，然后可以用D3DXCompileShaderFromFile函数(section 16.2.2)来编译它们。
作为介绍，下面是用HLSL编写的一个简单的顶点着色器，用记事本生成并保存成文本文件“Transform.txt”。全部工程都在标题为Transform的目录下，顶点着色器用组合视图和投影矩阵转换顶点，并设置顶点漫射光为蓝色。
   
   注意：这是一个顶点着色器的例子，不必关心顶点着色器做了什么，这是下一章包含的内容，现在的目标是熟悉HLSL编程的语法和格式。
// File: transform.txt
// Author: Frank D. Luna (C) All Rights Reserved
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP,
// MSVC++ 7.0

// Desc: 顶点着色器用组合视图和投影矩阵转换顶点，并设置顶点漫射光为蓝色.

//全局变量
//用来保存视图和投影的组合矩阵，在程序中初始化变量
matrix ViewProjMatrix;

// 初始化颜色变量（蓝色）
vector Blue = {0.0f, 0.0f, 1.0f, 1.0f};

// 结构
// Input结构用来描述输入到着色器的顶点，这个Input顶点只包含一个位置成员 
struct VS_INPUT
{
     vector position  : POSITION;
};

//Output结构用来描述从着色器输出的顶点，这个Output顶点包含位置和颜色成员
struct VS_OUTPUT
{
     vector position : POSITION;
     vector diffuse  : COLOR;
};

//主入口点，这个main函数接收一个Input顶点的拷贝作为参数，返回一个Output顶点的拷贝
VS_OUTPUT Main(VS_INPUT input)
{
     // 将output结构所有成员初始化
     VS_OUTPUT output = (VS_OUTPUT)0;
     // 将位置变换到投影空间
     output.position  = mul(input.position, ViewProjMatrix);
     // 设置顶点颜色
     output.diffuse = Blue;
     //Output the projected and colored vertex.
     return output;
}
   
16.1.1 全局变量
首先是2个全局变量：
matrix ViewProjMatrix;
vector Blue = {0.0f, 0.0f, 1.0f, 1.0f};
   第1个变量ViewProjMatrix是矩阵类型，它是一个在HLSL 内创建的4×4的矩阵类型。这个变量保存视图与投影的组合矩阵，它描述两者的变换。使用这种方法我们只要做一个向量和矩阵的乘法（而不是二个）。注意，在着色器源代码的任何地方都没有初始化这个变量，因为它是我们在应用程序的源代码里设置的，而不是在着色器中。从应用程序向着色器程序通讯是常用的操作，例子在16.2.1节。
   第二个变量Blue是built-in（内建）类型的4D向量，我们简单的将它初始化成蓝色，它是个RGBA的颜色向量。
   
16.1.2 输入和输出结构
   在全局变量定义之后，定义2个特殊的结构，我们调用输入和输出结构。对于顶点着色器而言，这些结构定义了顶点的数据，分别是：
struct VS_INPUT
{
     vector position : POSITION;
};

struct VS_OUTPUT
{
     vector position : POSITION;
     vector diffuse  : COLOR;
};
注意：给像素着色器的结构定义输入和输出像素数据。

   在例子中，INPUT 顶点着色器只包含位置成员（POSITION），OUTPUT顶点着色器包含位置和颜色成员（POSITION and COLOR）。
   特殊的冒号是一种语义，用于是声明变量。这与vertex结构中的自由顶点格式（FVF）相似。例如，在VS_INPUT中有成员：vector position : POSITION;
   ": COLOR"是说顶点的漫射光是用VS_OUTPUT结构的COLOR成员来说明的。在下一章中对于顶点和像素着色器中向量的标识符用法，我们将会有更多的讨论。
   注意：从底层来说，着色器变量的语义和语法同硬件寄存器是相关联的。即，input变量与input寄存器关联，output变量与output寄存器关联。例如，VS_INPUT中的position成员与顶点input的position寄存器相关联。同样，diffuse与顶点的output 的color寄存器关联。
   
16.1.3 函数的入口点
   在C++程序中，每个HLSL程序有一个入口点。在我们的着色器例子中，我们调用入口点函数main。然而名字不是强制的。入口点函数名可以是任何有效的函数名，入口点函数必须有一个input结构参数，它通过input顶点进入着色器。入口点函数必须返回一个output结构实例，在着色器中使用output操作顶点。
VS_OUTPUT Main(VS_INPUT input)
{
   
   注意：实际上，使用input、output结构不是强制的。例如，有时你将会看到使用类似下面的语法，特别是在像素着色器中：
float4 Main(in float2 base : TEXCOORD0,
            in float2 spot : TEXCOORD1,
            in float2 text : TEXCOORD2) : COLOR
{
...
}
   
   例子中，输入到着色器中的参数是3个纹理坐标。着色器输出（返回）一个颜色，COLOR语句在函数的声明以后。这种定义是类似于：
struct INPUT
{
     float2 base : TEXCOORD0;
     float2 spot : TEXCOORD1;
     float2 text : TEXCOORD2;
};

struct OUTPUT
{
     float4 c : COLOR;
};

OUTPUT Main(INPUT input)
{
...
}
   
   输入点函数负责根据给定的input顶点计算output顶点。例子中的着色器简单的变换input顶点到视图空间和投影空间，设置顶点颜色为蓝色，并返回结果顶点。首先我们定义VS_OUTPUT的实例并初始化所有成员为0。
VS_OUTPUT output = (VS_OUTPUT)0; // zero out all members
   然后着色器变换input顶点位置用ViewProjMatrix变量，使用mul 函数。它是一个built-in（内建）函数，实现向量与矩阵相乘，或矩阵与矩阵相乘。我们保存结果变换的向量（在output实例的position成员中）。
// 变换后为投影空间的位置
output.position = mul(input.position, ViewProjMatrix);
   然后设置output的成员diffuse的颜色为蓝色：
// 设置顶点颜色
output.diffuse = Blue;
   最后返回结果向量：
return output;
}
16.2 编译HLSL 着色器
16.2.1 常量表
   每个着色器有一个常量表，用来保存它的变量。D3DX库通过ID3DXConstantTable接口，提供给应用程序访问着色器的常量表。通过这个接口我们能够在应用程序中设置着色器源代码中的变量。
   我们现在描述一个被节选了的ID3DXConstantTable接口的方法列表的实现，全部的列表请查阅Direct3D文档。
16.2.1.1 取得常量句柄
   为了在应用程序中设置着色器中的一个特定变量，需要有一种方法去引用它，我们能够在应用程序中用D3DXHANDLE引用一个在着色器中的变量，下面的方法返回一个着色器中的变量的D3DXHANDLE，使用时，需要传递一个变量的名字作为参数：
D3DXHANDLE ID3DXConstantTable::GetConstantByName(
     D3DXHANDLE hConstant, // scope of constant
     LPCSTR pName          // name of constant
);
* Hconstant——我们要取得的父结构中变量句柄的D3DXHANDLE标识。例如，如果我们想获得一个特定数据结构中单一数据成员的句柄，我们可以传递结构实例的句柄。如果我们获得一个顶级变量的句柄，给这个参数设为0。
* PName——我们想获得的句柄的着色器代码中的变量的名字。

例如，如果在着色器中变量的名字为ViewProjMatrix，并且这是顶级变量，我们这么写：
// 取得着色器中ViewProjMatrix变量的句柄
D3DXHANDLE h0;
h0 = ConstTable->GetConstantByName(0, "ViewProjMatrix");

16.2.1.2 设置常量
   一旦应用程序有了一个D3DXHANDLE，要引用着色器代码中的具体变量，我们可以在应用程序中使用ID3DXConstantTable::SetXXX方法设置变量。如果我们想设置一个向量数组类型的变量，方法名是SetVectorArray。
   ID3DXConstantTable::SetXXX的一般语法是：
HRESULT ID3DXConstantTable::SetXXX(
     LPDIRECT3DDEVICE9 pDevice,
     D3DXHANDLE hConstant,
     XXX value
);
* PDevice：常量表所关联的设备的指针。
* HConstant：我们正在设置的变量句柄的引用。
* Value：我们要把变量设置成的值，XXX是我们设置的要替换的变量类型名，对于有些类型（bool, int, float），传递变量值的COPY，另外一些类型（vectors, matrices, structures），传递值的指针。
   
   下面列表描述了我们能用ID3DXConstantTable接口设置的类型列表。这里假定我们有一个有效的设备，和一个有效句柄。
SetBool—Used to set a Boolean value. Sample call:
bool b = true;
ConstTable->SetBool(Device, handle, b);

SetBoolArray—Used to set a Boolean array. Sample call:
bool b[3] = {true, false, true};
ConstTable->SetBoolArray(Device, handle, b, 3);

SetFloat—Used to set a float. Sample call:
float f = 3.14f;
ConstTable->SetFloat(Device, handle, f);

SetFloatArray—Used to set a float array. Sample call:
float f[2] = {1.0f, 2.0f};
ConstTable->SetFloatArray(Device, handle, f, 2);

SetInt—Used to set an integer. Sample call:
int x = 4;
ConstTable->SetInt(Device, handle, x);

SetIntArray—Used to set an integer array. Sample call:
int x[4] = {1, 2, 3, 4};
ConstTable->SetIntArray(Device, handle, x, 4);

SetMatrix—Used to set a 4 × 4 matrix. Sample call:
D3DXMATRIX M(…);
ConstTable->SetMatrix(Device, handle, &M);

SetMatrixArray—Used to set a 4 × 4 matrix array. Sample call:
D3DXMATRIX M[4];
// ...Initialize matrices
ConstTable->SetMatrixArray(Device, handle, M, 4);

SetMatrixPointerArray—Used to set an array of 4 × 4 matrix pointers. Sample call:
D3DXMATRIX* M[4];
// ...Allocate and initialize matrix pointers
ConstTable->SetMatrixPointerArray(Device, handle, M, 4);

SetMatrixTranspose—Used to set a transposed 4 × 4 matrix. Sample call:
D3DXMATRIX M(…);
D3DXMatrixTranspose(&M, &M);
ConstTable->SetMatrixTranspose(Device, handle, &M);

SetMatrixTransposeArray—Used to set an array of 4 × 4 transposed matrices. Sample call:
D3DXMATRIX M[4];
// ...Initialize matrices and transpose them.
ConstTable->SetMatrixTransposeArray(Device, handle, M, 4);

SetMatrixTransposePointerArray—Used to set an array of pointers to 4 × 4 transposed matrices. Sample call:
D3DXMATRIX* M[4];
// ...Allocate,initialize matrix pointers and transpose them.
ConstTable->SetMatrixTransposePointerArray(Device, handle, M, 4);

SetVector—Used to set a variable of type D3DXVECTOR4. Sample call:
D3DXVECTOR4 v(1.0f, 2.0f, 3.0f, 4.0f);
ConstTable->SetVector(Device, handle, &v);

SetVectorArray—Used to set a variable that is a vector array. Sample call:
D3DXVECTOR4 v[3];
// ...Initialize vectors
ConstTable->SetVectorArray(Device, handle, v, 3);

SetValue—Used to set an arbitrarily sized type, such as a structure. In the sample call, we use SetValue to set a D3DXMATRIX:
D3DXMATRIX M(…);
ConstTable->SetValue(Device, handle, (void*)&M, sizeof(M));
   
   
16.2.1.3 设置常量默认值
   下一个方法就是设置常量的默认值，这些默认值在声明时初始化。这个方法应该在应用程序建立（setup）期间被一次性调用（called once）。
   HRESULT ID3DXConstantTable::SetDefaults(
        LPDIRECT3DDEVICE9 pDevice
   );
   pDevice——关联到常量表的设备的指针。
   
16.2.2 编译HLSL着色器
   我们可以编译一个着色器——用我们已保存的着色器的文本文件——使用下列函数：
   HRESULT D3DXCompileShaderFromFile(
        LPCSTR               pSrcFile,
        CONST D3DXMACRO*     pDefines,
        LPD3DXINCLUDE        pInclude,
        LPCSTR               pFunctionName,
        LPCSTR               pTarget,
        DWORD                Flags,
        LPD3DXBUFFER*        ppShader,
        LPD3DXBUFFER*        ppErrorMsgs,
        LPD3DXCONSTANTTABLE* ppConstantTable
   );
   
* pSrcFile——要编译的包含着色器源代码的文本文件的文件名
* pDefines——参数可选，本书中指定为空。
* pInclude——ID3DXInclude接口指针。这个接口被设计成由应用程序实现，所以我们可以重载默认include的行为。通常，默认行为就可以了，而且我们可以通过将其指定为空忽略此参数。
* pFunctionName——指定入口点函数名的字符串。例如，如果着色器的入口点函数叫做Main，我们可以给此参数传递“Main”。
* pTarget——指定要编译成的HLSL着色器源文件的版本的字符串。有效的顶点着色器版本是：vs_1_1, vs_2_0, vs_2_sw。有效的像素着色器版本是2.0，我们可以给此参数传递vs_2_0。
   
   备注：有编译不同版本着色器的能力，是HLSL与汇编语言比的主要优势。用HLSL我们只需为需要的目标简单的重新编译，便可快速移植着色器到不同的版本。使用汇编，我们可能需要手动移植代码。
* Flags——可选的编译标记，指定为0标识没有标记。有效的选项是：
* D3DXSHADER_DEBUG——通知编译器写入调试信息
* D3DXSHADER_SKIPVALIDATION——通知编译器不要做任何代码检查。此项仅用于你已知着色器能够工作时
* D3DXSHADER_SKIPOPTIMIZATION——通知编译器不要执行任何代码优化。实践中，这个选项应该仅用于调试，因为这种情况下你不希望编译器以任何方式修改代码。
* ppShader——返回已编译的着色器代码的ID3DXBuffer指针。这个已编译过的着色器代码将作为另一个实际创建顶点/像素着色器函数的参数
* ppErrorMsgs——返回包含错误码和错误消息字符串的ID3DXBuffer指针
* ppConstantTable——返回包含此着色器常量表数据的ID3DXConstantTable指针
   
   这里是一个调用D3DXCompileShaderFromFile的例子：
   // Compile shader
   
   ID3DXConstantTable* TransformConstantTable = 0;
   ID3DXBuffer* shader      = 0;
   ID3DXBuffer* errorBuffer = 0;
   
   hr = D3DXCompileShaderFromFile(
        "transform.txt",      // shader filename
        0,
        0,
        "Main",               // entry point function name
        "vs 2 0",             // shader version to compile to
        D3DXSHADER_DEBUG,     // debug compile
        &shader,
        &errorBuffer,
        &TransformConstantTable);
   
   // output any error messages
   if( errorBuffer )
   {
        ::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
        d3d::Release<ID3DXBuffer*>(errorBuffer);
   }
   
   if (FAILED (hr))
   {
        ::MessageBox(0, "D3DXCreateEffectFromFile() - FAILED", 0, 0);
        return false;
   }

16.3 变量类型
   注意：除了下列各小节中描述的类型外，HLSL还有一些内建的对象类型（如：纹理对象）。但是，由于这些对象类型主要用于效果框架，我们将对其延迟到第19章讨论。

16.3.1 数值类型
HLSL支持下列数值类型（scalar type）：
* bool—True or false value. Note that HLSL provides the true and false keywords.
* int—32-bit signed integer
* half—16-bit floating-point number
* float—32-bit floating-point number
* double—64-bit floating-point number
   注意：一些平台不支持int, half, and double类型，这时我们使用 float类型模拟。
   
16.3.2 向量类型
HLSL有下列内建的向量类型（vector type）：
* vector——各分量为float类型的4D向量
* vector<T, n>——一个n维向量，其每个分量都为T类型。n维必须在1到4之间。这里是一个2D double向量的例子：
vector<double, 2> vec2;

   我们可以使用数组下标的语法访问向量的一个分量。例如，要设置向量vec的第i个分量，我们可以写成：
vec[i] = 2.0f;

   此外，我们可以像访问结构的成员一样访问向量vec的一个分量，使用已定义的分量名x，y，z，w，r，g，b和a。
vec.x = vec.r = 1.0f;
vec.y = vec.g = 2.0f;
vec.z = vec.b = 3.0f;

vec.w = vec.a = 4.0f;

   名称为r，g，b和a的分量分别对应x，y，z和w的分量。当使用向量来表示颜色时，RGBA符号是更适合的，因为它加强了向量所表示的颜色。

   作为选择，我们可以使用其它一些预定义类型，分别用来代表2D，3D和4D向量的类型：
float2 vec2;
float3 vec3;
float4 vec4;

   考虑向量u = (ux, uy, uz, uw)，假设我们要拷贝u的所有分量到一个像v = (ux, uy, uy, uw)这样的向量v。最直接的方法可能是逐个从u往v拷贝每个分量。但不管怎样，HLSL提供了一种特殊的语法做这些无序的拷贝，它叫做“鸡尾酒”（swizzles）：
vector u = {l.0f, 2.0f, 3.0f, 4.0f};
vector v = {0.0f, 0.0f, 5.0f, 6.0f}; 
v = u.xyyw; // v = {1.0f, 2.0f, 2.0f, 4.0f}

拷贝数组时，我们不必拷贝每个分量。例如，我们可以仅拷贝x和y分量，代码段举例如下：
vector u = {1.0f, 2.0f, 3.0f, 4.0f};
vector v = {0.0f, 0.0f, 5.0f, 6.0f}; 
v.xy = u; // v = {l.0f, 2.0f, 5.0f, 6.0f}

16.3.3 矩阵类型
HLSL有下列内建矩阵类型：
* matrix——一个4×4矩阵，其各项类型为float
* matrix<T, m, n>——一个m×n矩阵，其每个成员为类型T。矩阵维数m和n必须在1至4之间。

这里是一个2×2整型矩阵的例子：
matrix<int, 2, 2> m2x2;

作为选择，我们可以定义一个m×n矩阵，其m和n在1至4之间，使用下列语法：
floatmxn matmxn;
实例：
float2x2 mat2x2;
float3x3 mat3x3;
float4x4 mat4x4;
float2x4 mat2x4;
注意：类型不必是float类型——我们可以使用其它类型。举例来说，我们可以用整型，写成这样：
int2x2 i2x2;
int2x2 i3x3;
int2x2 i2x4;

我们可以用二维数组的下标语法访问矩阵中的项。例如，要设置矩阵M的第i，j个项，我们可以写成：
M[i] [j] = value;

此外，我们可以像访问结构的成员那样访问矩阵M的项。下列条目已定义：
以1为基数的：
M._11 = M._12 = M._13 = M._14 = 0.0f;
M._21 = M._22 = M._23 = M._24 = 0.0f;
M._31 = M._32 = M._33 = M._34 = 0.0f;
M._41 = M._42 = M._43 = M._44 = 0.0f;

以0为基数的：
M._m00 = M._m01 = M._m02 = M._m03 = 0.0f;
M._m10 = M._m11 = M._m12 = M._m13 = 0.0f;
M._m20 = M._m21 = M._m22 = M._m23 = 0.0f;
M._m30 = M._m31 = M._m32 = M._m33 = 0.0f;

有时，我们想要访问矩阵中一个特定的行。我们可以用一维数组的下标语法来做。例如，要引用矩阵M中第i行的向量，我们可以写：
vector ithRow = M[i]; // get the ith row vector in M

注意：可以使用两种语法在HLSL中初始化变量：
vector u = {0.6f, 0.3f, 1.0f, 1.0f};
vector v = {1.0f, 5.0f, 0.2f, 1.0f};

也可以，等价的，使用构造风格的语法：
vector u = vector(0.6f, 0.3f, 1.0f, 1.0f);
vector v = vector(1.0f, 5.0f, 0.2f, 1.0f);

其它一些例子:
float2x2 f2x2 = float2x2(1.0f, 2.0f, 3.0f, 4.0f);
int2x2 m = {1, 2, 3, 4};
int n = int(5);
int a = {5};
float3 x = float3(0, 0, 0);

16.3.4 数组
我们可以用类似C++的语法声明特定类型的一个数组。例如：
float  M[4][4];
half   p[4];
vector v[12];

16.3.5 结构
结构的定义和在C++里一样。但是，HLSL里的结构不能有成员函数。这是一个HLSL里的结构的例子：
struct MyStruct
{
     matrix T;
     vector n;
     float  f;
     int    x;
     bool   b;
};
MyStruct s; // instantiate
s.f = 5.0f; // member access

16.3.6 typedef关键字
   HLSL的typedef关键字功能和C++里的完全一样。例如，我们可以给类型vector<float, 3>用下面的语法命名：
typedef vector<float, 3> point;
然后，不用写成：
vector<float, 3> myPoint;
……我们只需这样写：
point myPoint;

这里是另外两个例子，它展示了如何对常量和数组类型使用typedef关键字：
typedef const float CFLOAT;
typedef float point2[2];

1.3.7 变量前缀
下列关键字可以做变量声明的前缀：
* static——如果带static关键字前缀，那它是全局变量。就表示它不是暴露于着色器之外的。换句话说，它是着色器局部的。如果一个局部变量以static关键字为前缀，它就和C++中static局部变量有相同的行为。也就是说，该变量在函数首次执行时被一次性初始化，然后在所有函数调用中维持其值。如果变量没有被初始化，它就自动初始化为0。static int x = 5;

* uniform——如果变量以uniform关键字为前缀，就意味着此变量在着色器外面被初始化，比如被C++应用程序初始化，然后再输入进着色器。

* extern——如果变量以extern关键字为前缀，就意味着该变量可在着色器外被访问，比如被C++应用程序。仅全局变量可以以extern关键字为前缀。不是static的全局变量默认就是extern。

* shared——如果变量以shared关键字为前缀，就提示效果框架（参见19章）：变量将在多个效果间被共享。仅全局变量可以以shared为前缀。

* volatile——如果变量以volatile关键字为前缀，就提示效果框架（参见19章）：变量将被时常修改。仅全局变量可以以volatile为前缀。

* const——HLSL中的const关键字和C++里的意思一样。也就是说，如果变量以const为前缀，那此变量就是常量，并且不能被改变。const float pi = 3.14f;
16.4关键字、语句和强制转换
16.4.1 关键字
为便于参考，这里给出一个HLSL定义的关键字列表：
asm        bool        compile       const          decl         do
double      else        extern         false          float         for
half        if           in            inline         inout         int 
matrix      out         pass           pixelshader    return        sampler 
shared      static       string          struct         technique     texture 
true        typedef     uniform        vector         vertexshader   void 
volatile     while

下面的集合显示了被保留并且未使用但是将来可能成为关键字的标识符：
auto            break        case           catch         char          class 
const_cast       continue      default         delete        dynamic cast   enum 
explicit         friend         goto           long         mutable       namespace 
new            operator       private         protected     public         register 
reinterpret_cast   short         signed          sizeof        static_cast     switch 
template         this          throw          try           typename     union 
unsigned         using        virtual 

16.4.2 基本程序流程
HLSL支持很多与C++相似的选择、重复、和一般程序流程语句。这些语句的语法和C++极为相似。
return语句：
return (expression);
if和if…else语句：
if( condition )
{
     statement(s);
}


if( condition )
{
     statement(s);
}
else
{
     statement(s);
}

for语句：
for(initial; condition; increment)
{
     statement(s);
}

while语句：
while( condition )
{
     statement(s);
}

do…while语句：
do
{
     statement(s);
}while( condition );

16.4.3 强制转换（casting）
   HLSL支持一种非常自由的强制转换设计。HLSL中强制转换的语法和C程序语言中的一样。例如要把float转换到matrix，我们写：
float f = 5.0f;
matrix m = (matrix)f;

   从本书的例子中，你就能推断出这个转换语法的意思。但是，如果想要得到更详细的受支持的转换的信息，那么在DirectX SDK里，Content（内容）标签页下，看DirectX Graphics\Reference\Shader Reference\High Level Shading Language\Type就可以了。

16.5 操作符
   HLSL支持很多类似C++的操作符。除了很少一些底下注释的例外以外，他们的用法和C++里的完全一样。下表列出了HLSL的操作符：
[]
?
>
<
< =
> =
! = 
= =
!
&&
?
?:
+
+ =
-
- =
*
*=
/
/=
%
%=
+ +
--
=
()
'




   虽然操作符的行为和C++很相似，但是也有一些差异。第一，求模%运算符对整型和浮点型都起作用。为了使用求模操作符，左边的值和右边的值都必须有相同的正负号（如：左边和右边必须都是正或者负）。

   第二，要注意HLSL操作是以每个分量为基础的。这是由于实际上向量和矩阵是语言内建的，并且这些类型是由若干个分量组成。通过将这些操作施加在分量级别之上，我们可以像使用数值类型一样完成诸如向量/矩阵的加法，减法和相等测试这些操作（），见下例：
   
注意：操作符的行为正如对数值操作一样（也就是说，按一般C++的方式）。
vector u = {1.0f, 0.0f, -3.0f, 1.0f};
vector v = {-4.0f, 2.0f, 1.0f, 0.0f};
// adds corresponding components
vector sum = u + v; // sum = (-3.0f, 2.0f, -2.0f, 1.0f)

增量一个向量就是增量其每个分量：
// before increment: sum = (-3.0f, 2.0f, -2.0f, 1.0f)
sum++; // after increment: sum = (-2.0f, 3.0f, -1.0f, 2.0f)

向量相乘也是按分量的：
vector u = {1.0f, 0.0f, -3.0f, 1.0f};
vector v = {-4.0f, 2.0f, 1.0f, 0.0f};

// multiply corresponding components
vector sum = u * v; // product = (-4.0f, 0.0f, -3.0f, 0.0f)

比较操作也是按分量进行的，并且返回一个每个分量都为bool类型的向量或者数组。作为结果的“bool”向量包含了每个分量比较的结果。例如：
vector u = { 1.0f, 0.0f, -3.0f, 1.0f};
vector v = {-4.0f, 0.0f, 1.0f, 1.0f};
vector b = (u == v); // b = (false, true, false, true)

最后，我们以讨论二元操作的变量提升（promotion）作为结束：

* 对于二元操作，如果（操作符的）左边和右边维数不同，则维数较少的一边提升（强制转换）到具有和维数较大的一边相同的维数。例如，如果x的类型为float，而y的类型为float3，在表达式(x + y)中变量x被提升到float3，并且计算出来的表达式的值的类型也为float3。提升使用已定义的转换完成。注意，若转换未定义则提升也是未定义的。例如，我们不能转换float2到float3，因为没有定义这个转换。
* 对于二元操作，如果左边和右边类型不同，那么较低精度的类型（the lower type resolution）被提升（强制转换）到具有同类型的较高精度的类型（the higher type resolution）。例如，如果x类型为int，y类型为half，则表达式(x + y)中的变量x被提升到half，并且计算出来的表达式的值的类型也为half。

16.6 用户定义函数
HLSL中的函数有下例属性：

* 函数使用类似C++的语法
* 参数总是按值传递
* 递归不被支持
* 函数总是inline的

此外，函数还加上了一些用于其上的额外的关键字。例如，考虑一个写在HLSL中的下面这个函数：
bool foo(in const bool b,   // input bool
         out int r1,        // output int
         inout float r2)    // input/output float
{
     if( b )               // test input value
     {
          r1 = 5;          // output a value through r1
     }
     else
     {
          r1 = 1;          // output a value through r1
     }
     
     // since r2 is inout we can use it as an input
     // value and also output a value through it
     r2 = r2 * r2 * r2;
     
     return true;
}

函数几乎和C++函数是一样的，除了in，out和inout关键字：

* in——指定型参（argument，特指传递给实参的变量）应该在函数开始前被拷贝给实参。传入参数不必强制指定，因为实参默认是in的。例如，下面两段是等价的：
float square(in float x)
{
     return x * x;
}

也可以不强制指定in：
float square(float x)
{
     return x * x;
}

* out——指定实参应该在函数返回时被拷贝给型参。这样可以通过参数返回值。out关键字是必须的，因为HLSL不允许传递一个引用或一个指针。我们要注意：如果实参标记为out，在函数开始前，型参就不拷贝给实参。换句话说，out实参仅可以被用于输出数据——它不能用于输入。
void square(in float x, out float y)
{
     y = x * x;
}
这里，我们输入了要被乘方的数x，并且通过参数y返回了x的乘方。

* inout——这是一个指示实参既用于输入又用于输出的快捷方法。如果要使用实参同时用作输入和输出，就指定inout。
void square(inout float x)
{
     x = x * x;
}
这里，我们输入了要被乘方的数x，同时又通过x返回了的x的乘方。

16.7内建函数
   HLSL有一个丰富的内建函数的集合，它们对3D图形来说非常有用。下表是一个删减了的列表。在下两章中，我们会使用这些函数中的一些进行实践。而现在，熟悉它们就够了。

注意：要得到更多的参考，可以参看DirectX文档中内建HLSL函数的完整列表，在Content页下，然后到DirectX Graphics\Reference\Shader Reference\High Level Shader Language\Intrinsic Functions。

译者注：以下表格中，//<variable>//表示变量variable的模（例如向量的绝对值）。
函数
描述
abs(x)
返回 |x|
ceil(x)
返回 ≥ x 的最小整数
clamp(x, a, b)
clamp(x, a, b)
clamp(x, a, b)
clamp(x, a, b)
cross(u, v)
返回 u × v（叉积）
degrees(x)
转换 x 从弧度到角度
determinant(M)
返回矩阵M的行列式det(M)
distance(u, v)
返回u点和v点之间的距离||v - u||
dot(u, v)
返回 u · v（点积）
floor(x)
返回 ≤ x 的最大整数
length(v)
返回 ||v||
lerp(u, v, t)
在u和v之间线性插值，根据参数 t ? [0, 1 ]
log(x)
返回 ln(x)
log10(x)
返回 log10(x)
log2(x)
返回 log2(x)
max(x, y)
如果x ≥ y，则返回 x；否则返回 y
min(x, y)
如果 x ≤ y，返回x；否则返回 y
mul(M, N)
返回矩阵乘积 MN. 注意：矩阵乘积必须是已定义的. 如果M是一个向量，它被作为一个行向量，则向量－矩阵（vector-matrix）乘法是已定义的。类似的,如果N 是一个向量，他被作为一个列向量，则矩阵－向量（matrix-vector）乘法是已定义的
normalize(v)
返回 v/∥v∥
pow(b, n)
返回 bn
radians(x)
转换 x 从 角度 到 弧度
reflect(v, n)
给定向量v和表面法线n，计算其反射向量
refract(v,n, eta)
给定向量v、表面法线n和两种材质的两个索引的比率eta，计算其折射向量. 翻看一下物理书中Snell的规则或者在互联网上搜索一下关于refraction（反射）的信息
rsqrt(x)
返回x的平方根的倒数
saturate(x)
返回clamp(x, 0.0, 1.0)
sin(x)
返回x的正弦,其中x单位为弧度
返回x的正弦,其中x单位为弧度
返回x的正弦和余弦，其中x单位为弧度
sqrt(x)
返回x的平方根
tan(x)
返回x的正切,其中 x 单位为弧度
transpose(M)
返回MT的转置

   大多数函数已经重载以使其可以对所有内建类型有意义。例如，abs对所有数值类型有意义，所以它为所有这些数值类型进行了重载。又例如，叉积的叉乘仅对3D向量有意义，所以它对所有类型的3D向量（比如：int，float，double的3D向量）进行了重载。另一方面，线性插值——lerp，对于数值、2D、3D和4D向量有意义，因此重载了这些类型。

注意：如果你传递进去一个非数值类型到一个（要求）数值类型的函数，也就是一个仅能对数值类型进行操作的函数（比如：cos(x)），那么这个函数会对传进去的每个分量进行操作。例如，你写：
floats v = float3 (0.0f, 0.0f, 0.0f);
v = cos(v);

那么函数将会对每个分量进行操作：v=(cos(x),cos(y),cos(z))。
下例展示了这些固有的函数可能被调用的方式：
float x = sin(1.0f);       // sine of 1.0f radian.
float y = sqrt(4.0f);      // square root of 4.

vector u = {1.0f, 2.0f, -3.0f, 0.0f};
vector v = {3.0f, -1.0f, 0.0f, 2.0f};
float  s = dot(u, v);      // compute dot product of u and v.

float3 i = {1.0f, 0.0f, 0.0f};
float3 j = {0.0f, 1.0f, 0.0f};
float3 k = cross(i, j);    // compute cross product of i and j.

matrix<float, 2, 2> M = {1.0f, 2.0f, 3.0f, 4.0f};
matrix<float, 2, 2> T = transpose(M); // compute transpose

16.8 摘要
* 我们在ASCII文本文件中编写了HLSL程序，并且在我们的应用程序中使用D3DXCompileShaderFromFile函数编译了它们。
* ID3DXConstantTable接口允许我们在应用程序中对着色器程序中的变量进行设置。这种通信是必须的，因为被着色器使用的变量可以按一帧一帧的变化而改变。例如，如果应用程序中的视图矩阵发生了改变，我们需要使用新的视图矩阵更新着色器的视图矩阵变量。我们可以用ID3DXConstantTable完成这种更新。
* 对于每个着色器，我们必须定义一个输入和一个输出结构，这些结构分别描述了着色器中输入和输出数据的格式。
* 每个着色器有一个入口点函数，它有一个输入结构参数用于传递数据进着色器。此外，每个着色器返回一个输出结构的实例，它用于从着色器输出数据。

