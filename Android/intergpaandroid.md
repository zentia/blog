---
title: Inter GPA Android真机调试
mathjax: true
date: 2019-06-04 12:07:56
tags:
    - Android
categories: Android
---
下载安装完成Intel GPA套件后，一共有五个工具，分别是：
Graphics Monitor
Graphics Frame Analyzer
Platform Analyzer
System Analyzer
Trace Analyzer
在Android平台进行图形调试的时候，我们主要用到的工具是System Analyzer和Graphics Frame Analyzer。
首先需要打开测试手机的开发者选项中的允许调试。
在一台安装配置完成Andriod开发环境的PC，使用USE连接需要测试的Android手机，然后在命令提示符中键入adb devices，饭回设备ID保证连接成功。
然后打开System Analyzer，界面会显示当前的Android设备ID，点击Connect，然后允许GPA软件在手机上的安装。
连接提示：

    Device error:setenforce:Couldn't set enforcing status to 'O':Permission denied