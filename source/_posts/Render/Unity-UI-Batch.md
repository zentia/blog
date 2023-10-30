---
title: Unity源码解读-UI合批
mathjax: true
date: 2019-04-09 01:32:27
tags:
categories:
    Unity
---
合批规则：
1. 共面 z值要相同
2. depth要相同
3. bound不相交
4. 同一个canvas下
5. 材质要相同

以下有几个指令：
1. 渲染UI指令 `RenderableUIInstruction`
2. UI指令 `UIInstruction`
3. 几何变化指令 `TransformGeometryInstruction`
4. 几何job指令 `GeometryJobInstruction`

以下是JobData
1. UI几何JobData `UIGeometryJobData`
