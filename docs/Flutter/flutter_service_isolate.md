---
title: ServiceIsolate
---
# 概述
在前面文章深入理解Dart虚拟机启动中，有讲到Dart虚拟机中有一个比较重要的isolate，创建的isolate名为"vm-service"，运行在独立的线程，也就是ServiceIsolate，这是用于系统调试相关功能的isolate，提供虚拟机相关服务，比如hot reload，timeline等。