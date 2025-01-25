# Compute Shader
如何创建：Project->Create->Compute Shader
```c
#pragma kernel CSMain

[numthreads(8,8,1)]

```

    #pragma kernel CSMain

制定了这个程序的入口函数（ComputeShader的Main函数）

    [numthreads(8,8,1)]

这一行指定了ComputeShader创建线程组的大小。GPU利用了大量的并行处理使得GPU创建的一些线程可以同时运行。

```CSharp
public ComputeShader shader;

void RunShader()
{
    int kernelHandle = shader.FindKernel("CSMain");

    RenderTexture tex = new RenderText("256,256,24");
    tex.enableRandomWrite = true;
    tex.Create();

    shader.SetTexture(kernelHandler, "Result", tex);
    shader.Dispatch(kernelHandler, 256/8,256/8);
}
```

