---
title: .Net .Net Framework mono
date: 2018-04-18 20:50:22
tags:
---
.net从一个抽象上来说其实是一个理念，使多种语言编写的程序能够通过一个通用的Runtime运行在不同的操作系统及硬件平台上。比如.net framework是在windows上实现的.net platform,mono是一个跨平台的.net platform
一个.net platform想到达到.net的目标，就需要一些组件，比如CLR(Common Language Runtime),比如FCL基础类库，比如各种语言编译器，编译器编译出来的东西想要在CLR中运行，那也需要遵循一定的标准，这就是CLI和CIL，CIL规定了编译输出的规则，而CLI规定了编译器输入语言的规则，只有符合这种标准的语言才能编译成CIL语言运行在CLR中。
好了现在有CIL和CLR，可以用符合CLI的语言比如C#编写程序了，然后将其编译成CIL，最后在CLR中运行
Unity能跨平台的原因就是实现了各个平台的"即时编译器"(C#代码->中间语言->各个平台的原生代码)