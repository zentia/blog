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