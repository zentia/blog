---
title: 基于il(Intermediate Language)的Flutter动态化方案
categories:
 - Flutter
---
# 背景
AOT(Ahead Of Time)要比JIT快，但是IOS无法热更新，ios无法动态修改函数地址还是啥来找，有点忘记了。
1. 生成IL
2. 执行IL

Flutter业务代码和SDK作为输入，首先会被解析成内存中的一颗抽象语法输（AST），AST可以被序列化成二进制的本地文件，被成文app.dill。Flutter在Debug阶段的产物kernel_blob.bin文件其实就是这个dill

# 自定义模版语言
代表方案：Flutter Fair - https://github.com/wuba/fair
核心思想就是下发一个json文件，然后解析成对ui的描述，因为json和dart的语言的都非常接近ast，所以这个转化不是很复杂。
这个方案的局限很明显：
- 不支持逻辑，json只有描述能力
- 脱离了dart生态，用json开发怎么能接受？

# 替换dart
mxflutter https://github.com/mxflutter/mxflutter

使用js替换掉了dart，用js生成widget树。

# 基于ast到处dsl

代表方案：Flag/Flutter web
https://tech.meituan.com/2020/06/23/meituan-flutter-flap.html

NDFlutter



# 运行dill文件
