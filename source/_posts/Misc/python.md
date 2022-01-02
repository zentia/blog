---
title: Python
mathjax: true
date: 2019-05-27 14:39:42
tags:
    - Python
categories: Python
---
# python3 怎么安装 PIL
关于Pillow与PIL

PIL(Python Imaging Library)是Python一个强大方便的图像处理库，名气也比较大。不过只支持到Python 2.7。

PIL官方网站：http://www.pythonware.com/products/pil/

Pillow是PIL的一个派生分支，但如今已经发展成为比PIL本身更具活力的图像处理库。目前最新版本是3.0.0。

Pillow的Github主页：https://github.com/python-pillow/Pillow
Pillow的文档(对应版本v3.0.0)：https://pillow.readthedocs.org/en/latest/handbook/index.html
Pillow的文档中文翻译(对应版本v2.4.0)：http://pillow-cn.readthedocs.org/en/latest/

Python 3.x 安装Pillow

给Python安装Pillow非常简单，使用pip或easy_install只要一行代码即可。

在命令行使用PIP安装：
pip install Pillow

或在命令行使用easy_install安装：
easy_install Pillow

安装完成后，使用from PIL import Image就引用使用库了。比如：
from PIL import Image
im = Image.open("bride.jpg")
im.rotate(45).show()