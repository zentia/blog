---
title: 添加脚本绑定到一个C++类中
categories:
- Engine
- Unity
- SourceCode
---
Unity使用自定义工具生成脚本绑定。这个工具在Tools/UnityBindingsParser目录下。我们定义该文件必须以.bindings后缀名。然后从.bindings文件我们生成一个C++绑定文件，一个C#接口文件，和脚本引用文档。
如果你创建一个新的脚本绑定.binding文件，你必须把他放在Runtime->Export目录下或者Editor->Mono目录下。

脚本绑定的类通常来讲是继承自Component或者Behaviour。assets资产的话需要继承Object。

创建好之后在bindings文件中集中处理C++RAW逻辑和CSRAW逻辑。然后到根目录直接使用`perl build.pl --processTxts`

# 创建一个新的脚本绑定.bindings文件

请用下面这个模板创建一个脚本绑定文件
```
C++RAW
#include "UnityPrefix.h"
#include "MonoExportUtility.h"
#include "BaseScript.h"

CSRAW
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Collections;

namespace UnityEngine
{
CLASS MyNewClass : Component
// ...your class script interface here...
END

CSRAW
}
```

# 使用生成的C++文件并且更新UnityEngine.dll

通过脚本绑定文件Something.bindings会生成一个Generated/Something.cpp文件。无论什么时候你新建一个.binding文件，你必须添加一个生成的C++文件到全部的项目中（XCode,CodeWarrior和Visual Studio项目中）。否则你会出现一些错误当启动游戏的时候（提示Mono找不到函数）。

无论啥时候，你只要改变脚本绑定文件，你就必须要重新生成C++文件并且编译UnityEngine.dll然后把它放到你的

```
CUSTOM [public|private|internal] [static] <c# method or constructor signature> <c++ function body>
    Generates a c# method or static method which has the c++ function body as its definition.
    public is optional and the default access level
```