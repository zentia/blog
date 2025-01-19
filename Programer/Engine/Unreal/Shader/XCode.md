---
title: XCode
date: 2022-01-04 21:26:00
sticky: 100
categories:
- Engine
- Unreal
- Shader
tags:
    - Unreal
---
# CPU Bound or GPU Bound?可能是个伪命题
一些老牌的优化大佬可能会和你说优化的第一步就是定位出到底是GPU Bound还是CPU Bound然后去针对优化，然而这样的做法移动平台上不一定使用。
因为移动平台需要考虑一个重要的影响因素就是发热耗电，当手机严重发热时，不仅是对玩家体验造成很大的影响，更会让CPU降频，降频之后不管你CPU优化的多厉害，都会GG。

造成发热耗电的很大的一个影响因素就是GPU消耗，按照本人的优化经验60fps模式，当GPU消耗超过10ms的时候就会引起设备发热，如果游戏一启动，直接“GPU Bound"(60帧下GPU消耗超过16.66ms)了，那几本几分钟内手机就会热到爆炸。

所以在移动设别上去谈性能优化的时候，可能发热耗电才是最需要考虑的。

下面说一下一些shader的调试方法。

# Update Shaders Live
截帧之后在bound resource中找到对应的vertex shader和fragment shader，打开代码之后修改对应的代码，点击reload按钮就可以看到修改之后的shader的效果，同时手机上的shader也会更新。
# 调试vertex shader
有些时候我们可能会遇到一些因为顶点数据出错的问题，比如顶点色错误导致渲染发黑，顶点法线错误导致高光计算错误，uv错误导致采样出错等。
选择Draw Call中的Geometry，就可以看到当前渲染的网格，鼠标点击其中的某个三角形，下面就会显示当前三角形的输入信息和输出信息。
如果还要深入看Vertex Shader中Shader的执行过程以及中间的变量值，点击面板的debug按钮，可以看到展开的Shader，右边的数据就是当前三角形在此vertex shader中执行的过程了，左边列表就是单步调试，下面就是监视器。
# 调试fragment shader
像素级别的渲染问题出现的就更多了，变黑，烂面，平台显示效果不一致，贴图丢失等，如果无法截帧的话，很难定位这些问题。xcode提供了很多强大的调试功能。
在xcode中截帧之后，在bound resources中可以看到所有的贴图，对应的就是材质球上的贴图槽。