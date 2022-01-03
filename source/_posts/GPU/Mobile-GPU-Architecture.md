---
title: 移动GPU架构浅析
mathjax: true
date: 2019-09-04 08:53:59
tags:
    - GPU
categories:
---
移动GPU相对桌面GPU只能算小弟弟，移动GPU的劣势主要表现在理论性能和带框。移动GPU受限于芯片面积，能耗以及成本所以必须牺牲部分性能和带宽来求得性价比和电池续航的平衡。与桌面GPU动辄256bit甚至512bit的带宽、1.2-1.5GHz的高频显存相比，移动GPU不仅要和CPU共享内存带宽，而且普遍使用的是双32bit位宽、LPDDR2-800或1066左右的内存系统，总带宽普遍在10GB/s以内。

![1](/images/GPU/Mobile-GPU-Architecture/1.png)
在上图中移动处理器中内存带宽最高的是iPad 3/4，因为他们使用Retina屏幕，2048x1536的高分辨率对GPU带宽要求更高，不过就算是这两款产品，17GB/s的带宽与PC显卡动辄200GB/s以上的带宽相比还是小儿科，没有带宽就没有大容量纹理数据，也就不会有高画质。尽管带宽不是制约移动GPU发展的唯一因素，但是在目前的限制下，移动GPU厂商关心的头等大事就是如何尽可能小的带宽需求下提升GPU性能及画质，纹理压缩是一个方法，还有一种就是使用不通的渲染架构。目前在GPU领域主要有IMR、TBR及TBDR等三种主流架构。
# 移动GPU的模型
## IMR模式
![IMR](/images/GPU/Mobile-GPU-Architecture/2.png)
IMR（Immediate Mode Rendering）就如字面意思一样，提交的每个渲染命令都会立即执行，并且该渲染命令会在整条流水线中执行完毕后才开始执行下一个渲染命令。

这种模式的优点：
- GPU架构比TBR模式简单直接。
- 在一帧里面执行FBO操作时，不会因为需要清空缓冲的渲染指令而影响性能。
- 不用像TBR架构一样需要片上高速缓存来保存中间结果。
- 不用像TBR架构一样缓存Triangle List，因此在有大量顶点运算的场景时比TBR有优势。例如PC上面的复杂模型可能有几百万个triangle。

这种模式的缺点就是：
- IMR的渲染会存在浪费带宽的情况。例如，当两次渲染有前后遮蔽关系时，IMR模式因为两次draw命令都要执行，因此会存在经过Pixel Shader后的Pixel被Depth test抛弃，这样就浪费了Shader Unit运算能力。不过幸运的是，目前几乎所有的IMR架构的GPU都会提供Early Z的判断方式，一般是在Rasterizer里面对图形的遮蔽关系进行判断，如果需要渲染的图形被遮挡住，那么就直接抛弃该图形而不需要执行Pixel Shader。
- IMR的另外一个缺点就是其渲染命令在执行需要随时读写frame buffer，depth buffer和stencil buffer，这带来大量的内存带宽消耗，在移动平台上面访问片外内存是最消耗电量和最耗时的操作。

因此在桌面GPU灵越，TBR节省带宽和低性能不符合PC机的要求，IMR一统江湖。但是在移动GPU领域，TBR的低带宽消耗，低功耗正好满足移动设备需求，与其在PC端的待遇相反，移动设备领域TBR几乎一统江湖。
IMR模式的代表是NVIDIA的Tegra和Vivante的GC系列芯片。在此列出Vivante GC芯片的内部架构图，我们可以看到Vivante的GPU架构相当简单。
![3](/images/GPU/Mobile-GPU-Architecture/3.png)
## TBR模式
![4](/images/GPU/Mobile-GPU-Architecture/4.png)
与IMR简单粗暴的做法不通，TBR（Tile Based Rendering）它将需要渲染的画面分成一个个的矩形区块（tile）,tile一般是4x4或者8x8的矩形块。模型的顶点金经过过Vertex Shader运算以后会组装成一个个的triangle，这些triangle会被缓冲在一个triangle cache里面。如果某个triangle需要在某个tile里面绘制，那么就会在该tile的triangle list中存在一个索引。等一帧里面所有的渲染命令都经过执行完Vertex Shader生成triangle以后，每个tile就会有一个triangle list，这list就包含了需要在该tile内部绘制的所有triangle。然后GPU在基于triangle list执行每个tile的raster和Per-fragment operation。

TBR的优点是执行raster和Per-fragment operation时不需要反复的访问frame buffer，depth buffer，stencil buffer。这是因为GPU可能把整个tile的frame buffer/depth buffer/stencil buffer保存在一个片上的高速缓冲中，这样GPU就直接访问tile，而不需要访问外部内存。这大大减少了内存的带宽消耗，也意味着能耗的降低。

TBR的缺点是需要保存Vertex Shader执行后的结构以及每个tile的triangle list。这意味着如果场景里面有很多的顶点，那么片上缓存就不可能存下这么多顶点信息和triangle list，就不能不依靠外部内存来存储，就会有额外的带宽消耗。不过庆幸的是当前的移动3D绘制都会不会有太多的triangle的场景。一个复杂的模型也就是1万多个triangle，因此一个通常的场景大概就是几十万triangle。随着移动游戏越来越复杂精美，模型的复杂程度也会快速上升，这也是TBR架构在未来将会面临的一大挑战。

如果在一帧里面有两遍及其以上的渲染，那么就需要使用Frame buffer object 来缓存中间结果，这对TBR又是一大性能损耗。根据我们前面的讲解，TBR需要缓冲一帧所有的图元，所有图元执行完毕后才开始raster和Per-fragment operation。在这种情况下，一旦后面的draw命令需要使用前面渲染生成的结果，那么就不得不在该命令前执行，要求GPU把缓存的所有draw命令都执行完毕，然后放弃当前缓存内容。在极端情况下，例如每次draw都需要读取前一次draw渲染的结果，那么TBR就会直接退化成IMR模式。
  
基于以上的缺点，我们可以看出在桌面GPU领域TBR没有任何优势，因此其完全退出桌面GPU市场。但是在移动GPU市场它更适应性能/带宽/能耗三者的平衡。

TBR的代表有ARM的Mali和高通的Adreno。下图是Mali的架构图：
![Mali](/images/GPU/Mobile-GPU-Architecture/5.png)
## TBDR模式
![TBDR](/images/GPU/Mobile-GPU-Architecture/6.png)
TBDR（Tile Based Deferred Rendering，贴图延迟渲染）算是TBR的近亲，它跟TBR原理相似，但是通过HSR（Hidden Surface Removal，隐藏面消除）操作，在执行Pixel Shader之前进一步减少了不需要渲染的fragment，降低了带宽需求。在执行Pixel Shader之前，对Raster生成的每个像素都做depth test比较，剔除被遮挡的像素，这就是HSR的原理。理论上经过HSR剔除以后，使得TBDR每帧需要渲染的像素上限就是屏幕像素的数量（没有考虑alpha blend的情况下）。而传统的TBR在执行复杂一点的游戏时可能需要渲染6倍于屏幕的像素。

TBDR是PowerVR的王牌，因为TBR的HSR带来的带宽和运算开销的降低，使得苹果手机的续航能力让人惊叹。下图是PowerVR的SGX系列的GPU架构图，可以看到其复杂程度大大超过Vivante这类IMR的架构。
![PowerVR](/images/GPU/Mobile-GPU-Architecture/7.png)