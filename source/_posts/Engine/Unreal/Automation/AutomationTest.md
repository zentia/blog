---
title: Automation Test
date: 2022-01-18 14:31:00
categories:
- Engine
- Unreal
- Automation
---

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