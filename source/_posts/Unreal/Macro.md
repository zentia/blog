---
title: Unreal 常见宏
mathjax: true
date: 2021-06-29 20:55:59
categories:
- Engine
- Unreal
tags:
    Unreal
---

# 类说明符
- Abstract

    抽象类说明符将类声明为“抽象基类”，防止用户在虚幻编辑器中向世界添加此类的参与者，或者游戏中创建此类的实例。这对于那些本身没有意义的类很有用。
  
- AdvancedClassDisplay

    强制类的所有属性只显示在“Detail”面板的"Advanced"部分中，默认情况下隐藏在”视图“中。若要在某个属性上重写此项，请在属性上使用SimpleDisplay说明符。
  
- AutoCollapseCategories=(Category1, Category2,...)/DontAutoCollapseCategories(Category,Category,...)

    AutoCollapseCategories类说明取消对父类AutoExpandCategories说明符的列出类别的影响。
    DontAutoCollapseCategories否定从父类继承的列出类别的AutoCollapseCategories的说明符。
  
- AutoExpendCategories=(Category1,Category2,...)

    指定应在该类的对象”虚幻编辑器“属性窗口中自动展开的一个或多个类别。若要自动声明为无类别的变量，请使用声明变量的类的名称
  
- Blueprintable/NotBlueprintable

    将此类公开为创建蓝图可接受的基类。默认是NotBlueprintable，除非继承，否则该说明符由子类继承
  
- BlueprintType

    将类型公开可用于蓝图中的变量的类型
  
- ClassGroup=GroupName

    表示在Actor浏览器中启用Group View后，Actor浏览器应该在指定的GroupName中包含此类以及它的任何子类
  
- CollapseCategories/DontCollapseCategories

    表示不应该将此类的属性分组到属性窗口的类别中。这个说明符将传播给子类，但是，子类可以用DontCollapseCategories说明符来覆盖它
  
- Config=ConfigName

    表示允许此类在配置文件中(.ini)中存储数据。如果有使用config或globalconfig说明符属性的任何类属性，则此指定夫将导致这些属性存储在命名的配置文件中。此说明符将传播到所有子类不能被否定，但是子类可以通过re-declaring配置说明符并提供不同的ConfigName来更改配置文件。常见的ConfigName值是"Engine","Editor","Input","Game"。
  
- Const

    该类中所有属性和函数都是const的，并以const的形式导出。该说明符由子类继承
  
- ConversionRoot

    Root转换符将一个子类限制为仅能够转换到为第一个Root类的子类，并上升到层级结构
  
- CustomConstructor

    组织自动生成构造函数声明
  
- DefaultToInstanced

    这个类的所有实例都被认为是”实例化“的。实例化的类（组件）在构建时被复制。该说明符由子类继承
  
- DependsOn=(ClassName1,ClassName2,...)

    列出的所有类将在该类之前编译。类必须在同一个（或上一个）包中指定一个一个类。可以使用由逗号分隔的单个取决于指定多个依赖项类，也可以为每个类使用单独指定。当类使用在另一个类中声明的结构或枚举时，这一点很重要，因为编译器只知道它已编译的类中内容。
  
- Deprecated

    这个类不推荐使用，而且这个类的对象在序列化的时候不会被保存。该说明符由子类继承
  
- EditInlineNew/NotEditInlineNew

    表示可以从”虚幻编辑器“属性窗口创建此类的对象，而不是从现有资产引用。默认行为时只有对现有对象的引用才可以通过属性窗口分配。此说明符被传播到所有子类；子类可以使用NotEditInlineNew说明符来重写此说明符。
  
- HideCategories=(Category1,Category2,...)/ShowCategories=(Category1,Category2,...)
    
    - HideCategories列出隐藏在此类对象的”虚幻编辑器“属性窗口的一个或多个类别。若要隐藏声明为”no“类型的属性，请使用声明该变量的名称。此说明符将传播到子类。
    - ShowCategories为所列出的类别取消一个HideCategories说明符（从基类继承）
    
- HideDropdown

防止在虚寒编辑器属性窗口组合框中显示此类

- HideFunctions=(Category1,Category2,...)/ShowFunctions=(Category1,Category2,...)

    - HideFunctions从属性查看器隐藏指定类别的所有函数
    - ShowFunctions从属性查看器中显示列出的类别中所有函数
    
- Intrinsic

    这表明这个类是直接用C++声明，并且没有UHT生成的generate文件。不要在新类上使用这个说明符。
  
- MinimalAPI

    仅导致要导出的类的类型信息供其它模板使用。类可以被强制转换，单不能调用类的函数（inline除外）。这提高了编译事件，因为不需要在其他模块中访问所有函数的类导出所有内容。
  
- NoExport

    表示此类的声明不应包含由UBT自动生成的generate文件。必须在单独的头文件中手动定义C++声明。仅对本机类有效。不要将此用于新类。
  
- Placeable/NotPlaceable

    表示可以在编辑器中创建此类，并将其放置于关卡、UI或蓝图中（具体取决于类类型）。此标志将传播到所有子类；子类可以使用NotPlaceable说明符来重写此标志
  
- Transient/NonTransient

    属于此类的对象将永远不会保存到磁盘。这与某些非永久性的自然类（如播放器或窗口）结合使用非常有用。此说明符将传播到子类，但可以由NonTransient说明符重写
  
- Within=OuterClassName

    这个类的对象不能存在于一个OuterClassName对象的实例之外。这意味着创建这个类的Object需要提供一个OuterClassName的实例将其作为外部对象
  
# 元数据修饰符

- BlueprintSpawnableComponent

    如果存在，组件类可以由蓝图生成
  
- BlueprintThreadSafe

    仅对蓝图函数库有效。此说明符将此类的中函数标记为在动画蓝图中的游戏线程上可调用
  
- ChildCannotTick/ChildCanTick

    - ChildCannotTick用于Actor和Component类。如果host不能勾选，基于此Actor或Component的蓝图类永远不会勾选，即使bCanBlueprintsTickByDefault为true
    - CHildCanTick用于Actor和Component类。如果host不能勾选，则基于此参与者或组件的蓝图类可以覆盖bCanEverTick标记，即使bCanBlueprintsTickByDefault是false。
    
- DeprecatedNode

    对于行为树节点，指示该类已弃用，并在编译时显示警告

- DeprecationMessage="Message Text"

    如果该类被启用，则在尝试编译使用它的蓝图时，此消息奖杯添加到警告中
  
- DisplayName="Blueprint Node Name"

    蓝图中此节点的名称将替换为此处提供的值，而不是代码生成的名称
  
- DontUseGenericSpaenObject

    不要在蓝图中使用泛型创建对象节点生成类的对象。此说明符仅对既非参与者也不ActorComponents的BlueprintType类是有意义的
  
- ExposedAsyncProxy

    在异步任务节点中公开此类的代理对象
  
- IgnoreCategoryKeywordsInSubcalasses

    用于使类的第一个子类忽略所有继承的ShowCategories和HideCategories说明符
  
- IsBlueprintBase="true/false"

    声明此类是（或不是）用于创建蓝图的可接受基类，类似与UCLASS说明
  
- KismetHideOverrides="Event1,Event2,..."

    不允许重写的蓝图事件列表
  
- ProhibiteInterfaces="Interface1,Interface2,..."

    列出与类不兼容的接口
  
- RestrictedToClasses="Class1,Class2,..."

    由蓝图函数库类使用，用于限定列表中命名的类的用法
  
- ShortToolTip

    在某些上下文中使用的简短工具提示，其中完整的工具提示可能是压倒性的，如父类选择器对话框
  
- ToolTip

    重写代码注释中自动生成的工具提示
  
- ShowWorldContextPin

    表示放置在此类所拥有的关系图中的蓝图节点必须显示它们的世界上下文，即使它们通常是隐藏的，因为此类的对象不能用作世界上下文。
  
- UsesHierarchy

    表示类使用分层数据。用于实例化detail面板中的分层编辑功能。
  
# Shader中的宏

- MOBLIE_MULTI_VIEW

  计算View复杂
  
- PACK_INTERPOLANTS

  是否使用了顶点雾，多了PackedInterps数组需要计算高度雾

- USE_WORLD_POSITION_EXCLUDING_SHADER_OFFSET 

  C++对应bNeedsWorldPositionExcludingShaderOffsets，是否需要世界位置，用于Decal计算

- USE_PS_CLIP_PLANE

  C++中对应r.AllowGlobalClipPlane，是否使用ps裁剪

- OUTPUT_GAMMA_SPACE

  C++中对应LDR_GAMMA_32，使用Gamma空间

- FULLY_ROUGH 
  
  c++对应材质IsFullyRough，是否是fullrough，可以减少计算

- NONMETAL

  C++ MATERIAL_NONMETAL 材质是否金属化，非金属计算量少，EnvBRDFApproxNonmetal EnvBRDFApprox

- LQ_TEXTURE_LIGHTMAP

  是否使用了Lightmap

- HQ_REFLECTIONS

  High Quality 反射

- ALLOW_CUBE_REFLECTIONS

  High Quality 反射中的cube反射

- MATERIAL_PLANAR_FORWARD_REFLECTIONS

  Planar 反射

- MATERIALBLENDING_SOLID

  Blend方式

- MATERIALBLENDING_MASKED

- MATERIALBLENDING_APLHACOMPOSITE

- MATERIALBLENDING_TRANSLUCENT

- MATERIALBLENDING_ADDITIVE

- MATERIALBLENDING_MODULATE

- MATERIAL_SHADINGMODEL_UNLIT

  - 无光照模式
  
- ENABLE_SKY_LIGHT

  是否拥有天光

- MAX_DYNAMIC_POINT_LIGHTS

  动态光

- NON_DIRECTIONAL_DIRECT_LIGHTING

这可以在DeferredLightingCommon.ush中找到，但似乎只在ForwardLightingCommon.ush定义

- SUPPORT_CONTACT_SHADOWS

为unreal的contact Shadows Feature提供支持。

- REFERENCE_QUALITY

在DeferredLightingCommon.ush的顶部定义为0，可能用于cinematic rendering

- ALLOW_STATIC_LIGHTING

  如果r.AllowStaticLighting的控制台变量设置为1，则为true。这与Project Settings>Rendering的Static Lighting support选项匹配

- USE_DEVELOPMENT_SHADERS

  如果COMPLIE_SHADERS_FOR_DEVELOPMENT是真的（并且平台支持它），则为true