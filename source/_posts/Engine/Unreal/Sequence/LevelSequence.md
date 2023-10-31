---
title: LevelSequnce源码解析
categories:
- Engine
- Unreal
- Sequencer
tags:
- Engine
- Unreal
- Sequencer
---
# 组织结构

![](LevelSequence.svg)

## `ULevelSequence : UMovieSceneSequence`
`LevelSequence`是Sequence的资产组织形式，也就是一个uasset。
### `MovieScene`
`LevelSequence`的资产对象，所有序列化字段存储的地方。
#### `Spawnables`
这个是Sequence手动创建对象的集合
#### `Possessables`

#### `Tracks`
这个是所有的轨道

#### `CameraCutTrack`
单独的相机轨道

#### `UMovieSceneSection`

### Sequence的Eval过程
`EvalTemplate`和`ExecutionTokens`一起完成了Track的模拟。

#### `EvaluateGroup`
1. 收集这个Group包含的所有`FMovieSceneEvaluationTrack`
# 参考

- [1][UE4 LevelSequence源码解析](https://zhuanlan.zhihu.com/p/544571505)
- [2][UE4 LevelSequence源码剖析（一）](https://zhuanlan.zhihu.com/p/157892605)
- [3][UE4 LevelSequence源码剖析（二）](https://zhuanlan.zhihu.com/p/157897158)
- [4][UE4 LevelSequence源码剖析（三）](https://zhuanlan.zhihu.com/p/157898653)