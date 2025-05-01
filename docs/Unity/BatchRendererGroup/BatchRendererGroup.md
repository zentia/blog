Graphics.DrawMeshInstanced相比于BatchRendererGroup，它具有以下限制：

- 需要提供带有矩阵的托管内存组来执行垃圾数据回收。并且，即使着色器不需要逆矩阵，CPU仍然会进行计算。
- 若想自定义除obj2world矩阵外的任何属性（例如每个实例具有一种颜色），就必须使用Shader Graph或者从零写一个自定义Shader。
- 矩阵或自定义数据在每次绘制时必须上传至CPU内存。Graphics.DrawMeshInstanced的GPU内存数据不能持续存在。在特定情况下，会造成巨大的性能冲击。

BatchRendererGroup(BRG)API可以高效的从C#生成绘制指令、发起GPU实例化的绘制调用。它不依赖于托管内存，所以可以用Burst Compiler来生成指令。

|优点|缺点|
|-|-|
|能从Burst作业快速生成DrawInstanced指令|必须手动将绘制指令捆绑成优化好的批次|
|可用持续存在的大型GPU缓存区来存储每个实例的自定义属性|必须手动管理GPU内存和自定义属性偏移分配|
|广受各类平台支持，包括OpenGLES3.0及以上||
|兼容标准SRP着色器（受光和不受光）。无需编写自定义着色器||