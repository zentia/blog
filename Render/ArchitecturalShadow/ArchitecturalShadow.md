首先看一下我们的效果需求，我们是一款SLG游戏，主程中的建筑都是3D的建筑，玩家到后期主城内有上百个建筑，而且建筑都是可以让玩家随意摆放的。我们的建筑要实现自身的阴影和在地面上的投影。如下图：

![](1.jpg)

一般我们在游戏中要实现建筑的阴影有两种方案：1.灯光的实时影音；2.使用Lightmap烘培阴影信息。

方案1在移动端使用的很少，如果没有实时光影的变化，类似天气系统之类的需求，基本上是不会使用的。要多一个pass去计算阴影，相当于场景中的三角面数翻倍。

方案2是目前大多数移动端上使用的，开销的话只是用第二套UV去采样一次Lightmap贴图而已。但是这种方法烘培Lightmap