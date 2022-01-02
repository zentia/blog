---
title: jobsystemnativecontainer
mathjax: true
date: 2019-05-10 14:14:20
tags:
    - Unity
categories: Unity
---
- Allocator.Temp has the fastest allocation.It is for allocations with a lefespan(寿命) of one frame or fewer.
- Allocator.TempJob is a slower allocation than Temp but is faster than Persistent.It is for allocations within a lifespan of four frames and is thread-safe. 
- Allocator.Persistent is the slowest allocation but can last as long as you need it to,