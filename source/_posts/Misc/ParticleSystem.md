---
title: ParticleSystem
mathjax: true
date: 2019-05-09 09:32:45
tags:
    - Unity
categories: Unity
---
```CSharp
ParticleSystem ps = GetComponent<ParticleSystem>();
var main = ps.main;
main.startDelay = 5.0f;
main.startLifeTime = 2.0f;
```
