---
title: MobileSceneRenderer
date: 2022-01-04 15:05:00
tags:
    - Unreal
---
```C++
void FMobileSceneRenderer::RenderSeparateMobileBasePass(FRHICommandListImmediate& RHICmdList, const TArrayView<const FViewInfo*> PassViews)
{

}

FRHITexture* FMobileSceneRenderer::RenderForward(FRHICommandListImmediate& RHICmdList, const TArrayView<const FViewInfo*> ViewList)
{
    ...
    // 1577 Line
    if (!((IsMobilePixelProjectedReflectionEnabled(View.GetShaderPlatform()) && GetMobilePixelProjectedReflectionQuality() != EMobilePixelProjectedReflectionQuality::Disabled && View.bOpaqueNeedSSPR) || (bRequrisesAmbientOcclusionPass && View.bOpaquePostAO)))
    {
        ...
        RenderSeparateMobileBasePass(RHICmdList, ViewList);
        ...
    }
    ...
}
```