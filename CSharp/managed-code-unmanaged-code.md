托管代码是-Microsoft的中间语言(IL)，他主要作用是在.Net FRAMEWORK的公共语言运行库(CLR)执行代码前去编译源代码，也就是说托管代码充当着翻译的作用，源代码在运行时分为两个阶段：

1. 源代码编译为托管代码，(所以源代码可以有很多种，如VB,C#,J#)
2. 托管代码编译为Microsoft的平台专用语言

编译器把代码编译成中间语言(IL),而不是能在你的电脑上运行的机器码。中间语言被封装在一个叫程序集(Assembly)的文件中，程序集中包含了描述你所创建的类，方法和属性(例如安全需求)的所有元数据。你可以拷贝这个程序集到另一台服务器上部署它。

托管代码在公共语言运行库(CLR)中运行。这个运行库给你的运行代码提供各种各样的服务，通常来说，他会加载和验证程序集，以此来保证中间语言的正确性

1. 托管代码是一种中间语言，运行在CLR上；
非托管代码被编译为机器码，运行在机器上
2. 托管代码独立于平台和语言，能更好的实现不同语言平台之间的兼容；
非托管代码依赖于平台和语言。
3. 托管代码可享受CLR提供的服务（如安全检测，垃圾回收），不需要自己完成这些操作
非托管代码需要自己提供安全检测、垃圾回收等操作。
