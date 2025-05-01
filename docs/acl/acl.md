---
title: acl
categories:
- Plugin
- acl
---
```C++
// acl_compressor.cpp 143
void GetCompressionSettings(acl::compression_settings& OutSettings); // 这里需要改成可配置
```
```C++
struct transform_metadata 
{
    const uint32_t* transform_chain     = nullptr;
    uint32_t parent_index               = k_invalid_track_index;
}
struct clip_context
{
    // 变换元数据
    transform_metadata* metadata        = nullptr;
}
// bitset.h 178
inline bool bitset_test()
// clip_context.h 118
bone_chain();
// cli_context.h 173
bone_chain get_bone_chain()
{
    return bone_chain(meta.transform_chain);
}
// quantize_streams.h 1107
// bone_index 根骨骼出问题
inline uint32_t calculate_bone_chain_indices(const clip_context& clip, uint32_t bone_index, uint32_t& out_chain_bone_indices)
{

}
// quantize_streams.h 1152
inline void find_optimal_bit_rates(quantization_context& context)
// quantize_streams.h
// lossy_clip_context 有损压缩clip上下文
inline void quantize_stream(allocator, lossy_clip_context, settings, raw_clip_context, additive_base_clip_context, out_stats);
// clip_context.h 353
inline bool initialize_clip_context()
{
    metadata.transform_chain = bone_chain;
}
// compress.transform.impl.h
// 有损压缩上下文
quantize_streams(allocator, lossy_clip_context, settings, raw_clip_context, )
```
父骨骼索引设置错了，Root的parentIdx应该是无效的，这里赋值了0

1. 需要加一个进度条
2. 需要加配置
3. 需要改成多线程

```C++
// Bone 数量119根，Unity没有地方限制骨骼数量吗
// AnimationCurve.cpp 209
// lhsIndex = 25
// rhsIndex = 26
void AnimationCurveTpl<T>::CalculateCacheData(int lhsIndex, int rhsIndex)
{
    // coeff is nan
    DebugAssert(IsFinite(cache.coeff[0]));
}
bool ACLCompressor::BuildBones(AnimationClip& clip,)
{
    Vector3f sample = element.curve.EvaluateClamp(k / clip.GetSampleRate());
}
```
# `ACLCompressor.h` Unity侧接口

`void ACLCompressor::Decompress(AnimationClip& clip, int trackIndex, float time, Vector3f& pos, Vector3f& scale, Quaternionf& quat)`
解压缩接口

# 参考

{% pdf simple_and_powerful_animation_compression_gdc2017.pdf %}