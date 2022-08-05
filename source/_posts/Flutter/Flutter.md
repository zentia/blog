---
title: Flutter源码阅读分析：引擎初始化与启动
categories:
 - Flutter
---
在引擎中存在两个FlutterActivity(shell/platform/android/io/flutter/app/FlutterActivity.java和shell/platform/android/io/flutter/embedding/android/FlutterActivity.java)。
`FlutterActivity`是将Flutter集成到Flutter到Android应用中的最简单最直接的方式，用于显示一个全屏的Flutter UI。主要职责是：
- 显示一个Android的launch screen
- 显示Flutter的splash screen
- 设置状态栏`configureStatusBarForFullscreenFlutterExperience()`
- 选择Dart执行应用包路径和入口点
- 选择Flutter的初始化路由
- 如果需要的话，渲染透明度`configureWindowForTransparency`
- 提供子类钩子，提供和配置FlutterEngine

# FlutterActivityAndFragmentDelegate
`onAttach`：
1. 初始化Flutter系统
2. 获取或创建一个`FlutterEngine`
3. 创建和配置`PlatformPlugin`
4. 将`FlutterEngine`附在`Activity`上
5. 通过`Host`的`configureFlutterEngine`方法配置`FlutterEngine`
`onCreateView`：
1. 在View树中创建了一个新的`FlutterView`
2. 在`FlutterView`中添加一个`FlutterUiDisplayListener`
3. 将`FlutterEngine`附着在`FlutterView`上
4. 返回这个新的View树

# FlutterView
`FlutterView`的作用是将Flutter UI通过对应的`FlutterEngine`绘制后，显示在Android设备上。这里有两种渲染模式：`surface`和`texture`。
在一般情况下，我们都使用`surface`模式，即将Flutter UI绘制在`SurfaceView`上，这种模式拥有最优的性能，缺点是无法置于两个其他Android View和z-index之间，同时也无法动画化或者进行变换。
# FlutterEngine
`FlutterEngine`是一个独立的Flutter运行环境，是Dart代码运行在Android应用的容器。
`DartExecutor`用于配置、启动、执行Dart代码。
在`FlutterActivityAndFragmentDelegate`的`setupFlutterEngine`方法中调用的`FlutterEngine`构造方法没有传入`FlutterJNI`，`FlutterJNI`则是沟通Android的Java与Flutter引擎核心C++代码的桥梁。
# FlutterJNI
`FlutterJNI`是Flutter内嵌Java代码和引擎C++代码之间的接口。
Flutter引擎的代码时使用C++实现的。Android Flutter嵌入则负责协调Android系统事件和应用用户交互事件。这些事件协调需要消息交流接口，这就需要用到JNI(Java Native Interface)来穿过Java/native边界。
在Flutter的设计中，所有的JNI接口都集中在`FlutterJNI`这个类中。这么做主要有以下几个原因：
- JNI调用都是静态的，且没有Java实现，因此没有理由将调用与不同的类关联；
- 所有的JNI调用必须在C/C++代码中注册，当增加额外的包含JNI调用时，这个注册会变得非常复杂；

大部分`FlutterJNI`中的调用都与特定的`platform view`相关，而`platform view`的数量可能会很多。所以，在执行了`attachToNative`方法后，每个`FlutterJNI`实例都持有一个本地`platform view`的ID，且这个ID与bendingC/C++引擎代码共享。这个ID会被传递到所有的具体platform view的本地方法。
```C++
// ./shell/platform/android/platform_view_android_jni_impl.cc
bool RegisterApi(JNIEnv* env) {
    static const JNINativeMethod flutter_jni_methods[] = {

    };
}
```
- `AndroidShellHolder`是C/C++的`Shell`持有类

# AndroidShellHolder
在`AndroidShellHolder`中保存有Flutter设置参数、FlutterJNI的Java引用、`PlatformViewAndroid`对象（该对象在后续创建）、`Shell`对象等。
```C++
// ./shell/platform/android/android_shell_holder.cc
AndroidShellHolder::AndroidShellHolder(
    flutter::Settings settings,
    std::shared_ptr<PlatformViewAndroidJNI> jni_facade)
    : settings_(std::move(settings), jni_facade_(jni_facade)) {
        auto mask = 
            ThreadHost::Type::UI | ThreadHost::Type::RASTER | ThreadHost::Type::IO;   
    }
```

- 创建`PlatformViewAndroid`，该类继承于`PlatformView`。各平台会根据自身特点继承`PlatformView`类，实现自身平台的`PlatformView`。

# Shell
`Shell`类是Flutter引擎中最重要的类之一。当嵌入应用创建了一个Flutter应用，都将创建一个`Shell`的实例，且嵌入者支持有一个`Shell`的unique指针。
`Shell`是Flutter应用的“中枢神经系统”，包含了多个组件，并继承它们相应的`Delegate`类。
下面看一下创建`Shell`的代码实现：
```C++
// ./shell/common/shell.cc
std::unique_ptr<Shell> Shell::Create(
    const PlatformData& platform_data,
    TaskRunners task_runners,
    Settings settings,
    const Shell::CreateCallback<PlatformView>& on_create_platform_view,
    const Shell::CreateCallback<Rasterizer>& on_create_rasterizer,
    bool is_gpu_disable) {

}

std::unique_ptr<Shell> Shell:CreateShellOnPlatformThread(
    DartVMRef vm,
    fml::RefPtr<fml::RasterThreadMerger> parent_merge,
    std::shared_ptr<ShellIOManager> parent_io_manager,
    const std::shared_ptr<ResourceCacheLimitCalculator>&
        resource_cache_limit_calculator,
    TaskRunners task_runners,
    const PlatformData& platform_data,
    Settings settings,
    fml::RefPtr<const DartSnapshot> isolate_snapshot,
    const Shell::CreateCallback<PlatformView>& on_create_platform_view,
    const Shell::CreateCallback<Rasterizer>& on_create_rasterizer,
    const Shell::EngineCreateCallback& on_create_engine,
    bool is_gpuu_disabled){}
```
从代码中可以看出，在platform线程中创建了`Shell`，之后分别在栅格化线程中创建`Rasterizer`，在platform线程中创建了`PlatformView`，在IO线程中创建`ShellIOManager`，在UI线程中创建了`Engine`，并将这四者设置到`Shell`中去。
`Shell`分别继承了四者的`Delegate`，四者通过相应的`Delegate`将事件传递到`Shell`。
下面分别看一下这四个类
## PlatformView
在Android平台中，真正实现的是`PlatformViewAndroid`类，主要方法实现的功能都是在栅格化线程中对`AndroidSurface`进行操作。看一下`NotifyCreated`方法的实现：
```C++
void PlatformViewAndroid::NotifyCreated(
    fml::RefPtr<AndroidNativeWindow> native_window) {
    if (android_surface_) {
        InstallFirstFrameCallback();

        fml::AutoResetWaitableEvent latch;
        fml::TaskRunner::RunNowOrPostTask(
            surface->SetNativeWindow(native_window);
            latch.Signal();
        )
    }

    PlatformView::NotifyCreated();
}
```
主要实现的功能就是将`native_window`设置到`surface`中，再将这个`surface`通知到`delegate`（也就是`Shell`）中。也就是说，`PlatformView`主要起到一个沟通`Surface`和`Shell`的作用。
## Rasterizer
`Rasterizer`是`Shell`的一个组成部分，运行在GPU线程中。每个`Shell`只能拥有一个`Rasterizer`实例。`Rasterizer`持有一个当前活动的屏幕中显示的绘制`Surface`。`Rasterizer`在这个`Surface`上绘制从`Engine`中提交的layer tree。合成器上下文和屏上绘制`Surface`是`Rasterizer`的主要组成部分：合成器上下文包含绘制帧的所有必要的GPU状态。

`on_create_rasterizer`方法在`AndroidShellHolder`的构造方法中给出：
```C++
Shell::CreateCallback<Rasterizer> on_create_rasterizer = [](Shell& shell) {
    return std::make_unique<Rasterizer>(shell);
  };
```
```C++
Rasterizer::Rasterizer(Delegate& delegate,
                       MakeGpuImageBehavior gpu_image_behavior)
    : delegate_(delegate),
      gpu_image_behavior_(gpu_image_behavior),
      compositor_context_(std::make_unique<flutter::CompositorContext>(*this)),
      user_override_resource_cache_bytes_(false),
      weak_factory_(this) {
  FML_DCHECK(compositor_context_);
}
```
在构造函数中创建了一个`CompositorContext`，这个类会在后续的文章中分析。
## ShellOManager
```C++
class ShellIOManager final : public IOManager {
 public:
  // Convenience methods for platforms to create a GrDirectContext used to
  // supply to the IOManager. The platforms may create the context themselves if
  // they so desire.
  static sk_sp<GrDirectContext> CreateCompatibleResourceLoadingContext(
      GrBackend backend,
      sk_sp<const GrGLInterface> gl_interface);

  ShellIOManager(
      sk_sp<GrDirectContext> resource_context,
      std::shared_ptr<const fml::SyncSwitch> is_gpu_disabled_sync_switch,
      fml::RefPtr<fml::TaskRunner> unref_queue_task_runner,
      std::shared_ptr<impeller::Context> impeller_context,
      fml::TimeDelta unref_queue_drain_delay =
          fml::TimeDelta::FromMilliseconds(8));

  ~ShellIOManager() override;

  // This method should be called when a resource_context first becomes
  // available. It is safe to call multiple times, and will only update
  // the held resource context if it has not already been set.
  void NotifyResourceContextAvailable(sk_sp<GrDirectContext> resource_context);

  // This method should be called if you want to force the IOManager to
  // update its resource context reference. It should not be called
  // if there are any Dart objects that have a reference to the old
  // resource context, but may be called if the Dart VM is restarted.
  void UpdateResourceContext(sk_sp<GrDirectContext> resource_context);

  fml::WeakPtr<ShellIOManager> GetWeakPtr();

  // |IOManager|
  fml::WeakPtr<IOManager> GetWeakIOManager() const override;

  // |IOManager|
  fml::WeakPtr<GrDirectContext> GetResourceContext() const override;

  // |IOManager|
  fml::RefPtr<flutter::SkiaUnrefQueue> GetSkiaUnrefQueue() const override;

  // |IOManager|
  std::shared_ptr<const fml::SyncSwitch> GetIsGpuDisabledSyncSwitch() override;

  // |IOManager|
  std::shared_ptr<impeller::Context> GetImpellerContext() const override;

 private:
  // Resource context management.
  sk_sp<GrDirectContext> resource_context_;
  std::unique_ptr<fml::WeakPtrFactory<GrDirectContext>>
      resource_context_weak_factory_;
  // Unref queue management.
  fml::RefPtr<flutter::SkiaUnrefQueue> unref_queue_;
  std::shared_ptr<const fml::SyncSwitch> is_gpu_disabled_sync_switch_;
  std::shared_ptr<impeller::Context> impeller_context_;
  fml::WeakPtrFactory<ShellIOManager> weak_factory_;

  FML_DISALLOW_COPY_AND_ASSIGN(ShellIOManager);
};
```
`ShellIOManager`继承自`IOManager`类。`IOManager`是管理获取GrContext资源和Skia队列的方法的接口类。这两者都属于图形绘制相关内容，在后续文章中进行分析。
`NotifyResourceContextAvailabel`和`UpdateResourceContext`方法是通知`GrContext`创建和获取的方法。
## Engine
`Engine`类是`Shell`的组成部分之一，运行于UI线程。其主要功能是管理根`Isolate`和它的运行时。每个`Shell`只能拥有一个`Engine`实例。Flutter应用的根`Isolate`会获取窗口绑定。通过这些绑定，一个用可以调度帧、推送layer tree用于渲染、请求解压图片并提交到GPU等等。
`Engine`管理根`Isolate`的全部生命周期。当`Engine`被回收时，其持有者会认为根`Isolate`已经关闭且资源已经被回收。
下面看一下`Engine`的构造方法：
```C++
Engine::Engine(Delegate& delegate,
               const PointerDataDispatcherMaker& dispatcher_maker,
               DartVM& vm,
               fml::RefPtr<const DartSnapshot> isolate_snapshot,
               TaskRunners task_runners,
               const PlatformData& platform_data,
               Settings settings,
               std::unique_ptr<Animator> animator,
               fml::WeakPtr<IOManager> io_manager,
               fml::RefPtr<SkiaUnrefQueue> unref_queue,
               fml::WeakPtr<SnapshotDelegate> snapshot_delegate,
               std::shared_ptr<VolatilePathTracker> volatile_path_tracker)
    : Engine(delegate,
             dispatcher_maker,
             vm.GetConcurrentWorkerTaskRunner(),
             task_runners,
             settings,
             std::move(animator),
             io_manager,
             std::make_shared<FontCollection>(),
             nullptr) {
  runtime_controller_ = std::make_unique<RuntimeController>(
      *this,                                 // runtime delegate
      &vm,                                   // VM
      std::move(isolate_snapshot),           // isolate snapshot
      settings_.idle_notification_callback,  // idle notification callback
      platform_data,                         // platform data
      settings_.isolate_create_callback,     // isolate create callback
      settings_.isolate_shutdown_callback,   // isolate shutdown callback
      settings_.persistent_isolate_data,     // persistent isolate data
      UIDartState::Context{
          task_runners_,                           // task runners
          std::move(snapshot_delegate),            // snapshot delegate
          std::move(io_manager),                   // io manager
          std::move(unref_queue),                  // Skia unref queue
          image_decoder_->GetWeakPtr(),            // image decoder
          image_generator_registry_.GetWeakPtr(),  // image generator registry
          settings_.advisory_script_uri,           // advisory script uri
          settings_.advisory_script_entrypoint,    // advisory script entrypoint
          std::move(volatile_path_tracker),        // volatile path tracker
      });
}
```
当`Engine`创建时，会立刻创建一个`RuntimeController`，在`RuntimeController`中又会立刻创建一个`DartIsolate`。
# 启动流程
启动流程从`FlutterActivity`的`onStart`方法开始分析。
先看`onStart`方法代码：
```Java

  @Override
  protected void onStart() {
    super.onStart();
    lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_START);
    if (stillAttachedForEvent("onStart")) {
      delegate.onStart();
    }
  }
```
该方法先将生命周期调整为`ON_START`状态，然后调用`FlutterActivityAndFragmentDelegate`的`onStart`方法。
下面是`FlutterActivityAndFragmentDelegate`的`onStart`方法：
```Java
void onStart() {
    Log.v(TAG, "onStart()");
    ensureAlive();
    doInitialFlutterViewRun();
    // This is a workaround for a bug on some OnePlus phones. The visibility of the application
    // window is still true after locking the screen on some OnePlus phones, and shows a black
    // screen when unlocked. We can work around this by changing the visibility of FlutterView in
    // onStart and onStop.
    // See https://github.com/flutter/flutter/issues/93276
    if (previousVisibility != null) {
      flutterView.setVisibility(previousVisibility);
    }
  }
```
这里先做了活动确认，然后启动`FlutterView`内的Dart。
```Java
private void doInitialFlutterViewRun() {
    // Don't attempt to start a FlutterEngine if we're using a cached FlutterEngine.
    if (host.getCachedEngineId() != null) {
      return;
    }

    if (flutterEngine.getDartExecutor().isExecutingDart()) {
      // No warning is logged because this situation will happen on every config
      // change if the developer does not choose to retain the Fragment instance.
      // So this is expected behavior in many cases.
      return;
    }
    String initialRoute = host.getInitialRoute();
    if (initialRoute == null) {
      initialRoute = maybeGetInitialRouteFromIntent(host.getActivity().getIntent());
      if (initialRoute == null) {
        initialRoute = DEFAULT_INITIAL_ROUTE;
      }
    }
    @Nullable String libraryUri = host.getDartEntrypointLibraryUri();
    Log.v(
        TAG,
        "Executing Dart entrypoint: "
                    + host.getDartEntrypointFunctionName()
                    + ", library uri: "
                    + libraryUri
                == null
            ? "\"\""
            : libraryUri + ", and sending initial route: " + initialRoute);

    // The engine needs to receive the Flutter app's initial route before executing any
    // Dart code to ensure that the initial route arrives in time to be applied.
    flutterEngine.getNavigationChannel().setInitialRoute(initialRoute);

    String appBundlePathOverride = host.getAppBundlePath();
    if (appBundlePathOverride == null || appBundlePathOverride.isEmpty()) {
      appBundlePathOverride = FlutterInjector.instance().flutterLoader().findAppBundlePath();
    }

    // Configure the Dart entrypoint and execute it.
    DartExecutor.DartEntrypoint entrypoint =
        libraryUri == null
            ? new DartExecutor.DartEntrypoint(
                appBundlePathOverride, host.getDartEntrypointFunctionName())
            : new DartExecutor.DartEntrypoint(
                appBundlePathOverride, libraryUri, host.getDartEntrypointFunctionName());
    flutterEngine.getDartExecutor().executeDartEntrypoint(entrypoint, host.getDartEntrypointArgs());
  }
```
因为在指定的`FlutterView`中不支持重载或者重启Dart，所以需要先对`DartExecutor`判断是否正在执行代码。
之后通过`FlutterActivity`中获取应用包路径和Dart入口方法，创建`DartEntryPoint`。根据这个`DartEntryPoint`执行Dart代码。
`DartExecutor`在`FlutterEngine`创建的时候创建出来，并在`dartExecutor.onAttachToJNI`方法中，将`DartMessage`设置到`FlutterJNI`中。
```Java
// DartExecutor.java
public void executeDartEntrypoint(
      @NonNull DartEntrypoint dartEntrypoint, @Nullable List<String> dartEntrypointArgs) {
    if (isApplicationRunning) {
      Log.w(TAG, "Attempted to run a DartExecutor that is already running.");
      return;
    }

    TraceSection.begin("DartExecutor#executeDartEntrypoint");
    try {
      Log.v(TAG, "Executing Dart entrypoint: " + dartEntrypoint);
      flutterJNI.runBundleAndSnapshotFromLibrary(
          dartEntrypoint.pathToBundle,
          dartEntrypoint.dartEntrypointFunctionName,
          dartEntrypoint.dartEntrypointLibrary,
          assetManager,
          dartEntrypointArgs);

      isApplicationRunning = true;
    } finally {
      TraceSection.end();
    }
  }
```
继续调用`FlutterJNI`的`runBundleAndSnapshotFromLibrary`方法。
`FlutterJNI`对线程和本地绑定确认后，调用native方法`nativeRunBundleAndSnapshotFromLibrary`。经过`AndroidShellHolder`的`Launch`调用后，最终会调用到`Shell`的`RunEngine`方法。
下面看一下`RunEngine`方法。
```C++
void Shell::RunEngine(
    RunConfiguration run_configuration,
    const std::function<void(Engine::RunStatus)>& result_callback) {
  auto result = [platform_runner = task_runners_.GetPlatformTaskRunner(),
                 result_callback](Engine::RunStatus run_result) {
    if (!result_callback) {
      return;
    }
    platform_runner->PostTask(
        [result_callback, run_result]() { result_callback(run_result); });
  };
  FML_DCHECK(is_setup_);
  FML_DCHECK(task_runners_.GetPlatformTaskRunner()->RunsTasksOnCurrentThread());

  fml::TaskRunner::RunNowOrPostTask(
      task_runners_.GetUITaskRunner(),
      fml::MakeCopyable(
          [run_configuration = std::move(run_configuration),
           weak_engine = weak_engine_, result]() mutable {
            if (!weak_engine) {
              FML_LOG(ERROR)
                  << "Could not launch engine with configuration - no engine.";
              result(Engine::RunStatus::Failure);
              return;
            }
            auto run_result = weak_engine->Run(std::move(run_configuration));
            if (run_result == flutter::Engine::RunStatus::Failure) {
              FML_LOG(ERROR) << "Could not launch engine with configuration.";
            }
            result(run_result);
          }));
}
```
前面提到，`Engine`创建、运行都在UI线程中的。所以此处`Engine`执行Dart代码需要在UI线程中执行。
```C++
Engine::RunStatus Engine::Run(RunConfiguration configuration) {
  if (!configuration.IsValid()) {
    FML_LOG(ERROR) << "Engine run configuration was invalid.";
    return RunStatus::Failure;
  }

  last_entry_point_ = configuration.GetEntrypoint();
  last_entry_point_library_ = configuration.GetEntrypointLibrary();
#if (FLUTTER_RUNTIME_MODE == FLUTTER_RUNTIME_MODE_DEBUG)
  // This is only used to support restart.
  last_entry_point_args_ = configuration.GetEntrypointArgs();
#endif

  UpdateAssetManager(configuration.GetAssetManager());

  if (runtime_controller_->IsRootIsolateRunning()) {
    return RunStatus::FailureAlreadyRunning;
  }

  // If the embedding prefetched the default font manager, then set up the
  // font manager later in the engine launch process.  This makes it less
  // likely that the setup will need to wait for the prefetch to complete.
  auto root_isolate_create_callback = [&]() {
    if (settings_.prefetched_default_font_manager) {
      SetupDefaultFontManager();
    }
  };

  if (!runtime_controller_->LaunchRootIsolate(
          settings_,                                 //
          root_isolate_create_callback,              //
          configuration.GetEntrypoint(),             //
          configuration.GetEntrypointLibrary(),      //
          configuration.GetEntrypointArgs(),         //
          configuration.TakeIsolateConfiguration())  //
  ) {
    return RunStatus::Failure;
  }

  auto service_id = runtime_controller_->GetRootIsolateServiceID();
  if (service_id.has_value()) {
    std::unique_ptr<PlatformMessage> service_id_message =
        std::make_unique<flutter::PlatformMessage>(
            kIsolateChannel, MakeMapping(service_id.value()), nullptr);
    HandlePlatformMessage(std::move(service_id_message));
  }

  return Engine::RunStatus::Success;
}
```
`CreateRunningRootIsolate`
```C++

bool DartIsolate::RunFromLibrary(std::optional<std::string> library_name,
                                 std::optional<std::string> entrypoint,
                                 const std::vector<std::string>& args) {
  TRACE_EVENT0("flutter", "DartIsolate::RunFromLibrary");
  if (phase_ != Phase::Ready) {
    return false;
  }

  tonic::DartState::Scope scope(this);

  auto library_handle =
      library_name.has_value() && !library_name.value().empty()
          ? ::Dart_LookupLibrary(tonic::ToDart(library_name.value().c_str()))
          : ::Dart_RootLibrary();
  auto entrypoint_handle = entrypoint.has_value() && !entrypoint.value().empty()
                               ? tonic::ToDart(entrypoint.value().c_str())
                               : tonic::ToDart("main");

  if (!FindAndInvokeDartPluginRegistrant()) {
    // TODO(gaaclarke): Remove once the framework PR lands that uses `--source`
    // to compile the Dart Plugin Registrant
    // (https://github.com/flutter/flutter/pull/100572).
    InvokeDartPluginRegistrantIfAvailable(library_handle);
  }

  auto user_entrypoint_function =
      ::Dart_GetField(library_handle, entrypoint_handle);

  auto entrypoint_args = tonic::ToDart(args);

  if (!InvokeMainEntrypoint(user_entrypoint_function, entrypoint_args)) {
    return false;
  }

  phase_ = Phase::Running;

  return true;
}
```