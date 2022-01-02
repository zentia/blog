---
title: 移动平台的AlphaTest效率问题
mathjax: true
date: 2019-04-25 23:58:38
tags:
    - Shader
categories: Shader
---
{% asset_img 1.jpg %}
# 为什么AlphaTest应该比AlphaBlend更慢？
AlphaTest在渲染时实际上和不透明物体类型，与其说是一种“透明物体”，不如说是一种“可以在上面打洞的不透明物体”。在Unity里，也可以允许AlpahTest和Opaque混合绘制。光照，是否可接受投影，在渲染管线的位置，AlphaTest的物体都是完全按Opaque处理的。
AlphaTest实际上就是一种特殊的Opaque，只是允许在像素阶段discard像素，让某些像素不绘制到屏幕上，从这个角度看，甚至还应该比Opaque更节约性能猜对。
而AlphaBlend则是纯粹的透明物体。透明物体多=效率低也是常见的“经验结论”，主要原因是ALphaBlend会在Blend的时候读取屏幕缓冲区，以及必须重复计算每个像素的多层内容，高overdraw会很快烧光fillrate。
# 但为什么会有人说移动平台AlphaTest比AlphaBlend更慢呢？
**是因为移动平台上广泛使用了early-DT（提前的Depth Test）**
只做移动平台的会产生一种误解：认为先画了一个物体，在某个区域写上了深度，然后在之后再画一个深度比它低的物体，因为被遮挡所以画不上去，那GPU就不应该花时间去画
{% asset_img 2.jpg %}
然而事实上，之前的GPU并没有那么“聪明”。
这个技术本身就是early-DT。虽然只有苹果自己在吹，实际上所有移动平台的GPU都使用这个技术。也只有使用了这个技术的GPU，才能实现刚才说的这种“绘制优化”。
{% asset_img 3.jpg 只有将DepthTest放在Frag Shader之前，才有可能中断之后Frag Shader获得性能提升 %}
而其他的GPU由于没有这个技术，DepthTest阶段都是在Frag Shader之后的（也就是later-DT），因此屏幕内的物体并不存在遮挡剔除。
PC和移动平台拥有ealy-DT，它的功能和Deferred Shading的功能部分重叠的，所以才不建议使用Deferred Shading，因为获得的收益很可能会反倒不如Deffed技术新增的成本。
了解了early-DT和later-DT后，还需要在谈一个东西：
# 并行化和串行化
硬件进行计算时，并不是一个像素一个像素地执行，而是同时多个像素同时进行，这就是并行化。
但是在这个并行化的多个线程中，他们并不总能保持齐头并进，在Frag阶段，总会由于各种原因导致他们结束的时间点出现不一致，有可能出现先画的像素反而后绘制的情况。
{% asset_img 4.jpg %}
并不是所有的像素都按要求顺序完成，但是假如我们强行设置了他们的绘制顺序，GPU也必须遵守，所以只能这样：
{% asset_img 5.jpg %}
当然，在这个Wait的阶段如果GPU能够找到什么别的不需要操作Frame Buffer的工作，它也不会闲着，但假如确实找不到任何“可以干的活”，它也就只能闲着了。
这就是串行化，显然，会造成性能下降。
然而，GPU其实也没有这么蠢。
一个显而易见的优化是：其实我只需要保证绘入Frame Buffer时的顺序是一致的，那么我把Frag Shader的结果堆起来放在一个队列里，保证这个队列顺序是一致的，那前面Frag Shader什么时候结束也就无所谓了。
在later-DT的时候确实是这样的：
{% asset_img 6.jpg %}
很容易被忽略掉的一点是，这个Depth Test过程，其实是一个读+写的过程，不透明物体在读取Depth之后，还需要马上把自己的Depth写回去，这样上一个像素把Depth写进去后，下一个才能读出来，所以Depth Test的顺序同样必须争取的。
所以换成early-DT之后
{% asset_img 7.jpg %}
虽然Frame Depth和Frame Color Buffer操作的时间点被错开了，但是分别的顺序还是一致的，并没有问题。
那么AlphaTest到底用的是later-DT还是early-DT呢？

PowerVR目前和别的GPU的主要区别是：它在TBR的DT阶段多了一次寻址最顶端三角形的操作（别人是TBR，它是TBDR）。
在全部移动平台上，有一种可能性，AlpahtTest只走later-DT，导致它自身无法被“遮挡（而AlphaBlend可以）”，这是效率比AlphaBlend低的主要原因。所以，只要AlphaTest较少被遮挡，劣势就并不存在，反而因为AlphaTest可以“遮挡”其它物体的原因，导致AlphaTest效率更高（但这必须让AlphaTest先于不透明物体绘制）。