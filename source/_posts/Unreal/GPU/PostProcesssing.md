---
title: Unreal PostProcessing
date: 2022-01-04 09:49:00
tags:
    - Unreal
---
Unreal的后处理，会处理这个下面的所有Pass。
```
enum class EPass : uint32
{
    Distortion,
    SunMask,
    BloomSetup,
    DepthOfField,
    Bloom,
    EyeAdaptation,
    SunMerge,
    SeparateTranslucency,
    Tonemap,
    PostProcessMaterialAfterTonemapping,
    TAA,
    FXAA,
    SMAA,
    HighResolutionScreenshotMask,
    SelectionOutline,
    EditorPrimitive,
    PrimaryUpscale,
    Visualize,
    HMDDistortion,
    MAX
};
```
# PrimaryUpscale
```
bool bDisableUpscaleInTonemapper = !VarTonemapperUpscale || VarTonemapperUpscale->GetValueOnRenderThread() == 0;

bool bShouldPrimaryUpscale = (View.PrimaryScreenPercentageMethod == EPrimaryScreenPercentageMethod::SpatialUpscale && View.UnscaledViewRect != View.ViewRect);
...
PassSequence.SetEnabled(EPass::PrimaryUpscale, bShouldPrimaryUpscale && bDisableUpscaleInTonemapper);
```
PostProcessUpscale.cpp
```C++
FScreenPassTexture AddUpscalePass(FRDGBuilder& GraphBuilder, const FViewInfo& View, const FUpscaleINputs& Inputs)
{

}
```

# MobileBasePass_PostAO
```C++
void FGMobileSceneRenderer::RenderSeparateMobileBasePass(FRHICommandListImmediate& RHICmdList, const TArrayView<const FViewInfo*> PassViews)
{
    
}
```