# 观察两个空间
阴影映射(Shadow Mapping)牵涉到两个空间的Z深度比较,一个是灯光空间,一个是相机空间,首先打开ShadowMapping_2下的场景,场景是默认的相机视角,单击下面的Light View按钮,可以切换到灯光视角,
13.1.2 两个视角的Z深度
分别单机LightView Depth和CamView Depth,然后到Shadow/ShadowMapping工程文件夹下面找到_CamViewZDepth和_LightViewZDepth两张Render Texture,查看一下渲染输出的ZDepth
13.1.3 渲染Z深度的材质
这两张ZDepth是分别在灯光视角和相机使用Replacement Shader 渲染出来的
在vertex函数vert中,主要操作除了对物体形体的必要输出外,就是UNITY_TRANSFERDEPTH(o.depth),打开UnityCG.cginc文件,会发现它一般情况下的操作就是o.depth = o.pos.zw,把物体在投影空间做的zw值赋给o.depth,再将它的值输出到一张Render Texture 之前,通过Linear01Depth(d)函数把Z的值变换到01空间,这样我们可以看到一张对比度更强的Z深度图,就像在上面所看到的那样,而不是一张大多数情况下都朦朦胧胧的Z深度图
13.2 投射Z深度
13.2.1 准备灯光视角的投影矩阵
 在进行映射阴影经典的Z深度比较之前,我们需要把灯光视角ZDepth深度图投射到相机视角.打开ShadowMapping_3下的场景.和上一个场景系相比,附加在Main Camera物体上的ShadowMapping_3.cs
13.3.4 对Z值进行偏移
显然,Z的精度是个问题,而且通过上述编码操作我们已经解决了Shadow ance的问题,但是主要问题依然,这个现象叫Peter Panning,源自迪尼斯的一部动画主角Peter Pan,他的影子可以脱离自身活动起来,打开

1. 分开处理反射面的绝缘体特性和金属属性，最后光照应该是Diffuse+Specular
2. 纯金属没有Diffuse，非金属主要是Diffuse，有一点反射
3. 光部分主要有3个东西影响：微表面的法线分布（NDF），微表面的入射和反射遮挡（Geometry Function），反射率和入射角的关系（Fresnel反射）。
