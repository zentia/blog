---
title: PrivateDependencyModuleNames
date: 2022-01-21 09:54:00
categories:
- Unreal
---
https://zhuanlan.zhihu.com/p/107270501

# 为什么模块代码文件要区分Public和Private目录？
为了满足模块导出，因为模块里的代码默认是不暴露给其它模块，所以UE4约定，把需要导出的头文件放在模块的Public目录下。

# IncludePathModuleNames,DependencyModuleNames的区别？
二者都能是本模块可以包含其它模块Public目录下的头文件，从而调用函数
但IncludePathModuleNames只能调用定义在头文件里的函数
因为编译时会将包含的头文件的内容都拷贝一份到.cpp里，所以定义全部在头文件里的函数都正常工作。
而这带来的问题是：同一个函数，被编译多次（根据它被包含的次数），在大型项目里将拖慢编译速度。所以我们为了让函数只编译一次，才会把函数定义放在CPP文件里。
使用DependencyModuleNames时，会对两个模块进行链接，故不存在此问题

`
*IncludePathModuleNames是过时的用法，现在一般只用DependencyModuleNames
`

# PublicDependencyModuleNames, PrivateDependencyModuleNames的区别？

Private隐藏所有信息，Public只传递头文件信息，链接永不传递，需显示在Build.cs里添加。
结论：建议只是用PrivateDependencyModuleNames。