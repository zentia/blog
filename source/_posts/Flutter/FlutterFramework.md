---
title: Flutter源码阅读分析：Framework层的启动
categories:
 - Flutter
Tags:
 - Flutter
---
在上一篇文章的最后，提到了引擎在启动时，会以"main"方法为入口函数，执行Dart代码。那么本文就从`main`入手，分析`Dart Framework`具体做了什么。
```dart
void main() {
  runApp(ModularApp(module: AppModule(), child: MyApp()));
}
```
- `runApp`方法的主要功能是填充给定的`Widget`并将其Attach到屏幕上。

```Dart
void runApp(Widget app) {
  WidgetsFlutterBinding.ensureInitialized()
    ..scheduleAttachRootWidget(app)
    ..scheduleWarmUpFrame();
}
```
如果重复调用`runApp`方法，那么会将之前的根`Widget`从屏幕中移除，并将新的指定的`Widget`替换到该位置。这个新的Widget树与前者进行对比，并将区别的地方应用到后续的渲染树。这里涉及到Flutter的渲染布局机制，会在后续讲解。

`WidgetsFlutterBinding`类是基于Widget框架的应用程序的具体绑定。这将框架和Flutter引擎绑定起来。
```Dart
class WidgetsFlutterBinding extends BindingBase with GestureBinding, SchedulerBinding, ServicesBinding, PaintingBinding, SemanticsBinding, RendererBinding, WidgetsBinding {
  /// Returns an instance of the binding that implements
  /// [WidgetsBinding]. If no binding has yet been initialized, the
  /// [WidgetsFlutterBinding] class is used to create and initialize
  /// one.
  ///
  /// You only need to call this method if you need the binding to be
  /// initialized before calling [runApp].
  ///
  /// In the `flutter_test` framework, [testWidgets] initializes the
  /// binding instance to a [TestWidgetsFlutterBinding], not a
  /// [WidgetsFlutterBinding]. See
  /// [TestWidgetsFlutterBinding.ensureInitialized].
  static WidgetsBinding ensureInitialized() {
    if (WidgetsBinding._instance == null)
      WidgetsFlutterBinding();
    return WidgetsBinding.instance;
  }
}
```
`ensureInitialized`方法返回一个`WidgetBinding`的实例。
再看一下`scheduleAttachRootWidget`和`scheduleWarmUpFrame`方法：
```Dart
  @protected
  void scheduleAttachRootWidget(Widget rootWidget) {
    Timer.run(() {
      attachRootWidget(rootWidget);
    });
  }
```
```Dart
  void scheduleWarmUpFrame() {
    if (_warmUpFrame || schedulerPhase != SchedulerPhase.idle)
      return;

    _warmUpFrame = true;
    final TimelineTask timelineTask = TimelineTask()..start('Warm-up frame');
    final bool hadScheduledFrame = _hasScheduledFrame;
    // We use timers here to ensure that microtasks flush in between.
    Timer.run(() {
      assert(_warmUpFrame);
      handleBeginFrame(null);
    });
    Timer.run(() {
      assert(_warmUpFrame);
      handleDrawFrame();
      // We call resetEpoch after this frame so that, in the hot reload case,
      // the very next frame pretends to have occurred immediately after this
      // warm-up frame. The warm-up frame's timestamp will typically be far in
      // the past (the time of the last real frame), so if we didn't reset the
      // epoch we would see a sudden jump from the old time in the warm-up frame
      // to the new time in the "real" frame. The biggest problem with this is
      // that implicit animations end up being triggered at the old time and
      // then skipping every frame and finishing in the new time.
      resetEpoch();
      _warmUpFrame = false;
      if (hadScheduledFrame)
        scheduleFrame();
    });

    // Lock events so touch events etc don't insert themselves until the
    // scheduled frame has finished.
    lockEvents(() async {
      await endOfFrame;
      timelineTask.finish();
    });
  }
```

这里涉及到`Widget`、`Element`、`Render`，都属于Flutter的渲染机制，后续在渲染机制中详细分析。

- `handleBeginFrame`方法主要作用是让framework准备好，去创建一个新的帧。它会调用所有通过`scheduleFrameCallback`注册进来的临时回调函数。
- `handleDrawFrame` 方法主要作用是创建一个新的帧，这个方法一般紧跟着`handleBeginFrame`方法后调用。

```Dart
  void handleDrawFrame() {
    assert(_schedulerPhase == SchedulerPhase.midFrameMicrotasks);
    _frameTimelineTask?.finish(); // end the "Animate" phase
    try {
      // PERSISTENT FRAME CALLBACKS
      _schedulerPhase = SchedulerPhase.persistentCallbacks;
      for (final FrameCallback callback in _persistentCallbacks)
        _invokeFrameCallback(callback, _currentFrameTimeStamp!);

      // POST-FRAME CALLBACKS
      _schedulerPhase = SchedulerPhase.postFrameCallbacks;
      final List<FrameCallback> localPostFrameCallbacks =
          List<FrameCallback>.of(_postFrameCallbacks);
      _postFrameCallbacks.clear();
      for (final FrameCallback callback in localPostFrameCallbacks)
        _invokeFrameCallback(callback, _currentFrameTimeStamp!);
    } finally {
      _schedulerPhase = SchedulerPhase.idle;
      _frameTimelineTask?.finish(); // end the Frame
      assert(() {
        if (debugPrintEndFrameBanner)
          debugPrint('▀' * _debugBanner!.length);
        _debugBanner = null;
        return true;
      }());
      _currentFrameTimeStamp = null;
    }
  }
```
`handleDrawFrame`会调用所有通过`addPersistentFrameCallback`方法注册进来的回调函数（这些回调函数通常都是驱动渲染管线），以及通过`addPostFrameCallback`方法注册进来的回调函数。
在`RendererBinding`类的`initInstances`方法调用了`addPersistentFrameCallback`方法。其添加的回调函数如下：
```Dart
  void _handlePersistentFrameCallback(Duration timeStamp) {
    drawFrame();
    _scheduleMouseTrackerUpdate();
  }

  void drawFrame() {
    assert(renderView != null);
    pipelineOwner.flushLayout();
    pipelineOwner.flushCompositingBits();
    pipelineOwner.flushPaint();
    if (sendFramesToEngine) {
      renderView.compositeFrame(); // this sends the bits to the GPU
      pipelineOwner.flushSemantics(); // this also sends the semantics to the OS.
      _firstFrameSent = true;
    }
  }
```
`drawFrame`方法主要是让渲染管线产生出一帧。
- `scheduleFrame`则是在需要的时候，通过调用`platformDispatcher`的`scheduleFrame`方法安排一个新的帧。
```Dart
void scheduleFrame() {
    if (_hasScheduledFrame || !framesEnabled)
      return;
    assert(() {
      if (debugPrintScheduleFrameStacks)
        debugPrintStack(label: 'scheduleFrame() called. Current phase is $schedulerPhase.');
      return true;
    }());
    ensureFrameCallbacksRegistered();
    platformDispatcher.scheduleFrame();
    _hasScheduledFrame = true;
  }
```
这里的`platformDispatcher`类是引擎内实现的，通过Dart引擎扩展API，调用到C++的`ScheduleFrame`方法：
```c++
void PlatformConfigurationNativeApi::ScheduleFrame() {
  UIDartState::ThrowIfUIOperationsProhibited();
  UIDartState::Current()->platform_configuration()->client()->ScheduleFrame();
}
```
# 渲染管线
下面看一下`PipelineOwner`，这个类是渲染树的持有者，维护布局、合成、绘制和可达性的状态。
`PipelineOwner`主要方法有以下几个
```dart
void flushLayout() {
    if (!kReleaseMode) {
      Map<String, String> debugTimelineArguments = timelineArgumentsIndicatingLandmarkEvent;
      assert(() {
        if (debugProfileLayoutsEnabled) {
          debugTimelineArguments = <String, String>{
            ...debugTimelineArguments,
            'dirty count': '${_nodesNeedingLayout.length}',
            'dirty list': '$_nodesNeedingLayout',
          };
        }
        return true;
      }());
      Timeline.startSync(
        'LAYOUT',
        arguments: debugTimelineArguments,
      );
    }
    assert(() {
      _debugDoingLayout = true;
      return true;
    }());
    try {
      while (_nodesNeedingLayout.isNotEmpty) {
        final List<RenderObject> dirtyNodes = _nodesNeedingLayout;
        _nodesNeedingLayout = <RenderObject>[];
        for (final RenderObject node in dirtyNodes..sort((RenderObject a, RenderObject b) => a.depth - b.depth)) {
          if (node._needsLayout && node.owner == this)
            node._layoutWithoutResize();
        }
      }
    } finally {
      assert(() {
        _debugDoingLayout = false;
        return true;
      }());
      if (!kReleaseMode) {
        Timeline.finishSync();
      }
    }
  }
  void flushCompositingBits() {
    if (!kReleaseMode) {
      Timeline.startSync('UPDATING COMPOSITING BITS', arguments: timelineArgumentsIndicatingLandmarkEvent);
    }
    _nodesNeedingCompositingBitsUpdate.sort((RenderObject a, RenderObject b) => a.depth - b.depth);
    for (final RenderObject node in _nodesNeedingCompositingBitsUpdate) {
      if (node._needsCompositingBitsUpdate && node.owner == this)
        node._updateCompositingBits();
    }
    _nodesNeedingCompositingBitsUpdate.clear();
    if (!kReleaseMode) {
      Timeline.finishSync();
    }
  }
  void flushPaint() {
    if (!kReleaseMode) {
      Map<String, String> debugTimelineArguments = timelineArgumentsIndicatingLandmarkEvent;
      assert(() {
        if (debugProfilePaintsEnabled) {
          debugTimelineArguments = <String, String>{
            ...debugTimelineArguments,
            'dirty count': '${_nodesNeedingPaint.length}',
            'dirty list': '$_nodesNeedingPaint',
          };
        }
        return true;
      }());
      Timeline.startSync(
        'PAINT',
        arguments: debugTimelineArguments,
      );
    }
    assert(() {
      _debugDoingPaint = true;
      return true;
    }());
    try {
      final List<RenderObject> dirtyNodes = _nodesNeedingPaint;
      _nodesNeedingPaint = <RenderObject>[];
      // Sort the dirty nodes in reverse order (deepest first).
      for (final RenderObject node in dirtyNodes..sort((RenderObject a, RenderObject b) => b.depth - a.depth)) {
        assert(node._layerHandle.layer != null);
        if (node._needsPaint && node.owner == this) {
          if (node._layerHandle.layer!.attached) {
            PaintingContext.repaintCompositedChild(node);
          } else {
            node._skippedPaintingOnLayer();
          }
        }
      }
      assert(_nodesNeedingPaint.isEmpty);
    } finally {
      assert(() {
        _debugDoingPaint = false;
        return true;
      }());
      if (!kReleaseMode) {
        Timeline.finishSync();
      }
    }
  }
  void flushSemantics() {
    if (_semanticsOwner == null)
      return;
    if (!kReleaseMode) {
      Timeline.startSync('SEMANTICS', arguments: timelineArgumentsIndicatingLandmarkEvent);
    }
    assert(_semanticsOwner != null);
    assert(() {
      _debugDoingSemantics = true;
      return true;
    }());
    try {
      final List<RenderObject> nodesToProcess = _nodesNeedingSemantics.toList()
        ..sort((RenderObject a, RenderObject b) => a.depth - b.depth);
      _nodesNeedingSemantics.clear();
      for (final RenderObject node in nodesToProcess) {
        if (node._needsSemanticsUpdate && node.owner == this)
          node._updateSemantics();
      }
      _semanticsOwner!.sendSemanticsUpdate();
    } finally {
      assert(_nodesNeedingSemantics.isEmpty);
      assert(() {
        _debugDoingSemantics = false;
        return true;
      }());
      if (!kReleaseMode) {
        Timeline.finishSync();
      }
    }
  }
```

- `flushLayout`方法用于更新所有“脏”`RenderObject`的布局信息。该方法是渲染管线中的核心步骤之一。布局信息需要在绘制之前处理“干净”，这样`RenderObject`就可以在屏幕