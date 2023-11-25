---
title: Niagara
sticky: 2000
categories:
- Engine
- Unreal
- Niagara
tags:
- Engine
- Unreal
- Niagara
---
Flocks
Over
Spawn Modules
Spawn Burst Instantaneous 一次性生成
Spawn Per Frame 每帧生成
Spawn Per Unit 每单位生成
Spawn Rate 生成速率
Spawn Group
相同ID组就可以驱动相同ID的属性（位置偏移，颜色，AGE）

# 渲染器
- `FNiagaraRendererComponents` 组件渲染器 无法使用GPU模拟
- `FNiagaraRendererLights` 光线渲染器 无法使用GPU模拟
- `FNiagaraRendererDecals` 贴花渲染器 无法使用GPU模拟
- `FNiagaraRendererMeshes` 网格体渲染器
- `FNiagaraRendererRibbons` 条带渲染器
- `FNiagaraRendererSprites` Sprite渲染器
- `UNiagaraRendererProperties` 几何体缓存渲染器 无法使用GPU模拟
- `FNiagaraRendererVolumes` 体积渲染器
# `FNiagaraDataSet`
数据传输
全局有两块buffer
`Buffer<float> NiagaraParticleDataHalf;`
`Buffer<float> NiagaraParticleDataFloat;`
# 编译
Niagara分为CPU和GPU两个执行方式，Niagara中的System Spawn、System Update、Emitter Spawn、Emitter Update这四个阶段，无论是Niagara Emitter设置为CPU执行还是GPU执行，其都是使用CPU的方式，将逻辑编译为虚拟机代码。
只有在Particle Spawn、Particle Update以及每个Simulation Stage这几个Stage中，按照CPU或GPU的选择，执行对应的虚拟机代码或HLSL指令，但是在编译过程中，这几个stage仍然是按照两种模式共同编译的，即编译结果既有CPU的虚拟机代码，也有hlsl指令集。
## `FNiagaraSystemViewModel`
## `TNiagaraHlslTranslator`
![](Niagara.svg)
# 虚拟机
编译器节点
# Module
# 参考
- [1][UE4 Niagara源码解析](https://zhuanlan.zhihu.com/p/362638250)
- [2][Niagara源码解析(编译部分)](https://km.woa.com/articles/show/507631?kmref=search&from_page=1&no=4)
- [3][UE5 Niagara虚拟机 VectorVM源码探析](https://km.woa.com/articles/show/568081)
- [4][创建视觉效果](https://docs.unrealengine.com/5.3/zh-CN/creating-visual-effects-in-niagara-for-unreal-engine/)
- [5][UE4引擎Niagara基础属性（一）](https://zhuanlan.zhihu.com/p/400113639)
- [6][【UE5】从零开始的Niagara教程 - 1.星光飘落](https://www.bilibili.com/video/BV1Wm4y1Y7h9/?spm_id_from=333.788&vd_source=cb0963cc837d74dea93b0b5ba5c736f1)