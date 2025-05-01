我们知道RenderTarget是渲染到目标，而RenderTexture是显存上的纹理。
那么申请一个RenderTexture的一生经历了什么呢？
首先我们创建一个RenderTexture
```C#
// RenderTexture.cs 254
public RenderTexture(int width, int height, int depth, RenderTextureFormat format)
{
    Internal_CreateRenderTexture(this);
    this.width = width;
    this.heigh = heigh;
    this.depth = depth;
    this.format = format;
    Internal_SetSRGBReadWrite(this, QualitySettings.activeColor == ColorSpace.Linear);
}
```
这个函数中，对于我来说最感兴趣的莫过于format了。我们知道，不同格式的RT在GPU中处理方式是不同的。不同的编码方式对于性能要求也不同，通常解码率越高执行效率也越高。但是不同设备之间的对纹理的格式处理不一定都兼容，所以我们使用ARGB32不压缩去处理方式。
depth表示深度缓冲区，只支持（0，16，24，32）
如果是0的话，表示不创建。
如果是16的话，
