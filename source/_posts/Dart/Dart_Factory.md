---
title: Dart中Factory关键词的使用
date: 2022-8-17 20:25
categories: Dart
tags: Dart
---
# 描述
当你使用factory关键词时，你能控制在使用构造函数时，并不总是创建一个新的该类的对象，比如它可能会从缓冲中返回一个已有的实例或者是返回子类的实例。
# 使用场景
1. 避免创建过多的重复实例，如果已创建该实力，则从缓存中拿出来。
2. 调用子类的构造函数（工厂模式factory pattern）
3. 实现单例模式

# 情景分析
```Dart
class Logger {
    final String name;
    // 缓存已创建的对象
    static final Map<String, Logger> _cache = <String, Logger>{};

    factory Logger(String name) {
        return _cache.putIfAbsent(
            name, ()=> Logger._internal(name));
    }

    Logger._internal(this.name) {
        print("生成新实例：$name");
    }
}
```