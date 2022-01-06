---
title: MobileBasePassVertexShader
date: 2022-01-04 19:34:00
tags:
    - Unreal
---
```C++
void Main(
    FVertexFactorInput Input
    , out FMobileShadingBasePassVSOutput Output
#if INSTANCED_STEREO
#elif MOBILE_MULTI_VIEW
#endif
)
```