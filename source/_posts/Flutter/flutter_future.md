---
title: 深入理解Flutter异步Future机制
date: 2022-11-12 31:37:00
tags: Flutter
categories:
    - Flutter
---
# 概述
Flutter框架层才用dart语言，在Dart中随处可见的异步代码，有大量的库函数返回的是Future对象，dart本身是单线程执行模型，dart应用在其主isolate执行应用的main()方法时开始运行，当main()执行完成后，住isolate所在线程在逐个处理队列中的任务，包括但不限于通过Future所创建的任务，但整个过程都运行在同一个单线程。
```dart
new Future(()=>print("Hello World"));
```
这行代码中print操作便是运行在UI线程，需要注意的是如果这是一行耗时操作，则要使用独立的isolate或者worker来并行，已保证UI线程的及时响应。
### InitDartInternal
[-> flutter/lib/ui/dart_runtime_hooks.cc]
```c++
static void InitDartInternal(Dart_Handle builtin_library, bool is_ui_isolate) {
  Dart_Handle print = InvokeFunction(builtin_library, "_getPrintClosure");

  Dart_Handle internal_library = Dart_LookupLibrary(ToDart("dart:_internal"));

  Dart_Handle result =
      Dart_SetField(internal_library, ToDart("_printClosure"), print);
  PropagateIfError(result);

  if (is_ui_isolate) {
    // Call |_setupHooks| to configure |VMLibraryHooks|.
    Dart_Handle method_name = Dart_NewStringFromCString("_setupHooks");
    result = Dart_Invoke(builtin_library, method_name, 0, NULL);
    PropagateIfError(result);
  }

  Dart_Handle setup_hooks = Dart_NewStringFromCString("_setupHooks");

  Dart_Handle io_lib = Dart_LookupLibrary(ToDart("dart:io"));
  result = Dart_Invoke(io_lib, setup_hooks, 0, NULL);
  PropagateIfError(result);

  Dart_Handle isolate_lib = Dart_LookupLibrary(ToDart("dart:isolate"));
  result = Dart_Invoke(isolate_lib, setup_hooks, 0, NULL);
  PropagateIfError(result);
}
```
### SendPort::New
`third_party/dart/runtime/vm/object.cc`

```dart
SendPortPtr SendPort::New(Dart_Port id,
                          Dart_Port origin_id,
                          Heap::Space space) {
  ASSERT(id != ILLEGAL_PORT);
  SendPort& result = SendPort::Handle();
  {
    ObjectPtr raw =
        Object::Allocate(SendPort::kClassId, SendPort::InstanceSize(), space,
                         SendPort::ContainsCompressedPointers());
    NoSafepointScope no_safepoint;
    result ^= raw;
    result.StoreNonPointer(&result.untag()->id_, id);
    result.StoreNonPointer(&result.untag()->origin_id_, origin_id);
  }
  return result.ptr();
}
```
