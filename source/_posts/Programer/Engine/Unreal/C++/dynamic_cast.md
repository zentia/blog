---
title: UE-关于dynamic_cast的冷知识
date: 2022-01-26 15:38:00
categories:
- Engine
- Unreal
- C++
---
众所周知UE4把RTTI给禁止了，这导致dynamic_cast的支持是不完全的
```C++
#define dynamic_cast UE4Casts_Private::DynamicCast
```