---
title: EngineLoop
date: 2022-01-22 11:31:00
categories:
- Engine
- Unreal
tags:
    Unreal
---
{% plantuml %}
scale 2
class UObjectBase
{
    {static} EmitBaseReferences(UClass *RootClass);
    LowLevelRename(FName NewName,UObject *NewOuter = NULL);
    RegisterDependencies();
    Register(const TCHAR* PackageName,const TCHAR* Name);
    DeferredRegister(UClass *UClassStaticClass,const TCHAR* PackageName, const TCHAR* Name);
    -AddObject();
    +IsValidLowLevel() const;
}
note left of UObjectBase::"EmitBaseReferences(UClass *RootClass)"
Emit GC token for UObjectBase, this might be UObject::StaticClass or Default_Class
end note

class UClass
{

}
{% endplantuml %}
{% plantuml %}
class FRunnable
{
    +Init();
    +Run();
    +Stop();
    +Exit();
}
note left: 线程执行体，控制改变线程的状态

FRunnable <|-- FRenderingThread
class FRenderingThread
{
    # FString ThreadName;
}
{% endplantuml %}
![](EngineLoop.svg)