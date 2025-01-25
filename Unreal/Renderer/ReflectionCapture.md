---
title: Mobile Reflection Capture 反射清晰度提升
date: 2022-01-08 08:51:00
tags:
    - Unreal
---

我们在做玻璃，或者光滑金属，或者基于cubeMap的室内等等效果的时候，往往需要较清楚的环境反射，但是在UE4的移动端中，使用廉价的`Reflection Capture`却得不到清晰的反射结果。这边文章讲的是如何改进UE4实现，以提升`Reflection Capture`作为镜面反射时的清晰度。
