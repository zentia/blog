---
title: IntelliJ-IDEA
mathjax: true
date: 2019-07-02 21:35:48
tags:
    - JX3M
categories: JX3M
---
# 调试工程的Lua
去官网下载，然后破解，下载LuaForWindows5.3吧，我开始的时候下载的是5.1结果对于long类型无法识别。后来改成5.3还是不行。
设置根目录为StreamingAssets，这一点很重要。
然后Attach Unity.exe就可以调试了。
还是有一堆问题的，调试下一步按钮无法点击，只能靠F8来执行。
而且F8热键回会PS，有道快捷键冲突，所以先关闭这两个软件。
写代码的时候要关闭调试，否则很卡。
# 快捷键
|快捷键|说明|
|--|--|
|F7|跳到函数内部|
|F9|继续执行代码|
|Ctrl+鼠标左键|跳转定义|
|Ctrl+Alt+←|后退|
|Ctrl+Alt+→|前进|
|Ctrl+Shitf+N|快速打开文件|
|双击Shift|快速查找|
|Alt+F7|查找引用|