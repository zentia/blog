# Render Passes

## Forward Rendering

- Forward Rendering = Forward Shading，通常被译作“正向（前向）渲染（着色）”
- 基本思想：依次遍历场景中的每个物体，将所有光源对它的影响考虑在内，计算光照结果并渲染该物体。
- 优劣
  - 实现思想简单直接、符合直觉，收到广泛硬件支持
  - ## 对性能影响大，Overdraw 严重

## Buffers

### Z-Buffer

- Z-Buffer = Depth Buffer，Z 表示纵深方向，存储着各像素在该方向上的深度值（Z-Value）

### G-Buffer

- G-Buffer = Geometric Buffer，

### D-Buffer

- D-Buffer = Decal Buffer，是 UE 中用于存储延迟贴画材质信息的缓冲区，与 G-Buffer 类似，由多张纹理组成。
- D-Buffer 用于渲染 Deferred Decal，此时还需要得到场景的深度信息，因此也会读取 Z-Buffer。
- D-Buffer 将作为输入信息，参与渲染 G-Buffer（往 G-Buffer 中写入数据）的过程

## Passes

### What

- 整个渲染管线可被拆分成一系列的 Pass，每个 Pass 在这个过程中有所共享，协力呈现出最终的渲染结果
- Pass 本质上 GPU 执行一系列 Draw Call（蘸上那种颜料，往哪些物体上涂抹）的过程，内部包括多个子步骤，将这些步骤组织成单个 Pass 在依次执行，像是将多余语句封装成一个个独立的函数在依次条用，从而保证渲染管线能够合理、有序地运转。

### Cost

- 如果一个 Pass 需要处理的 polygon(triangle/quad)数量过多，则属于 geometry-bound
- 如果一个 Pass 需要处理的像素过多，而且处理像素的复杂度太高，那么就属于 pixel-bound
- 如果一个 Pass 需要读取或输出的信息量太大，则瓶颈位于内存和读写带宽(bandwidth)

### BasePass

### early-z test

传统的 depth test(late z test)是发生在 fragment shading 之后的。但是后来人们发现，其实在光栅化之后 fragment shading 之前，fragment 的深度就可以计算出来了，有了深度其实很多情况下就可以做 depth test (early z test)了，这样就可以提前剔除掉被遮挡的 fragment，大大节省 GPU 的计算能力。

### Depth prepass

depth prepass 就是在一个 render pass 中，首先先渲染物体的深度，然后在渲染物体的颜色。在渲染深度时，只为了更新 depth buffer，所以需要打开 depth test 以及 depth write mask，同时关闭 color 的 write mask，depth buffer 更新完成后，渲染该物体的 color 时，打开 depth test 并设置比较函数为"Equal"，打开 color write mask 来着色，由于 depth 之前已经更新好了所以这时候应该关闭 depth write mask，这样就可以完成该物体的渲染。
depth prepass 的目的主要是为了移除 overdraw。

#### Overview

- BasePass 将处理场景中所有可见的 3D 模型，执行它们所有相关材质的 shader 逻辑，然后将计算好的几何信息写入 G-Buffer 中
  - 在 Deffered Rendering 中，BasePass 就是它的第一阶段：Geometry Pass，写入 G-Buffer 的信息后续将被用于计算光照，以及基于屏幕空间的 Pass(e.g. Dynamic Reflection, Ambient Occlusion 等)
  - 在 Forward Rendering 中，BasePass 即处理集合，也处理光照，直接影响最终的着色结果
- BasePass 的执行过程还包括对延迟贴花(Deferred Decal)的处理，由于贴花会影响物体的几何效果，BasePass 阶段会读取 D-Buffer 中的信息，并将它和即将写入 G-Buffer 的信息融合，使物体在融合了贴花效果之后，再在后续接受光照处理

## 分块延迟渲染 Tile-BasedDeferred Rendering

作为传统 Deffereed Rendering 的另一种主要改进，分块延迟渲染（Tile-Based Deferred Rendering，TBDR）旨在合理分摊开销（amortize overhead），自 SIGGRAPH 2010 上提出依赖逐渐被业界所了解。
