---
title: AssetRegistry分析
categories:
- Engine
- Unreal
---
https://zhuanlan.zhihu.com/p/76964514

## FAssetData

- FName PackageName: 这个asset所在Package的名称，形如/Game/Path/Package

最近接了一个需求，有点恶心，要重定向依赖。
从上述文章我们已知，资产的依赖其实不一定会加载的。数据之间的存储都是通过形如FAssetData FDenpendNode FAssetIdentifier之类的结构完成的。
但是这些数据结构是如何拼装的，笔者目前还是摸不着头脑，我们带着这个问题，来开启此次讨论。
在此记录一下追踪过程
现在我们找到
```C++
// AssetDataGatherer.cpp 
// 这个类是一个多线程类

uint32 FAssetDataGatherer::Run()
{
    while (true)
    {
        bool LocalIsDiscoveringFiles = false;

        {
            // 原子锁
            FScopeLock CritSectionLock(&WorkerThreadCriticalSection);

            AssetResults.Append(MoveTemp(LocklAssetResults));

        }

        if (LocalFilesToSearch.Num() > 0)
        {
            for (const FDiscoveredPackageFile& AssetFileData : LocalFilesToSearch)
            {
                if (StopTaskCounter.GetValue() != 0)
                {
                    // We have been asked to stop, so don't read any more files
                    break;
                }
                if (bLoadAndSaveCache)
                {
                    FDiskCachedAssetData* DiskCachedAssetData = DiskCachedAssetDataMap.Find(PackageName);
                }
                if (bGatherDepensData)
                {
                    LocalDependencyResults.Add(DiskCachedAssetData->DependencyData);
                }
            }
        }
    }
}

// 获取和裁剪搜索结果 848
bool FAssetDataGatherer::GetAndTrimSearchResults(TBackgroundGatherResults<FAssetData*>& OutAssetResults, )
{
    // 原子操作
    FScopeLock CritSectionLock(&WorkerThreadCriticalSection);

    // 收集结果
    OutAssetResults.Append(MoveTemp(AssetResults));
    // 重置应该是要丢缓冲池里面，暂时不管
    AssetResults.Reset();
}


```
