CGPROGRAM和ENDCG包裹的部分就是CG\HLSL语言部分。

1. Vertex & Fragment Shader: 顶点/片元着色器。它是最基本，也是非常强大的着色器类型。一般用于2D场景、特效之类的。Unlit Shader(无光照着色器，不包含无效)
在世界三维空间中，一开始传入Shader处理的数据其实就是网格数据(Mesh Data)
但是一般情况下，光是网格数据不能满足我们处理画面的需求，这时就需要引入一些常数属性数据(Properties)
属性的声明规则如下：
```c
	_Name("Display Name", type)=defaultValue[{options}]
```
 - \_Name是属性的名字，也就是变量名，在之后整个Shader代码中将使用这个名字来获取该属性的内容，切记要添加下划线。
 - Displayer Name 这个字符串将显示在Unity的Inspector中作为Shader的使用者可读的内容，即显示的名称。
 - type属性的类型。常用的有几种：Color颜色，一般为RGBA的数组；2D纹理，宽高为2的幂次尺寸；Rect纹理，对应非2的幂次尺寸；Cube立方体，即6张2D纹理组成；Float和Range，都是浮点数，但是Range要求定义最小值和最大值，以Range(min,max)形式显示；Vector四位数。
 - defauleValue默认值，与类型直接挂钩。一开始赋予该属性的初始值，但是在检视窗口调整过属性值之后，不再有效。Color以0~1定义rgba颜色，比如(1,1,1,1)；2D/Rect/Cube，对于纹理来说，默认可以为一个代表默认tint颜色字符串，可以使空字符串或者"white"，"black"等中的一个；Float和Range为某个指定的浮点数；同样，Vector的是一个四维数值，(x,y,z,w)的形式。
 - Options可选项，它只对2D，Rect或者Cube纹理有关，一般填入OpenGL中TexGen的模式。
这样我们可以尝试解读上图中的那些属性声明的是声明了。比如_MainTex("A Texture", 2D)=""{},就是声明了一个变量名为_MainTex的2次幂尺寸纹理，它在检视窗口中显示的名称是A Texture，默认是空的。

2. 语义
语义（Semantics）是附加到着色器输入或输出的字符串，其传达关于参数的预期使用的信息。对于在着色器级之间传递的所有变量都需要语义，通常，在流水线级之间传递的数据是完全通用的，并且不被系统唯一地解释；允许任意语义没有特殊意义。
3. Surface Shader: 表面着色器。它拥有更多的光照运算，其实在系统内部它会被编译成一个比较复杂的顶点/片元着色器。Standard Surface Shader(标准光照模型表面着色器)
4. Standard Shader: 标准着色。它是表面着色器升级版，因为它使用了Physically Based Rendering（简称PBR）技术，即基于物理的渲染技术。所以在这个着色器中开放了更多处理光照与材质的参数。

# Tag

表面着色器可以被若干的标签(tags)所修饰，而硬件将通过判定这些标签来决定什么时候调用该着色器。比如我们的粒子中SubShader的第一句
Tags{"RenderType"="Opaque"}
告诉了系统应该在渲染非透明物体的时调用我们。Unity定义了一系列这样的渲染过程，与RenderType是Opaque相对应的显而易见的是"RenderType"="Transparent"，表明渲染含有透明效果的物体时调用它。在这里Tags其实暗示了你的Shader输出的是什么，如果是渲染非透明物体，那添加"Queue"="Opaque"标签；如果渲染透明或者半透明的像素，那添加"Queue"="Transparent"。
另外比较有用的标签"IgnoreProject"="True"(不背Projectors影响)，"ForceNoShadowCasting"="True"(从不产生阴影)以及"Queue"="xxx"(指定渲染顺序队列)。这里想要赵忠说一下的是Queue这个标签，如果你使用Unity做过一些透明和不透明物体的混合的话，很可能已经遇到过不透明物体无法呈现在透明物体之后的情况。这种情况很可能是由于Shader的渲染顺序不正确导致的。Queue指定了物体的渲染顺序，预定义的Queue有：

- Background 	- 最早被调用的渲染，用来渲染天空盒或者背景
- Geometry 		- 这是默认值，用来渲染非透明的物体（普通情况下，场景中的绝大多数物体应该是非透明的）
- AlphaTest 	- 用来渲染经过Alpha Test的像素，单独为AlphaTest设定一个Queue是出于对效率的考虑
- Overlay 		- 用来渲染效果的叠加，是渲染的最后阶段（比如镜头光晕等效果）

这些预设值本质是一组整数，Background = 1000(背景), Geometry = 2000(几何体), AlphaTest = 2450(alpha测试), Transparent = 3000(透明),最后Overlay = 4000。在我们实际设置Queue值时，不仅能使用上面的几个预定义值，我们也可以指定自己的Queue值，写成类似这样："Queue"="Transparent+100",表示一个在Transparent之后100的Queue上进行调用。通过调整Queue值，我们可以确保某些物体一定在另一些物体之前或者之后渲染。

# LOD
LOD很简单，它是Level of Detail的缩写，在这里例子里我们指定了其为200（其实这是Unity的内建Diffuse着色器的设定值）。这个数值决定了我们能用上面样的Shader。在Unity的Quality Settings中我们可以设定允许最大LOD，当设定的LOD小于SubShader所指定的LOD时，这个SubShader将不可用。Unity内建Shader定义了一组LOD的数值，我们在实现自己的Shader的时候可以将其作为参考来设定自己的LOD数值，这样在之后调整根据设备图形性能来调整画质时可以紧急性比较精确的控制。

- VertexLit及其系列 = 100
- Decal,Reflective VertexLit = 150
- Diffuse = 200
- Diffuse Detail,Reflective Bumped Unlit,Reflective Bumped VertexLit = 250
- Bumped,Specular = 300
- Bumped Specular = 400
- Parallax = 500
- Parallax Specular = 600

# Shader本体

```c
	CGPROGRAM
	#pragma surface surf Lambert

	sampler2D _MainTex;

	struct Input {
		float2 uv_MainTex;
	};

	void surf(Input IN, inout SurfaceOutput o){
		half4 c = tex2D(_MainTex, IN.uv_MainTex);
		o.Albedo = c.rgb;
		o.Alpha = c.a;
	}
	ENDCG
```
还是逐行来看，首先是CGPROGRAM。这是一个开始标记，表明从这里开始是一段CG程序（我们在写Unity的Shader时用的是Cg/HLSL语言）。最后一行的ENDCG与CGPROGRAM是对应的，表明CG程序到此结束。
接下来是一个编译指令：#program surface surf Lambert,它表明了我们要写一个表面Shader，并指明了光照模型。它的写法是这样的
	
	#pragma surface surfaceFunction lightModel [optionalparams]

- surface -表明的是一个表明着色器
- surfaceFunction -着色器代码的方法的名字
- lightModel -使用的光照模型
我们声明了一个表面着色器，实际的代码在surf函数，使用Lambert(也就是普通的diffuse)作为光照模型。
接下来一句sampler2D MainTex,sampler2D是个啥？其实在CG中，sampler2D就是和Texture所绑定的一个数据容器接口。等等..这个说法还是太复杂了，简单理解的话，所谓加载以后的texture(贴图)说白了不过是一块内存存储的，使用了RGB(也许还有A)通道，且每个通道8bits的数据。而具体想知道像素与坐标的对应关系，以及获取这些数据，我们总不能一次次去自己计算内存地址或者偏移，因此可能通过sampler2D来对贴图进行操作。更简单理解，sampler2D就是GLSL中的2D贴图类型，相应的，还有smapler1D,sampler2D,samplerCube等等格式。
解释通了sampler2D是什么之后，还需要解释下为什么在这里需要一句对_MainTex的声明，之前我们不是已经在Properties里声明过它是贴图了么。答案是我们用来实力的这个shader其实是由两个相对独立的块组成的，外层的属性声明，回滚等等是Unity可以直接使用和编译的ShaderLab;而现在我们是CGPROGRAM..ENDCG这样一个代码块中，这是一段CG程序。对于这段CG程序，要想访问在Properties中所定义的变量的话，必须使用和之前变量形同的名字进行声明。于是其实sampler2D \_MainTex;做的事情就是再次声明并链接了_MainTex,使接下来的CG程序能够使用这个变量。
解释通了sampler2D是什么之后，还需要解释为什么在这里需要一句对_MainTex的声明，之前我们不是已经在Properties里声明过它是贴图了吗。答案是我们用来实力的这个shader其实是由两个独立的快组成的，外层的属性声明，会。
-

在CG中，函数就像C中那样声明。你可以随意的指定传递给函数的参数，以及将被函数啊发你会的值。下面是一个简单的函数声明：
```c
float getX(float3 v)
{
	return v.x;
}
```
这个函数采用了一个三元向量v作为一个参数，并且将v的x分量作为返回值，其类型为float。关键字return被用来返回函数的结果。你可以像调用任何其它Cg函数那样调用getX函数：
```c
//声明一个临时使用的向量
float3 myVector = float3(0.5,1.0,-1.0);

//取得myVector的x分量
float x = getX(myVector);
//现在 x=0.5
```
有些时候，你想要一个函数返回几个结果而不仅仅是一个结果。在这种情况下，你能够使用out修饰符来指定一个程序的某个特定的参数只用于输出。下面的例子用一个向量作为输入，然后返回它的x、y和z分量。
```c
void getComponents(float3 vector, out float x, out float y, out float z)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
}
```
注意这个函数被声明为void类型，因为它通过参数来返回所有的值。下面的代码示例显示了getComponents是如何被使用的：
```c
//声明一个临时使用的向量
float3 myVector = float3(0.5,1.0,-1.0);

//声明一个临时使用的变量
flaot x,y,z;
//获得myVector的x、y和z分量
getComponents(myVector, x,y,z);
//现在x=0.5,y=1.0,z=-1.0
```
# 一个光照函数

因为光照是一个复杂的过程，你能够编写许多不同类型的光照函数，每个函数都能接受不同的参数。现在，你只需要采用你实现的简单模型，并为它创建一个函数。下面是这个函数的最基本的样子：
```c
float3 lighting(float Ke, float3 Ka, float3 Kd,float3 Ks,float shininess,float3 lightPosition,float3 lightColor,float3 globalAmbient,float3 P,float3 N,float3 eyePosition)
{
	//在这里计算光照
}
```
这个方法的一个主要问题是这个函数需要很多参数。把这些参数组成“材质参数”和“光参数”，然后把每个参数集当成一个单独变量来传递，这将使得整个函数整洁许多。幸运的是Cg支持这种结构，恰好能够提供这种功能。
# 结构

Cg的结构使用与C和C++同样的方法来声明。struct关键字被用来声明结构，它后面跟随的是结构的成员。下面是一个结构的例子，它分封装了基于基本光照模型的某个材质的所有性质：

# Fog

语法:

1. Fog {Fog Commands}
2. Mode Off | Global | Linear | Exp | Exp2(雾的模式，缺省值是Global)
3. Color ColorValue(雾的颜色)
4. Density FloatValue(雾的浓度，影响Exp/Exp2)
5. Range FloatValue,FloatValue(雾的开始和结束距离，影响Linear)

注意如果使用了片段着色器的话，着色器种的雾设定仍然是有效的。另外可以通过菜单Edit->Render Settings可以打开渲染设置

# Alpha Test和Alpha Blending

Alpha Test是不需要关闭ZWrite的。Alpha Test要么完全透明，要么完全不透明

Alpha Blending 需要关闭ZWrite，如果不关闭ZWrite，那么在进行深度检测的时候，它背后的物体本来可以被外面看到的，但由于深度检测时大于它的深度就被剔除了，从而我们就看不到它后面的物体了。因此，我们需要保证物体的渲染顺序是从后往前，并且关闭该半透明对象的ZWrite。

Appha Blending 只是关闭ZWrite，但是不会关闭ZTest。这意味着，在输出一个Alpha Blending的fragment时，它还是会判断和当前Color Buffer种的fragment的深度关闭，如果它比当前的fragement深度更远，那么它就不会再做后续的混合操作了；

# ColorMask

ColorMask可让我们指定渲染结果的输出通道，而不是通常情况下的RGBA这4个通道皆会被写入。可选参数是RGBA的任意组合以及0，这将意味着不会写入到任何通道，可以用来单独做一次Z Test，而不将结果写入颜色通道。

# \_Time

\_Time是个4维向量，跟Unity3D中的deltaTime（这个是一维的，数值）不同。

|名称|类型|说明|
|:--|:--|:--|
|_Time|float4|t是自该场景加载开始所经过的时间，4个分量分别是(t/20,t,t*2,t*3)|
|_SinTime|float4|t是时间的正弦值，4个分量分别是(t/8,t/4,t/2,t)|
|_CosTime|float4|t是时间的余弦值，4个分量分别是(t/8,t/4,t/2,t)|
|unity_DeltaTime|float4|dt是时间增量，4个分量分别是(dt,1/dt,smoothDt,1/smoothDt)|
{% asset_img head.gif %}
# shader中函数的基本理解
1. smoothstep获取较为平滑的过渡效果length(uv)降维效果
2. length(uv)将2D转换为1D
3. atan(u,v)获取角度，配合length可以得到极坐标
4. pow(f,n)将曲线变化变得平滑或尖锐
5. sin cos 周期函数，用于实现周期的效果（如来回移动，循环的移动等）
6. hash获取标志ID，常用于基于空间划分的效果的实现
7. noise同时具有随机性和连续性的函数
8. fbm在基本函数的基础上，不断的叠加高频信息，丰富细节
9. frac返回小数部分
10. fmod(x, y) 返回x/y的余数

假如uv是(-0.5,-0.5)到(.5,0.5)
1. smoothstep eg:绘制smoothstep曲线
f(x)=x*x*(3.0-2.0*x)
具有淡入淡出效果
```c
float3 DrawSmoothstep(float2 uv){
	uv+=0.5；
	float val = smoothstep(0.0,1.0,uv.x);
	val = step(abs(val-uv.y),0.01);
	return float3(val,val,val);
}
```