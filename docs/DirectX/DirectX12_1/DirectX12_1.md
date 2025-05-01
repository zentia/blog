---
title: DirectX12
date: 2022-01-15 20:41:00
categories:
- RHI
- DirectX
- DirectX12
tags:
- RHI
- DirectX
- DirectX12
---

https://zhuanlan.zhihu.com/p/57061190
DX12的绘制流程大概是这样的：
首先，绘制需要一个绘制的环境。绘制环境是在代码层对硬件环境的一种抽象，所有的绘制都是在这个绘制环境中完成的，可以说绘制环境才是我们的“硬件”。DX12对绘制环境的抽线是ID3D12Device。

然后，绘制自然需要资源。这里的资源就包括用来暂存渲染结果的缓冲区，包括渲染缓冲区和深度缓冲区。

最后，绘制需要绘制指定。必须让GPU知道我们想怎么样绘制才行。
# 绘制环境
DX12使用的是COM技术，所谓COM技术，简单的理解就是从一个统一的公共积累继承出来的子类，用这种技术最大的好处就是资源的分配和释放可以不用太操心。

在创建Device的时候，我们要做两手准备。一是直接创建默认的适配器Device。二是如果这个默认显示适配器的Device无法创建，那就创建一个软件适配器的Device。

```C++
ComPtr<IDGIFactory4> g_pDXGIFactory = nullptr; // dxgi工厂
ComPtr<ID3D12Device> g_pDevice = nullptr;

D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS g_msQualityLevels; // 多重采样

void CreateDevice() {
    // 创建一个基于硬件的Device
    HRESULT hardwareResult = D3D12CreateDevice(
        nullptr, // 那个显示适配器，nullptr表示默认的显示适配器 
        D3D_FEATURE_LEVEL_11_0, // 希望支持的功能集
        IID_PPV_ARGS(&g_pDevice) // 输出的ComPtr<ID3D12Device>对象
    );
    // 如果没法创建基于硬件的Device，那就创建一个WARP(Windows Advanced Rasterization platform)软件适配器的Device
    // DXGI是微软提供的一个基础库，它给提供了与图形API有关但是又不适合放入图形API的功能
    // 比如枚举系统中的显示适配器，显示器，支持的显示分辨率刷新率等等
    // 这里我们用到一个重要的接口是IDXGIFactory4
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&g_pDXGIFactory)));
    if (FAILED(hardwareResult)) {
        ComPtr<IDXGIAdapter> pWarpAdapter; // 显示适配器的抽象
        ThrowIfFailed(g_pDXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter))); // 枚举显示适配器

        // 创建与适配器相关的Device
        ThrowIfFailed(D3D12CreateDevice(
            pWarpAdapter.Get(), // 获取COM的原始指针
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&g_pDevice) // 获取COM的指针的地址
        ));
    }

    // 检查支持4X的MSAA的质量等级
    // 所有Direct3D11的兼容设备都是4X的MSAA，所以我们只需要检查一下支持到什么等级
    g_msQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 缓冲格式
    g_msQualityLevels.SampleCount = 4; // 采样数
    g_msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    g_msQualityLevels.NumQualityLevels = 0;
    ThrowIfFailed(g_pDevice->CheckFeatureSupport(
        D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
        &g_msQualityLevels,
        sizeof(g_msQualityLevels)
    ));
}
```
为了枚举拥有机器拥有的所有适配器，我们需要用到微软DXGI（DirectX Graphics Infrasturcture）库，这个库中包含了图形变成需要用到但是不适合包含DirectX中的功能，比如枚举显示适配器，显示器，机器支持的分辨率，刷新率等。

# 创建命令队列和命令列表

命令队列主要负责接受CPU生成好的命令（列表），然后通知GPU执行。
资源屏障用来适当的同步过程的控制。还有一些时候必须使用规定好的权限，那么资源屏障就提供了这个安全距离的控制，接下来我们详细讲一下。

命令队列有六种，其中有5个对应的接口是：`ExecuteCommandLists`,`Signal Wait`,`UpdateTileMapping`,`CopyTileMapping`,`ISwapChain:Present`。

命令队列中，前面五个都是可以并发访问的。不过，即使有多个CPU线程并发的向命令队列中添加命令，由于只有一个GPU线程从命令队列中取出并执行，所以同一个命令队列上的不同命令之间的执行不可能重叠。但是，同一命令内部的执行是可能存在并发的。在硬件层面，GPU有3个可以并发执行的引擎，复制引擎，计算引擎和图形引擎。D3D12的初衷就是希望应用程序为这三个引擎各自创建一个相应的命令队列。其中，复制命令只能在复制引擎上执行。计算命令可以在计算引擎和复制引擎上执行，直接命令队列可以在任意引擎上执行。

对于同一命令内部的并发性，我们希望能够进行控制，于是就有了资源屏障的概念。

最常见的控制同步的方式是Fence。通过接口`CreateFence`可以创建。他有四个参数，第一个是创建`Fence`的初始值。第二个参数用来指定跨进程或者跨显示适配器共享。第三个和第四个就是我们熟悉的rrid和PPV。

CPU和GPU都可以往`Fence`里写入，调用`Signal`方法，会将`Signal`命令添加到队列命令中。当GPU线程执行`Signal`命令时，会写入`Fence`值。CPU和GPU都可以等待`Fence`，知道`Fence`的值大于或者等于某个指定值时，等待结束。不过CPU更加灵活，它会得到一个提示，不一定会等待。CPU可以调用`SetEventOnCompletion`方法，并设定一个值，当`Fence`大于或者等于该值的时候，这个事件会被触发。

命令列表是不允许CPU和GPU线程并发访问的，为此，命令列表被分为记录和关闭状态。只有当命令列表处于记录状态时，才允许向其中添加命令。只有当命令列表关闭时，才运行将其作为`ExecuteCommandList`的实参。实际上，命令列表并不允许被CPU线程并发访问。当向命令列表中添加命令时，命令列表会在命令分配器中分配内存。但是命令列表并不会维护其在命令分配器中的相关信息。也就是说，命令列表在销毁的时候并不会释放它在命令分配器中的内存，这个需要程序自己负责释放命令列表在命令分配器中的内存。D3D12提供了CommandList的Reset方法来释放命令列表的内存，同时会创建一个新的命令列表用来执行。只有在命令列表处于关闭状态的时候才能Reset。



# 创建交换链

交换链是一种资源，但它比较特殊，它是当前正在显示的数据和下一帧将显示的数据的保存地。现代的绘制方式通常是这样，一个交换链有2个缓冲区，分辨是前缓冲区和后缓冲区，前缓冲区中的数据是当前显示（能看到的）数据，后缓冲区的数据是下一帧要显示的数据，当需要显示后缓冲区中的数据时，将后缓冲区设置为“前”缓冲区就行了，这是一个非常巧妙的设定。

创建交换链的难点在于要描述什么样的交换链，这个描述结构的成员很多很复杂，可以参考官方的[说明文档](https://docs.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc)。创建交换链的流程非常简单，只有三步：
1. 释放已有的交换链
2. 填充描述结构，指明要创建什么样的交换链。
3. 调用Factory的CreateSwapChain函数创建交换链。

具体的内容请看下面代码：

```C++
ComPtr<IDXGISwapChain> g_pSwapChain = nullptr; // 交换链接口的指针

void CreateSwapChain() {
    // 释放交换链，我们要重新生成。
    g_pSwapChain.Reset();

    // 交换链描述结构，用于定义创建什么样的交换链。
    DX_GI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = g_ScreenWidth;
    sd.BufferDesc.Height = g_ScreenHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60; // 刷新率，分子
    sd.BufferDesc.RefreshRate.Denominator = 1; // 刷新率，分母
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT 枚举，用于定义显示格式，这里表示一个32位无符号规范化的整数格式，每个通道8位，包括透明通道

}
```
# 创建描述符资源
在DX12中，微软将后缓冲区，深度/模板缓冲区统统抽象成资源。当我们提交一个绘制命令的时候，我们需要绑定资源到渲染管线上，这样GPU才能获取到资源。但是，GPU资源不是直接被绑定的，而是通过一种引用的方式来绑定。这个资源描述符（descriptor）就是用来引用资源的工具。我们可以把它当作是一个轻量级的结构体，为GPU描述了资源是做什么用的。

为什么要多出这么一个中间层呢？官方的解释是，GPU资源本省就是一些内存块，用这种原始的保存方式可以在渲染管线的不同阶段被访问到。从GPU的角度来看，资源就是一块内存中的数据，而只有描述符才知道这块内存到底是做什么用的，是渲染目标，还是深度缓冲。

由于历史原因，描述符（descriptor）和视图（view）通常是等价的，所以你经常会看到一些以view为名的函数或者变量。
```C++
// DSV，表示depth-stencil view
D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
dsvHeapDesc.NumDescriptors = 1;
dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // 描述符堆的类型
dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 描述符堆标志（还是默认）
dsvHeapDesc.NodeMask = 0;
// 创建描述符堆
ThrowIfFailed(g_pDevice->CreateDescriptorHeap(
    &dsvHeapDesc,
    IID_PPV_ARGS(g_pDsvHeap.GetAddressOf())
));
```

# 创建缓冲区

资源描述符只是描述一块内存是什么资源，但是这块内存我们还是需要分配的，这才是真正的资源，缓冲区就是一种资源。我们把创建缓冲区放到单独的函数中。

```C++
uint32_t g_currentBackBuffer = 0;
ComPtr<ID3D12Resource> g_SwapChainBuffer[g_FrameCOunt]; // 渲染缓冲区，渲染缓冲区有2个，一前一后
ComPtr<ID3D12Resource> g_DepthStencilBuffer; // 深度/模板缓冲区

void CreateBuffers() {
    // 释放缓冲区的资源，我们会重新创建
    for (int i = 0; i < g_FrameCount; ++i>) {
        g_SwapChainBuffer[i].Reset();
    }
    g_DepthStencilBuffer.Reset();

    // 根据窗口大小重新分配缓冲区大小
    ThrowIfFailed(g_pSwapChain->ResizeBuffers(
        g_FrameCount, // 缓冲区数量
        g_ScreenWidth, g_ScreenHeight, // 窗口的宽高
        DXGI_FORMAT_R8G8B8A8_UNORM, // 缓冲区格式
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH // 交换链的行为指定。允许应用通过ResizeTarget切换模式，当从窗口切换到全屏时
    ));

    g_currentBackBuffer = 0; // 当前后缓冲区重置为0

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(g_pRtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (int i = 0; i < g_FrameCount; ++i>) {
        // 获取缓冲区资源，保存到全局变量中，需要从交换链中获取。
        ThrowIfFailed(g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&g_swapChainBuffer[i])));
        // 为缓冲区创建渲染目标视图（Render Target View），这是一种子隐患，需要跟描述符关联起来s
        g_pDevice->CreateRenderTargetView(g_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
        // 堆上下一个资源描述符入口
        rtvHeapHandle.Offset(1, g_rtvDescriptorSize);
    }

    // 创建深度/模板缓冲区/视图
    // 深度/模板缓冲区描述，定义创建的资源格式
    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 缓冲区维数（1维纹理还是2维纹理？）
    depthStencilDesc.Alignment = 0; // 对齐
    depthStencilDesc.Width = g_ScreenWidth; // 缓冲区宽度
    depthStencilDesc.Height = g_ScreenHeight; // 缓冲区高度
    depthStencilDesc.DepthOrArraySize = 1; // 纹理的深度，以纹素为单位，或者是纹理数组
    depthStencilDesc.MipLevels = 1; //多级纹理级数
    depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; // 纹素格式
    depthStencilDesc.SampleDesc.Count = 1; // 多重采样数
    depthStencilDesc.SampleDesc.Quality = 0; // 多重采样采样质量
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; // 纹理布局
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // 其他资源标识s

    // 清空缓存时候的默认值
    D3D12_CLEAR_VALUE optClear;
    optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //缓存数据格式
    optClear.DepthStencil.Depth = 1.0f; // 深度值
    optClear.DepthStencil.Stencil = 0; // 模板值
    // 创建，提交一个资源到特定的堆，这个资源属性是我们指定的。
    ThrowIfFailed(g_pDevice->CrreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // 特定的堆的属性。
    ))
}

```

# 重新分配大小
当窗口大小改变的时候，我们就要重新分配缓冲区的大小了。原有的资源肯定是要释放掉的，然后在创建新资源，那么有没有办法重用之前写过的代码呢？当然有，之前的创建缓冲区的函数就可以宠用，这样代码就少了很多。同时，我们还需要把视口和裁剪区的位置保存下来。
```C++
void OnResize() {
    // 只有Device、交换链、命令分配器都创建了之后才能重新分配缓冲区的大小
    if (g_pDevice == nullptr || g_pSwapChain == nullptr || g_pCommandAllocator == nullptr) {
        return;
    }

    // 确保所有的资源都不在使用了。
    FlushCommandQueue();

    ThrowIfFailed(g_pCommandList->Reset(g_pCommandAllocator.Get(), nullptr));

    // 创建缓冲区
    CreateBuffers();

    // 执行重新创建的命令
}
```
# 绘制

在这里，我们先简单的用一个颜色填充整个窗口。作为一个入门的工程，这种效果绰绰有余了。
```C++
void Draw() {
    // 重用记录命令的内存块。
    // 只有GPU执行完绘制命令后，才能调用Allocator的Reset函数。
    ThrowIfFailed(g_pCommandAllocator->Reset());

    // 命令列表在提交之后就可以充值了，不需要等到命令执行完。
    // 重用命令列表的内存块。
    ThrowIfFailed(g_pCommandList->Reset(g_pCommandAllocator.Get(), nullptr));

    // 把后缓冲区的资源状态切换成Render Target。
    // ResourceBarrier函数创建了一个通知驱动同步资源访问行的命令，简单地来说就是切换的状态。
    g_pCommandList->ResourceBarrier(1,
        // D3D12_RESOURCE_BARRIER是资源栅栏（暂时这么翻译），用来表示对资源的操作。
        // CD3DX12_RESOURCE_BARRIER类是D3D12_RESOURCE_BARRIER结构的辅助类，提供更便利的使用接口。Transition的作用正如其函数名一样，创建资源状态的操作，其返回的是CD3DX12_RESOURCE_BARRIER。
        &CD3DX12_RESOURCE_BARRIER::Transition(g_SwapChainBuffer[g_currentBackBuffer].Get(),
        )
    );

    // 把后缓冲区切换成PRESENT状态
    g_pCommandList->ResourceBarrier(
        1,
        &CD3DX12_RESOURCE_BARRIER::Transition(
            g_SwapChainBuffer[g_currentBackBuffer].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT
        )
    );

    // 命令保存完毕，关闭命令列表。
    ThrowIfFailed(g_pCommandList->Close());

    // 添加命令列表到队列中并执行。
    ID3D12CommandList* cmdsLists[] = {g_pCommandList.Get()};
    g_pCommandQueue->ExecuteCommandList(_countof(cmdsLists), cmdsLists);

    // 交换前后缓冲区
    ThrowIfFailed(g_pSwapChain->Present(0, 0));
    g_currentBackBuffer = (g_currentBackBuffer + 1) % g_FrameCount;

    // 强制刷新，让GPU指令命令，这个操作并不搞笑，但是对一个简单实例来说，就已经足够了。
    FlushCommandQueue();
}
```

主要功能已经完成了，我们还需要把代码组装起来。这里我不提供Windows上创建窗口的代码，因为这和DX12无关。
```C++
void InitDirect3D12() {
    CreateDevice();

    CreateCommandObjects();

    CreateSwapChain();

    CreateDescriptorHeaps();

    OnResize();
}
```
没错，就是这个流程，最开始是Device，我们代码角度的“硬件”。然后是命令对象，包括命令列表和命令队列，之所以在这里就创建，是因为在创建交换链的时候要用到命令队列。紧接着就是创建交换链，之后是创建描述符堆，包括渲染和深度两个描述符堆，虽然深度只有一个元素。最后是重新分配大小，因为这里面有创建缓冲区的过程，创建缓冲区的时候需要和资源描述符匹配起来。至于绘制在哪里调用，自然是在消息缓冲中：
```C++
int Run() {
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            Draw(); // 不处理消息的时候绘制界面
        }
    }
    return msg.wParam;
}
```
相关代码请参考：
[Panda](https://gitee.com/zentia/panda)
[USD](https://github.com/PixarAnimationStudios/USD.git)

# 参考
{% pdf dx12-whitepaper-v2.pdf %}