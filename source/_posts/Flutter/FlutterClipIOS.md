---
title: iOS减包实践
---

# 背景

## Flutter瘦身需求

当App引入Flutter带来一个明显问题，包体积增大。对于3端分离模式，包体积增量在Android上计委Flutter的framework产物。因此，要解决包体积问题，需要对aar和framework进行优化。

## 本文内容涉及的开发环境

- Flutter 1.17.1 channel stable
- Mac OS X 10.15.4
- Xcode - develop for iOS and macOS (Xcode 11.4)
- Python 2.7.16

# iOS Framework产物分析

## framework结构

使用tree命令查看Flutter framework目录结构，以Release为例。iOS产物为两个framework，其中App.framework是Dart业务代码产物，Flutter.framework是Flutter SDK中拷贝过来的引擎产物。

## Framework体积分析（Release模式下）

|名称|大小|说明|
|---|---|---|
|App|7.3M|Dart业务代码AOT编译产物|
|flutter_assets|2M|图片、字体等资源文件|
|Flutter|11M|引擎|
|icudtl.dart|884k|国际化支持相关文件|
|其他第三方插件|800K|Flutter_boost等第三方插件|

# iOS减包思路

上一节我们分析了framework产物的组成占比，下面根据每一个核心占比部分逐个优化

## flutter_assets

flutter_assets目录存放的资源文件，可以直接移除。具体有以下两个思路。

- 修改flutter_tools编译打包脚本。在生成framework过程中，九江flutter_assets移除保存到别处，即生成的framework天生与flutter_asses就是分开的。优点是得到即可用，隐藏中间过程。
- 生成framework后进行二次处理。在生成framework后，通过脚本将framework中的flutter_assets移除。优点是不需要修改打包工具flutter_tools源码，缺点是增加了脚本对framework的处理，拉长了工作流程。

当前使用的是第二种方法，直接对产物进行二次处理，只为一个flutter_assets修改打包源码优点得不偿失。

查看源码发现flutter_assets在FlutterDartProject.mm的DefaultSettingsForProcess函数中被使用，初始化过程中会在app目录检查是否有flutter_assets，并且将路径保存在Settings的assets_path变量中，供程序后续使用。

```C++
// FlutterDartProject.mm
// Check to see if the flutter assets directory is already present.
if (settings.assets_path.size() == 0) {
    NSString* assetsName = [FlutterDartProject flutterAssetsName:bundle];
    NSString* assetsPath = [bundle pathForResource:assetsName ofType:@""];

    
}
```