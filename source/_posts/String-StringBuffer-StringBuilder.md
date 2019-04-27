---
title: String-StringBuffer-StringBuilder
date: 2018-03-21 17:02:30
tags:
---
# JAVA
String 字符串常量
StringBuffer 字符串变量（线程安全）
StringBuilder 字符串变量（非线程安全）
String类型和StringBuffer类型的主要性能区别其实在于String是不可变的对象，因此每次对String类型进行改变的时候其实都等同于生成了一个新的String对象，然后将指针指向新的String对象，所以经常改变内容的字符串最好不要用String，因为每次生成对象都会对系统性能产生影响，特别当内存中无引用对象多了以后，JVM的GC就会开始工作，
# C#
String和JAVA中的一样