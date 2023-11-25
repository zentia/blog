---
title: PackageContent
date: 2022-01-03 20:04:00
tags:
- Unreal
---
D:\WindowsNoEditor>tree /a /f
|    Blank425.exe
|
+---Engine
|   +---Binaries
|   |   \---ThirdParty
|
\---Blank425
    +---Binaries
    |   \---Win64
    |           Blank425.exe
    |
    \---Content
        \--Paks
                Blank425-WindowsNoEditor.pak

安卓不能直接看到pak文件，而是通过obb压缩包需要解压缩才可以看到
D:\Android_ASTC>tree /a /f
|   AndroidManifest.xml
|
+---assets
|   |   ···
|   |   main.obb.png
|   |   ···
|   |
|   \--beaconcomp
|           comp_list
|           libBeacon.so.arm64-v8a
|           libBeacon.so.armeabi
+---lib
|   \---armeabi-v7a
|           ···
|           libUE4.so
|           ···
+---original
|   |   AndroidManifest.xml
|   |
|   \---META-INF
|           ···
+---res
|   +---anim
|   |       ··· 
|
\---unknown
        ---

D:\iOS>tree /a /f
|   ···
|   Blank425
|   Info.plist
|   LaunchScreenIOS.png
|   ···
|   ue4commandline.txt
|
+---cookeddata
|   \---blank425
|       \---content
|           +---movies
|           |       ···
|           |
|           \--paks
|                   blank425-ios.pak
|
+---Engine
|   \---Content
|       \---SlateDebug
|           \---Fonts
|                   LastResort.ttf
|
+---LaunchScreen.storyboardc
|   |   ···
|
+---MidasIAPSDK.bundle
|   |   ···
|
+---MSDKAppSetting.bundle
|   |   ···
|
+---MSDKWebViewResource.bundle
|   |   ···
|
\---_CodeSignature
        CodeResources
