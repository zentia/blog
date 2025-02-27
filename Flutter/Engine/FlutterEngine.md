---
title: FlutterEngine编译与调试
date: 2022-08-17 15:13:00
tags: Flutter
categories:
 - Flutter
---
FlutterEngine主要是由c++实现的。
是用ninja编译的。
ninja是什么呢？
ninja是一个编译器，http://martine.github.io/ninja/
是有google开发的。目的是解决makefile编译太慢的问题。
大家大部分都是前端开发，可能这块不太熟悉，其实我也不太熟悉，所以我现在想为大家分享一下c++编译的东西。
大家大学的时候肯定学过c/c++如果是计算机相关的专业的话。
一个通过c++文件生成可执行文件的过程，都是预处理器、编译器、链接器生成可执行文件。
预处理器阶段：预处理器cpp根据以字符#开头的明亮，修改原始的C程序，通常生成.i作为文件扩展名。
编辑阶段：编译器ccl将文本文件.i文件翻译成.s文件，它包含了一个汇编语言。
汇编阶段：汇编器将.s文件生成.o文件，.o文件是机器识别码
链接阶段：将.o文件生成可执行文件
gcc和clang是什么？
gcc和clang都是编译器。这个编译器是指可以生成整条流水线的编译器。
那make和ninja是什么？
它们两个都是快速批处理工具，本身没有编译和链接的功能，而是用类似批处理的方式通过调用配置文件来执行链接编译工作。
ninja处理速度比make快，仅此而已。
cmake是为上诉服务的，大家写这边批处理功能也很麻烦，然后cmake就帮我们生成了这些批处理工具，但是大家也觉得cmake也不方便，所以最近出来又出来个xmake，国人写的，赞一个。
源码编译和调试这个东西感觉和unity引擎编译一样迷，前几天死活编译不过，今天又好了。因为刚接触，所以里面的一些错误不太能解决，所以先不扯，网上看到的解决方案其实都是大家摸索出来的，
但是自己并没有这个摸索的能力，主要还是基本能力不够，编译引擎其实才是一个考验能力的活，一般大家都不以为然，本来我想最后说的，但是配置ide需要编译后的产物，所以我先说一下如何编译引擎。
其实大家网上搜索肯定写的比我仔细，但是本篇主要是用来分享的，所以我就在唠叨一遍。
google作为一个成立24年的公司，其历史包袱肯定很重，我所在的项目组由于继承王者那一套代码，历史包袱也很重，但是其实Google的代码还是很清晰的，所以不得不佩服人家，同时也不得不痛很我们的项目组，哀其不幸怒其不争。
所以flutter引擎编译起来会比较麻烦，他不像unity或者unreal直接拉下来，编译运行就可以跑了。
首先你要有代理，可以上Google的源代码网站，我这里是https://user.stotik.com/user 这个代理，大家如果想买的花，可以买个最便宜的，我认为是够了。我这边买的是V6高端套餐有点小后悔。
代理要设置http和https代理，还有ssh，还要有git代理，这里也网上搜一下就可以了。
然后下载dep_tools，使用git下载就可以了，这个东西是个仓库工具包，其实就是git增强版。
然后设置环境变量，export PATH=$PATH:/your_path/dep_tools
下一步开始设置引擎，我们的引擎最好就是从某个版本开始统一，我们可以定时更新，这样大家环境统一一些，自己也可以做一些定制化的事情。
目前我这里还是把自己的引擎更新到下载的flutter sdk里面指定的引擎，未来我们应该要使用我们自己的flutter引擎构建出来的sdk，而且最好打通自动化构建流程。
首先你要先fork flutter engine。
然后创建.gclient文件
```
solutions = [
  {
    "managed": False,
    "name": "src/flutter",
    "url": "git@github.com:zentia/engine.git",
    "custom_deps": {},
    "deps_file": "DEPS",
    "safesync_url": "",
  },
]
```
接着拉去代码。拉去速度取决于你的网速，所以公司看要不要统一办一个高带宽的梯子。
然后开始编译，编译主要使用两个工具，gn和ninja，我们前面已经说过ninja和makefile一样是一个批处理工具，速度要比makefile快。
那么gn其实是和cmakefile一样，是生成ninja配置的工具，毕竟Google，自己造轮子的能力还是很强大的。
最大的优势其实就是快，所谓天下武功，唯快不破，不论这个快是构建上的快，还是编写代码维护上的快，总之核心目的都是为了提升生产效率。让马儿吃草吃少点，干活干多点，本质和生产队的驴并没有什么区别。
编译分为debug、profiler、release版本，且每个版本都要区分优化为优化。
至此，我们的引擎编译出来了。接着就是分析代码，本篇着重分析。
工欲善其事，必先利其器。所以我们先搭建ide环境。
这里我们使用clion，clion是jetbrain推出的一款跨平台c++ide。
首先google代码看起来很舒服，编码规范符合Google开源编码规范，笔者曾写过不少c++代码，每个项目的相统一编码规范极其困难。
代码主要是有.h文件和.cc文件，cc文件是unix中常用，而非unix中常用cpp，但是其实市面上见的大多还是cpp文件，哪怕是unix系统。
一般来说一个项目分析，先看结构，了解个大概，然后在串流程。
引擎总共大概编译五千左右的文件，我初略看了一下，大部分是3rd的。容器是使用std的，其实自动c++17以来，std的容器已经做的很好的，所以没必要去造轮子了，当然如果内存要自己管理就另当别论的。
Unreal和项目组的数据结构就是因为考虑到内存自己的管理的原因实现自己的一套容器结构。其实std也已经把内存分配起暴露出来了，但是不和匹配项目组，或者来说，项目没有能力驾驭，或者没必要驾驭的一些想法。毕竟面对复杂的环境，命运不掌握在自己的手中却是有点难搞。
引擎中似乎没有反射，
分为
# Runtime
这里是dart虚拟机的主要实现。
## isolate
Dart是单线程，脚本语言基本都是单线程，我见到过的，至于为什么，我想答案应该是，你都把实现放到了脚本里面的，那你肯定认为开发效率和便捷性等等之类的要求是排在性能之后的，而且多线程你也把握不住啊，所以干脆不如把口子干掉算了。
这种做法我个人是不敢苟同的，但是站在项目的角度上分析，其实是正确的做法。dart为了弥补这部分的缺失，所以提供的isolate，他的意思是隔离的，就是我们知道线程之间数据共享，但是isolate之前数据不共享，有没有点sky的味道？
这样的好处就是，自己跑自己的，不用关心别人，自然也不会发生死锁，也就不会加锁。
# Common
公用配置文件
# third_party
这里面很大，其中vm就是在这里实现的。
# 调试
断点调试Flutter引擎来一步步观察引擎代码如何运行，是学习Flutter引擎代码的一个好办法。在介绍如何调试Flutter引擎前我们先来看看Flutter开发可能遇到哪些调试场景：
1. 调试Flutter App Dart 代码
2. 调试Flutter SDK Dart 代码
3. 调试Flutter 引擎 Java 代码
4. 调式Flutter 引擎 C++ 代码

第一种场景非常简单，只要在Android Studio中给Flutter App中的Dart代码打上断点即可进行调试
第二种场景也比较简单，在Android Studio中配置Dart & Flutter插件，运行调试第三方库和Flutter SDK Dart代码即可在相关源码中设置断点进行调试
# 调试Java代码
再来看第三种场景，调试Flutter引擎中的Java代码。
- [Debugging Android builds with Android Studio](https://github.com/flutter/flutter/wiki/Debugging-the-engine#debugging-android-builds-with-android-studio)

步骤如下：
1. 将`engine/src/flutter/shell/platform/android`导入到Android Studio。需要Android SDK29 和JDK8。
2. 使用自定义Flutter引擎运行Flutter App
3. Flutter 引擎工程中给源码设置断点并启动Debugger连接到已启动的Flutter App进程

# 调试C++代码
最后来看怎样调试Flutter引擎C++代码。主要参考的是：
- [Flutter Engine C++ 源码调试初探 | 区长](https://fucknmb.com/2019/12/06/Flutter-Engine-C-%E6%BA%90%E7%A0%81%E8%B0%83%E8%AF%95%E5%88%9D%E6%8E%A2/)
- [Flutter Engine源码调试 | xinbao的书屋](https://xinbaos.github.io/Flutter%20Engine%E6%BA%90%E7%A0%81%E8%B0%83%E8%AF%95/)