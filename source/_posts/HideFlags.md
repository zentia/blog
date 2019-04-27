---
title: HideFlags
mathjax: true
date: 2019-04-03 22:49:42
tags:
    - Unity
categories: Unity
---
# DontSave 保留对象到新场景

- 如果GameObject对象被HideFlags.DontSave标识，则在新Scene中GameObject的所有组件将被保留下来，但其子类GameObject对象不会被保留到新Scene中。
- 不可以对GameObject对象的某个组件如Transform进行HideFlags.DontSave标识，否则无效。
- 即使程序已经退出，被HideFlags.DontSave标识的对象会一直存在于程序中，造成内存泄漏，对HideFlags.DontSave标识的对象在不需要或程序退出时需要使用DestroyImmediate手动销毁。