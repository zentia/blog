---
title: Unity Transform
categories:
- Engine
- Unity
---
```C++
class EXPORT_COREMODULE Transform : public Unity::Component
{
    typedef dynamic_array<ImmediatePtr<Transform>> TransformComList;
    typedef TransformComList::iterator iterator;

    TransformComList m_Children;
#if UNITY_EDITOR
    // 编辑器下的坐标系方向，比如是按照XYZ或者YZX显示
    math::RotationOrder     m_RotationOrder;
#endif
}
```