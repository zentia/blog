---
title: Unity3D中自带事件函数的执行顺序
date: 2018-04-11 16:16:30
tags:
  - Unity
---
在Unity3D继承自MonoBehavior的脚本中，有几个Unity3D自带的事件函数按照预定的顺序作为脚本执行。其执行顺序如下：
# 编辑器（Editor）
- **Reset**：Reset函数被调用来初始化脚本属性当脚本第一次被附到对象上，并且在Reset命令被使用时也会调用。Reset是在用户点击Inspector面板上Reset按钮或者首次添加该组件时被调用。Reset最常用于在检视面板中给定一个默认值。

# 第一次场景加载（First Scene Load）
这些函数会在一个场景开始（场景中每个物体只调用一次）时被调用。
- **Awake**：这个函数总是在任何`Start()`函数之前一个预设被实例化之后调用，如果一个GameObject是非激活的(inactive)，在启动期间Awake函数是不会被调用的直到它是活动的(active)。
- **OnEnable**：只有在对象激活(active)状态下才会被调用，这个函数只有在object被启动(enable)后才调用。这会发生在一个MonoBehavior实例被创建，例如当一个关卡被加载或者一个带有脚本组件的GameObject被实例化。

注意：当一个场景被添加到场景中，所有脚本上的Awake()和OnEnable()函数将会被调用在Start()、Update()等它们中任何函数被调用之前。自然的，当一个物体在游戏过程中被实例化时这不能被强制执行。

# 第一帧更新之前（Before the first frame update）
- **Start**：只有在脚本被启用了Start()函数将会在Update()函数第一帧之前被调用。对于那些被添加到场景中的物体，所有脚本上的Start()函数将会在它们中任何的Update()函数之前被调用，当一个物体在游戏过程中被实例化时这不能被强制执行。

# 在帧之间（In between frames）
- **OnApplicationPause**：这个函数将会被调用在暂停被检测有效的在正常的帧更新之间的一帧的结束时。在OnApplicationPause被调用后将会有额外的一帧用来允许游戏显示图像表示在暂停状态下。

# 更新顺序（Update Order）
当你在跟踪游戏逻辑和状态，动画，相机位置等的时候，有几个不同的事件函数你可以使用。常见的模式是在Update()函数中执行大多数人物，但是也有其它的函数你可以使用。
- **FixedUpdate**：**常被用作逻辑Tick函数**，FixedUpdate函数经常会比Update函数更频繁的被调用。它一帧会被调用多次，如果帧率低它可能不会在帧之间被调用，就算帧率是高的。所有的图形计算和更新在FixedUpdate之后会立即执行。当在FixedUpdate里执行移动计算，你并不需要Time.deltaTime乘以你的值，这是因为FixedUpdate是按真实时间，独立于帧被调用的。
- **Update**：Update每一帧都会被调用，对于帧更新它是主要的负荷函数。
- **LateUpdate**：LateUpdate会在Update结束之后每一帧被调用，任何计算在Update里执行结束当LateUpdate开始时。LateUpdate常被为第三人称视角相机跟随。

# 渲染（Rendering）
- **OnPreCull**：在相机剔除场景前被调用。剔除是取决于哪些物体对于摄像机是可见的，OnPreCull尽在剔除起作用之前被调用。
- **OnBecameVisible/OnBecameInvisible**：当一个物体对任意摄像机变得可见/不可见时被调用。
- **OnPreRender**：在摄像机开始渲染场景之前被调用。
- **OnRenderObject**：在指定场景渲染完成之后调用，你可以使用GL类或者Graphics.DrawMeshNow来绘制自定义几何体在这里。
- **OnPostRender**：在摄像机完成渲染之后调用。
- **OnRenderImage(Pro Only)**：在场景渲染完成之后允许屏幕图像后期处理调用。
- **OnGUI**：为了响应GUI事件，每帧会被调用多次（一般最低两次）。布局Layout和Repaint事件会首先处理，接下来是通过Layout和键盘/鼠标事件对应的每个输入事件。
- **OnDrawGizmos**：用于可视化的绘制一些小玩意在场景视图中。

# 协同程序（Corotines）
正常的协同程序更新是在Update函数饭回之后运行。一个协同程序是可以暂停执行(yield)直到给出的依从指令（YieldInstruction）完成，携程的不同运用。
- **yield**：在所有的Update函数都已经被调用的下一帧该协程将持续执行。
- **yield WaitForSeconds**：一段指定的时间延迟之后继续执行，在所有的Update函数完成调用的那一帧之后。
- **yield WaitForFixedUpdate**：所有脚本上的FixedUpdate函数已经执行调用之后持续。
- **yield WWW**：在WWW下载完成之后持续。
- **yield StartCoroutine**：协同程序链，将会等到MuFunc函数协程执行完成首先。

# 销毁（When the Object is Destroyed）
- **OnDestroy**：这个函数会在一个对象销毁前一帧调用，会在所有帧更新一个对象存在的最后一帧之后执行，对象也许会响应Object.Destroy或一个场景关闭时被销毁。

# 退出游戏（When Quitting）
这些函数会在你场景中所有的激活的物体上调用：
- **OnApplicationQuit**：这个函数在应用退出之前的所有游戏物体上调用，在编辑器（Editor）模式中会在用户停止PlayMode时调用，在网页播放器（web Player）中会在网页视图关闭时调用。
- **OnDisable**：当行为变为非启用(disable)或非激活（inactive）时调用。

# 脚本的生命周期流程图
{% asset_img 1.png %}

在Update中使用Time.deltaTime，获取到的是这一帧的时间，如果游戏卡，帧率低，那这个值就大。如果游戏流畅，帧率高，这个值就小，Time.deltaTime=1.0f/帧率

1. MonoBehaviour.Update 更新渲染帧
当MonoBehaviour启用时，其Update在每一帧被调用。

2. MonoBehaviour.FixedUpdate 固定更新逻辑帧
当MonoBehaviour启用时，其 FixedUpdate在每一帧被调用。
处理Rigidbody时，需要用FixedUpdate代替Update。例如：给刚体加一个作用力时，你必须应用作用力在FixedUpdate里的固定帧，而不是Update中的帧(两者帧长不同)。

3. MonoBehaviour.LateUpdate 晚于更新渲染帧末尾
当Behaviour启用时，其LateUpdate在每一帧被调用。
LateUpdate是在所有Update函数调用后被调用。这可用于调整脚本执行顺序。例如:当物体在Update里移动时，跟随物体的相机可以在LateUpdate里实现。

# Update和FixedUpdate的区别

Update跟当前平台的帧数有关，而FixedUpdate是CPU tick 的时间，所以处理物理逻辑的时候要把代码放在FixedUpdate而不是Update。
Update是在每次渲染新的一帧的时候才会调用，也就是说，这个函数的更新频率和设备的性能有关以及被渲染的物体（可以认为是三角形的数量）。在性能好的机器上可能fps 30，差的可能小些。这会导致同一个游戏在不同的机器上效果不一致，有的快有的慢。因为Update的执行间隔不一样了。
而FixedUpdate，是在固定的时间间隔执行，不受游戏帧率的影响。有点像Tick。所以处理Rigidbody的时候最好用FixedUpdate。

FixedUpdate的时间间隔可以在项目设置中更改，Edit->ProjectSetting->time  找到Fixedtimestep。就可以修改了。
     
# Update和LateUpdate的区别

LateUpdate是在所有Update函数调用后被调用。这可用于调整脚本执行顺序。例如：当物体在Update里移动时，跟随物体的相机可以在LateUpdate里实现。
LateUpdate是晚于所有Update执行的。例如：游戏中有2个脚步，脚步1含有Update和LateUpdate，脚步2含有Update，那么当游戏执行时，每一帧都是把2个脚步中的Update执行完后才执行LateUpdate 。虽然是在同一帧中执行的，但是Update（渲染帧）会先执行，LateUpdate（渲染帧末尾）会晚执行。
现在假设有2个不同的脚本同时在Update中控制一个物体，那么当其中一个脚本改变物体方位、旋转或者其他参数时，另一个脚步也在改变这些东西，那么这个物体的方位、旋转就会出现一定的反复。如果还有个物体在Update中跟随这个物体移动、旋转的话，那跟随的物体就会出现抖动。 如果是在LateUpdate中跟随的话就会只跟随所有Update执行完后的最后位置、旋转，这样就防止了抖动。

# 影响渲染顺序因素地总结

1. Camrea Depth 相机组件上设置的相机深度，深度越大越靠后渲染。
2. Sorting Layer 在Tags & Layers设置中可见。
3. Order In Layer 相当于Sorting Layer的子排序，用这个值做比较时只有都在同一层时才有效。
4. RenderQueue Shader中对Tags设置的"Queue"

Camera Depth永远最高。Camera Depth小的一定先进渲染管线。
当Sorting Layer和Order in Layer相同时
RenderQueue小的先进渲染管线。

当Sorting Layer和Order In Layer不相同时！
当两个材质使用了不同的RenderQueue,且这两个RenderQueue都在[0~2500]或[2501~5000]时，SortingLayer和OrderInLayer的排序生效。
当两个材质使用了不同的RenderQueue,且这两个RenderQueue分别再[0~2500]或[2501~5000]时，则一定会按照RenderQueue绘制，无视SortingLayer,OrderInLayer的排序。