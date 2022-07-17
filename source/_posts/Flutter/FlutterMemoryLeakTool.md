---
title: Flutter内存泄漏检测工具
---
 Android: LeakCanary
 在垃圾回收线程扫描它所管辖的内存区域的过程中，一旦发现了指具有弱引用的对象，不管当前内存是否足够与否，都会回收它的内存。

 iOS: MLeakFinder

 # Expando
 Dart也有弱引用，它叫Expando。我们通过expando[key]=value让`expando`以弱引用的方式持有key。
 
