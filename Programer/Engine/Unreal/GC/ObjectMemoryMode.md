# Unreal GC 
Unreal 为我们搭建了一套UObject对象系统，并且加入了垃圾回收机制，使我们使用C++进行游戏开发时更加方便，而且游戏本身也可以极大程度的避免了内存泄漏问题。

## GC何时进行
### 主动引发
可以在执行一些操作时手动调用GC，比如卸载一个资源后，立即调用一次GC进行清理。

而且方式有多种，游戏中可以调用`ForceGarbageCollection`来让World下次Tick时进行垃圾回收。
```C++
if (GEngine)
{
    GEngine->ForceGarbageCollection();
}
```
也可以直接调用`CollectGarbage`进行垃圾回收，引擎中大部分情况都用这种方式主动引发。

### 自动引发


# 防止GC的办法
1. `UPROPERTY()`用法（针对UObject系列对象，不能使用TSharedPtr）
   ```C++
   UPROPERTY()
   UObject* MyObj;
   ```
2. AddToRoot和RemoveFromRoot用法（针对于在FObject中引用UObject指针对象）
   ```C++
   // 必须成对出现
   // 初始化函数
   UMyObject* MyObj = NewObject<UMyObject>();
   MyObj.AddToRoot();

   // 析构函数
   MyObj->RemoveFromRoot()
   MyObj = nullptr; 
   ```

3. FObject系列（使用TSharePtr智能指针管理）
    ```C++
    TSharePtr<FObject> ObjPtr = MakeSharable(new FObject())
    ```
4. 在FObject中只是临时使用UObject指针对象

    ```C++
    TWeakPtr<Class UObject> Obj*; // 使用弱指针来保存UObject指针，不负责管理
    ```

5. FStreamableManager 用法

    ```C++
    FSoftObjectPath AssetPath(TEXT("/Game/Mannequin/Animations/ThirdPersonWalk.ThirdPersonWalk"));
    FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
    // hold object in memory.TSharedPtr<FStreamableHandle> Handle = AssetLoader.RequestSyncLoad(AssetPath, true);
    UObject* Obj = Handle->GetLoadedAsset();
    //free memory of object.Handle->ReleaseHandle();
    ```

6. FGCObjectScopeGuard 用法

    ```C++
    FGCObjectScopeGuard(UObject* GladOS = NewObject<...>(...));
    ```