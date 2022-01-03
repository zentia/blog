---
title: LevelSequencer
date: 2022-01-03 20:07:00
tags:
   - Unreal
---
https://www.bilibili.com/video/BV16y4y1S7yk?spm_id_from=333.999.0.0
https://www.bilibili.com/video/BV1YV411a7SV/?p=2&spm_id_from=pageDriver



1. LevelSequencer的`CutCameraTrack`的意思是用来切换相机的，且`MovieScene`会有一条单独的`CutSceneTrack`去管理。
2. LevelSequencer文件分类是由`TrackEditor`，`Track`，`Section`，`Channel`，`Template`等组成。
   1. 有一个`MovieScene3DTransform`但是这个文件彷佛是一个误导逻辑，实际的求值不会走。
   2. `(&GetComponentTransform, &SetComponentTransformAndVelocity);`这两个函数才是真正会走的地方。
   3. LevelSequencer是多线程的，调试经常容易卡死。