多光源在不同的渲染管线有不同的解决方案。

# Traditional Forward Shading

传统的前向渲染，只使用一个Pass来渲染每个物体，在渲染之前，需要先确定每个物体接受的所有光源的光照。

传统的做法是先给物体进行分组，每组物体接受的光照光源尽量相似。这样对于每组物体，我们在渲染时使用一些相同的光照列表进行着色计算。每组物体的渲染是一次Batch。

当Batch较大时，Batch的切换会比较少，也就是说相同的光照组成的Batch数量越多，那么执行的次数越少。当Batch较小时，可以增加光照计算的效率，但是会导致Batch切换比较频繁。

除此之外，在某些情况下，也会造成运行效率降低。当一个很大的物体，受到很多小光源影响时，会导致很多无效的着色计算。当一个很大的光源，影响很多很小的物体时，又会增加很多执行光照的过程。

2. Traditional Deferred Shading
3. Tiled Deferred/Forward Shading
4. Clustered Deferred/Forward Shading

