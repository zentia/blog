---
title: 一文掌握Flutter插件开发
---
# 概述
随着开发技术的发展，几乎所有主流的开发语言都有自己的包管理工具，Node开发npm、Android开发有Gradle，Flutter也有自己的Dart Packages仓库。插件的开发和服用能够提高开发效率，降低工程的耦合度，向网络请求(http)、用户授权（permission handler）等客户端常用的功能模块，我们只需要引入对应插件就可以为项目快速集成相关能力，专注于具体业务功能的实现。

除了使用仓库中的流行组件以外，在Flutter项目开发过程中面对通用业务逻辑拆分、或者需要对原生能力封装等场景时，开发者仍然需要开发新的组件。本文以一个具体的native_image_view插件为例，将从Flutter组件的创建、开发、测试和发布等多个方面进行介绍，力图完整的展示整个Flutter组件的开发和发布流程。

# Flutter与Native通信

在Flutter插件开发过程中，几乎都会需要进行Flutter与Native端的数据交互，因此在进行插件开发之前，我们先简单了解下Platform Channel机制。

Flutter与Native的通信是通过Platform Channel实现的，它是一种C/S模型，其中Flutter作为Client，iOS和Android平台作为Host，Flutter通过该机制向Native发送消息，Native在接受消息后调用平台自身的API进行实现，然后将处理结果在返回Flutter页面。
![](FlutterPlugin1.png)
Flutter中的Platform Channel机制提供了三种交互方式：
- `BasicMessageChannel`，用于传递字符串和半结构化信息
- `MethodChannel`，用于传递方法调用和处理回调
- `EventChannel`，用于数据流的监听与发送
这三种channel虽然用途不同，但都包含了三个重要的成员变量：
- `String name`，表示channel的名字，在一个项目中可能会有很多的channel，每个channel都应该使用唯一的命名标识，否则可能会被覆盖。推荐的命名方式是组织名称🏠插件的名称，例如：`com.tencent.game/native_image_view`，如果一个插件中包含了多个channel在根据功能模块进一步区分。
- `BinaryMessage message`，作为Native与Flutter通信的载体，能够将codec转换后的二进制数据在Native与Flutter之间进行传递。每个channel在初始化时都要生成或提供对应的messager，如果channel注册了对应的handle，则message会维护一个name与handle的映射关系。Native平台在收到对方发来的消息后，如果channel注册了对应的handler，则messager会维护一个name与handler的映射关系。Native平台在收到对方发来的消息后，messager会讲消息内容分发给对应的handler进行处理，在处理完成后开可以将结果放回给Flutter。
![](FlutterPlugin2.png)
