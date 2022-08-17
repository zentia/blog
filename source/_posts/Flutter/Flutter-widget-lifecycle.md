---
title: Flutter-widget生命周期
date: 2022-08-17 15:13:00
categories: Flutter
tags: Flutter
---

# 背景
页面生命周期是开发者在学习移动端技术栈（包括Android、iOS、Flutter）接触最早的一个知识点，因为业务开发中涉及最多的通常是UI交互。
# 生命周期存储
StatefulWidget通过State对象存储widget的生命周期，有两种方式：

(1) createState

(2) 创建Steam或者ChangeNotifier
# 生命周期分析
StatefulWidget生命周期使用问题简言之是：在错误的阶段执行了指定的业务逻辑。widget的生命周期有以下阶段：

createState

自定义widget是否必须继承重写：是

调用时机：

1、widget创建第一个阶段

2、如果有多个位置插入该widget，那么分别各调用一次

3、当widget从widget树删除之后再次插入，也会执行

initState

自定义widget是否必须继承重写：是

调用时机：

1、createState执行后的下一个阶段

didUpdateWidget

自定义widget是否必须继承重写：否，如果页面数据不依赖widget配置参数，可以不用重写这个方法。

调用时机：widget配置参数变化时

容易出问题的地方：

1.didUpdateWidget执行后系统会自动调用build方法，所以不要在didUpdateWidget里面再手动调用setState避免重复刷新。

案例一：
```

```
# 与状态相关的-Key
# 减少刷新对性能的影响
# 拆解build