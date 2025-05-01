---
title: Unreal Kawaii
date: 2022-01-02 11:31:00
tags:
    - Unreal
---
# kawaii物理设置
`FKawaiiPhysicsSettings`
`Damping`衰减
`WorldDampingLocation`世界位置衰减
`WorldDampingRotation`世界旋转衰减
`Stiffness`硬度
`Radius`半径
`EdgeRadius`边半径
`LimitAngle`限制监督
`LimitAngleAlgorithm`限制角度算法
`Mass`块
`ClothBendK`衣服弯曲

# Kawaii物理修改骨骼
`BoneRef`骨骼引用
`ParentIndex`父亲索引
`ChildIndexs`孩子索引
`FKawaiiPhysicsSettings PhysicsSettings`物理设置
`Location` 位置
`PrevLocation` 上一个位置
`PredictLocation` 预测位置
`PoseLocation`姿势位置
`PoseRotation`姿势旋转
`PoseScale`姿势缩放
`PoseRelativeTransform`姿势相对变换
`PoseTopBose`T-Pose骨骼
`PoseTopBoseIndex` T-Pose索引
`WorldDampingOffset`世界衰减偏移
`WindVelocity`风速
`LengthFromRoot`从跟的长度
`bDummy` 是不是假的
`bSurfaceAdjusted` 表面调整
`SurfaceAlpha`表面Alpha值
`bClothIntersected` 衣服交叉
`FrameNumber`帧数

```C++
// 更新姿势变换
// @param BoneContainer 骨骼约束
// @param Pose 姿势
void UpdatePoseTranform(const FBoneContainer, FCSPose<FCompactPose>& Pose);
```