---
title: Unity3D笔记
date: 2018-04-11 16:16:30
tags:
  - Unity3D
---
在Update中使用Time.deltaTime，获取到的是这一帧的时间，如果游戏卡，帧率低，那这个值就大。如果游戏流畅，帧率高，这个值就小，Time.deltaTime=1.0f/帧率

1、MonoBehaviour.Update 更新渲染帧
当MonoBehaviour启用时，其Update在每一帧被调用。

2、MonoBehaviour.FixedUpdate 固定更新逻辑帧
当MonoBehaviour启用时，其 FixedUpdate在每一帧被调用。
处理Rigidbody时，需要用FixedUpdate代替Update。例如：给刚体加一个作用力时，你必须应用作用力在FixedUpdate里的固定帧，而不是Update中的帧(两者帧长不同)。

3、MonoBehaviour.LateUpdate 晚于更新渲染帧末尾
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

