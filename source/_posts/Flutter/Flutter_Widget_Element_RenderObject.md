---
title: Flutter渲染机制：Widget、Element和RenderObject
date: 2022-08-17 14:49:00
tags:
 - Flutter
categories:
 - Flutter
---
在上一篇文章中我们分析了Framework层的启动流程，其中讲到了在`runApp`方法中，调用到了`attachRootWidget`方法：
```Dart
  void attachRootWidget(Widget rootWidget) {
    final bool isBootstrapFrame = renderViewElement == null;
    _readyToProduceFrames = true;
    _renderViewElement = RenderObjectToWidgetAdapter<RenderBox>(
      container: renderView,
      debugShortDescription: '[root]',
      child: rootWidget,
    ).attachToRenderTree(buildOwner!, renderViewElement as RenderObjectToWidgetElement<RenderBox>?);
    if (isBootstrapFrame) {
      SchedulerBinding.instance.ensureVisualUpdate();
    }
  }
```
这个方法获取一个`Widget`并将其辅导`renderViewElement`上，在必要的时候创建这个`renderViewElement`。其中涉及到了`Widget`、`Element`和`Render`，都属于Flutter渲染机制。本文将对Flutter渲染机制进行分析。

首先看一下`RenderObjectToWidgetAdapter`这个类和其构造方法：
```Dart
class RenderObjectToWidgetAdapter<T extends RenderObject> extends RenderObjectWidget {
  /// Creates a bridge from a [RenderObject] to an [Element] tree.
  ///
  /// Used by [WidgetsBinding] to attach the root widget to the [RenderView].
  RenderObjectToWidgetAdapter({
    this.child,
    required this.container,
    this.debugShortDescription,
  }) : super(key: GlobalObjectKey(container));
  ...
}
```
这个类的作用是桥接`RenderObject`和`Element`树，其中`container`就是`RenderObject`，而`Element`树则插入在其中。类型参数`T`是一种`RenderObject`，是`container`期望孩子的类型。在看一下`RenderObjectToWidgetAdapter`类的`attachToRenderTree`方法：
```Dart
  RenderObjectToWidgetElement<T> attachToRenderTree(BuildOwner owner, [ RenderObjectToWidgetElement<T>? element ]) {
    if (element == null) {
      owner.lockState(() {
        element = createElement();
        assert(element != null);
        element!.assignOwner(owner);
      });
      owner.buildScope(element!, () {
        element!.mount(null, null);
      });
    } else {
      element._newWidget = this;
      element.markNeedsBuild();
    }
    return element!;
  }
```
这个方法填充了`widget`，并且将结果`RenderObject`设置为`container`的孩子。如果`element`为空，那么则会创建一个新的`element`，否则的话，给定的`element`会调度一个更新，使得与当前`widget`关联。

从上面可以看出，对于Flutter框架来说，主要关注的就是`Widget`、`Element`、`RenderObject`。下面我们来分析一下这三者的特点和关系。

# Widget

在上文的例子中，`rootWidget`是用户开发的Flutter应用的根节点，是一个`Widget`类。在`Widget`类的注释宏，官方给出的定位是用于描述`Element`的配置。`Widget`是Flutter框架的中心类结构。一个`Widget`是UI中一个固定不变的部分。可以被填充成`Element`，而`Element`又管理底层的渲染树。`Widget`本身是没有可变状态的，所有的成员变量都是`final`的。入股偶需要和一个`Widget`关联的可变状态，可以使用`StatefulWidget`，这个类会创建一个`StatefulWidget`，而它又会填充成`element`和合并到树中的时候创建一个`State`对象。一个给定的`Widget`可以被包含到树中0次或更多次。特别是`Widget`可以被多次放置到树中。每次`Widget`被放置到树中，都会填充成一个`Element`。看一下`Widget`基类的方法生命：
```Dart
@immutable
abstract class Widget extends DiagnosticableTree {
  /// Initializes [key] for subclasses.
  const Widget({ this.key });

  Element createElement();

  static bool canUpdate(Widget oldWidget, Widget newWidget) {
    return oldWidget.runtimeType == newWidget.runtimeType
        && oldWidget.key == newWidget.key;
  }

  static int _debugConcreteSubtype(Widget widget) {
    return widget is StatefulWidget ? 1 :
           widget is StatelessWidget ? 2 :
           0;
  }
}
```
分别介绍一下这几个方法和成员变量。首先是`key`这个成员变量，它用于控制在树中一个`Widget`如何替换另一个。主要有以下几种方式：更新`Element`、替换`Element`以及换位置。
通常情况下，如果一个`Widget`是另一个的唯一孩子，那么不需要明确的`key`。
`createElement`方法用于将配置填充为一个具体的实例。`canUpdate`方法用于判断`newWidget`能否用于更新当前以`oldWidget`为配置的`Element`。`_debugConcreteSubtype`方法返回一个编码值，用于指示`Widget`的实际子类型，1表示`StatefulWidget`，2表示`StatelessWidget`。`StatefulWidget`和`StatelessWidget`都是`Widget`的抽象子类，下面看一下这两个子类的具体情况。

# StatelessWidget

`StatelessWidget`用于不需要可变状态的情况。一个无状态`Widget`通过建立一些列其他更完整描述UI的`Widget`的方式，来描述部分UI。这个构建过程是一个递归的过程，直到这个描述已经被完全实现。当部分UI以来的只有其自身配置信息和`BuildContext`是，`StatelessWidget`就非常有用了。

```Dart
abstract class StatelessWidget extends Widget {
  ...
  @protected
  Widget build(BuildContext context);
}
```
`build`方法会在当前`Widget`被插入到给定`BuildContext`内的树中时被调用。框架会用这个方法返回的`Widget`更新当前`Widget`的子树，可能是更新现有子树，也可能是移除子树。然后根据返回的`Widget`填充一个新的子树。通常情况下，这个方法的实现，会返回一个新建的`Widget`系列，构建信息是根据从当前`Widget`构造函数和给定`BuildContext`中传递进来的信息来配置。

# StatefulWidget

`StatefulWidget`拥有一个可变的状态。这个状态`State`在`Widget`建立时可以同步地被读取，而在`Widget`的整个生命周期中可能会改变。`StatefulWidget`可用于可动态变化的用户描述。比如，依赖于一些系统状态或者时钟驱动的情况。

```Dart
abstract class StatefulWidget extends Widget {
    
}
```