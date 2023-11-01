---
title: Gamma
mathjax: true
date: 2019-03-29 13:35:58
tags:
- Engine
- Unity
categories: 
- Engine
- Unity
---
Unity在UnityCG.cginc头文件中提供了GammaToLinearSpace和LinearToGammaSpace进行空间转化，其中的算法是近似算法，效率比较高
```C
inline half3 GammaToLinearSpace(half3 sRGB)
{
    return sRGB * (sRGB * (sRGB * 0.305306011h + 0.682171111h) + 0.012522878h);

    // 精确版，用来调试
    // return half3(GammToLinearSpaceExact(sRGB.r), GammaToLinearSpaceExact(sRGB.g), GammaToLinearSpaceExact(sRGB.b));
}
inline half3 LinearToGammaSpace(half3 linRGB)
{
    linRGB = max(linRGB, half3(0h,0h,0h));
    return max(1.055h*pow(linRGB,0.416666667h) - 0.055h, 0h);
}
inline bool IsGammaSpace()
{
#if defined(UNITY_NO_LINEAR_COLORSPACE)
    return true;
#else
    return unity_ColorSpaceLuminance.w == 0;
#endif
}
```

![](1.gif)