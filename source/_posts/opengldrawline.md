---
title: 500行代码学懂OpenGL之二画线
mathjax: true
date: 2019-05-27 13:11:47
tags:
    - 光栅
    - 计算机图形学
    - OpenGL
categories: OpenGL
---
# 画线
画线的同时我要记得，我们是画在一个一个整数的pixel上。
## 尝试一：按照参数绘制直线
```c
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    for (float t =0;t<1;t+=.01)
    {
        int x = x0 + (x1-x0)*t;
        int y = y0 + (y1-y0)*t;
        image.set(x,y,color);
    }
}
```
这里的问题有两个：
- 效率低
- t如何控制

t取大了画出来的并不是线，而是一堆点。t取小了会浪费，会很多重复的x和y。

## 尝试二：按x的增加画线
```c
void line(int x0, int y0, int x1, int y1, TGAmage &image, TGAColor color){
    for (int x=x0;x<x1;x++){
        float t = (x-x0)/(float)(x1-x0);
        int y = y0+(y1-y0)*t;
        image.set(x,y,color);
    }
}
```
我们想要节约，就每次x增加1，然后来画y。
这样画线是对的因为我们假设$y=mx+b$，直线斜率是m，截距是b
${y_1-y_0\over x_1-x_0}=m$
$y_0=mx_0+b$
$y=y_0+{y1-y0 \over x_1 - x_0}(x-x_0)$
所以
$y = y_0+mx-mx_0=mx+(y_0-mx_0)=mx+b$
同时它的问题：
- 如果直线斜率太大，比如m=3，那么x每增加1一个像素，y增加3个像素，这样画出来的就是分离的点。
- 只能适用于$x_0 < x_1 $的状况。

## 尝试三

所以想法是：
- 如有必要交换x0和x1，这样使得x0一定小于x1
- 如果斜率比较大，则交换x和y
```c
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1))
    {
        std::swap(x0,y0);
        std::swap(x1,y1);
        steep = true;
    }
    if (x0>x1){
        std::swap(x0,x1);
        std::swap(y0,y1);
    }
    for(int x=x0;x<=x1;x++){
        float t = (x-x0)/(float)(y-y0);
        int y = y0+(y1-y0)*t;
        if (steep){
            image.set(y,x,color);
        }else{
            image.set(x,y,color);
        }
    }
}
```
这样就可以完善上述出现的问题来画线了。
这里其实还是有一些可以进步的空间，比如出现了浮点数t，同时也依旧