---
title: Automation Test
date: 2022-01-18 14:31:00
categories:
- Engine
- Unreal
- Automation
---

https://docs.unrealengine.com/4.27/zh-CN/TestingAndOptimization/Automation/TechnicalGuide/

```C++
// AutomationTest.h
/** Flags for specifying automation test requirements/behavior */
namespace EAutomationTestFlags
{
    enum Type
    {
        //~Application context required for the test
        // Test is suitable for running within the editor
        EditorContext   = 0x00000001,
        // Test is suitable for running within the client
        ClientContext       = 0x00000002,
        // Test is suitable for running within a commandlet
        CommandletContext   = 0x00000008,
        ApplicationContextMask = EditorContext | ClientContext | ServerContext | CommandletContext,
        // Performance Test
        PerfFilter      = 0x08000000,
        // Stress Test
        StressFilter    = 0x10000000,
        // Negative Test. For tests whose correct expected outcome if failure.
        NegativeFilter  = 0x20000000,
        FilterMask = SmokeFilter | EngineFilter | ProjectFilter | PerfFilter | StressFilter | NegativeFilter
    };
}
```
`UE4Editor.exe "[PathFromUnrealEditorBinarayToYourProjectDirectory]/[ProjectName].uproject -Game -ExecCmds="Automation RunTests [TestPrettyName]" -log`

# IMPLEMENT_SIMPLE_AUTOMATION_TEST

# 复杂测试
复杂测试用于在许多输入上运行相同的代码。这些测试通常是压力测试。例如，加载所有地图或编译所有蓝图将非常适合复杂的自动化测试。注意，必须覆盖“RunTest”和“GetTests”函数。
会将GetTests里面的返回值的信息罗列在面板上面。

# 潜在命令(Latent)

