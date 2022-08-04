---
title: Flutter源码阅读分析：引擎初始化与启动
categories:
 - Flutter
---
在引擎中存在两个FlutterActivity(shell/platform/android/io/flutter/app/FlutterActivity.java和shell/platform/android/io/flutter/embedding/android/FlutterActivity.java)。
`FlutterActivity`是将Flutter集成到Flutter到Android应用中的最简单最直接的方式，用于显示一个全屏的Flutter UI。主要职责是：
- 显示一个Android的launch screen
- 显示Flutter的splash screen
- 设置状态栏`configureStatusBarForFullscreenFlutterExperience()`
- 选择Dart执行应用包路径和入口点
- 选择Flutter的初始化路由
- 如果需要的话，渲染透明度`configureWindowForTransparency`
- 提供子类钩子，提供和配置FlutterEngine

# FlutterActivityAndFragmentDelegate
`onAttach`：
1. 初始化Flutter系统
2. 获取或创建一个`FlutterEngine`
3. 创建和配置`PlatformPlugin`
4. 将`FlutterEngine`附在`Activity`上
5. 通过`Host`的`configureFlutterEngine`方法配置`FlutterEngine`
`onCreateView`：
1. 在View树中创建了一个新的`FlutterView`
2. 在`FlutterView`中添加一个`FlutterUiDisplayListener`
3. 将`FlutterEngine`附着在`FlutterView`上
4. 返回这个新的View树

# FlutterView
`FlutterView`的作用是将Flutter UI通过对应的`FlutterEngine`绘制后，显示在Android设备上。这里有两种渲染模式：`surface`和`texture`。
在一般情况下，我们都使用`surface`模式，即将Flutter UI绘制在`SurfaceView`上，这种模式拥有最优的性能，缺点是无法置于两个其他Android View和z-index之间，同时也无法动画化或者进行变换。
# FlutterEngine
`FlutterEngine`是一个独立的Flutter运行环境，是Dart代码运行在Android应用的容器。
`DartExecutor`用于配置、启动、执行Dart代码。
在`FlutterActivityAndFragmentDelegate`的`setupFlutterEngine`方法中调用的`FlutterEngine`构造方法没有传入`FlutterJNI`，`FlutterJNI`则是沟通Android的Java与Flutter引擎核心C++代码的桥梁。
# FlutterJNI
`FlutterJNI`是Flutter内嵌Java代码和引擎C++代码之间的接口。
Flutter引擎的代码时使用C++实现的。Android Flutter嵌入则负责协调Android系统事件和应用用户交互事件。这些事件协调需要消息交流接口，这就需要用到JNI(Java Native Interface)来穿过Java/native边界。
在Flutter的设计中，所有的JNI接口都集中在`FlutterJNI`这个类中。这么做主要有以下几个原因：
- JNI调用都是静态的，且没有Java实现，因此没有理由将调用与不同的类关联；
- 所有的JNI调用必须在C/C++代码中注册，当增加额外的包含JNI调用时，这个注册会变得非常复杂；