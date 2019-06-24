---
title: Android Manual
date: 2018-07-21 17:35:00
tags:
    Android
---
# AndroidManifest
- 为应用的JAVA软件包命名。软件包名称充当应用的唯一标识符
- 描述应用的各个组件，包括构成应用的Activity、服务、广播接收器和内容提供程序。它还为实现每个组件的类命名并发布其功能，例如它们可以处理Intent消息。这些声明向Android系统告知有关组件以及可以启动这些组件的条件的信息。
- 确定托管应用组件的进程。
- 声明应用必须具备哪些权限才能访问API中受保护的部分并与其他应用交互。还声明其他应用与该应用组件交互所需具备的权限。
- 列出Instrumentation类，这些类可在应用运行时提供分析和其他信息。这些声明只会在应用处于开发阶段时出现在清单中，在应用发布之前会将移除。
- 声明应用所需的最低Android API级别。
- 列出应用必须链接到的库

### <manifest>元素

首先，所有的xml都必须包含`<manifest>`元素。这是文件的根节点。它必须要包含`<application>`元素，并且指明xmlns:android和package属性。

xmlns:android
这个属性定义了Android命名空间。必须设置成"http://schemas.android.com/apk/res/android"。不要手动修改。

package
这是一个完整的Java语言风格包名。包名由英文字母（大小写均可）、数字和下划线组成。每个独立的名字必须以字母开头。

构建APK的时候，构建系统使用这个属性来做两件事：
生成R.java类时用这个名字作为命名空间（用于访问APP的资源）比如：package被设置成net.pixelgame.unity3d

android:name
该属性以字符串形式指定了APP要用的硬件或软件功能。

android:required

andorid:glEsVersion

`<application>`元素

此元素描述了应用的配置。这是一个必备的元素，它包含了很多子元素来描述应用的组件，它的属性影响到所有的子组件。

# Android Studio快捷键
1. 在当前窗口查找文本Ctrl+F
2. 在当前窗口查找文本Ctrl+Shift+F
3. 查找类Ctrl+N
4. 查找文件Ctrl+Shitf+N
5. 查找项目中的方法或变量Ctrl+Shitf+Alt+N
6. 查找类/方法/变量引用的地方,先定位光标,右键选择"Find Usages"(或者快捷键Alt+F7)
7. Ctrl+o查看所有可以重写的地方
8. 在类中快速定位某个方法或属性Ctrl+F12

# Android分辨率
2018.1月手机分辨率
2560*1440
1920*1080(16:9)
1280*720 (16:9)
2960*1440
2040*1080

# 如何连接木木模拟器并查看log
```
adb connect 127.0.0.1:7555 # 连接木木模拟器
adb connect 127.0.0.1:62001 # 连接夜神模拟器
adb shell
logcat
```