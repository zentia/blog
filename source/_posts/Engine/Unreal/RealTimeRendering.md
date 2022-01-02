# Real Time Rendering in UE4
1. Intro
2. Before Rendering
3. Geometry Rendering
4. Rasterizing and GBuffer
5. Textures
6. Pixel Shaders and Materials
7. Reflections
8. Static Lighting/Shadows
9. Dynamic Lighting/Shadows
10. Fog/Transparency
11. Post Processing

## Intro

Direct Lighting
    Specular:
        GGX Shading model
    Diffuse
        IES (PRECAL) (REAL TIME)
        Light Functions (REAL TIME)
    Shadows:
        CSM (REAL TIME)
        Dynamic shadows (REAL TIME)
        Distance Field shadows (REAL TIME)
        Inset shadows (REAL TIME)
        Per object shadows (REAL TIME)
        Contact shadows (REAL TIME)
Indirect:
    Specular:
        Planar(PRECALCULATED)(REAL TIME)
        Reflection Cap(PRECALCULATED)
        SSR (REAL TIME)
    Diffuse:
        Lightmaps (PRECAL)
        Indirect Lighting Cache (PRECALCULATED)
    Shadows:
        DFAO (REAL TIME)
        SSAO (REAL TIME)
        Capsule shadows (REAL TIME)

## Before Rendering

Frame 1 - Time 33 ms - Draw Thread (mostly CPU)

Occlusion process - Builds up a list of all visible models/objects
Happens per object - Not per polygon

4 Stage process - in order of execution

1. Distance Culling
2. Frustum Culling
3. Precomputed Visibility
4. Occlusion Culling

## Geometry Rendering

Drawcalls Performance Implications
If you merge meshes following rules make better candidates:
1. The more common a mesh AND the lower poly the better
2. Merge only meshes wihtin the same area(你应该只合并同一区域的网格体)
3. Merge only meshes sharing the same material(只合并相同材质的)
4. Meshes with no or simple collision are better for merging(没有碰撞，或者碰撞简单的更适合合并)
5. Smaller meshes or meshes receiving only dynamic are better candidates(更小的网格体或者只接受动态光照的)
6. Distant geometry is usually great to merge(远距离的网格体也适合)

## Rasterizing and GBuffer
Rasterization and Overshading Performance Implications

1. Polygons are more expensive to render in great density(多边形如果越密集渲染起来损耗就更大)
2. When seen at a distance the density increases(从远处看，密度会自动增加。因为这个球体的多边形数量始终一致，但是显示仙童数量多边形的像素变得更少，从远处观察的时候，每个像素的多边形密度会增加)
3. Thus reduce polygon count at a distance (lodding/culling) is critical(你应该使用LOD或者剔除在远距离上，减少多边形数量从而克服这点问题)
4. The more complex the initial pixel shader pass it the more expensive overshading is. Thus forward rendering receives a bigger hit on performance from this than deferred（因此，过度着色器对前向渲染效率，在延迟渲染中，过度着色并不是最大的问题，它并不会非常慢，过度着色会增加损耗，但并不会太大）