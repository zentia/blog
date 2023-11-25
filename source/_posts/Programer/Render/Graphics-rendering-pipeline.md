---
title: Graphics rendering pipeline
mathjax: true
date: 2019-08-28 10:00:36
tags:
categories: Graphics
---
{% asset_img 1.bmp %}
图形渲染管线是实时渲染的核心组件。
# 三角形遍历(Triangle Traversal)
其实三角形遍历的操作我们在前面基本都说过了，通过屏幕空间的坐标组装三角形后，我们遍历这些三角形图元覆盖了哪些片段的采样点，随后得到该图元所对应的片元。