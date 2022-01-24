---
title: Zlib/Oodle/ZSTD 压缩算法性能对比
date: 2022-01-02 15:38:00
categories:
- Engine
- Unreal
- Profiling
---

# Zlib/Oodle/ZSTD 压缩算法性能对比

https://ue5wiki.com/wiki/30732/

## 总结

压缩率和解压性能，是一个相互取舍的因素，从对比数据来看，不管是Oodle/ZSTD，都在保持压缩率的同时，解压效率都远超Zlib。
从对比数据来看，ZSTD的22级性能与Oodle的Leviathan相近，Kraken则在解压速度上更胜一筹，所以在游戏中使用Kraken+fast是比较合理的方案。
虽然选用其它的`CompressLevel`如`Optimal2 / Optimal5`，但是会增加数倍的压缩事件，在项目资源量巨大的情况下，有些得不尝失。