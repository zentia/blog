---
title: RenderDoc调试UE Shader
date: 2021-12-30 21:03:00
sticky: 100
categories:
- Engine
- Unreal
- Shader
tags:
   Unreal
---
# `ConsoleVariables.ini`
`r.ShaderDevelopmentMode = 1`，当缺省material编译失败后，Unreal提示你重新编译shader。如果不设置为1，Unreal会直接崩溃掉，如果它不能编译缺省material。
1. 修改Shader文件，保存，在控制台输入
`recompileshaders changed`
2. 调用`ShaderCompiler.cpp RecompileShaders`，根据指令的内容进入不同的分支，先去匹配具体的命令内容

# RenderDoc调试UE Shader

打开
截帧后Shader只能看到汇编代码片段，若想看到源代码，需要打开Engine\Config\ConsoleVariables.ini配置文件，取消以下两行的注释
```
r.Shaders.Optimize=0
r.Shaders.KeepDebugInfo=1
```
使用低优化编译器和保留调试信息。更低优化器会降低编译时间，保留debug信息会让RenderDoc更好的调试。
`r.DumpShaderDebugInfo = 1`和`r.DumpShaderDebugShortNames=1`在特定场合下非常有用。它将会生成后的HLSL写入硬盘（空工程，约2G），但是它会对RenderDoc调试非常有帮助。第二个变量会让变量名改变，以符合OS最大路径长度限制。
## 打开RenderDoc插件
在Edit->Plugins的内置插件中搜索RenderDoc插件。
开启后重启
开始抓帧
当我们看到事件列表的时候，我们选择我们要调试的一个事件
![1](1.png)
在Mesh Viewer面板中选中一个点，然后右键，选择debug this vertex
![2](2.png)
就会打开调试面板
![3](3.png)

# Slate性能跟踪
## 查看UI绘制批次
1. 建立空场景，加入需要测试的UI，一定要在-game模式下运行
   1. 输入`stat slate`，NumBatcher就是DrawCall次数

2. 查看具体批次绘制
   1. 安装RenderDoc安装包，UE4启用RenderDoc插件
   2. 输入`renderdoc.CaptureAllActivity 1`
   3. 输入`renderdoc.CaptureFrame` 

# 参考
- [1][Slate 性能跟踪](https://zhuanlan.zhihu.com/p/31499684)
