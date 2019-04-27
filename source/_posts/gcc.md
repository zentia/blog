---
title: gcc
date: 2017-10-23 10:39:28
tags:
  - Linux
  - gcc
---

# extern static
extern 指为导出到其它文件所使用的非statice变量
static是当前文件的静态变量
二者不能同时定义
setarch i386 ./config -m32 
编译32位的openssh
缺少 gnu/stubs-32.h

    yum install glibc-devel.i686

gcc没有找到

    yum install libstdc++-devel.i686
    yum install -y gcc gcc+ gcc-c++
