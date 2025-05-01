---
title: 如何系统地进行性能优化？
date: 2018-05-17 11:41:24
tags:
    - Unity
---

### 运行帧率低

低配机器上运行帧率比较低是常见问题。
a)首先是做分类，把通常的问题归类到几种瓶颈下来做讨论：GPU Bound、CPU Bound、Bandwidth Bound，还有Memory。这种分类方式基本是按照硬件资源的方式来进行的。
b)GPU Bound是一种目前手游中比较常见的瓶颈，降低分辨率如果对帧率有非常明显的提升的话，非常大的可能就是GPU Bound。
c)CPU Bound,降低分辨率的方式如果并不能提高帧率，那么可能是CPU Bound。CPU通常和Draw Call祥光，或者和复杂的游戏逻辑相关，通过Unity的Profiler工具可以比较明确看出是否是CPU Bound，以及这些计算时间都消耗在了哪里。
TimeLine可以比较明确的看到瓶颈（5.3.8没有，暂时不研究）
d)Bandwith Bound 在手游中也比较常见，一种比较简单的判断方式是使用NVIDIA Tegra Graphics Debugger这个工具（对于Tegra硬件更友好，但是比较难找。。。）中将所有贴图替换成2 * 2的大小，如果帧率有明显提升可能是Bandwidth瓶颈。当然也可以自己协代码所图来确认，比如使用Unity的Quality Setting里的Texture Quality。
<img src="http://oxcvfpext.bkt.clouddn.com/How-to-systematically-optimize-performance-1.png">
e)内存就用Unity地Profiler来看吧，不过这个对于帧率没什么影响。
