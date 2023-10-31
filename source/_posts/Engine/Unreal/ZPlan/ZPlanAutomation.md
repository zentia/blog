---
title: Unreal自动化工具
date: 2022-01-04 14:44:00
password: zplan
categories: 
- Engine
- Unreal
- ZPlan
tags: 
- Engine
- Unreal
- private
---
# 目的
自动化工具是为了检测游戏性能而存在的。

https://docs.unrealengine.com/4.27/zh-CN/TestingAndOptimization/

# 设计
## 核心逻辑
游戏业务接入之后自动测试性能。
## 语言
python C++
## 功能点
### 场景
1. 事前事后扫描场景Shader复杂度并提出切换其他材质类型的性能对别，顶点三角形数量，LOD，光照图大小。

### 角色
1. 事前事后扫描角色顶点三角形数，材质复杂度，LOD。

### 动画
1. 事前事后扫描LevelSequence是否规范。

### 纹理
1. 事前事后扫描Texture纹理是否适合。
2. 检查依赖，并提供删除备份逻辑。
3. 检查贴图中无用的透明区域

### Wwise
https://blog.uwa4d.com/archives/TechSharing_268.html

### 语言
1. TS/C++静态扫描，C++扫描潜在

### 美术资源

### 自动化
1. 通过自动跑游戏逻辑的方式去检测游戏性能，对Game Draw RHI线程跑出高于10ms或内存高于指定大小的地方，发出性能告警，并收集统计和上报日志。 

# Automation Framework
![1](1.png)
![1](2.png)
![1](/images/Unreal/Automation/3.png)
![1](/images/Unreal/Automation/4.png)
![1](/images/Unreal/Automation/5.png)
![1](/images/Unreal/Automation/6.png)
![1](/images/Unreal/Automation/7.png)
![1](/images/Unreal/Automation/8.png)
每个开发人员通过全套测试来检查他们的更改。
![1](/images/Unreal/Automation/9.png)
TeamCity去进行单元测试，只有测试通过的单元，才会被自动合入发布分支。
![](/images/Unreal/Automation/10.png)
本地修改同步到远端时，开启自动构建验证。
Internal QA Playtests 测试人员工作不饱和和去跑一些边界或者自动测试跑不到的地方。
Player feedback and reporting 灰度用户
`FAutomationTestBase`
`bool RunTest(const FString& Params)`

# ZPlan Automation
## Test Automation
![](/images/Unreal/Automation/11.png)
通过Window->Test Automation打开
## FBX Test Builder
通过Window->Automation Tools->FBX Test Builder打开。