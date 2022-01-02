---
title: gdb
date: 2017-10-15 13:50:12
tags:
  - linux
---

# 断点
- break filename:linenum 在源文件filename的linenum行处停住。
d 清除所有断点
|--|--|
|step(s)|执行下一句，如果有函数则进入函数里面执行|
set follow-fork-mode parent
glibc-2.17-196.el7.i686 libgcc-4.8.5-16.el7.i686 libstdc++-4.8.5-16.el7.i686
gdb debugme pid
clear 删除所在行的所有断点
clear 12 //删除12行的所有断点
clear list.c:12 //删除该文件该行号的所有断点
b list.c:12 // 在该文件该行号添加断点
backtrace\bt 打印全部栈帧的简要信息，按Ctrl-c可终止打印。
finish 跳出当前函数。

# 调试守护进程
如果需要调试子进程，在启动gbd后：
set follow-fork-mode child
并在子进程代码设置断点
此外还有detach-on-fork参数，只是gdb在fork之后是否断开(detach)某个进程的调试，或者都交由GDB控制。

# 观察点(WatchPoint)
在变量读、写或变化时中断，这类方式常用来定位bug。
- watch <expr> 变量发生变化时中断
- rwatch <expr> 变量被读时中断
- awatch <expr> 变量值被读或写时中断

# 条件断点
(gdb) b 13 if i == 8

# 程序运行参数
set args 可指定运行时的参数。(如：set args 10 20 30 40 50
show args 命令可以查看设置好的运行参数

# 打印堆栈

bt 
显示所有的函数调用的栈帧信息，每个帧一行。