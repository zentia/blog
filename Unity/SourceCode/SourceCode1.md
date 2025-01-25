模板类`PPtr`里面会记录一个实例ID，如果是debug模式还会记录一个`DEBUGPTR`就是本身地址
这个东西就是包裹了一个实例的对象。记录引用的。
https://cloud.tencent.com/developer/article/1005786
会同时在C#的managed heap和C++ native heap中分配内存。
# ModuleTemplate.inc.h
`ModuleMetadata`:全局的静态管理容器。
每个可以在IDE中显示的文件必须包含在jam中。

# Jam
https://www.boost.org/doc/libs/1_31_0/tools/build/jam_src/Jam.html
没有中文文档。
是一个程序构建工具
Unity用的是Jamplus，jam的一个升级版 https://github.com/jamplus/jamplus 这玩意一直在维护

`SubDir TOP External acl ;` 父子目录
`C.LinkPrebuiltLibraries` 包含libraries目录
`EDITOR_INCLUDES` 相对包含目录，一般来说，这里最好处理差异化，和UE的模块中的包含目录概念有点像了
`C.IncludeDirectories` 可以包含文件，也可以包含目录，可以是绝对，也可以相对
`#` 这个是注释

`RuntimeFiles.jam` 运行时配置环境
`EditorFiles.jam` 编辑器配置环境

## EditorFiles.jam
```jam
CopyFile # 把文件拷贝到目标目录中，第三方库需要执行
```
# Configuration
# Documentation
# Editor
# Extensions
- guisystem
# External 第三方库
- 7z
- acl
# Modules
# PlatformDependent
# Projects
## Jam
下面的每一个jam都会组建一个工程
# Runtime
# Shaders
# Tests
# Tools