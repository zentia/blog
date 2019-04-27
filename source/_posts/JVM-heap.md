---
title: JVM的新生代、老年代、MinorGC、MajorGC
date: 2018-03-21 17:30:17
tags:
	Java
---
JVM中的堆，一般分为三大部分：新生代、老年代、永久代：
一、新生代
主要时用来存放新生的对象。一般占据堆的1/3空间。由于频繁创建对象，所以新生代会频繁发MinorGC进行垃圾回收。
新生代又分为Eden区、ServivorFrom、ServivorTo三个区。
