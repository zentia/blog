---
title: 通过USB访问iPhone手机文件
date: 2022-01-02 17:05:00
tags:
    - Unreal
---
# 安装libimobiledevice
```
brew update
brew install libimobiledevice
# libimobiledevice中并不包含ipa的安装命令，所以还需安装
brew install ideviceinstaller
```
# 常用功能
## 获取设备已安装app的bundleID
```
ideviceinstaller -l
```
## 安装ipa包，卸载应用
```
ideviceinstaller -i xxx.ipa
```
命令卸载应用，需要知道此应用的bundleID
```
ideviceinstaller -U [bundleID]
```

如果连接了多部手机需要分别安装时，请使用UUID指定：ideviceinstaller -u uuid -i *.ipa

查看系统日志
```
idevicesyslog
```
查看当前已连接设备的UUID
```
idevice_id --list
```

截图
```
idevicescreenshot
```
查看设备信息
```
ideviceinfo
```
获取设备时间
```
idevicedate
```
设置代理
```
iproxy
    usage: iproxy LOCAL_TCP_POPRT DEVICE_TCP_PORT [UDID]
```
获取设备名称
```
idevicename
```
查看和描述设备的描述文件
```
ideviceprovision list
```
# 挂载文件系统工具:ifuse
## 安装方式
```
brew install osxfuse
```
官网安装osxfuse
https://osxfuse.github.io