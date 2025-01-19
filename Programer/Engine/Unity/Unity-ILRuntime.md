---
title: Unity热更新方案-ILRuntime
date: 2018-03-23 18:24:17
categories:
- Engine
- Unity
---
ILRuntime的优势同市面上的其他热更方案相比，ILRuntime主要有以下优点：
- 无缝访问C#工程的线程代码，无需额外抽象脚本API
- 直接使用VS2015进行开发，ILRuntime的解释编译引擎支持.Net 4.6编译的DLL
- 执行效率是L#的10-20倍
- 选择性的CLR绑定使跨域调用更快速，绑定后跨域调用的性能能达到slua的2倍左右(从脚本调用GameObject之类的接口)
- 支持跨域集成
- 完整的泛型支持
- 拥有Vs2015调试插件，可以实现真机源码级调试(WIP)

https://github.com/Ourpalm/ILRuntime
https://ourpalm.github.io/ILRuntime/public/v1/guide/tutorial.html

如果你希望在Unity中使用ILRuntime,推荐的方式是直接使用ILRuntime源代码，这样ILRuntime可以根据你的发布设置自动进行优化。

你需要将下列源代码目录复制到Unity工程的Assets目录：

- Mono.Cecil.20
- Mono.Cecil.Pdb
- ILRuntime

需要注意的是，需要删除这些目录里面的bin、obj、Properties子目录，以及.csproj文件。此外由于ILRuntime使用了unsafe代码来优化执行效率，所以你需要在Unity中开启unsafe模式：

- 在Assets目录里建立一个名为smcs.rsp的文本文件
- 在smcs.rsp文件中假如-unsafe

如果你使用的是Unity5.4及以前的版本，并且使用的编译设置是.Net 2.0而不是.Net 2.0 Subset的话，你需要将上述说明中的smcs.rsp文件名改为gmcs.rsp。
如果你使用的是Unity5.5以上的版本，你需要将上述说明的smcs.rsp文件名改为mcs.rsp

## 从Visual Studio开始

如果你希望在VisiualStudio的C#项目中使用ILRuntime，你只需要引用编译好的ILRuntime.dll,Mono.Cecil.20.dll以及Mono.Cecil.Pdb即可。

### 使用之前

ILRuntime项目提供了一个测试用例工程ILRuntimeTest,用来验证ILRuntime的正常运行，在运行测试用例前，需要手动生成一下TestCases里面的工程，生成DLL文件。
