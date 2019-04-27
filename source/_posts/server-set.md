---
title: 服务器搭建和日常维护
date: 2017-10-24 13:02:03
tags:
  - server
---

# 1 概述
# 2 系统软硬件要求
软件：
所有的服务器程序都可以在linux和windows系统上。推荐使用linux系统。
我们一般使用的系统是suse10.1，不过按要求，我们在tlinux的服务器上实验编译并运行了我们的所有服务器成。
Gcc版本：gcc(GCC)4.8.5
数据库使用MYSQL。版本：5.6.38
硬件：
参考先前提供的硬件配置资料
另外，请在启动文件/etc/rc.d/rc.local中设置共享内存、网络参数以及core文件模式
```
#increase Linux SHM max
echo 300000000 > /proc/sys/kernel/shmmax

#increase Linux TCP buffer limits
echo 8388608>/proc/sys/net/core/rmem_max
echo 8388608>/proc/sys/net/core/wmem_max
echo 262114>/proc/sys/net/core/rmem_default
echo 262114>/proc/sys/net/core/wmem_default

#increase Linux autotruning TCP buffer limits
echo "4096 87380 8388608">/proc/sys/net/ipv4/tcp_rmem
echo "4096 65536 8388608">/proc/sys/net/ipv4/tcp_wmem
echo "8388608 8388608 8388608">/proc/sys/net/ipv4/tcp_mem
echo "840">/proc/sys/net/ipv4/
echo "core-%e-%p-%s">/proc/sys/kernel/core_pattern
```
# 3 维护人员账号配置
为了安全的原因，请不要使用root账号进行服务器的维护工作。
# 4 MySQL的配置
# 5 服务程序简介
# 6 服务程序的版本辨认
# 7 日志服务程序启动过程
## 7.1 启动日志服务
## 7.2 检查
# 8 游戏服务器启动过程
## 8.1 启动日志服务
## 8.2 启动游戏总控框架
## 8.3 启动游戏场景线
## 8.4 检查
# 9 服务组关闭过程
# 10 YYCGZS的维护
