python2.7
GnuWin32 Tools https://sourceforge.net/projects/getgnuwin32/ 执行download.bat和install.bat完成安装。添加环境变量

{% asset_img 1.jpg %}

CLang是基于LLVM。所谓LLVM，就是一个小小的虚拟机。这个虚拟机抽象了不同的硬件平台，如x86/arm/mips等。最近还抽象了GPU。有点像Java的VM，但是又和Java的VM很不同。Java的VM是比较高层的，它的byte code包括很多硬件平台并不能直接支持的功能。而LLVM的byte code则是更加接近硬件(CPU/GPU)的实际功能，只不过它是独立于任何一个具体硬件存在的。非常简单粗糙比喻的话，各种CPU/GPU就好比各个地方的人，说各个地方的方言；而LLVM的byte code则有些像普通话，与方言有着类似1对1的对应关系。

所以，首先需要迁出LLVM的代码，如下操作：

`svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm`

Clang是作为LLVM的一个前端，即，把C/C++翻译成LLVM可以懂的byte code的工具。LLVM再把byte code翻译成具体的机器指令。执行下面的命令可以迁出Clang的代码并放在LLVM妥当的位置。
```
cd llvm\tools
svn co http://llvm.org/svn/llvm-project/cfe/trunk clang
cd ../..
mkdir build
"C:\PROGRAM FILES (X86)\MICROSOFT VISUAL STUDIO\2017\COMMUNITY\COMMON7\IDE\COMMONEXTENSIONS\MICROSOFT\CMAKE\CMake\bin\cmake.exe" -G "Visual Studio 15" ../llvm
msbuild LLVM.sln
```

`Visual Studio 15 Win64`指定编译平台为64位.

编译完成之后，我们来测试我们编译出的clang是否有问题。首先我们需要将生成物的目录加入环境变量PATH，以便在命令行能够找到它
{% asset_img 2.jpg %}
重启命令行，检查是否可以找到clang
```
C:\Users\Administrator>clang -v
clang version 9.0.0 (trunk 361927) (llvm/trunk 361929)
Target: i686-pc-windows-msvc
Thread model: posix
InstalledDir: D:\build\Debug\bin
```
然后确保我们目前处于LLVM的顶级目录，就是下面有llvm和build这两个目录的那一集目录，执行下面的命令：
```
D:\>python.exe build/Debug/bin/llvm-lit.py -sv --param build_mode=Debug --param clang_site_config=build/tools/clang/test/lit.site.cfg build/tools/clang/test --param=build_config=Debug
```
我这里的环境执行会失败，python提示找不到一些测试用的程序。需要修改`build\tools\clang\test\lit.site.cfg`
把所有的`%(build_mode)`改为`%(build_config)`
如果用vim修改，可以用`:%s/build_mode/build_config/g`
最后`clang main.cpp`编译我们的项目。
http://clang.llvm.org/get_started.html

# Linux 编译
我这边的环境是Centos7.2的虚拟机环境。

    yum install docker
    systemctl start docker
    docker pull tim03/clang
    docker run -it --rm -v$(pwd):/usr/src tim03/clang
    bash-4.4# 
    bash-4.4# clang main.c

    
