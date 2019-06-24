---
title: OpenGL ES 3.0
mathjax: true
date: 2019-05-12 17:18:55
tags:
    - "OpenGL ES 3.0"
categories: "OpenGL ES 3.0"
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
屏幕上可见的帧缓冲区由一个像素数据的二维数组表示。我们可以将屏幕上显示图像视为绘制时简单地更新可见帧缓冲区中地像素数据。但是，直接在可显示缓冲区上更新像素有一个严重的问题，在典型的显示系统中，物理屏幕以固定的速率从帧缓冲区内存中更新。如果我们直接绘制到帧缓冲区，那么用户在部分更新帧缓冲区时会看到伪像。
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

## 创建和编译一个着色器
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