---
title: Subsystem
date: 2022-01-18 19:56:00
categories:
- Unreal
---

# 引擎

```C++
class UMyEngineSubsystem : public UEngineSubsystem {};
```

引擎子系统的模块加载时，子系统将在模块的`Startup()`函数执行返回后执行`Initialize()`。子系统将在模块的`Shutdown()`函数返回后执行`Deinitialize()`。

这些子系统将通过GEngine进行访问：
```C++
UMyEngineSubsystem MySubsystem = GEngine->GetEngineSubsystem();
```

# 编辑器

```C++
class UMyEditorSubsystem : public UEditorSubsystem {};
```

编辑器子系统的模块加载时，子系统将在模块的`Startup()`函数返回后执行`Initialize()`。子系统将在模块的`Shutdown()`函数返回后执行`Deinitialize()`。

```C++
UMyEditorSubsystem MySubsystem = GEditor->GetEditorSubsystem();
```

# GameInstance

# LocalPlayer

# 来自python的子系统
如果使用Python来进行编辑器脚本编写，则可使用内置存取器在子系统中进行获取：
```python
my_engine_subsystem = unreal.get_engine_subsystem(unreal.MyEngineSubsystem)
my_editor_subsystem = unreal.get_editor_subsystem(unreal.MyEditorSubsystem)
```