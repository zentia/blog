---
title: 云主机CentOS7.2密钥丢失重新注入密钥方法
date: 2017-10-13 18:20:12
tags:
  - Linux
---

# 一、单用户模式重置ROOT密码（如有root密码，则跳过该步骤）

1. 登录控制台，输入VNC密码并回车

2. 点击【Send CtrlAltDel】重启linux系统；
3. 在弹出界面，按键盘‘e’；
4、如图修改前/修改后，将ro修改成rw init=/sysroot/bin/sh  去除console=ttyS0 115200，确认修改完成后，键盘输入Ctrl加x。

5、依次输入如下命令：chroot /sysroot/   passwd root  touch /.autorelabel进行root密码修改
6、返回云主机侧点击【重启】使得密码修改生效
7、待云主机重启完成后，进入VNC输入用户名root，修改后的密码进入系统
