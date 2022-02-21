---
title: presentandsync
date: 2018-07-05 11:25:56
tags:
	uwa
---

# WaitForTargetFPS

该参数一般出现在CPU开销过低，且通过设定了目标帧率地情况下(Application.targetFrameRate)。当上一帧产生一个WaitForTargetFPS地空闲等待消耗时，以维持目标帧率。

解析：该项在Unity引擎地主循环中其实是最早执行地，即引擎实际上是根据上一帧地CPU耗时，在当前帧通过增补WaitForTargetFPS的方式来将运行FPS维持到目标值。比如，目标帧率为30帧/秒，上一帧耗时15ms，那么当前帧中WaitForTargetFPS将会是18（33-15）ms，但是这一帧中其他耗时为28ms，那么在Profiler中这一帧的总耗时就变成了46（18+28
）ms。
因此，由该值造成了Profiler开销较高的现象，其实是耗时的“假象”，在优化过程中，你对它可以“视而不见”。

# Gfx.WaitForPresend && Graphics.PresentAndSync

这两个参数在Profiler中经常出现CPU占用较高的情况，且仅在发布版本中可以看到。究其原因，其实是CPU和GPU之间垂直同步（VSync）导致的，之所以会有两种参数，主要是与项目是否开启多线程渲染有关。当项目开启多线程渲染时，你看到的是Gfx.WaitForPresent；当项目未开启多线程渲染时，看到的则是Graphics.PresentAndSync。

Graphics.PresentAndSync是指主线程进行Present时的等待时间和等待垂直同步的时间。

Gfx.WaitForPresent其字面意思同样也是进行Present时需要等待的时间，但这里其实省略了很多的内容，其真实的意思应该是为了在渲染子线程（Rendering Thread）中进行Presend，当前主线程（MainThread）需要等待的时间。