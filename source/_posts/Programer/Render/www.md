---
title: www
date: 2017-10-16 10:27:44
tags:
  - Unity
---
StartCoroutine开启一个协程，yield return是迭代器返回调用迭代的地方。
协程至少产生十几还是多少B的GC具体是多少我忘记了，至少在5.3.8f2之前的版本是，未来如果有时间和有机会我会讲解一个可以避免掉GC的办法。CoroutineTool，本篇不再介绍范围内。
一个协程的执行可以在任何地方用yield语句来暂停，yield return的值决定了什么时候协程恢复执行。
WWW的加载方式，本身会增加FileSystem的崩溃率，然后还增加显存占用提高了Present的崩溃率。