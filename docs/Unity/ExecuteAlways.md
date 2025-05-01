# 描述
使脚本的实例始终执行，作为Play Mode的一部分和Editing。

默认情况下，MonoBehaviours仅在Play Mode下执行。

当希望脚本作为编辑器工作的一部分执行某些操作时，可以使用[ExecuteAlways]属性，该工具不一定与构建

- 仅在Scene发生改变是调用Update
- 当Game 试图接收到它不使用的非编辑器事件（例如，EventType.ScrollWheel）会调用OnGUI
- 在场景试图或者Game试图的每次重绘都会调用OnRenderObject和其他渲染回调函数