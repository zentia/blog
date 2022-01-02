
```C++
/**
 * 约束：
 * - 骨骼索引数组：
 * - 骨骼切换数组：
*/
struct FBoneContainer
{
private:
    /** Array of RequiredBonesIndices. In increasing order.
     * 这个的数组长度会和Compact相同里面的值和SkeletonToCompactPose不同
    */
    TArray<FBoneIndexType> BoneIndicesArray;
    TArray<int32> CompactPoseToSkeletonIndex;
    TArray<int32> SkeletonToCompactPose;
    TArray<FCompactPoseBoneIndex> SkeletonToCompactPose;
    TArray<FCompactPoseBoneIndex> CompactPoseParentBones;
    TArray<FTransform> CompactPoseRefBoseBones;
};

class SkeletalMeshComponent
{
    /** Temporary array of bone indices required this frame. Filled in by UpdateSkelPose.*/
    TArray<FBoneIndexType> RequiredBones;
};
```