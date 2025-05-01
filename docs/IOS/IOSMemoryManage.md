---
title: iOS内存管理机制
date: 2022-01-13 14:57:00
tags:
- Engine
- Unreal
- IOS
categories:
- Engine
- Unreal
- IOS
---
https://www.cnblogs.com/kekec/p/14611999.html
与其他的操作系统一样，iOS系统也会对虚拟内存和物理内存进行分页，虚拟内存到物理内存的映射都是以页（Page）为最小粒度的。
A7以前处理器的系统，物理内存和虚拟内存按照4KB分页。A7和A8处理器的内存，物理内存按照4KB分页，虚拟内存按照16B分页。A9处理器的系统，物理和虚拟内存都是以16KB进行分页。
UE4的log打印的页面大小以16KB:
```
[2021.04.10-11.50.39:503][  0]LogInit: |IOSPlatformMisc.cpp:89|Computer: kekechenbuteki-iPhone
[2021.04.10-11.50.39:503][  0]LogInit: |IOSPlatformMisc.cpp:90|User: GenericUser
[2021.04.10-11.50.39:503][  0]LogInit: |IOSPlatformMisc.cpp:94|CPU Page size=16384, Cores=6
[2021.04.10-11.50.39:503][  0]LogInit: |IOSPlatformMisc.cpp:97|High frequency timer resolution =24.000000 MHz
[2021.04.10-11.50.39:503][  0]LogInit: |IOSPlatformMisc.cpp:99|Free Memory at startup: 1950 MB
[2021.04.10-11.50.39:504][  0]LogInit: |ApplePlatformMemory.cpp:277|Memory total: Physical=2.0GB (3GB approx) Pagefile=0.0GB Virtual=2.0GB
[2021.04.10-11.50.39:506][  0]LogInit: |PhysLevel.cpp:292|Physics initialised using underlying interface: PhysX
[2021.04.10-11.50.39:507][  0]LogInit: |TextLocalizationManager.cpp:204|Overriding language with game language configuration option (zh-Hans).
[2021.04.10-11.50.39:507][  0]LogInit: |TextLocalizationManager.cpp:204|Overriding language with game locale configuration option (zh-Hans).
```
系统将内存页分为3种状态：
活跃内存页（active pages）：这种内存页已经映射到物理内存中，而且近期被访问过，处于活跃状态。
非活跃内存页（inactive pages）：这种内存页已经被映射到物理内存中，但是近期没有被访问过。
可用的内存页（free pages）：没有关联到虚拟内存页的物理内存页几何。即违背使用的物理内存页。

**Jetsam**是MacOS和iOS共有的一套内存管理机制。可用的内存页降低到一定阈值时，Jetsam就会采取低内存应对措施。
在iOS中，则会触发**Memory Warning**，如果App没有处理低内存警告并且还在占用太多内存，然后则有可能被杀掉。
UE4实现的applicationDidReceiveMemoryWarning如下：
```C++
/* UnrealEngine\Engine\Source\Runtime\ApplicationCore\Private\IOS\IOSAppDelegate.cpp */
- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    /*
    Tells the delegate when the application receives a memory warning from the system
    */
    FPlatformMisc::HandleLowMemoryWarning();
}
```
在stackoverflow上，有人对单个app能够使用的最大内存做了统计：[iOS app max memory budget](https://stackoverflow.com/questions/5887248/ios-app-maximum-memory-budget/15200855#15200855)
```
device: (crash amount/total amount/percentage of total)

iPad1: 127MB/256MB/49%
iPad2: 275MB/512MB/53%
iPad3: 645MB/1024MB/62%
iPad4: 585MB/1024MB/57% (iOS 8.1)
iPad Mini 1st Generation: 297MB/512MB/58%
iPad Mini retina: 696MB/1024MB/68% (iOS 7.1)
iPad Air: 697MB/1024MB/68%
iPad Air 2: 1383MB/2048MB/68% (iOS 10.2.1)
iPad Pro 9.7": 1395MB/1971MB/71% (iOS 10.0.2 (14A456))
iPad Pro 10.5”: 3057/4000/76% (iOS 11 beta4)
iPad Pro 12.9” (2015): 3058/3999/76% (iOS 11.2.1)
iPad Pro 12.9” (2017): 3057/3974/77% (iOS 11 beta4)
iPad Pro 11.0” (2018): 2858/3769/76% (iOS 12.1)
iPad Pro 12.9” (2018, 1TB): 4598/5650/81% (iOS 12.1)
iPad 10.2: 1844/2998/62% (iOS 13.2.3)
iPod touch 4th gen: 130MB/256MB/51% (iOS 6.1.1)
iPod touch 5th gen: 286MB/512MB/56% (iOS 7.0)
iPhone4: 325MB/512MB/63%
iPhone4s: 286MB/512MB/56%
iPhone5: 645MB/1024MB/62%
iPhone5s: 646MB/1024MB/63%
iPhone6: 645MB/1024MB/62% (iOS 8.x)
iPhone6+: 645MB/1024MB/62% (iOS 8.x)
iPhone6s: 1396MB/2048MB/68% (iOS 9.2)
iPhone6s+: 1392MB/2048MB/68% (iOS 10.2.1)
iPhoneSE: 1395MB/2048MB/69% (iOS 9.3)
iPhone7: 1395MB/2048MB/68% (iOS 10.2)
iPhone7+: 2040MB/3072MB/66% (iOS 10.2.1)
iPhone8: 1364MB/1990MB/70% (iOS 12.1)
iPhone X: 1392MB/2785MB/50% (iOS 11.2.1)
iPhone XS: 2040MB/3754MB/54% (iOS 12.1)
iPhone XS Max: 2039MB/3735MB/55% (iOS 12.1)
iPhone XR: 1792MB/2813MB/63% (iOS 12.1)
iPhone 11: 2068MB/3844MB/54% (iOS 13.1.3)
iPhone 11 Pro Max: 2067MB/3740MB/55% (iOS 13.2.3)
```
# Clean memory & dirty memory & Swapped memory
