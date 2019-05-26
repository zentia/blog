---
title: 500行代码学懂OpenGL之一画点
mathjax: true
date: 2019-05-26 20:40:18
tags:
    - 光栅
    - 计算机图形学
    - OpenGL
categories: OpenGL
---
项目来自https://github.com/ssloy/tinyrenderer，作者尝试用500行代码来写一个tiny render，让我们明白OpenGL是怎样工作的。
# 从画点开始
## TGAImage
生成图像我们使用TGAImage，这个使用起来很简单：
```c
#include "tgaimage.h"

const TGAColor white = TGAColor(255,255,255,255);
const TGAColor red = TGAColor(255,0,0,255);

int main(int argc, char ** argv)
{
    TGAImage image(100,100,TGAImage::RGB);
    image.set(52,41,red);
    image.filp_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
```

{%asset_img 1.jpg %}

项目:https://github.com/zentia/tinyrender.git
# wavefront obj
然后我们来学习一种3d格式文件，wavefront obj file:
```
# List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.0.
  v 0.123 0.234 0.345 1.0
  v ...
  ...
  # List of texture coordinates, in (u, [v ,w]) coordinates, these will vary between 0 and 1, v and w are optional and default to 0.
  vt 0.500 1 [0]
  vt ...
  ...
  # List of vertex normals in (x,y,z) form; normals might not be unit vectors.
  vn 0.707 0.000 0.707
  vn ...
  ...
  # Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement ( see below )
  vp 0.310000 3.210000 2.100000
  vp ...
  ...
  # Polygonal face element (see below)
  f 1 2 3
  f 3/1 4/2 5/3
  f 6/4/1 3/5/3 7/6/5
  f 7//1 8//2 9//3
  f ...
  ...
  # Line element (see below)
  l 5 8 1 2 4 9
```
我们现在只需要知道顶点是v，现在我们想把一个文件中的3d模型的顶点v(x,y,z)给画出来，(因为我们已经知道怎么在图上相应的位置放像素)这个文件所有的x,y,z∈[-1,1]，所以我们需要把它们映射到合适范围，然后注意我们画的点```image.set(52,41,red);```,这里的是52和41是int，映射之后需要转成int，因为我们总是画在一个一个像素点上。
写一个简单的parser读入文件建立模型。
核心部分长这样：
```c
for (int i = 0; i != model->nverts();i++)
{
    Vec3f v = model->vert(i);
    Vec2i p = world2screen(v);
    image.set(p.x,p.y,white);
}
```

