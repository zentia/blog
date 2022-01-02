---
title: Unreal Build
date: 2022-01-02 11:28:00
tags:
    - Unreal
---
# BuildCookRun命令行参数
常用常熟如下，详细参数见源文件，`Engine\Source\Programs\AutomationTool\AutomationUtils\ProjectParams.cs`，注意有些参数不能同时指定，具体见`ValidateAndLog`函数里的参数检查规则。
`Engine\Source\Programs\AutomationTool\AutomationUtils\Automation.cs`有一些全局参数定义

整体打包过程如下：Build->Cook->Stage->Package->Archive

- -TargetPlatform(-Platform):目标平台，定义在枚举UnrealTargetPlatform中，包含Win32/Win64/Mac/XboxOne/PS4/IOS/Android/HTML5/Linux/AllDesktop/TVOS/Switch
- -ServerTargetPlatform(-ServerPlatform):Dedicated Server目标平台，定义同上

# 基本参数
- -CrashReporter:编译CrashReporter
- -Clean:重新完整构建
- -SingedPak:使用加密的pak文件
- -SingPak=C:\Encryption.keys或者-signpak=0x.....:指定加密pak文件密钥内容或者密钥文件路径，密钥内容以0x开头，同时-SignedPak参数也生效
- -RunAssetNativization:将蓝图资产转为代码
- -Pak:是否将资源文件打包到pak中，但是并不是说不指定这个命令行就不用pak，有些平台只能用pak(比如Android打包成apk)，不管有没有这个参数，具体见各个平台对`Engine\SourcePrograms\AutomationTool\AutomationUtils\Platform.cs`类中`RequiresPak`函数的重载
- -SkipPak:使用上一次的pak文件，包含-Pak
- -UTF8Output:log，控制台窗口等输出使用UTF-8编码

# Build相关参数
- -Build:执行编译过程
- -SkipBuildClient:不编译客户端
- -SkipBuildEditor:不编译编辑器
- -NoXGE:不适用并行编译
- -ForceDebugInfo:在非Debug版本中加入调试信息