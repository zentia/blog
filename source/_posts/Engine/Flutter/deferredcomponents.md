---
title: 关于Flutter动态化的新姿势
categories:
 - Flutter
---
# 背景

Flutter的release产物会生成libapp.so以及放入assets的资源，包含了所有业务代码及所用资源。而随着业务越来越多，产物也越来越大。

某业务如果要下发，需要整体更新，牵一发而动全身，流量消耗也很客观。这时自然会产生一个想法，各业务能否独立生成产物，在用到时才下载运行。

而在Flutter的官方git上，已有不少的issue提出了这个问题，比如：

- https://github.com/flutter/flutter/issues/53672
- https://github.com/flutter/flutter/issues/16813
- https://github.com/flutter/flutter/issues/62229
- https://github.com/flutter/flutter/issues/75079
- ....

所以在https://github.com/flutter/flutter/issues/57617 这个issue，官方终于开始支持此特性，并命名为deferred components，并在这个issue同步进展。

可以看到，3月份代码已经合入master，并在gallery的demo演示了此能力，但是文档迟迟没有出来，对于磨刀霍霍的我们已经等不及直接分析代码了。

# 分析

支持deferred components，其实包含了工程结构，构建工具，底层支持等等各个方面，我们尽可能看一下都是如何实现的。既然gallery已经有了demo，那我们就从gallery开始。

# Flutter Gallery简介

- 项目地址： https://github.com/flutter/gallery.git


Flutter Gallery包含的内容：

- Material Design Component: Android Style Component
- Cupertino Component: iOS Style Component
- Behavior Controller Feature
- Some Small Feature

Flutter支持的平台：目前Flutter支持Android iOS Web Macos Linux Windows

使用Android Studio打开。


deferred components只在能非debug版本开启，而且只支持aab产物，同时crane模块使用了dymanic-feature，还需要bundle-tools，所以需要如下编译：
1. `flutter build appbundle`，生成aap.aab
2. `java -jar bundletool-all-1.5.0.jar build-apks --connected-device --bundle=app.aab --output==app.apks`，生成app.apks
3. `java -jar bundletool-all-1.5.0.jar install-apks --apks=app.apks`，安装

## demo工程分析
gallery的pubspec.yaml直接就可以看到`deferred-components`的定义，不难看出crane模块做了延迟加载
```
flutter:
  deferred-components:
    - name: crane
      libraries:
        # Only one library from the loading unit is necessary.
        - package:gallery/studies/crane/app.dart
      assets:
        - packages/flutter_gallery_assets/crane/destinations/eat_1.jpg
        - packages/flutter_gallery_assets/crane/destinations/eat_2.jpg
```
crane的调用在`libs/routes.dart`里。
```dart
Path(
      r'^' + crane_routes.defaultRoute,
      (context, match) => StudyWrapper(
        study: DeferredWidget(crane.loadLibrary,
            () => crane.CraneApp(), // ignore: prefer_const_constructors
            placeholder: const DeferredLoadingPlaceholder(name: 'Crane')),
      ),
      openInSecondScreen: true,
    ),
```

crane的定义看import,
```dart
import 'package:gallery/studies/crane/app.dart' deferred as crane;
import 'package:gallery/studies/crane/routes.dart' as crane_routes;
import 'package:gallery/studies/fortnightly/app.dart' deferred as fortnightly;
import 'package:gallery/studies/fortnightly/routes.dart' as fortnightly_routes;
import 'package:gallery/studies/rally/app.dart' deferred as rally;
import 'package:gallery/studies/rally/routes.dart' as rally_routes;
import 'package:gallery/studies/reply/app.dart' as reply;
import 'package:gallery/studies/reply/routes.dart' as reply_routes;
```
使用了deferred关键字，也看到不只有crane使用了关键字，对于deferred关键字要特别注意下，在dart doc的解释：
>Deferred loading (also called lazy loading) allows a web app to load a library on demand, if and when the library is needed. Here are some cases when you might use deferred loading
>Only dart2js supports deferred loading. Flutter, the Dart VM, and dartdevc don’t support deferred loading. For more information, see [issue #33118](https://github.com/dart-lang/sdk/issues/33118) and [issue #27776](https://github.com/dart-lang/sdk/issues/27776).

文档写了deferred只适用于web，在其他平台是忽略的，但是galery的代码明显不是这种情况，所以应该是deferred components对该关键字做了扩展，但是官方文档还没有更新。从工程目录上可以看到了deferred关键字的模块都有独立的目录。

deferred as添加了loadLibrary方法，是一个Future，就是用来延迟加载产物的。DeferredWidget是用来占位的，在loadLibrary没返回前显示一个loading，返回后就创建了真正的widget显示。

再来看android代码，crane成了独立的module，从build.gradle看到使用了dynamic feature，并添加了两个目录到src
![](deferredcomponents1.png)
crane的AndroidManifest.xml启用了dynamic feature的onDemand:
```xml
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:dist="http://schemas.android.com/apk/distribution"
    package="com.example.gallery.crane">

    <dist:module
        dist:instant="false"
        dist:title="@string/craneName">
        <dist:delivery>
            <dist:on-demand />
        </dist:delivery>
        <dist:fusing dist:include="true" />
    </dist:module>
</manifest>

```
而app的AndroidManifest.xml有两个地方不同寻常，一是application使用的是`FlutterPlayStoreSplitApplication`，而是多个了`meta-data`：
```xml
<application
    android:label="Flutter Gallery"
    android:name="io.flutter.app.FlutterPlayStoreSplitApplication"
    android:icon="@mipmap/ic_launcher">
	......
	<meta-data android:name="io.flutter.embedding.engine.deferredcomponents.DeferredComponentManager.loadingUnitMapping" android:value="2:crane,3:,4:,5:,6:,7:,8:,9:,10:,11:"/>
</application>
```
直接看下`FlutterPlayStoreSplitApplication`，在onCreate里创建了`PlayStoreDeferredComponentManager`，名字上就可以看出来适用于延迟加载的，注释里也说明了用途，实现的是从Google Play下载dynamic module的延迟加载：
```java
/**
 * Flutter default implementation of DeferredComponentManager that downloads deferred component from
 * the Google Play store as a dynamic feature module.
 */
```
虽然国内用不了Google Play，但这个实现方式对我们理解deferred components还是很有帮助的。
构造方法调用了`initLoadingUnitMappingToComponentNames`，我们来看一下，
```java

```

然后看`installDeferredComponent`这个重要的方法，
```dart

```