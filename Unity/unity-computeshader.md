# Compute Shader
如何创建：Project->Create->Compute Shader
```c
#pragma kernel CSMain

RWTexture2D<float4> Result;

[numthreads(8,8,1)]
void CSMain(uint3 id : SV_DispatchThreadID)
{
    Result[id.xy] = float4(id.x & id.y, (id.x & 15)/15.0, (id.y & 15)/15.0, 0.0);
}

```

    #pragma kernel CSMain

制定了这个程序的入口函数（ComputeShader的Main函数）

    [numthreads(8,8,1)]

这一行指定了ComputeShader创建线程组的大小。GPU利用了大量的并行处理使得GPU创建的一些线程可以同时运行。
在GPU编程中，我们可以将所有要执行线程划分为一个个线程组，一个线程组在单个流多处理器（Stream Multiprocessor，简称SM）上被执行。如果我们的GPU架构有16个SM，那么至少需要16个线程组来保证所有SM有事可做。为了更好的利用GPU，每个SM至少需要两个线程组，因为SM可以切换到处理不同族中的线程来隐藏线程阻塞（如果着色器需要等待Texture处理的结果才能继续执行下一条指令，就会出现阻塞）。

每个线程组都有一个各自的共享内存（Shared Memory），该组中的所有线程都可以访问组对应的共享内存，但是不能访问别的组对应的共享内存。因此线程同步操作在线程之间进行，不同的线程组不能进行同步操作。

每个线程组中又是由n个线程组成的，线程组中的线程数量就是通过numthreads来定义的，格式如下：

    numthreads(tx,ty,tz)

其中tx*ty*tz的值即线程的总数量，例如numthreads(4,4,1)和numthreads(16,1,1)都代表着有16个线程。

每个核函数前面我们都需要定义numthreads，否则会编译报错。

不同版本对tx,ty,tz的约束如下

|Computer Shader version|max tz|max thread number|
|--|--|--|
|cs_4_0|1|768|
|cs_5_0|64|1024|

如果是NVIDIA的显卡，线程组中的线程又会被划分为一个个Warp，每个Warp由32个线程组成，一个Warp通过SM调度。在SIMD32下，当SM操控一个Warp执行一个指令，意味着有32个线程同时执行相同的指令。假如我们使用numthreads设置每个线程组只有10个线程， 但是由于SM每次调度一个Warp就会执行32个线程，这就会造成有22个线程是不干活的（静默状态），从而在性能无法达到最有。因此针对NVIDIA显卡，我们应该线程组的线程数量设置32的倍数来达到最佳性能，如果是AMD的显卡的话，线程组中的线程是被划分成一个个由64个线程组成Wavefront，那么线程组中的线程数应该是64的倍数。因此建议numthreads值设置为64的倍数，这样可以同时兼容两大主流的显卡。

在Direct3D12中，可以通过`ID3D12GraphicsCommandList::Dispatch(gX,gY,gZ)`方法创建`gX*gY*gZ`个线程组。注意顺序，先numthreads定义好每个核函数对应线程组里线程的数量`(tx*ty*tz)`，再用Dispath定义用多少线程组`(gx*gy*gz)`来处理这个函数。

gX,gY,gZ的约束如下

|CS Version|max gX max gY | max gz|
|--|--|--|
|cs_4_x|65535|1|
|cs_5_0|65535|65535|

|参数|值类型|含义|计算公式|
|--|--|--|--|
|SV_GroupID|int3|当前线程所在的线程组的ID，取值范围为(0,0,0)到(gx-1,gy-1,gz-1)||
|SV_GroupThreadID|int3|当前线程在所在线程组内ID，取值范围为(0,0,0)到(tx-1,ty-1,tz-1)||
|SV_DispathThreadID|int3|当前线程在所有线程组中的所有线程里ID，取值范围为(0,0,0)到(gx*gtx-1,gy*ty-1,gz*tz-1)|假设该线程的`SV_GroupID=(a,b,c)`，`SV_GroupThreadID=(i,j,k)`那么`SV_DispathThreadID=(a*tx+i,b*ty+j,c*tz+k)`|
|SV_GroupIndex|int|当前线程所在线程组内下标，取值范围为0到tx*ty*tz-1|假设该线程的SV_GroupThreadID=(i,j,k)那么SV_GroupIndex=k*tx*ty+j*tx+i|

这里需要注意的是，不管是Group还是Thread，他们的顺序都是先X在Y后Z。

```c
void KernelFunction(uint3 groupId : SV_GroupID,uint3 groupThreadId:SV_GroupThreadID,uint3 dispatchThreadId:SV_DispatchThreadID,uint groupIndex:SV_GroupIndex)
{}
```

Unity默认的核函数体内执行的代码就是为我们Texture中下标id.xy的像素赋值一个颜色。

举个例子，以往我们想要给一个x*y分辨率的Texture每个像素进行赋值，单线程的情况下，我们的代码如下：
```C#
for (int i = 0; i < x; i++)
    for (int j = 0; j < y; j++)
        Result[uint2(x,y)] = float4(a,b,c,d);
```



    RWTexture2D<float4> Result;

RWTexture2D中，RW其实是Read和Write的意思，Texture2D就是二维纹理，因此它的意思就是一个可以被CS读写的二维纹理。如果我们只想读不想写，那么可以使用Texture2D的类型。

我们知道纹理是由一个个像素组成的，每个像素都有它的下标，因此我们可以通过像素的下标来访问它们，例如：Result[uint2(0,0)]。

同样的每个像素会有它的一个对应值，也就是我们要读取或者要写入的值。这个值的类型就被卸载<>当中，通常对应一个rgba的值，因此是float4类型。通常情况下，我们会在CS中处理好纹理，然后在PS中进行采样。

在CS中读写的类型除了RWTexture还有RWBuffer和RWStruecturedBuffer。

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

# ComputeBufferType

在例子中，我们new一个ComputeBuffer的时候并没有使用到ComputeBufferType的参数，默认使用了ComputeBufferType.Default。实际上我们的ComputeBuffer可以有多种不同的类型对应HLSL中不同的Buffer，来在不同的场景下使用，一共由如下几种类型。

|||
|--|--|
|Default|ComputeBuffer的默认类型，对应HLSL shader中的StructuredBuffer或RWStructuredBuffer，常用于自定义Struct的Buffer传递|
|Raw|Byte Address Buffer，把里面的内容(byte)做偏移，可用作寻址。它对应HLSL shader中的ByteAddressBuffer或RWByteAddressBuffer，用于着色器访问的底层DX11格式为五类型的R32|
|Append|Append and Consume Buffer，允许我们像Stack一样处理Buffer，例如动态添加和删除元素。它对应HLSL shader中的AppendStruecturedBuffer或ConsumeStructuredBuffer|
|Counter|用作计数器，可以为RWStructuredBuffer添加一个计数器，然后在ComputeShader中使用IncrementCounter或DecrementCounter方法来增加或减少计数器的值，因此我们需要一个额外的小buffer用来做计数器|
|Constant|constant buffer(uniform buffer)，该Buffer可以被当作Shader.SetContentBuffer和Material.SetConstantBuffer中的参数。如果想要绑定一个structured buffer那么还需要添加ComputeBufferType.Structured，但是在有些平台（例如DX11）不支持一个buffer即使constant又是structured|
|Structured|如果没有使用其他的ComputeBufferType那么等价于Default|
|IndirectArguments|被用作Graphics.DrawProceduralIndirect，ComputeShader.DispatchIndirect或Graphics.DrawMeshInstancedIndirect这些方法的参数。buffer大小至少要12字节，DX11底层为UAV为R32_UINT,SRV为无类型R32|
