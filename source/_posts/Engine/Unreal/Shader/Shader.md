---
title: RenderDoc调试UE Shader
date: 2021-12-30 21:03:00
tags:
    - Unreal
---

1. 修改Shader文件，保存，在控制台输入
`recompileshaders changed`
2. 调用`ShaderCompiler.cpp RecompileShaders`，根据指令的内容进入不同的分支，先去匹配具体的命令内容

# RenderDoc调试UE Shader
## 修改配置文件`ConsoleVariables.ini`
打开
```
r.Shaders.Optimize=0
r.Shaders.KeepDebugInfo=1
r.DisableEngineAndAppRegistration=1
```
## 打开RenderDoc插件
在Edit->Plugins的内置插件中搜索RenderDoc插件。
开启后重启
开始抓帧
