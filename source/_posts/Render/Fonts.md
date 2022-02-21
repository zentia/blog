---
title: 第九章 字体(Fonts)
mathjax: true
date: 2019-04-11 15:12:44
tags:
categories: Direct3D
---
# ID3DXFont
在D3DX库中提供了一个ID3DXFont接口，它能被用于在Direct3D应用程序中绘制文字。这个接口是使用GDI来绘制文字的，因此我们能够使用这个接口来执行这个操作。无论如何，因为ID3DXFont使用的是GUI，所以它能够联合字体句柄和格式化字体。
## 创建一个ID3DXFont
　　我们能够使用D3DXCreateFontIndirect函数来创建一个ID3DXFont接口。
HRESULT D3DXCreateFontIndirect(
    LPDIRECT3DDEVICE9 pDevice, // device to be associated with the font
    CONST LOGFONT* pLogFont, // LOGFONT structure describing the font
    LPD3DXFONT* ppFont // return the created font
);
　　下面的代码片段显示了怎样使用这个函数：
```C++
LOGFONT lf;
ZeroMemory(&lf, sizeof(LOGFONT));
lf.lfHeight = 25; // in logical units
lf.lfWidth = 12; // in logical units
lf.lfWeight = 500; // boldness, range 0(light) - 1000(bold)
lf.lfItalic = false;
lf.lfUnderline = false;
lf.lfStrikeOut = false;
lf.lfCharSet = DEFAULT_CHARSET;
strcpy(lf.lfFaceName, "Times New Roman"); // font style
ID3DXFont* font = 0;
D3DXCreateFontIndirect(Device, &lf, &font);
　　我们必须填充一个LOGFONT结构来描述想创建的字体类型。
　　注意：你也能够使用D3DXCreateFont函数来获得一个ID3DXFont接口指针。
9.1.2绘制文本
　　一旦我们获得了ID3DXFont接口指针，绘制文本就是很简单的事情了，我们只要调用ID3DXFont::DrawText方法就可以实现了。
INT ID3DXFont::DrawText(
    LPCSTR pString,
    INT Count,
    LPRECT pRect,
    DWORD Format,
    D3DCOLOR Color
);
```
* pString — 指向要绘制的文字。
* Count — 字符串中特征字符的数量。假如字符是以null结束的字符串则可将其指定为-1。
* pRect — 指向一个RECT结构，它定义一个文字被绘制在屏幕上的范围。
* Format — 可选参数，指定文字怎样被格式化；要获得更详细的信息请查看SDK文档。
* Color — 文字的颜色。
　　例子：
```C++
Font->DrawText(
    "Hello World", // String to draw.
    -1, // Null terminating string.
    &rect, // Rectangle to draw the string in.
    DT_TOP | DT_LEFT, // Draw in top-left corner of rect.
     0xff000000
     ); // Black.
```
9.1.3计算每秒的渲染帧数
　　这一章的ID3DXFont和Cfont例子是计算和显示每秒渲染的帧数（FPS）。这一部分说明怎样计算FPS。
　　首先，我们定义如下三个全局变量：
```C++
DWORD FrameCnt; // The number of frames that have occurred.
float TimeElapsed; // The time that has elapsed so far.
float FPS; // The frames rendered per second.
```
　　我们计算每一秒的FPS；它给我们一个很好的平均。另外，在同一秒中内只保存一个FPS，这给了我们足够时间来读取它，在它再一次改变之前。
　　因此每一帧我们增加FrameCnt并且把从上一帧到现在流逝的时间写进TimeElapsed：
FrameCnt++;
TimeElapsed += timeDelta;
这里timeDelta是两帧之间的时间。
　　    在一秒种结束以后，我们能够用下面的公式来计算FPS：
FPS = (float)FrameCnt / TimeElapsed;
我们从新设置FrameCnt和TimeElapsed为计算下一秒的FPS做准备。下面就是合在一起的代码：
void CalcFPS(float timeDelta)
{
    FrameCnt++;
    TimeElapsed += timeDelta;
    if(TimeElapsed >= 1.0f)
    {
        FPS = (float)FrameCnt / TimeElapsed;
        TimeElapsed = 0.0f;
        FrameCnt = 0;
    }
}
9.2 CD3DFont
　　DirectX SDK给我们提供了一些很有用的代码，它们在你的DXSDK目录下的\Samples\C++\Commond下。CD3DFont类代码是使用纹理三角形和Direct3D。因为CD3DFont使用Direct3D代替GDI来渲染， 这比ID3DXFont快的多。然而，CD3DFont不能够联合字体句柄和格式化ID3DXFont。假如你追求速度和只需要一些简单的字体，CD3DFont类就能满足你的要求了。
　　使用CD3DFont类，你需要添加下列文件到你的程序中：d3dfont.h, d3dfont.cpp, d3dutil.h, d3dutil.cpp, dxutil.h和dxutil.cpp。这些文件可以在刚才所说目录下的Include和Src目录下。
9.2.1创建一个CD3DFont
　　为了创建一个CD3DFont实例，我们只需要简单地象一般的C++对象那样实例化就可以了；下面是它的构造原型：
CD3DFont(const TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L);
* strFontName — 以null结束的字符串，它指定字体类型。
* dwHeight — 字体的高度。
* dwFlags — 可选参数；你能设置该参数为0或者用下面参数；D3DFONT_BOLD, D3DFONT_ITALIC, D3DFONT_ZENABLE。
　　实例化一个CD3DFont对象以后，我们必须调用下面的方法来初始化字体：
Font = new CD3DFont("Times New Roman", 16, 0); // instantiate
Font->InitDeviceObjects( Device );
Font->RestoreDeviceObjects();
9.2.2绘制文本
　　现在我们已经创建和初始化了一个CD3DFont对象，这已经为绘制文字做好了准备。绘制文字是使用下面的方法：
HRESULT CD3DFont::DrawText(FLOAT x, FLOAT y, DWORD dwColor,
    const TCHAR* strText, DWORD dwFlags=0L);
* x — 文字在屏幕上开始绘制的x坐标。
* y —文字在屏幕上开始绘制的y坐标。 
* dwColor — 文字的颜色。
* strText — 要绘制的文字。
* dwFlags — 可选参数；你能设置该参数为0或者用下面参数；D3DFONT_CENTERED, D3DFONT_TWOSIDED, D3DFONT_FILTERED。
　　例子：
     Font->DrawText(20, 20, 0xff000000, “Hello, World”);
9.2.3 清除
　　在删除一个CD3DFont对象之前，我们必须首先调用一些清除程序，就象下面列举的代码片段：
Font->InvalidateDeviceObjects();
Font->DeleteDeviceObjects();
delete Font;
9.3 D3DXCreateText
　　最后的函数是被用来创建一个3D 文字网格。图9.1显示了本章FontMes3D实例渲染的3D文字网格。

图9.1
　　该函数的原型是：
```C++
HRESULT D3DXCreateText(
    LPDIRECT3DDEVICE9 pDevice,
    HDC hDC,
    LPCTSTR pText,
    FLOAT Deviation,
    FLOAT Extrusion,
    LPD3DXMESH* ppMesh,
    LPD3DXBUFFER* ppAdjacency,
    LPGLYPHMETRICSFLOAT pGlyphMetrics
);
```
这个函数如果调用成功则返回D3D_OK。
* pDevice — 和mesh关联的device。
* hDC — 我们将要用来产生mesh的包含描述字体的设备环境句柄。 
* pText — 指向以null结束的字符串的指针，此字符串是用来指定创建什么文字mesh。
* Deviation — 字型轮廓段数间距。该值必须大于等于0。当它为0时，段数等于字体原始设计单位（该值越接近0，那么字体就越光滑）。
* Extrusion — 文字在z轴方向的深度。
* ppMesh — 返回创建的mesh。
* ppAdjacency — 返回创建mesh的相关信息。假如你不需要它可以将其指定为null。
* pGlyphMetrics — 一个指向LPGLYPHMETRICSFLOAT结构数组的指针，它包含了字型米数据。假如你不关心此数据，你可以把它设置为0。
　　 下面的示例代码展示的是使用这个函数来创建一个文字3D 网格模型。
```C++
// Obtain a handle to a device context.
HDC hdc = CreateCompatibleDC( 0 );

// Fill out a LOGFONT structure that describes the font’s properties.
LOGFONT lf;
ZeroMemory(&lf, sizeof(LOGFONT));

lf.lfHeight = 25; // in logical units
lf.lfWidth = 12; // in logical units
lf.lfWeight = 500; // boldness, range 0(light) - 1000(bold)
lf.lfItalic = false;
lf.lfUnderline = false;
lf.lfStrikeOut = false;
lf.lfCharSet = DEFAULT_CHARSET;
strcpy(lf.lfFaceName, "Times New Roman"); // font style

// Create a font and select that font with the device context.
HFONT hFont;
HFONT hFontOld;
hFont = CreateFontIndirect(&lf);
hFontOld = (HFONT)SelectObject(hdc, hFont);

// Create the 3D mesh of text.
ID3DXMesh* Text = 0;
D3DXCreateText(_device, hdc, "Direct3D", 0.001f, 0.4f, &Text, 0, 0);

// Reselect the old font, and free resources.
SelectObject(hdc, hFontOld);
DeleteObject( hFont );
DeleteDC( hdc );
```
现在你便能简单地调用mesh的DrawSubset方法来渲染一个3D文字：
```c++
Text->DrawSubset(0);
```
