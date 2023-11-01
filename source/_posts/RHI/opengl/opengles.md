---
title: OpenGL ES 3.0
mathjax: true
date: 2019-05-12 17:18:55
tags:
    - RHI
    - opengl
categories: 
- RHI
- opengl
---

# Windows配置OpenGLES3开发环境

OpenGL ES 3.0实现了具有可编程着色功能的图形管线，由两个规范组成：OpenGL ES 3.0 API规范和OpenGL ES着色语言3.0规范（OpenGL ES SL）。下图展示了OpenGL ES 3.0图形管线。图中带有阴影的方框表示OpenGL ES 3.0中管线的可编程阶段。下面概述OpenGL ES 3.0 图形管线的各个阶段。
{%asset_img 1.jpg%}
### 顶点着色器
本节简单概述顶点着色器。顶点和片段着色器将在后面的章节中深入介绍。顶点着色器实现了顶点操作的通用可编程方法。
顶点着色器的输入包括：
- 着色器程序：描述顶点上执行操作的顶点着色器程序源代码或者可执行文件。
- 顶点着色器输入（或者属性）：用顶点数组提供的每个顶点的数据。
- 统一变量（uniform）：顶点（或者片段）着色器使用的不变数据。
- 采样器：代表顶点着色器使用纹理的统一变量类型。

顶点着色器的输出在OpenGL ES 2.0中称作可变（varying）变量，但在OpenGL ES 3.0中改名为顶点着色器输出变量。在图元光栅化阶段，为每个生成的片段计算顶点着色器输出值，并作为输入传递给片段着色器。用于从分配给每个图元顶点的顶点着色器输出生成每个片段值的机制称作插值（Interpolation）。此外，OpenGL ES 3.0增加了一个新功能----变化反馈，使顶点着色器输出可以选择性地写入一个输出缓冲区（除了传递给片段着色器之外，也可能代替这种传递）。例如，在第14章中介绍的变换反馈示例中，在顶点着色器中实现了一个粒子系统，其中的粒子用变换反馈输出到一个缓冲区对象。顶点着色器的输入和输出如下图所示。

{%asset_img 2.jpg%}

顶点着色器可以用于通过矩阵变换位置、计算照明公式来生成逐顶点颜色以及生成或者变换纹理坐标等基于顶点的传统操作。此外，因为顶点着色器由应用程序规定，所以它可以用于执行自定义计算，实施新的变换、照明或者较传统的固定功能管线所不允许的基于顶点的效果。
下面展示了用OpenGL ES着色语言编写的一个顶点着色器。我们将在本文后面详细说明顶点着色器，这里提供的着色器只是让你对顶点着色器有个概念。下面代码中的顶点着色器取得一个位置及相关的颜色数据作为输入属性，用一个4x4矩阵变换位置，并输出变换后的位置和颜色。
```c
#version 300 es
uniform mat4 u_mvpMatrix; // matrix to convert a_position from model space to normalized device space
// attributes input to the vertex shader
in vec4 a_position; // position value
in vec4 a_color; // input vertex color

// output of the vertex shader - input to fragment shader
out vec4 v_color; // output vertex color
void main()
{
    v_color = a_color;
    gl_Position = u_mvpMatrix * a_position;
}
```
第1行提供了着色语言的版本-这一信息必须出现在着色器的第1行（#version 300 es表示OpenGL ES着色语言v3.00）。第2行描述了统一变量u_mvpMatrix，它存储组合的模型视图和投影矩阵。第4行和第8行描述了顶点着色器的输入，被称作顶点属性。a_position是输入顶点位置属性，a_color是输入顶点颜色属性。第8行声明了输出变量v_color，用于存储描述每个顶点颜色的顶点着色器输出。内建变量gl_Position是自动声明的，着色器必须将变换后的位置写入这个变量。片段着色器的顶点有一个单一入口点，称作主函数。第9行~13行描述顶点着色器main函数。在第11行中，读入顶点属性输入a_color，并将其写入顶点输出颜色v_color。第12行中，变换后的顶点位置写入gl_Position输出。
### 图元装配
顶点着色器之后，OpenGL ES 3.0图形管线的下一阶段是图元装配。图元（Primitive）是**三角形、直线或者点精灵**等几何对象。图元的每个顶点被发送到顶点着色器的不同拷贝。在图元装配期间，这些顶点被组合成图元。
对于每个图元，必须确定图元是否位于视锥体（屏幕上可见的3D空间区域）内。如果图元没有完全在视锥体内，则可能需要进行裁剪。如果图元完全处于该区域之外，它就会被抛弃。裁剪之后，顶点位置杯转换为屏幕坐标。也可以执行一次淘汰操作，根据图元面向前方或者后方抛弃它们。裁剪和淘汰之后，图元便准备传递给管线的下一阶段----光栅化阶段。
### 光栅化
下一阶段是光栅化如下图，在此阶段绘制对应的图元（点精灵、直线或者三角形）。<font color="red">光栅化是将图元转化为一组二维片段的过程</font>，然后，这些片段由片段着色器处理。这些二维片段代表着可在屏幕上绘制的像素。
{%asset_img 3.jpg%}
### 片段着色器
片段着色器为片段上的操作实现了通用的可编程方法。如下图所示，对光栅化阶段生成的每个片段执行这个着色器，采用如下输入：
- 着色器程序----描述片段上所执行操作的片段着色器程序源代码或者可执行文件。
- 输入变量----光栅化单元用插值为每个片段生成的顶点着色器输出。
- 统一变量----片段（或者顶点）着色器使用的不变数据。
- 采样器----代表片段着色器所用纹理的特殊统一变量类型。

{%asset_img 4.jpg%}

片段着色器可以抛弃片段，也可以生成一个或者颜色值作为输出。一般来说，除了渲染到多重渲染目标之外，片段着色器只输出一个颜色值；在多重渲染目标的情况下，为每个渲染目标输出一个颜色值。光栅化阶段生成的颜色、深度、模板和屏幕坐标位置$(x_w,y_w)$变成OpenGL ES 3.0管线逐片段操作阶段的输入。
### 逐片段操作
在片段着色器之后，下一个阶段是逐片段操作。光栅化生成的屏幕坐标为$(x_w,x_y)$的片段只能修改帧缓存区中位置为$(x_w,y_w)$的像素。下图描述了OpenGL ES 3.0逐片段操作阶段。

{% asset_img 5.jpg %}

在逐片段操作阶段，在每个片段上执行如下功能：
- 像素归属测试----这个测试确定帧缓冲区中位置$(x_w,y_w)$的像素目前是不是归OpenGL ES所有。这个测试使窗口系统能够控制帧缓冲区中的哪些像素属于当前OpenGL ES上下文。例如，如果一个显示OpenGL ES帧缓冲区窗口的窗口被另一个窗口所遮蔽，则窗口系统可以确定被遮蔽的像素不属于OpenGL ES上下文，从而完全不显示这些像素。虽然像素归属测试OpenGL ES的一部分，但是它不由开发人员控制，而是在OpenGL ES内部进行。
- 裁剪测试----裁剪测试确定$(x_w,y_w)$是否位于作为OpenGL ES状态的一部分的裁剪矩形范围内。如果该片段位于裁剪区域之外，则被抛弃。
- 模板和深度测试----这些测试在输入片段的模板和深度值上进行，以确定片段是否应该被拒绝。
- 混合----混合将生成的片段颜色值与保存在帧缓冲区$(x_w,y_w)$位置的颜色值组合起来。
- 抖动----抖动可用于最小化因为使用有限精度在帧缓冲区中保存颜色值而产生的假象。

在逐片段操作阶段的最后，片段或者被拒绝，或者在帧缓冲区的$(x_w,y_w)$位置写入片段的颜色、深度或者模板值。写入片段颜色、深度和模板值取决于启用的相应写入掩码。写入掩码可以更精细地控制写入相关缓冲区的颜色、深度和模板值。例如，可以设置颜色缓冲区的写入掩码，使得任何红色值不被写入颜色缓冲区。此外，OpenGL ES 3.0提供一个接口，以从帧缓冲区读回像素。

## OpenGL ES 3.0新功能
### 纹理
- sRGB纹理和帧缓冲区----允许应用程序执行伽马矫正渲染。纹理可以保存在经过伽马矫正的sRGB空间，在着色器中读取时反矫正到线性空间，然后在输出到帧缓冲区时换回sRGB伽马矫正空间。通过在线性空间中正确地进行照明和其他计算，可能得到更高的视觉保真度。
- 2D纹理数组----保存一组2D纹理的纹理目标。例如，这些数组可以用于执行纹理动画。在2D纹理数组出现之前，这种动画一般通过在单个2D纹理中平铺动画帧并修改纹理坐标改变动画帧实现。有了2D纹理数组，动画的每个帧可以在数组的一个2D切片中指定。
- 3D纹理----一些OpenGL ES 2.0实现通过扩展支持3D纹理，而OpenGL ES 3.0将此作为强制的功能。3D纹理对于许多医学成像应用是必不可少的，例如执行三维数据（如CT、MRI或者PET数据）直接体渲染的程序。
- 深度纹理和阴影比较----启用存储在纹理中的深度缓冲区。深度纹理的最常见用途是渲染阴影，这时深度缓冲区从光源的角度渲染，然后用于在渲染场景时比较，以确定片段是否在阴影中。除了深度纹理之外，OpenGL ES 3.0可以在读取时比较深度纹理，从而在深度纹理上完成双线性过滤（也称作百分比渐进过滤（PCF））。
- 无缝立方图----在OpenGL ES 2.0中，利用立方图渲染可能在立方图之间的边界产生伪像。
- 浮点纹理----OpenGl ES 3.0大大扩展了所支持的纹理格式，支持并可以过滤浮点半浮点（16位）纹理，也支持全浮点（32位）纹理，但是不能过滤。访问浮点纹理数据的能力有许多应用，包括高动态范围纹理和多功能计算。
- ETC2/EAC纹理压缩----多种OpenGL ES 2.0实现支持供应商专用压缩纹理格式（例如高通的ATC、Imagination Technologies的PVRTC）。在OpenGl ES 3.0中强制支持ETC2/EAC。ETC2/EAC的格式为RGB888、RGB8888和单通道及双通道有符号/无符号纹理数据。纹理压缩具有很多好处，包括更好的性能（因为更好地利用了纹理缓存）以及减少GPU内存占用。
- 整数纹理----OpenGL ES 3.0引入了渲染和读取保存为未规范化有符号或者无符号8位、16位和32位整数纹理的能力。
- 其他纹理格式----除了前面提到的格式之外，OpenGL ES 3.0还包含了对11-11-10RGB浮点纹理、共享指数RGB 9-9-9-5纹理、10-10-10-2整数纹理以及8位分量有符号规范化纹理的支持。
- 非2幂次纹理（NPOT）----纹理现在可以指定为不为2的幂次的尺寸。这在许多情况下都很有用，例如来自视频或者摄像头的以不为2的幂次尺寸捕捉/记录的纹理。
- 纹理细节级别（LOD）功能----现在，可以强制使用用于确定读取哪个Mipmap的LOD参数。此外，可以强制基本和最大Mipmap级别。这两个功能组合起来，可以流化Mipmap。在更大的Mipmap级别可用时，可以提供基本级别，LOD值可以平滑地增加，以提供平滑的流化纹理。这一功能非常实用，例如，用于通过网络连接下载纹理Mipmap。

|后缀|整数类型|C语言类型|GL类型|
|--|--|--|--|
|b|8位有符号整数|signed char|GLbyte|
|ub|8位无符号整数|unsigned char|GLubyte、GLboolean|
|s|16位有符号整数|short|GLshort|
|us|16位无符号整数|unsigned short|GLushort|
|i|32位有符号整数|int|GLint|
|ui|32位无符号整数|unsigned int|GLuint、GLbitfield、GLenum|
|x|16.16定点数|int|GLfixed|
|f|32位浮点数|float|GLfloat、GLclampf|
|i64|64位整数|khronos_int64_t(取决于平台)|GLint64|
|ui64|64位无符号整数|khronos_uint64_t(取决于平台)|GLuint64|
最后，OpenGL ES定义了GLvoid类型。该类型用于接受指针的OpenGL ES命令。
## 错误处理
若不正确使用OpenGL ES命令，应用程序会生成一个错误代码。这个错误代码将被记录，可以用glGetError查询。在应用程序用glGetError查询第一个错误代码之前，不会记录其他错误。一旦查询到错误代码，当前错误代码便被复位为GL_NO_ERROR。

错误代码|描述
--|--
GL_NO_ERROR|从上一次调用glGetError以来没有生成任何错误
GL_INVALID_ENUM|GLenum参数超出范围。忽略生成该错误的命令
GL_INVALID_VALUE|数值型参数超出范围。忽略生成这个错误的命令
GL_INVALID_OPERATION|特定命令在当前OpenGL ES状态下不能执行。忽略生成该错误的命令
GL_OUT_OF_MEMORY|内存不足时执行该命令。如果遇到这个错误，除非是当前错误代码，否则OpenGL ES管线的状态被认为未定义

## 基本状态管理
每个管线阶段都有一个可以启用或者警用的状态，每个上下文维护相应的状态值。状态的例子有混合启用、混合因子、剔除启用、提出曲面。在初始化OpenGL ES上下文(EGLContext)时，状态用默认值初始化。启用状态可以用glEnable和glDisable命令设置。
```c
void glEnable(GLenum cap);
void glDisable(GLenum cap);
```
glEnable和glDisable启用各种功能。除了GL_DITHER被设置为GL_TRUE之外，其他功能的初始化均被设置为GL_FALSE。如果cap不是有效的状态枚举值，则生成错误代码GL_INVALID_ENUM。
cap是要启用或者禁用的状态，可以是：
```c
GL_BLEND
GL_CULL_FACE
GL_DEPTH_TEST
GL_POLYGON_OFFSET_FILL
GL_PRIMITIVE_RESTART_FIXED_INDEX
GL_SAMPLE_APLHA_TO_COVEARAGE
GL_SAMPLE_COVERAGE
GL_SCISSOR_TEST
GL_STENCIL_TEST
```
# 你好，三角形：一个OpenGL ES 3.0实例
- 用EGL创建屏幕上的渲染表面
- 加载顶点和片段着色器
- 创建一个程序对象，连接顶点和片段着色器，并链接程序对象
- 设置视口
- 清除颜色缓冲区
- 渲染简单图元
- 时颜色缓冲区的内从在EGL窗口表面中可见

## 显示后台缓冲区
终于到了将三角形绘制到帧缓冲区的时候了。现在，我们必须介绍最后一个细节：如何在屏幕上真正显示帧缓冲区的内容。在介绍这一点之前，我们先讨论双缓冲区（Double Buffering）的概念。
屏幕上可见的帧缓冲区由一个像素数据的二维数组表示。我们可以将屏幕上显示图像视为绘制时简单地更新可见帧缓冲区中地像素数据。但是，直接在可显示缓冲区上更新像素有一个严重的问题--也就是说，在典型的显示系统中，物理屏幕以固定的速率从帧缓冲区内存中更新。如果我们直接绘制到帧缓冲区，那么用户在部分更新帧缓冲区时会看到伪像。
为了解决这个问题，我们使用所谓的双缓冲区。在这种方案中，有两个缓冲区：前台缓冲区和后台缓冲区。所有渲染都发生在后台缓冲区，它位于不可见于屏幕的内存区域。当所有渲染完成后，这个缓冲区被“交换”到前台缓冲区（或者可见缓冲区）。然后前台缓冲区变成下一帧的后台缓冲区。
使用这种技术，我们在一帧上的所有渲染完成之前不显示可见表面。在OpenGL ES应用程序中，这种活动通过EGL函数eglSwapBuffers控制（我们的框架在调用Draw回调函数之后调用该函数）：
`eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);`
这个函数通知EGL切换前台缓冲区和后台缓冲区。发送到eglSwapBuffers的参数时EGL显示器和表面。这两个参数分别代表物理显示器和渲染表面。

|属性|描述|默认值|
|--|--|--|
|EGL_BUFFER_SIZE|颜色缓冲区中所有颜色分量的位数|0|
|EGL_RED_SIZE|颜色缓冲区中红色分量位数|0|
|EGL_GREEN_SIZE|颜色缓冲区中绿色分量位数|0|
|EGL_BLUE_SIZE|颜色缓冲区中蓝色分量位数|0|
|EGL_LUMINANCE_SIZE|颜色缓冲区中亮度位数|0|
|EGL_ALPHA_SIZE|颜色缓冲区中Alpha值位数|0|
|EGL_ALPHA_MASK_SIZE|掩码缓冲区中Alpha掩码位数|0|
|EGL_BIND_TO_TEXTURE_RGB|如果可以绑定到RGB纹理，则为真|EGL_DONT_CARE|
|EGL_BIND_TO_TEXTURE_RGBA|如果可以绑定到RGBA问你，则为真|EGL_DONT_CARE|
|EGL_COLOR_BUFFER_TYPE|颜色缓冲区类型：EGL_RGB_BUFFER或EGL_LUMINANCE_BUFFER|EGL_RGB_BUFFER|
|EGL_CONFIG_CAVEAT|和配置相关的任何注意事项|EGL_DONT_CARE|
|EGL_CONFIG_ID|唯一的EGLConfig标识符值|EGL_DONT_CARE|
|EGL_CONFORMANT|如果用这个EGLConfig创建的上下文兼容，则为真|-|
|EGL_DEPTH_SIZE|深度缓冲区位数|0|
|EGL_LEVEL|帧缓冲区级别|0|
|EGL_MAX_PBUFFER_WIDTH|用这个EGLConfig创建的PBuffer的最大宽度|-|
|EGL_MAX_PBUFFER_HEIGHT|用这个EGLConfig创建的PBuffer的最大高度|-|
|EGL_MAX_PBUFFER_PIXELS|用这个EGLConfig创建的PBuffer的最大尺寸|-|
|EGL_MAX_SWAP_INTERVAL|最大缓冲区交换间隔|EGL_DONT_CARE|
|EGL_MIN_SWAP_INTERVAL|最小缓冲区交换间隔|EGL_DONT_CARE|
|EGL_NATIVE_RENDERABLE|如果原生渲染库可以渲染到用EGLConfig创建的表面，则为真|EGL_DONT_CARE|
|EGL_NATIVE_VISUAL_ID|关于应原生窗口系统可视ID句柄|EGL_DONT_CARE|
|EGL_NATIVE_VISUAL_TYPE|关于原生窗口系统可视类型|EGL_DONT_CARE|
|EGL_RENDERABLE_TYPE|由EGL_OPENGL_ES_BIT、EGL_OPENGL_ES2_BIT、EGL_OPENGL_ES3_BIT_KHR(需要EGL_KHR_create_context扩展)、EGL_OPENGL_BIT或EGL_OPENVG_BIT组成的位掩码，代表配置支持的渲染接口|EGL_OPENGL_ES_BIT|
|EGL_SAMPLE_BUFFERS|可用多重采样缓冲区数量|0|
|EGL_SAMPLES|每个像素的样本数量|0|
|EGL_STENCIL_SIZE|模板缓冲区为主|0|
|EGL_SURFACE_TYPE|支持的EGL表面类型，可能是EGL_WINDOW_BIT、EGL_PIXMAP_BIT、EGL_PBUFFER_BIT、EGL_MULTISAMPLE_RESOLVE_BOX_BIT、EGL_SWAP_BEHAVIOR_PRESERVED_BIT、EGL_VG_COLORSPACE_LINER_BIT或者EGL_VG_ALPHA_FORMAT_PRE_BIT|EGL_WINDOW_BIT|
|EGL_TRANSPARENT_TYPE|支持的透明度|EGL_NONE|
|EGL_TRANSPARENT_RED_VALUE|解读为透明的红色值|EGL_DONT_CARE|
|EGL_TRANSPARENT_GREEN_VALUE|解读为透明的绿色值|EGL_DONT_CARE|
|EGL_TRANSPARENT_BLUE_VALUE|解读为透明的蓝色值|EGL_DONT_CARE|

## 与窗口系统通信

EGL提供了OpenGL ES 3.0（和其他Khrons图形API）和运行于计算机上的原生窗口系统（如GNU/Linux系统上常见的X Window系统、Microsoft Windows或者Mac OS X的Quartz）之间的一个“结合”层次。在EGL能够确定可用的绘制表面类型（或者底层系统的其他特性）之前，它必须打开和窗口系统的通信渠道。注意，Apple提供自己的EGL API的iOS实现，称为EAGL。
因为每个窗口系统都有不同的语义，所以EGL提供基本的不透明类型--EGLDisplay，该类型封装了所有系统相关，用于和原生窗口系统接口。任何使用EGL的应用程序必须执行的第一个操作是创建和初始化与本地EGL显示的连接。这采用例3-1所示的两次调用序列完成。
例3-1 初始化EGL
```
EGLint majorVersion;
EGLint minorVersion;

EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
if (display == EGL_NO_DISPLAY)
{
    // Unable to open connection to local windowinng system
}

if (!eglInitialize(display,&majorVersion,&minorVersion))
{
    // Unable to initialize EGL; handle and recover
}
```
调用如何函数打开与EGL显示服务器的连接：
`EGLDisplay eglGetDisplay(EGLNativeDisplayType displayId)`
displayId 指定显示连接，默认连接为EGL_DEFAULT_DISPLAY

## 检查错误
EGL中的大部分函数在成功时返回EGL_TRUE，否则返回EGL_FALSE。但是，EGL所做的不仅是告诉你调用是否失败--它将记录错误，指示故障原因。不过，这个错误代码不会直接返回给你；你需要明确地查询EGL错误代码，为此可以调用如何函数完成：
`EGLint eglGetError()`
## 初始化EGL
成功地打开连接之后，需要初始化EGL，这通过调用如何函数完成：
`EGLBoolean eglInitialize(EGLDisplay display,EGLint *majorVersion,EGLint *minorVersion)`
display         指定EGL显示连接
majorVersion    指定EGL实现返回的主版本号，可能为NULL
minorVersion    指定EGL实现返回的次版本号，可能为NULL
## 确定可用的表面配置
一旦初始化了EGL，就可以确定可用渲染表面的类型和配置，这有两种方法：
- 查询每个表面配置，找出最好的选择。
- 指定一组需求，让EGL推荐最佳匹配。

## 查询EGLConfig属性
现在，我们说明与EGLConfig相关的EGL值，并说明如何检索这些值。
EGLConfig包含关于EGL启用的表面的所有信息。这包括关于可用颜色、与配置相关的其他缓冲区（如后面将要讨论的深度和模版缓冲区）、表面类型和许多其他特性。下面是可以从EGLConfig中查询的属性的一个列表。在本章中我们只讨论一个子集，表3-1中列出了完整的列表作为参考。
使用如何函数可以查询与EGLConfig相关的特定属性：
`EGLBoolean eglGetConfigAttrib(EGLDisplay display,EGLConfig config,EGLint attribute,EGLint *value)`
display 指定EGL显示连接
config 指定要查询的配置
attribute 指定返回的特定属性
value 指定返回值
上述函数在调用成功时返回EGL_TRUE，失败时返回EGL_FALSE，如果attribute不是有效的属性，则还要返回EGL_BAD_ATTRIBUTE错误。
## 创建屏幕上的渲染区域：EGL窗口
```c
EGLSurface eglCreateWindowSurface(EGLDisplay display, // 指定EGL显示连接
    EGLConfig config, // 指定配置
    EGLNativeWindowType window, // 指定原生窗口
    const EGLint * attribList // 指定窗口属性列表；可能为NULL
    );
```
下表是eglCreatePbufferSurface失败时可能的错误

|错误代码|描述|
|--|--|--|
|EGL_BAD_ALLOC|pbuffer因为缺乏资源而无法分配时发生这种错误|
|EGL_BAD_CONFIG|如果提供的EGLConfig不是系统支持的有效配置，则发生这种错误|
|EGL_BAD_PARAMETER|如果属性列表中提供的EGL_WIDTH或EGL_HEIGHT是负值，则产生这种错误|
## 同步渲染
你可能会碰到一些情况，即需要协调多个图形API在单个窗口中的渲染。例如，你可能发现使用OpenVG更容易找到比OpenGL ES 3.0更适于窗口绘制字符的原生窗口系统字体渲染函数。在这种情况下，需要让应用程序允许多个库渲染到共享窗口。EGL有几个函数有助于同步任务。
## 着色器和程序
需要创建两个基本对象才能用着色器进行渲染：着色器对象和程序对象。理解着色器对象和程序对象的最佳方式是将它们比作C语言的编译器和链接程序。C编译器为一段源代码生成目标代码（例如，.obj或者.o文件）。在创建目标文件之后，C链接程序将对象文件链接为最后的程序。
编译之后，着色器对象可以连接到一个程序对象。程序对象可以连接多个着色器对象。在OpenGL ES中，每个程序对象必须连接一个顶点着色器和一个片段着色器（不多也不少），这和桌面OpenGL不同。程序对象被链接为用于渲染的最后“可执行程序”。
获得链接后的着色器对象的一般过程包括6个步骤：
1. 创建一个顶点着色器对象和一个片段着色器对象。
2. 将源代码连接到每个着色器对象。
3. 编译着色器对象。
4. 创建一个程序对象。
5. 将编译后的着色器对象连接到程序对象。
6. 链接程序对象。

### 创建和编译一个着色器
使用着色器对象的第一步是创建着色器，着用glCreateShader完成。
```c
GLuint glCreateShader(GLenum type//创建的着色器类型可以是GL_VERTEX_SHADER或者GL_FRAGMENT_SHADER
    );
```
调用glCreateShader将根据传入的type参数创建一个新的顶点或者片段着色器。返回值是指向新着色器对象的句柄。当完成着色器对象时，可以用glDeleteShader删除。
```c
void glDeleteShader(GLuint shader // 要删除的着色器对象的句柄
    );
```
如果一个着色器链接到一个程序对象，那么调用glDeleteShader不会立刻删除着色器，而是将着色器标记为删除，在着色器不再连接到任何程序对象时，它的内存将被释放。
一旦创建了着色器对象，下一件事通常时用glShaderSource提供着色器源代码。
```c
glShaderSource(GLuint shader,// 指向着色器对象的句柄
    GLsizei count,//着色器源字符串的数量。着色器可以由多个源字符串组成，但是每个着色器只能有一个main函数。
    const GLchar* const *string,// 指向保存数量为count的着色器源字符串的数组指针。
    const GLint *length);//指向保存每个着色器字符串大小且元素数量为count的整数数组指针。如果length为NULL，着色器字符串被认定为空。如果length不为NULL，则它的每个元素保存对应于string数组的着色器的字符数量。如果任何元素的length值均小于0，则该字符串被认定以null结束。
```
```c
void glLinkProgram(GLuint program//指向程序对象的句柄
    );
```
链接操作负责生成最终的可执行程序。链接程序将检查各种对象的数量，确保成功链接。
链接程序之后，你必须检查链接是否成功，可以使用glGetProgramiv检查链接状态。
```c
void glGetProgramiv(GLuint program, // 需要获取信息的程序对象句柄
    GLenum pname,
    GLint *params // 指向查询结果整数存储位置的指针
    );
```
pname 获取信息的参数，可以是：
```
GL_ACTIVE_ATTRIBUTES
GL_ACTIVE_ATTRIBUTE_MAX_LENGTH
GL_ACTIVE_UNIFORM_BLOCK
GL_ACTIVE_UNIFORM_BLOCK_MAX_LENGTH
GL_ACTIVE_UNIFORMS
GL_ACTIVE_UNIFORM_MAX_LENGTH
GL_ATTACHED_SHADERS
GL_DELETE_STATUS
GL_INFO_LOG_LENGTH
GL_LINK_STATUS
GL_PROGRAM_BINARY_RETRIEVABLE_HINT
GL_TRANSFORM_FEEDBACK_BUFFER_MODE
GL_TRANSFORM_FEEDBACK_VARYINGS
GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH
GL_VALIDATE_STATUS
```
## 统一变量和属性
一旦链接了程序对象，就可以在对象上进行许多查询。首先，你可能需要找出程序中的活动统一变量。统一变量（uniform）---- 在关于着色语言的下一章中详细介绍----是存储应用程序通过OpenGL ES 3.0 API传递给着色器的只读常数值的变量。
统一变量被组合成两类统一变量块。第一类是命名统一变量块，统一变量的值由所谓的统一变量缓冲区对象（下面将详细介绍）支持。命名统一变量块被分配一个统一变量块索引。下面的例子声明一个名为TransformBlock并包含3个统一变量（matViewProj、matNormal和matTexGen）的统一变量块：
```
uniform TransformBlock
{
    mat4 matViewProj;
    mat3 matNormal;
    mat3 matTexGen;
};
```
第二类是默认的统一变量块，用于在命名块统一变量块之外声明的统一变量。和命名统一变量块不同，默认统一变量块没有名称或者统一变量块索引。下面的例子在命名统一变量块之外生命同样的3个统一变量：
```
uniform mat4 matViewProj;
uniform mat3 matNormal;
uniform mat3 matTexGen;
```
我们将在5.14节更详细地说明统一变量块。
如果统一变量在顶点着色器和片段着色器中均有声明，则声明的类型必须相同，且在两个着色器中的值也需相同。在链接阶段，链接程序将为程序中与默认统一变量块相关的活动统一变量指定位置。
### 获取和设置统一变量
要查询程序中活动统一变量的列表，首先要用GL_ACTIVE_UNIFORMS参数调用glGetProgramiv。这样可以获得程序中活动统一变量的数量。这个列表包含命名统一变量块中的统一变量、着色器代码中生命的默认统一变量块中统一变量以及着色器代码中使用的内建统一变量。如果统一变量被程序使用，就认为它是“活动”的。换言之，如果你在一个着色器中声明了一个统一变量，但是从未使用，链接程序可能会在优化时将其去掉，不在活动统一变量列表中饭回。你还可能发现程序中最大统一变量名称的字符数量（包括null终止符）；这可以用GL_ACTIVE_UNIFORM_MAX_LENGTH参数调用glGetProgramiv获得。
知道活动统一变量和存储统一变量名称所需的字符数之后，我们可以用glGetActiveUniform和glGetActiveUniformsiv找出每一个统一变量的细节。
`void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);`
program 程序对象句柄
index 查询的统一变量索引
bufSize 名称数组中的字符数
length 如果不是NULL，则是名称数组中写入的字符数（不含null终止符）
size 如果查询的统一变量时个数组，这个变量便将写入程序中使用的最大数组元素（加1）；如果查询的统一变量不是数组，则该值为1
type 将写入统一变量类型；可以为：
```
GL_FLOAT,GL_FLOAT_VEC2,GL_FLOAT_VEC3,GL_FLOAT_VEC4,
GL_INT,GL_INT_VEC2,GL_INT_VEC3,GL_INT_VEC4,GL_UNSIGNED_INT,GL_UNSIGNED_INT_VEC2,GL_UNSIGNED_INT_VEC3,
GL_UNSIGNED_INT_VEC4,GL_BOOL,GL_BOOL_VEC2,GL_BOOL_VEC3,GL_BOOL_VEC4,GL_FLOAT_MAT2,
GL_FLOAT_MAT3,GL_FLOAT_MAT4,GL_FLOAT_MAT2x3,GL_FLOAT_MAT2x4,GL_FLOAT_MAT3x2,GL_FLOAT_MAT3x4,
GL_FLOAT_MAT4x2,GL_FLOAT_MAT4x3,GL_SAMPLER_2D,GL_SAMPLER_3D,GL_SAMPLER_CUBE,
GL_SAMPLER_2D_SHADOW,GL_SAMPLER_2D_ARRAY,GL_SAMPLER_2D_ARRAY_SHADOW,
GL_SAMPLER_CUBE_SHADOW,GL_INT_SAMPLER_2D,GL_INT_SAMPLER_3D,GL_INT_SAMPLER_CUBE,
GL_INT_SAMPLER_2D_ARRAY,
GL_UNSIGNED_INT_SAMPLER_2D,
GL_UNSIGNED_INT_SAMPLER_3D,
GL_UNSIGNED_INT_SAMPLER_CUBE,
GL_UNSIGNED_INT_SAMPLER_2D_ARRAY
```
name 写入统一变量名称，最大字符数为bufSize,这是一个null终止的字符串。
`void glGetActiveUniformsiv(GLuint program, GLsizei count, const GLuint *indeices, GLenum pname, GLint *params);`
program 程序对象句柄
count 索引（indices）数组中的元素数量
indices 统一变量索引列表
pname   统一变量索引中每个统一变量的属性，将被写入params元素；可以是：
        GL_UNIFORM_TYPE, GL_UNIFORM_SIZE，
        GL_UNIFORM_NAME_LENGTH,GL_UNIFORM_BLOCK_INDEX,
        GL_UNIFORM_OFFSET, GL_UNIFORM_ARRAY_STRIDE，
        GL_UNIFORM_MATRIX_STRIDE, GL_UNIFORM_IS_ROW_MAJOR
params 将被写入由对应统一变量的pname所指定的结果
### 统一变量缓冲区对象
可以使用缓冲区对象存储统一变量数据，从而在程序中的着色器之间甚至程序之间共享统一变量。这种缓冲区对象乘坐统一变量缓冲区对象。使用统一变量缓冲区对象，你可以在更新大的统一变量块时降低API开销。此外，这种方法增加了统一变量的可用存储，因为你的可以不受默认统一变量块大小的限制。
要更新统一变量缓冲区对象中的统一变量数据，你可以用glBufferData、glBufferSubData、glMapBufferRange和glUnmapBuffer等命令修改缓冲区对象的内容。
在统一变量缓冲区对象中，统一变量在内存中以如下形式出现：
- 类型为bool、int、uint和float的成员保存在内存的特定偏移，分别作为单个unity、int、unit和float类型的分量。
- 基本数据类型bool、int、uint或者float的向量保存在始于特定便宜的连续内存位置中，第一个分量在最低偏移处。
- C列R行的列优先矩阵被当成C浮点列向量的一个数组对待，每个向量包含R个分量。相类似，R行C列的行优先矩阵被当成R浮点行向量的一个数组，每个向量包含C个分量。列向量或者行向量连续存储，但是有些实现的存储中可能有缺口。矩阵中两个向量之间的偏移量被称作列跨距或者行跨距（GL_UNIFORM_MATRIX_STRIDE），可以在链接的程序中用glGetActiveUniformsiv查询。
- 标量、向量和矩阵的数组按照元素的顺序存储与内存中，成员0放在最低偏移处。数组中每个元素之间的偏移量时一个常熟，称作数组跨距（GL_UNIFORM_ARRAY_STRIDE），可以在链接的程序中用glGetActiveUniformsiv查询。

## 着色器编译器
当你要求OpenGL ES编译和链接着色器时，光花一点时间思考OpenGL ES实现必须做到的事。着色器代码通常解析为某种中间表现形式，这和大部分编译语言相同（例如，抽象语法树）。编译器必须将抽象表现形式转化为硬件的机器指令。理想状态下，这个编译器还应该进行大量的优化，例如无用代码删除、常量传播等。进行这些工作需要付出代价----主要是CPU时间和内存。
OpenGL ES 3.0实现必须支持在线着色器编译（用glGetBooleanv检索的GL_SHARDER_COMPILER值必须是GL_TRUE）。你可以指定着色器使用glShaderSource，就像我们在示例中所做的那样。你还可以尝试缓解着色器编译对资源的影响。也就是说，一旦完成了应用程序中着色器的编译，就可以调用glReleaseShaderCompiler。这个函数提示OpenGL ES实现你已经完成了着色器编译器的工作，可以释放它的资源了。注意，这个函数只是一个提示，如果决定用glCompileShader编译更多的着色器，那么OpenGL ES实现需要重新为编译器分配资源。
`void glReleaseShaderCompiler(void)`
提示OpenGL ES实现可以释放着色器编译器使用的资源。因为这个函数只是个提示，所以有些实现可能忽略对这个函数的调用。
## 程序二进制码
程序二进制码是完全编译和链接的程序的二进制表现形式。它们很有用，因为可以保存到文件系统供以后使用，从而避免在线编译的代价。你也可以使用程序二进制码，这样就没有必要在实现中分发着色器源代码。

# OpenGL ES着色语言
在前面几章中你已经看到，着色器是OpenGL ES 3.0API的一个基础和兴概念。每个OpenGL ES 3.0程序都需要一个顶点着色器和片段着色器，以渲染有意义的图片。考虑到着色器是API概念的核心，我们希望确保你在深入了解图形API的更多细节之前，掌握编写着色器的基础知识。
本章的目标是确保你理解着色语言中的如下概念：
- 变量和变量类型
- 向量和矩阵的构造及选择
- 常量
- 结构和数组
- 运算符、控制流和函数
- 输入/输出变量、统一变量、统一变量块和布局限定符
- 预处理器和指令
- 统一变量和插值器打包
- 精度限定符和不变性

我们在第2章中的例子里已经介绍了这些概念的少数细节。现在，我们将用更多的细节来充实这些概念，确保你理解如何编写和阅读着色器。
## OpenGL ES着色语言基础知识
在阅读本书时，你会看到许多着色器。如果你开始开发自己的OpenGL ES 3.0应用程序，则很有可能编写许多着色器。目前，你应该已经理解了着色器作用的基本概念以及它融入管线的方式。如果还不理解，可以复习第1张，在那里我们介绍了管线，并且描述了顶点着色器和片段着色器融入其中的方式。
现在我们要关注的是着色器究竟是由什么组成的。你可能已经观察到，着色器的语法和C编程语言有很多相似之处。如果你能够理解C代码，理解着色器的语法就没有太大的难度。但是，两种语言之间当然有一些重大的区别，首先是版本规范和所支持的原生数据类型。
## 着色器版本规范
`#version 300 es`
没有声明版本号的着色器被认定使用OpenGL ES着色语言的1.00版本。着色语言的1.00版本用于OpenGL ES 2.0。对于OpenGL ES 3.0，规范的作者决定匹配API和着色语言的版本号。
## 变量和变量类型
在计算机图形中，两个基本数据类型组成了变换的基础：向量和矩阵。这两种数据类型在OpenGL ES着色语言中也是核心。表5-1具体描述了着色语言中存在的基于标量、向量和矩阵的数据类型。

表 5-1 OpenGL ES 着色语言中的数据类型

|变量分类|类型|描述|
|--|--|--|
|标量|float,int,uint,bool|用于浮点、整数、无符号整数和布尔值的基于标量的数据类型|
|浮点向量|float,vec2,vec3,vec4|有1、2、3、4个分量的基于浮点的向量类型|
|整数向量|int,ivec2,ivec3,ivec4|有1、2、3、4个分量的基于整数的向量类型|
|无符号整数向量|uint,uvec2,uvec3,uvec4|有1、2、3、4个分量的基于无符号整数的向量类型|
|布尔向量|bool,bvec2,bvec3,bvec4|有1、2、3、4个分量的基于无符号整数的向量类型|
|矩阵|mat2（或mat2x2),mat2x3,mat2x4,mat3x2,mat3(或mat3x3),mat3x4,mat4x2,mat4x3,mat4(或mat4x4)|2x2,2x3,2x4,3x2,3x3,3x4,4x2,4x3或4x4的基于浮点的矩阵|

##　变量构造器
OpenGl ES着色语言在类型转换方面有非常严格的规则。也就是说，变量只能赋值为相同类型的其他变量或者相同类型的变量进行运算。在语言不允许隐含类型转换的原因是，这样可以避免着色器作者遇到可能导致难以跟踪的缺陷的意外转换。

对于矩阵的构造，着色语言很灵活。下面是构造的一些基本规则：
- 如果只为矩阵构造器提供一个标量参数，则该值被放在矩阵的对角线上。例如，mat4(1.0)将创建一个4x4的单位矩阵。
- 矩阵可以从多个向量参数构造。例如，mat2可以从两个vec2构造。
- 矩阵可以从多个标量参数构造，每个参数代表矩阵中的一个值，从左到右使用。

OpenGL ES中的矩阵以列优先顺序存储。使用矩阵构造器时，参数按列填充矩阵。下面的例子中的注释说明了矩阵构造参数如何映射到列中。
```c
mat3 myMat3 = mat3(1.0,0.0,0.0,// First column
    0.0,1.0,0.0,//Second column
    0.0,1.0,1.0);// Third column
```
## 常量
可以将任何基本类型声明为常数变量。常数变量是着色器中不变的值。声明常量时，在声明中加入const限定符。常数变量必须在声明时初始化。下面是const声明的一些例子：
```c
const float zero = 0.0f;
const float pi = 3.14159;
const vec4 red = vec4(1.0,0.0,0.0,1.0);
const mat4 identity = mat4(1.0);
```
正如在C或者C++中那样，声明为const的变量是只读的，不能在源代码中修改。
## 结构
除了使用语言中提供的基本类型之外，还可以和C语言一样将变量聚合成结构。
OpenGL ES着色语言中声明结构的语言如下例所示：
```c
struct fogStruct
{
    vec4 color;
    float start;
    float ent;
}fogVar;
```
上述定义的结果是一个名为fogStruct的新用户类型和一个新变量fogVar。
结构可以用构造器初始化。在定义新的结构类型之后，也用与类型相同的名称顶一个新的结构构造器。
## 函数
函数的声明方法和C语言相通。

|限定符|描述|
|--|--|
|in|（没有指定时的默认限定符）这个限定符指定参数按值传送，函数不能修改|
|inout|这个限定符规定变量按照引用传入函数，如果该值被修改，它将在函数退出后变化|
|out|这个限定符表示该变量的值不被传入函数，但是在函数返回时将被修改|

## 统一变量
OpenGL ES着色语言中的变量类型限定符之一是统一变量。统一变量存储应用程序通过OpenGL ES 3.0 API传入着色器的只读值，对于保存着色器所需的所有数据类型（如变化矩阵、照明参数和颜色）都很有用。本质上，一个着色器的任何参数在所有顶点或者片段中都应该以统一变量的形式传入。在编译时已知值的变量应该是常量，而不是统一变量，这样可以提供效率。

|限定符|描述|
|--|--|
|shared|shared限定符指定多个着色器或者多个程序中统一变量块的内存布局相同。要使用这个限定符，不同定义中的row_major/column_major值必须相等。覆盖std140和packed（默认）|
|packet|packet布局限定符指定编译器可以优化统一变量块的内存布局。使用这个限定符时必须查询偏移位置，而且统一变量块无法在顶点/片段着色器或者程序见共享。覆盖std140和shared|
|std140|std140布局限定符指定统一变量块的布局基于OpenGL ES 3.0规范的“标准统一变量快布局”中定义的一组标准规则。
|raw_major|矩阵在内存中以行优先顺序布局|
|column_major|矩阵在内存中以列优先顺序布局（默认）|

## 顶点和片段着色器输入/输出
这个着色的两个顶点输入变量a_position和a_color的数据由应用程序加载。本质上，应用程序将为每个顶点创建一个顶点数组，该数组包含位置和颜色。
```c
// Vector Shader
#version 300 es
uniform mat4 u_matViewProjection;
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_color
out vec3 v_color
void main(void)
{
    gl_Position = u_matViewProjection * a_position;
    v_color = a_color;
}
// Fragment Shader
#version 300 es
precision mediump  float 

// Input from vertex shader
in vec3 v_color;
// Output of fragment shader
layout(location = 0) out vec4 o_fragColor;
void main()
{
    o_fragColor = vec4(v_color, 1.0);
}
```
layout限定符用于指定顶点属性的索引。布局限定符是可选的，如果没有指定，链接程序将自动为顶点输入变量分配位置。
和统一变量一样，底层硬件通常可在输入顶点着色器的属性变量数目上有限制。OpenGL ES实现支持的最大属性数量由内建变量gl_MaxVertexAttribs给出（也可以使用glGetIntegerv查询GL_MAX_VERTEX_ATTRIBS得到）。OpenGL ES 3.0实现可支持的最小属性为16个。不同的实现可以支持更多变量，但是如果想要编写保证能在任何OpenGL ES 3.0实现上运行的着色器，则应该将属性限制为不多于16个。
## 插值限定符
在没有限定符时，默认的插值行为是执行平滑着色。也就是说，来自顶点着色器的输出变量在图元中线性插值，片段着色器接受线性插值之后的数值作为输入。我们可以明确的请求平滑着色，而不是依赖默认行为，如下：
```c
// ...Vertex shader...
// Vertex shader output
smooth out vec3 v_color;

// ..Fragment shader
// Input from vertex shader
smooth in vec3 v_color;
```
OpenGL ES 3.0引入了平面着色。在平面着色中，图元的值没有进行插值，而是将一个顶点设为驱动顶点(Provoking Vertex)，该顶点的值被用于图元中的所有片段。我们可以声明如下的平面着色输出/输入：
```c
// ...Vertex Shader...
// Vertex shader output
flat out vec3 v_color;

// ...Fragment shader...
// Input from fragment shader
flat in vec3 v_color;
```
最后，可以用centroid关键字在插值器中添加另一个限定符。质心采样(centroid sampling)
```c
// ..Vertex shader
// Vertex shader output
smooth centroid out vec3 v_color;

// ...Fragment shader
// Input from vertex shader
smooth centroid in vec3 v_color;
```
## 预处理器和指令
```c
#define
#undef
#if
#ifdef
#ifndef
#else
#elif
#endif
```
注意，宏不能定义为带有参数(在C++的宏中可以这样)。#if、#else和#elif指令可以使用defind测试来查看宏是否已经定义。下面的宏是预先定义的，接下来将作说明：
```c
__LINE__    // Replaced with the current line number in a shader
__FILE__    // Always 0 in OpenGL ES 3.0
__VERSION__ // The OpenGL ES shading language version
GL_ES       // This will be defind for ES shaders to a value of 1
```

|扩展行为|描述|
|--|--|
|require|扩展是必须的，因为预处理器在扩展不受支持时将抛出错误。如果指定了all，将总是抛出错误|
|enable|扩展被启用，因为扩展不受支持时预处理将抛出警告。如果扩展被启用，该语言将被处理。如果指定all，将总是抛出错误|
|warn|对于扩展的任何使用均提出警告，除非这种使用时另一个已经启用扩展所必需的。如果指定all，则在使用扩展时都将抛出警告。而且，如果扩展不受支持，将抛出警告|
|disable|扩展被禁用，因此如果使用扩展将被抛出错误。如果指定all（默认），则不启用任何扩展|

## 统一变量和插值器打包
统一变量通常保存在所谓的“常量存储”中，这可以看作向量的物理数组。顶点着色器输出/片段着色器输入一般保存在插值器中，这通常也保存为一个向量数组。
在OpenGL ES 3.0中，定义插值器和统一变量映射到物理存储空间的方式。打包规则基于物理存储空间被组织为一个每个存储位置4列（每个向量分量一列）和1行的网格的概念。打包规则寻求打包变量，使生成代码的复杂度保持不变。换言之，打包规则不景行重排序操作（这种操作需要编译器生成合并打包数据的额外指令），而是试图在部队运行时性能产生负面影响的情况下，优化物理地址空间的使用。
我们来看一组统一变量声明的例子，看看如何打包它们：
```c
uniform mat3 m;
uniform float f[6];
uniform vec3 v;
```
如果完全不进行打包，你可能发现许多常量存储空间将被浪费。矩阵m将占据3行，数组f占据6行，向量v占据1行，共需要10行才能存储这些变量。下表展示了任何不进行任何打包的结果。

|位置|X|Y|Z|W|
|--|--|--|--|--|
|0|m[0].x|m[0].y|m[0].z|-|
|1|m[1].x|m[1].y|m[1].z|-|
|2|m[2].x|m[2].y|m[2].z|-|
|3|f[0]|-|-|-|
|4|f[1]|-|-|-|
|5|f[2]|-|-|-|
|6|f[3]|-|-|-|
|7|f[4]|-|-|-|
|8|f[5]|-|-|-|
|9|v.x|v.y|v.z|-6|

下表表示打包之后的结果

|位置|X|Y|Z|W|
|--|--|--|--|--|
|0|m[0].x|m[0].y|m[0].z|f[0]|
|1|m[1].x|m[1].y|m[1].z|f[1]|
|2|m[2].x|m[2].y|m[2].z|f[2]|
|3|v.x|v.y|v.z|f[3]|
|4|-|-|-|f[4]|
|5|-|-|-|f[5]|

在使用打包规则时，只需硧6个物理常量位置。你将会注意到，数组f的元素会跨越行的边界，原因是GPU通常会按照向量位置索引对常量存储进行索引。打包必须时数组跨域行边界，这样索引才能够起作用。
所有打包对OpenGL ES着色语言的用户都是完全透明的，除了一个细节：打包影响统一变量和顶点着色器输出/片段着色器输入的计数方式。如果你想要编写保证能够在所有OpenGL ES 3.0实现上运行的机器，就不应该使用打包之后超过最小运行存储大小的统一变量或者插值器。
## 精度限定符
精度限定符使着色器创作者可以指定着色器变量的计算精度。变量可以声明为低、中高。这些限定符用于提示编译器允许在较低的范围和精度上执行变量计算。在较低的精度上，有些OpenGL ES实现在运行着色器时可能更快，或者电源效率更高。
当然，这种效率的提升是以精度为代价的，在没有正确使用精度限定符时可能造成伪像。
注意，OpenGL ES规范中没有规定底层硬件中必须支持多种精度，所以某个OpenGL ES实现在最高精度上进行所有运算并见到那地忽略限定符是完全正常地。不过，在某些是线上，使用较低地精度可能带来好处。
精度限定符可以用于指定任何基于浮点数或者整数的变量的精度。指定精度的关键字是lowp、mediump和highp。下面是一些带有精度限定符的声明示例：
```c
highp vec4 position;
varying lowp vec4 color;
mediump float specularExp;
```
除了精度限定符之外，还有默认精度的概念。也就是说，如果变量声明时没有使用精度限定符，它将拥有该类型的默认精度。默认精度限定符在顶点或者片段着色器的开头用如下语法指定：
```c
precision highp float;
precision mediump int;
```
在顶点着色器中，如果没有指定默认精度，则int和float的默认精度都为highp。也就是说，顶点着色器中所有没用精度限定符声明的变量都使用最高的精度。片段着色器的规则与此不同。在片段着色器中，浮点值没有默认的精度值：每个着色器必须声明一个默认的float精度，或者为每个float变量指定精度。
## 不变性
OpenGL ES着色语言中引入的invariant关键字可以用于任何可变的顶点着色器输出。不变性是什么意思，为什么它很必要呢？问题在于着色器需要编译，而编译器可能进行导致指令重新排序的优化。这种指令重排意味着两个着色器之间等价计算不能保证产生完全相同的结果。这种不一致性在多遍着色器特效时尤其可能称为问题，在这种情况下，相同的对象Alpha混合绘制在自身上方。如果用于计算输出位置的数值的精度不完全一样，精度差异就会导致伪像。这个问题通常表现为“深度冲突”(Z fighting)，每个像素的Z（深度）精度差异导致不同遍着色相互之间有微小的偏移。
invariant关键字可以用于变量声明，或者用于已经声明的变量。下面是一些例子：
```c
invariant gl_Position;
invariant texCoord;
```
一旦某个输出变量声明了不变性，编译器便保证相同的计算和着色器输入条件下结果相同。例如，两个顶点着色器通过将视图投影矩阵和输入位置位置相乘计算输出位置，你可以保证这些位置不变。
```c
#version 300 es
uniform mat4 u_viewProjMatrix;
layout(location = 0) in vec4 a_vertex;
invariant gl_Position;
void main()
{
    // Will be the same value in all shaders with the same viewProjMatrix and vertex
    gl_Position = u_viewProjMatrix * a_vertex;
}
```
也可以用#pragma指定让所有变量全都不变：
`#pragma STDGL invariant(all)`
警告：因为编译器需要保证不变性，所以可能限制它所做的优化。因此，invariant限定符应该只在必要时使用；否则可能导致性能下降。由于这个原因，全局启用不变性的#pragma指令之应该在不变性对于所有变量都必需的时候使用。还要注意，虽然不变性表示在指定GPU上计算会得到相同的结果，但是并不意味着计算在任何OpenGL ES实现之间保持不变。
## 复制缓冲区对象
迄今为止，我们已经说明如何用glBufferData、glBufferSubData和glMapBufferRange加载缓冲区对象。所以这些技术都涉及从应用程序到设备的数据传输。OpenGL ES 3.0还可以从一个缓冲区对象将数据完全复制到设备，这可用glCopyBufferSubData函数完成。
`void glCopyBufferSubData(GLenum readTarget,GLenum writeTarget,GLintptr readoffset,GLinptr writeoffset,GLsizeptr size)`
readtarget 读取的缓冲区对象目标
writetarget 写入的缓冲区对象目标。readtarget和writetarget都可以设置为如下目标中的任何一个（尽管它们不必设置为同一个目标）
`GL_ARRAY_BUFFER`
`GL_ELEMENT_ARRAY_BUFFER`
`GL_COPY_READ_BUFFER`
`GL_COPY_WRITE_BUFFER`
`GL_PIXEL_PACK_BUFFER`
`GL_PIXEL_UNPACK_BUFFER`
`GL_TRANSFORM_FEEDBACK_BUFFER`
`GL_UNIFORM_BUFFER`
readoffset 需要复制的读缓冲区数据中的偏移量，以字节表示
writeoffset 需要复制的写缓冲区数据中的偏移量，以字节表示
size 从读缓冲区数据复制到写缓冲区数据的字节数

调用glCopyBufferSubData将从绑定到readtarget的缓冲区复制指定的字节到writetarget。缓冲区绑定根据每个目标的最后一次glBindBuffer调用确定。任何类型的缓冲区对象（数组、元素数组、变化反馈等）都可以绑定到GL_COPY_READ_BUFFER或GL_COPY_WRITE_BUFFER目标。这两个目标是一种方便的措施，使得应用程序在执行缓冲区的复制不改变任何真正的缓冲区绑定。

## 小结
本章探索了OpenGL ES3.0中指定顶点属性和数据的方法，特别是介绍了如下主题：
- 如何使用glVertexAttrib*函数指定常量顶点属性和用glVertexAttrib[I]Point函数指定顶点数组。
- 如何在顶点数组状态在顶点数组对象中如何封装，以及如何使用VAO改进性能。
- 加载缓冲区对象数组的各种方法：glBuffer[Sub]Data、glMapBufferRange和glCopyBufferSubData。
现在我们知道了指定顶点数据的方法，下一章将介绍在OpenGL ES中可以使用顶点数据的各种图元。

# 图元装配和光栅化

本章描述OpenGL ES支持的图元和几何形状和对象的类型，并说明绘制它们的方法。然后描述发生在顶点着色器处理图元处理顶点之后的图元装配阶段。在这一阶段，执行裁剪、透视分割和视口变换操作，对这些操作将做详细的讨论。本章以光栅化阶段的描述作为结束。光栅化是将图元转换为一组二维片段的过程，这些片段由片段着色器处理，代表可以在屏幕上绘制的像素。
## 图元

图元是可以用OpenGL ES中的glDrawArrays、glDrawElements、glDrawRangeElements、glDrawArraysInstanced和glDrawElementsInstanced命令绘制的几何形状对象。图元由一组表示顶点位置的顶点描述。其他如颜色、纹理坐标和几何法线等信息也作为通用属性和每个顶点关联。
OpenGL ES3.0可以绘制如下图元：
- 三角形
- 直线
- 点精灵

### 三角形
三角形代表着描述由3D应用程序渲染的几何形状对象时最常用的办法。OpenGL ES支持的三角形图元有GL_TRIANGLES、GL_TRIANGLE_STRIP和GL_TRIANGLE_FAN。
图7-1展示了支持的三角形图元类型示例。
{% asset_img 6.jpg %}
GL_TRIANGLES绘制一系列单独的三角形。在图7-1中，绘制了顶点为$(v_0,v_1,v_2)$和$(v_3,v_4,v_5)$的两个三角形。总共绘制了n/3个三角形，其中n是前面提到的glDraw***API中的Count指定的索引。
GL_TRIANGLE_STRIP绘制一系列相关连接的三角形。在图7-1的例子中，绘制了3个顶点为$(v_0,v_1,v_2)$、$(v_2,v_1,v_3)$（注意顺序）和$(v_2,v_3,v_4)$的三角形。总共绘制了n-2个三角形，其中n是glDraw***API中的Count指定的索引。
GL_TRIANGLE_FAN也绘制了一系列相连的三角形。

### 纹理过滤和mip贴图
到目前为止，我们对2D纹理的介绍仅限于单个2D图像。尽管这使得我们能够解释纹理的概念，但是OpenGL ES纹理的指定和使用还有一些其他的方法。这种复杂性与使用单个纹理贴图时发生的视觉危象和性能问题有关。正如我们到目前为止所描述的那样，纹理坐标用于生成一个2D索引，以从纹理贴图中读取。当缩小和放大过滤器设置为GL_NEAREST时，就会发生这样的情况：一个纹素将在提供的纹理坐标位置上读取。这称作点采样或者最近采样。
但是，最近采样可能产生严重的视觉危象，这是因为三角形在屏幕空间中变得较小，在不同像素间的插值中，纹理坐标有很大的跳跃。结果是，从一个大的纹理贴图中取得少量样本，造成
### 纹理坐标包装
纹理包装模式用于指定纹理坐标超出[0.0,1.0]范围是所发生的的行为，用glTexParamter 
