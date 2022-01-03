---
title: GameplayAbility深入解析之技能同步基础
date: 2022-01-03 23:59:00
tags:
  - Unreal
---
# [GameplayAbility深入解析]之技能同步基础

https://zhuanlan.zhihu.com/p/159007942

```C++
/** 
 *	Associative container of GameplayAbilitySpecs + PredictionKeys --> FAbilityReplicatedDataCache. Basically, it holds replicated data on the ability system component that abilities access in their scripting.
 *	This was refactored from a normal TMap. This mainly servers to:
 *		1. Return shared ptrs to the cached data so that callsites are not vulnerable to the underlying map shifting around (E.g invoking a replicated event ends the ability or activates a new one and causes memory to move, invalidating the pointer).
 *		2. Data is cleared on ability end via ::Remove.
 *		3. The FAbilityReplicatedDataCache instances are recycled rather than allocated each time via ::FreeData.
 * 
 **/

FGameplayAbilityReplicatedDataContainer

／**  
  * GameplayAbilitySpecs + PredictionKeys的关联容器——> FAbilityReplicatedDataCache。 基本上，它在能力系统组件上保存复制的数据，能力在脚本中访问。  
  * 这是从一个普通的TMap重构的。 这主要服务于:  
  * 1。 返回共享的ptrs到缓存的数据，这样调用站点就不会受到底层映射移动的影响(例如调用一个复制的事件结束了这个能力或激活了一个新的事件，导致内存移动，使指针失效)。  
  * 2。 数据清除能力结束通过::Remove。  
  * 3。 FAbilityReplicatedDataCache实例每次都被回收，而不是通过::FreeData分配。  
  ＊  
  ** /  

 ```
