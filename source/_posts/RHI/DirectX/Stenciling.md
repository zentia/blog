---
title: 模版(Stenciling)
mathjax: true
date: 2019-04-11 15:11:29
categories: 
- RHI
- DirectX
---
模版缓存是一个远离屏幕的缓存，我们能够用它来完成一些特效。模版缓存与后缓存和深度缓存有相同的定义，因此在模版缓存中的ijth像素与后缓存和深度缓存中的ijth像素是相协调的。就象名字所说，模版缓存就象一个模版它允许我们印刷渲染后缓存的某个部分。
举例，当要实现一个镜子时，我们只需要简单地反射一个物体细节到镜子平面上；然而，我们仅仅想只绘制镜子里的反射结果。我们能用模版缓存来印制渲染它。图8.1清楚的显示了这一点。

图8.1
模版缓存是Direct3D中的一小部分，它是通过一个简单的表面而被约束的。就象混合，这个简单的表面提供了可变的强大的设置能力。有效地学习使用模版缓存最好的方法是通过学习实际的应用程序。一旦你学懂了一点应用程序中的模版缓存，你将会得到一个更好的用于你自己需要特效的主意。
正因为这个原因，这一章我们特别安排学习两个使用模版缓存的应用程序。
目标
* 理解模版缓存是怎样工作的，怎样创建一个模版缓存以及怎样控制它。
* 学习怎样实现一个镜面效果，使用模版缓存来防止绘制反射到不在镜子表面上的物体。
* 利用模版缓存怎样渲染阴影和防止“双倍混合”。
8.1使用模版缓存
　　为了使用模版缓存，我们在初始化Direct3D时必须首先请求一个，然后必须启用它。我们在8.1.1中讲述怎样请求一个模版缓存。为了启用模版缓存，我们必须设置D3DRS_STENCILENABLE渲染状态并且指定它为true（关闭它即可指定为false）。下面的代码是启用和关闭模版缓存的代码：
Device->SetRenderState(D3DRS_STENCILENABLE, true);
... // do stencil work
Device->SetRenderState(D3DRS_STENCILENABLE, false);
我们可以使用IDirect3DDevice9::Clear方法来清除模版缓存并让其拥有默认值。回忆一下，同样的方法被用在清除后缓存和深度缓存中。
Device->Clear(0, 0,
    D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
    0xff000000, 1.0f, 0 );
注意我们已经添加了D3DCLEAR_STENCIL到第三个参数中，它表示我们想把模版缓存和目标（后缓存）以及深度缓存一起清除。有6种值可以用来指定清除后的模版缓存；在这个例子中我们将它清除为0。
8.1.1请求一个模版缓存
　　在我们创建深度缓存的同时一个模版缓存能够被创建。当指定深度缓存格式的时候，我们同时指定模版缓存的格式。这样，模版缓存和深度缓存分享同一个离屏表面缓存，但是每个像素被指定到各自缓存内存片段中。下面列出了3种深度/模版缓存的格式：
* D3DFMT_D24S8—这种格式是说创建一个32位深度/模版缓存，其中24位为深度缓存，8位为模版缓存。
* D3DFMT_D24X4S4—这种格式是说创建一个32位深度/模版缓存，其中24位为深度缓存，4位为模版缓存，还有4位留着不用。
* D3DFMT_D15S1—这种格式是说创建一个16位深度/模版缓存，其中15位为深度缓存，1位为模版缓存。
注意，还有一些格式没有分配任何位给模版缓存。例如，D3DFMT_D32格式是说只创建一个32位深度缓存。
    同样，不同硬件对模版缓存的支持也是不同的。例如有些显卡就不支持8位模版缓存。
8.1.2模版测试
    如前所述，我们能够使用模版缓存来阻止渲染后缓存中的某些部分。阻止特殊像素被写是通过模版测试（stencil test）来决定的，这是通过下面的表达式来完成的：
(ref & mask) ComparisonOperation (value & mask)
模版测试是对每个像素进行的，假设模版是被允许。将有两个操作：
* 左手边操作数（LHS=ref&mask）
* 右手边操作数（RHS=value&mask）
模版测试比较LHS和RHS，通过比较运算来指定。全部的运算都得到一个布尔值（true/false）。假如测试的结果是true，那么我们把像素写入后缓存。假如测试的结果是false,我们就阻止像素被写入后缓存。当然，如果像素不能被写入后缓存，那么它也不能被写入深度缓存。
### 控制模版测试
Direct3D允许我们控制变量用于模版测试。换句话说，我们可以指定参考值（stencil reference）和掩码(mask value)，以便进行比较运算。虽然我们不能明确地设定模版值（stencil value）,但是我们能够控制写入模版缓存的值。
#### 模版参考值（Reference Value）
模版参考值ref的默认值为0，但是我们能够通过设置D3DRS_STENCILREF渲染状态来改变它。例如，下面的代码就是设置模版参考值为1：
```c
Device->SetRenderState(D3DRS_STENCILREF, 0x1);  
```
注意我们往往使用16进制，因为这让它看起来比整数更容易象一个位队列，并且当我们做位操作时这样看起来更有用，比如相加。
#### 模版掩码
模版掩码值mask是被用来掩饰（隐藏）在ref和value变量中的位。它的默认值是0xffffffff，也就是没有掩饰任何位。我们能够通过设置D3DRS_STENCILMASK渲染状态来改变它。下面的例子就是掩饰高16位：
```c
Device->SetRenderState(D3DRS_STENCILMASK, 0x0000ffff);
```
#### 模版值（Stencil Value）
作为以前的规定，在模版缓存中我们进行模版测试的当前像素。例如，假如我们对ijth像素进行模版测试，那么该值将被写入ijth模版缓存。我们不能明确地设置个别模版值，但是可以清除模版缓存。我们能够使用模版渲染状态来控制将什么写入模版缓存。
#### 比较运算
我们能够通过设置D3DRS_STENCILFUNC渲染状态来设置比较运算。这个比较运算能够被D3DCMPFUNC的任何成员类型列举：
```c
typedef enum _D3DCMPFUNC {
    D3DCMP_NEVER = 1,          // 模版测试永不成功
    D3DCMP_LESS = 2,           // 假如LHS < RHS，那么模版测试成功
    D3DCMP_EQUAL = 3,          // 假如LHS = RHS，那么模版测试成功
    D3DCMP_LESSEQUAL = 4,      // 假如LHS <= RHS，那么模版测试成功
    D3DCMP_GREATER = 5,        // 假如LHS > RHS，那么模版测试成功
    D3DCMP_NOTEQUAL = 6,       // 假如LHS <> RHS，那么模版测试成功
    D3DCMP_GREATEREQUAL = 7,
    D3DCMP_ALWAYS = 8,
    D3DCMP_FORCE_DWORD = 0x7fffffff
} D3DCMPFUNC;
```
* D3DCMP_GREATEREQUAL——假如LHS >= RHS，那么模版测试成功。
* D3DCMP_ALWAYS——模版测试总是成功。
### 更新模版缓存
除了决定是否写或阻止一个特殊像素被写入后缓存以外，我们能够定义模版缓存基于三种可能的案例怎样被更新：
* 对于ijth像素模版测试失败。我们能够定义怎样更新在模版缓存中的ijth，通过设置D3DRS_STENCILFAIL渲染状态来适应这种情形：
Device->SetRenderState(D3DRS_STENCILFAIL, StencilOperation);
* 对于ijth像素深度测试失败。我们能够定义怎样更新在模版缓存中的ijth，通过设置D3DRS_STENCILZFAIL渲染状态来适应这种情形：
Device->SetRenderState(D3DRS_STENCILZFAIL, StencilOperation);
* 对于ijth像素模版测试和深度测试都成功。我们能够定义怎样更新在模版缓存中的ijth，通过设置D3DRS_STENCILPASS渲染状态来适应这种情形：
Device->SetRenderState(D3DRS_STENCILPASS, StencilOperation);
其中StencilOperation能够是下面预先定义的常数：
* D3DSTENCILOP_KEEP——指定不改变模版缓存。
* D3DSTENCILOP_ZERO——指定设置模版缓存入口为0。
* D3DSTENCILOP_REPLACE——指定用模版参考值（reference value）来替换模版缓存入口。
* D3DSTENCILOP_INCRSAT——指定增加模版缓存入口。假如增加的值超过了允许的最大值，我们就设置它为最大值。
* D3DSTENCILOP_DECRSAT——指定减少模版缓存入口。假如减少后的值小于了0，我们就设置它0。
* D3DSTENCILOP_INVERT——指定按位取反模版缓存入口。
* D3DSTENCILOP_INCR——指定增加模版缓存入口。假如增加的值超过了允许的最大值，我们就设置它为0。
* D3DSTENCILOP_DECR——指定减少模版缓存入口。假如减少后的值小于了0，我们就设置它为允许的最大值。
### 模版写掩码
　　除了已经提及的模版渲染状态之外，我们能够设置一个写掩码（write mask）它将掩饰我们写进模版缓存的任何值的位。我们能够通过D3DRS_STENCILWRITEMASK渲染状态来设置写掩码。它的默认值是0xffffffff。下面的例子是掩饰高16位：
Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0x0000ffff);
8.2实例程序：镜子
　　在自然界中的很多表面象镜子一样允许我们通过它的反射来看物体。这一部分讲了我们怎样用3D应用程序来模拟镜子。注意为了简单我们只模拟平面镜。举点例子，一辆擦亮的小汽车能够反射；然而，小车的车身是光滑的，圆的，不是一个平面。我们渲染反射是这些，象光滑的大理石地板、挂在墙上的镜子。换句话说就是在一个平面的镜子。
　　实现镜子的程序需要我们解决两个问题。第一，我们必须学习沿着一个面怎样反射一个物体以便能够正确地绘制反射结果。第二，我们必须只能在一个镜子范围内显示反射结果。即，我们必须掩饰一个表面作为一个镜子，且只渲染那些在镜子里物体。图8.1就是说的这个内容。
　　第一个问题只需要用一些几何向量就可以简单解决。我们能够利用模版缓存解决第二个问题。下两小节分别介绍怎样解决这两个问题。第三小节把它们柔和在一起并且介绍一下本章的第一个应用程序实例代码——镜子。
8.2.1反射数学
　　我们现在演示怎样计算点V=（Vx, Vy, Vz）被平面n*p+d=0反射的点V’=（V’x, V’y, V’z）。图8.2贯穿整个讨论。
　　
　　图8.2
　　根据Part I中的“平面”部分，我们能够知道q=v-kn,这里k是有符号的从v到平面的距离。下面是v相对与平面（n，d）的反射推导：
　　
　　我们用下面的矩阵来实现从v到v’的转换：
　　
　　在D3DX库中用下面的函数来创建反射矩阵R。
D3DXMATRIX *D3DXMatrixReflect(
    D3DXMATRIX *pOut, // The resulting reflection matrix.
    CONST D3DXPLANE *pPlane // The plane to reflect about.
);
一旦我们说到反射变换的话题，就让我们看看其他3种特殊的反射变换。它们是关于三个坐标平面的反射—yz平面，xz平面，和xy平面—它们分别通过下面三个矩阵来表现：

通过yz平面反射一个点，我们只需要简单的将x分量取反就可以了。同样的，通过xz平面反射一个点，我们只需要简单的将y分量取反。通过xy平面反射一个点，我们只需要简单的将z分量取反。这种反射是非常容易理解的。
8.2.2镜面实现流程
当实现一个镜面，一个物体假如在一面镜子前那么它就会被反射。然而，我们不想测试空间假如一个物体在一面镜子前，要做它是非常复杂的。因此，为了简化事情，我们总是反射物体并且无限制地渲染它。但是这样就有一个象本章开头的图8.1一样的问题。即，物体反射被渲染到了没有镜子的表面。我们能够用模版缓存来解决这个问题，因为模版缓存允许我们阻止渲染在后缓存中的特定区域。因此，我们使用模版缓存来阻止渲染被反射的不在镜子里的茶壶。下面的步骤简要的说明了怎样实现：
1、 正常渲染所有的场景——地板，墙，镜子和茶壶——不包含反射的茶壶。注意这一步没有修改模版缓存。
2、 清除模版缓存为0。图8.3显示了后缓存和模版缓存。

图8.3
3、 渲染只有镜子部分的图元到模版缓存中。设置模版测试总是成功，并且假如测试成功就指定模版缓存入口为1。我们仅仅渲染镜子，在模版缓存中的所有像素都将为0，除了镜子部分为1以外。图8.4显示了更新以后的模版缓存。也就是说，我们在模版缓存中对镜子像素做了标记。

图8.4
4、 现在我们渲染被反射的茶壶到后缓存和模版缓存中。但是假如模版测试通过，我们就只渲染后缓存。假如在模版缓存中的值为1，那么我们设置模版测试通过。这样，茶壶就仅仅被渲染到模版缓存为1的地方了。因为只有镜子对应的模版缓存值为1，所以反射的茶壶就只能被渲染到镜子里。
8.2.3代码和解释
　　这个例子的相关代码在RenderMirror函数中，它首先渲染镜子图元到模版缓存，然后渲染那些能被渲染到镜子里的反射茶壶。我们现在一行一行的分析RenderMirror函数的代码，并解释为什么要这么做。
　　假如你想使用8.2.2部分的步骤实现代码，注意我们从第3步开始，因为对模版缓存来说1和2步已经没有什么事做了。同样我们通过这个解释来讨论通过镜子渲染的信息。
　　注意我们将分成几个部分来讨论它。
8.2.3.1第一部分
　　我们通过允许模版缓存和设置渲染状态来开始：
```C++
void RenderMirror()
{
    Device->SetRenderState(D3DRS_STENCILENABLE, true);
    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    Device->SetRenderState(D3DRS_STENCILREF, 0x1);
    Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILWRITEMASK,0xffffffff);
    Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
```
　　这是非常容易理解的。我们设置模版比较运算为D3DCMP_ALWAYS,这就是说让所有模版测试都通过。
　　假如深度测试失败了，我们指定D3DSTENCILOP_KEEP，它表明不更新模版缓存入口。即，我们保存当前值。这样做的原因是假如深度测试失败了，那么就意味着像素被“模糊”了。我们不想渲染被“模糊”的反射像素。
　　同样假如模版测试失败了，我们也指定D3DSTENCILOP_KEEP。但是在这里这样做不是必须的，因为我们指定的是D3DCMP_ALWAYS，当然这样的测试也就永远不会失败。然而，我们只改变比较运算的一位，那么设置模版失败渲染状态是必须的。我们现在就这样做。
　　假如深度测试和模版测试都通过了，我们就指定D3DSTENCILOP_REPLACE，更新模版缓存入口，设置模版参考值为0x1。
8.2.3.2第二部分
　　这下一步阻止渲染镜子代码，除了模版缓存。我们通过设置D3DRS_ZWRITEENABLE并指定为false来阻止写深度缓存。我们能够防止更新后缓存，混合和设置源混合要素为D3DBLEND_ZERO目的混合要素为D3DBLEND_ONE。将这些混合要素代入混合等式，我们得到后缓存是不会改变的：
```C++
    // disable writes to the depth and back buffers
    Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    // draw the mirror to the stencil buffer
    Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
    Device->SetFVF(Vertex::FVF);
    Device->SetMaterial(&MirrorMtrl);
    Device->SetTexture(0, MirrorTex);
    D3DXMATRIX I;
    D3DXMatrixIdentity(&I);
    Device->SetTransform(D3DTS_WORLD, &I);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
    // re-enable depth writes
    Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
```
8.2.3.3第三部分
　　在模版缓存中，符合镜子可视像素的为0x1，因此对已经渲染的镜子区域做记号。我们现在准备渲染被反射的茶壶。回忆一下，我们仅仅想渲染镜子范围内的反射像素。我们现在可以很容易的做到了，因为在模版缓存中这些像素已经被做了记号。
　　我们设置下面的渲染状态：
    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
     Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
　　用一个新的比较运算设置，我们进行下面的模版测试：
    (ref & mask == (value & mask)
    (0x1 & 0xffffffff) == (value & 0xffffffff)
     (0x1)== (value & 0xffffffff)
　　这说明了只有当value=0x1时模版测试才成功。因为在模版缓存中只有镜子相应位置的值才是0x1，若我们渲染这些地方那么测试将会成功。因此，被反射的茶壶只会在镜子里绘制而不会在镜子以外的表面上绘制。
　　注意我们已经将渲染状态由D3DRS_STENCILPASS变为了D3DSTENCILOP_KEEP，简单的说就是假如测试通过那么就保存模版缓存的值。因此，在下一步的渲染中，我们不改变模版缓存的值。我们仅仅使用模版缓存来对镜子相应位置的像素做标记。
8.2.3.4第四部分
　　RenderMirror函数的下一部分就是计算在场景中反射位置的矩阵：
    // position reflection
    D3DXMATRIX W, T, R;
    D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
    D3DXMatrixReflect(&R, &plane);
    D3DXMatrixTranslation(&T,
        TeapotPosition.x,
        TeapotPosition.y,
        TeapotPosition.z);
     W = T * R;
　　注意我们首先确定没有反射的茶壶位置，然后就通过xy平面来反射。这种变换规则是通过矩阵相乘来指定的。
8.2.3.5第五部分
　　我们已经为渲染反射茶壶做好了准备。然而，假如我们现在就渲染它，它是不会被显示的。为什么呢？因为被反射的茶壶的深度比镜子的深度大，因此镜子的图元将把被反射茶壶的图元弄模糊。为了避免这种情况，我们清除深度缓存：
     Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
　　并不是所有问题都解决了。假如我们简单的清除深度缓存，被反射的茶壶会被绘制到镜子的前面，物体看起来就不对了。我们想做的是清除深度缓存并且要混合被反射的茶壶和镜子。这样，被反射的茶壶看起来就象在镜子里了。我们能够通过下面的混合等式来混合被反射的茶壶和镜子：

　　因为原像素（sourcePixel）来自被反射的茶壶，目的像素（DestPixel）来自镜子，我们能够通过这个等式明白它们是怎么被混合到一起的。我们有如下的代码：
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
     Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
　　最后，我们准备绘制被反射的茶壶：
    Device->SetTransform(D3DTS_WORLD, &W);
    Device->SetMaterial(&TeapotMtrl);
    Device->SetTexture(0, 0);
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
     Teapot->DrawSubset(0);
　　回顾一下8.2.3.4部分的W，它能够正确的将被反射的茶壶变换到场景中恰当的位置。同样，我们也要改变背面拣选模式。必须这样做的原因是当一个物体被反射以后，它的正面和背面将会被交换。因此为了改变这种情况，我们必须改变背面拣选模式。
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    Device->SetRenderState( D3DRS_STENCILENABLE, false);
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
} // end RenderMirror()
8.3实例程序：平面阴影
在场景中被灯光照射的地方会产生阴影，这将使场景变的更真实。在这一部分我们将演示怎样实现平面阴影，即在平面上的阴影（如图8.5）。

图8.5
注意这种阴影是“快砍”，虽然它们增强了场景效果，但是这并不是现实中的阴影。阴影值是一个高级的概念，要深入研究它已经超出了本书的范围。然而，特别值得提及的是在DirectX SDK中有一个示例程序演示了阴影值。
　　为了实现平面阴影，我们首先必须找到物体投射到平面上的阴影并进行几何建模以便我们能够渲染它。用一些3D数学就能很容易的实现它。我们然后用50%透明度的黑色材质来渲染描述阴影的多边形。渲染阴影时可能出现“双倍混合”，我们将用一小部分进行解释。我们使用模版缓存来防止双倍混合发生。
8.3.1平行光阴影

图8.6
图8.6显示了物体在平行光照射下得到的阴影。光线是从平行光源放射出的，它的方向是L,通过顶点p得到r（t） = p + tL。光线r（t）和平面n * p + d = 0 相交得到 s 。交点s 通过射线和平面相交测试是非常容易得到的：
把r(t)带进平面等式
求解t

那么：

8.3.2点光源阴影

图8.7
图8.7显示了物体在点光源照射下得到的阴影。点光源的位置是L。光线通过顶点p，则得到 r(t) = p + t ( p – L )。光线r（t）和平面n * p + d = 0 相交得到 s 。用8.3.1同样的方法我们可以得到s。
注意：在点光源和平行光中的L是不同的。对于点光源，我们用L来表示点光源的位置。而对于平行光，我们则是用L来表示平行光的照射方向。
8.3.3阴影矩阵
　　注意图8.6中所示的平行光，影子本质上是把物体按照灯光照射方向平行地投射到平面n*p+d=0之上。同样的，图8.7中所示的点光源，影子本质上是把物体按照透视画法从光源投射到平面n*p+d=0之上。
　　我们能够使用一个矩阵来表示从一个顶点p变换到平面n*p=d=0上的s的变化。而且，我们能够用同一个矩阵来表现正交投影和透视投影。
　　我们用一个4D向量（nx, ny, nz, d）来表示将要用于投射阴影平面的平面等式中的各个系数。让4D向量L=（Lx, Ly, Lz, Lw）来表示平行光的照射方向或点光源的位置。我们用w来区别：
１． 假如w＝０，那么L表示平行光的照射方向。
２． 假如w＝1 ，那么L表示点光源的位置。
规格化的平面是非常不逊的，我们让k＝（nx, ny, nz, d）*（Lx, Ly, Lz, Lw）= nxLx+nyLy+nzLz+dLw
那么我们就可得到表示点p到点s的变换矩阵，即阴影矩阵：

因为在其他地方已经被推导出来了，对于我们来说推导它并没有重大的意义，在这里我们就不再演示推导怎样得到这个矩阵的过程了。但是对与感兴趣的读者可以自己到网上查找相应的信息。
　　　　在D3DX库中已经给我们提供了一个建立阴影矩阵的函数。其中当w＝０时表示平行光，当w＝１时表示点光源：
```C++
D3DXMATRIX *D3DXMatrixShadow(
    D3DXMATRIX *pOut,
    CONST D3DXVECTOR4 *pLight, // L
    CONST D3DXPLANE *pPlane // plane to cast shadow onto
);
```
8.3.4用模版缓存防止双倍混合
　　几何学上，当我们将一个物体投影到一个平面上时，很可能会有两个或者更多的投影三角形被重叠到一起。若我们就这样渲染，那么有重叠三角形的地方就会被多次混合以至这些地方将会变得更黑。图8.8就是这种情况。
　　
　　图8.8
　　我们能够使用模版缓存来解决这个问题。我们设置模版测试为允许像素第一次被渲染。即，当把影子像素渲染到后缓存时，我们同时在模版缓存中做好标记。然后，如果试图把像素向一个已经渲染过的地方写，那么模版测试将会失败。这样，我们就防止了重复写像素也就是防止了双倍混合的发生。
8.3.5代码和解释
　　下面的代码就是讲解影子例子。本例的相关代码都在RenderShadow函数中。注意我们假设模版缓存都已经被清除为０了。
　　首先设置模版渲染状态。将模版比较运算设为D3DCMP_EQUAL且将D3DRS_STENCILREF渲染状态设置为0x0，因此假如在模版缓存中相应的值为0x0，那么就指定渲染阴影到后缓存中。
　　因为模版缓存是被清除为0x0的，所以我们第一次将影子像素写入的时候总是正确的；不过因为我们设置D3DRS_STENCILPASS为D3DSTENCILOP_INCR,假如你试图将已经写过的像素写入的话，这个测试将会失败。在第一次写入的时候模版像素已经被写成了0x1，因此假如你再一次写入，模版测试将会失败。因此，我们避免了重复写像素，也避免了双倍混合。
```C++
void RenderShadow()
{
    Device->SetRenderState(D3DRS_STENCILENABLE, true);
    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    Device->SetRenderState(D3DRS_STENCILREF, 0x0);
    Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
　　Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
　　下一步，我们计算阴影变换并将它放置到场景中适当的位置。
    // compute the transformation to flatten the teapot into a shadow.
    D3DXVECTOR4 lightDirection(0.707f, -0.707f, 0.707f, 0.0f);
    D3DXPLANE groundPlane(0.0f, -1.0f, 0.0f, 0.0f);
    D3DXMATRIX S;
    D3DXMatrixShadow(&S, &lightDirection, &groundPlane);
    D3DXMATRIX T;
    D3DXMatrixTranslation(&T, TeapotPosition.x, TeapotPosition.y,
    TeapotPosition.z);
    D3DXMATRIX W = T * S;
     Device->SetTransform(D3DTS_WORLD, &W);
```
　　最后，我们设置一个50%透明度的黑色材质，关闭深度测试，渲染阴影，然后开启深度缓存同时关闭alpha混合和模版测试。我们关闭深度缓存来防止z-fighting，它是当两个不同的表面在深度缓存中有同样的深度值时出现的虚拟物体；深度缓存不知道那一个是在前面，此时就会产生讨厌的闪动。因为阴影和地板是在同一个平面上，z-fighting很可能就会出现。通过先渲染地板然后用深度测试屏蔽阴影，这样我们就能够保证阴影将绘制在地面只之上。
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK,
    d3d::BLACK, d3d::BLACK, 0.0f);
    mtrl.Diffuse.a = 0.5f; // 50% transparency.
    // Disable depth buffer so that z-fighting doesn't occur when we
    // render the shadow on top of the floor.
    Device->SetRenderState(D3DRS_ZENABLE, false);
    Device->SetMaterial(&mtrl);
    Device->SetTexture(0, 0);
    Teapot->DrawSubset(0);
    Device->SetRenderState(D3DRS_ZENABLE, true);
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    Device->SetRenderState(D3DRS_STENCILENABLE, false);
}//end RenderShadow()

