iOS GPU不管怎么降画质，耗时一直在十几毫秒。
`r.DefaultFeature.AntiAliasing 0` 关闭抗锯齿，但是会造成白屏
`r.BloomQuality 0` 关闭Bloom
场景受影响比较严重，先改成LOD1。
`r.DepthOfFieldQuality 0` 关掉景深
`r.LightShaftQuality` 我们没用这个，我们用的上帝之光，半透明的mask材质，不知道那个损耗低一些。抽空试一下。
`r.RefractionQuality`我们没用
`r.MobileContentScaleFactor` 移动内容设备缩放系数

为了能在XCode上捕获GPU信息，需要进入UE4Game的Scheme里面设置

# GPU优化三要素
## Shader复杂度 
Shader复杂度分为VS和PS。XCODE分析的时候的ALU就是执行耗时，一般来说，浮点数运算，指令数太多都会影响。
## 屏占比
因为PS是逐像素绘制的，所以屏占比越高，执行次数越多，虽然GPU是多线程的，但是当超出线程上限的时候，仍需要排队执行。
## Overdraw
过度绘制