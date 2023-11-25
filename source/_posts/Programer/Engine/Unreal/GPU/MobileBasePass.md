---
title: MobileBasePassRendering
date: 2022-01-04 15:49:00
tags:
    - Unreal
---
`r.Mobile.PixelProjectedReflectionQuality`
```C++
UENUM()
namespace EMobilePixelProjectedReflectionQuality
{
    enum Type
    {
        Disabled = 0 UMETA(DisplayName = "Disabled", ToolTip = "Disabled."),
        BestPerformance = 1 UMETA(DisplayName = "Best Performance", ToolTip = "Best performance but may have some artifacts in some view angles.")
        BetterQuality = 2 UMETA(DisplayName = "Better Quality", ToolTip = "Better quality and reasonale performance and could fix some artifacts but the  PlannarReflection mesh has to render twice."),
        BestQuality = 3 UMETA(DisplayName = "Best Quality", ToolTip = "Best quality but will be much heavier.")
    }
}
```
```C++
void SetupMobileBasePassUniformParameters(
    FRHICommandListImmediate& RHICmdList,
    const TArray<FVisibleLightInfo, SceneRenderingAllocator>& VisibleLightInfos,
    const FViewInfo& View,
    bool bTranslucentPass,
    bool bCanUseCSM,
    bool bTransluceUseBasePass,
    bool bUseCopySceneDepth,
    FMobileBasePassUniformParameters& BasePassParameters)
{

}
```