---
title: Unity 文件路径
mathjax: true
date: 2019-03-22 15:54:30
tags:
    - Unity
categories:
- Engine
- Unity
---

1. Resources
   Resources文件夹是Unity里自动识别的一种文件夹，可在Unity编辑器的Project窗口创建，并将资源放置在里面。Resources文件夹下的资源不管是否有用，全部会打包进.apk或者.ipa，并且打包时会将里面的资源压缩处理。加载方法是Resources.Load<T>(文件名)，需要注意：文件名不包括扩展名，打包后不能更改Resources下的资源内容，但是从Resources文件夹中加载出来的资源可以更改。
2. Application.dataPath
   这个属性返回的是程序的数据文件所在的文件夹，例如在Editor中就是项目的Assets文件夹的路径，通过这个路径可以访问项目中任何文件夹中的资源，但是在移动端它是完全没用。
3. Application.streamingAssetsPath
   这个属性用于返回流数据的缓存目录，返回路径为相对路径，适合设置一些外部数据文件的路径。在Unity工程的Assets目录下起一个名为"StreamingAssets"的文件夹即可，然后用Application.streamingAssetsPath访问，这个文件夹中的资源在打包时会原封不动的打包进去，不会压缩，一般放置一些资源数据。在PC/MAC中可实现对文件的“增删改查”等操作，但在移动端是一个只读路径。


|||IOS|Android|Windows|Mac|
|--|-------|--|--|--|--|
|Application.dataPath|只读不写|Application/xxxxxx/xxx.app/Data|/data/app/xxx.xxx.xxx.apk|/Assets|Assets|
|Application.streamAssetsPath|只读不写，首包资源目录|Application/xxxxxx/xxx.app/Data/Raw|jar:file:///data/app/xxx.xxx.xxx.apk/!/assets|Assets/StreamingAssets|Assets/StreamingAssets|
|Application.peristentDataPath|可读可写，更新资源包目录|Application/xxxxxx/xxx.app/Documents|/data/data/xxx.xxx.xxx/files|C:/Users/Admin/AppData/LocalLow/CompanyName/ProductName|/Users/xxxx/Library/Caches/CompanyName/ProductName|
|Application.temproaryCachPath||Application/xxxxxx/xxx.app/Library/Caches|/data/data/xxx.xxx.xxx/cache|C:/Users/Admin/AppData/Local/Temp/CompanyName/ProductName|/var/folders/57/6b4_9w8113x2fsmzx_yhrhvh0000gn/T/CompanyName/Product|
