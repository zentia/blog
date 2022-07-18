---
title: Unity Animation
categories:
- Engine
- Unity
---
# AnimationClip 序列化
```C++
class AnimationClip
{
    // 序列化字段
    dynamic_array<ACLTrackInfo> m_ACLTrackInfo;
    // 编辑器获取曲线数据，ACL需要生成一下。
    FloatCurves& GetEditorCurvesSync()
    {

    }

    // 获取某一条轨道上某一刻的轨道数据
    void EvaluateACLTrack(float time, int TrackIndex, ACLBoneTrackData& trackData)
    {

    }
}
```
# Animation 序列化
```C++
class Animation
{
    PPtr<AnimationClip>     m_Animation;
    Animations              m_Animations;
    // 非序列化字段，这里是骨骼的并集
    ACLBoundTracks          m_ACLBoundTracks;
    AnimationState*         m_ActiveAnimationStates[32];
    // 混合优化
    void BlendOptimized()
    {
        const ACLTrackInfo* track = state->GetACLTracks()[c];
    }
    // 这里初始化m_ACLBoundTracks
    void BindACLTracks()
    {

    }

    void RebuildStateForEverything()
    {
        for (size_t s = 0; s < m_AnimationStates.size(); s++>)
        {
            state = m_AnimationStates[s];
            AnimationClip* clip = state->m_Clip;
            if (clip == NULL)
                continue;
            if (state->IsACLClip())
            {
                // 这里有个问题就是trackIDLookup的value由于多个state导致一直在递增 
                InsertAnimationClipACLTrackIDs(trackIDLookup, *clip);
            }
        }
    }
    // 计算那些状态要进行融合的标记为
    bool RebuildBoundStateMask()
    {

    }
}

```
# AnimationState 控制动画混合
```C++
class AnimationState
{
    ACLTracks m_Tracks;
    // 只有相同的或者父子关系的骨骼才可以做混合
    bool ShouldMixTransform(Transform& transform)
    {

    }
}
```

`AnimationBinder.h`


