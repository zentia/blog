---
title: EventSource
mathjax: true
date: 2019-05-10 14:03:59
tags:
    - GameFramework
categories: GameFramework
---
1. 原则上不会出现托管堆数据，不会List接口，数据统一申请，按需释放。
2. 充分利用CPU，防止让线程之间切换时间片的发生。