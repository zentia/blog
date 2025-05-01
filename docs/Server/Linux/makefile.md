---
title: makefile
date: 2017-10-15 15:35:44
tags:
- Server
- Linux
categories:
- Server
- Linux
---
# make: Nothing to be done for xxxx
# 中间文件、链接文件
C、C++要把源文件编译成中间代码文件，在Windows是.obj文件，在Linux是.o文件，即Object File，这一步叫做编译。然后把这些Object File合成可执行文件，这一步叫做链接(link)。
编译时，编译器需要的是语法正确，函数和变量声明正确，然后编译中中间目标文件。一般来说每个源文件都对应一个中间目标文件（O文件或者OBJ文件）。链接主要是链接函数和全局变量，所以我们可以使用这些中间目标文件来链接我们的可执行文件。链接器并不考虑源文件，只考虑中间目标文件（OBJ文件），大点的项目文件成千上百，导致会变成生成太多的中间文件，而链接时需要明确指出中间目标文件名，这对于编译很不方便，所以出现了库文件来合并这些目标文件windows下“库文件”（Library File）.lib文件，在Liunx下叫Archive File（.a）文件，所以一般包Link 错误就是指没有找到这个中间文件，即没有找到该声明的实现（一般情况下）。
# Makefile
make命令执行时，需要一个Makefile（一般是.mk 缺省Makefile或者makefile，作者习惯Makefile.mk）文件，告诉make命令如何编译和链接程序。
规则：
```js
target ... : prerequisites ...
	command
	...
	...
```
target也就是一个目标文件，可是是Object File，也可以是执行文件。还可以是一个标签（Label）。
prerequisites就是，要生成那个target所需要的文件或目标。
command是make需要执行的命令。（Shell命令）
这是一个依赖关系，就是说，target这一个或多个目标文件依赖于prerequisties中的文件，而规则是由command决定的。也就是说，prerequisites中如果有一个以上的文件比target文件要新的话，command所定义的命令就会被执行。这就是Makefile的规则。
1. ifeq的用法
	ifeq($(SRCFILE),)
	else ifeq()
	endif
如果没有变量值，则判断是否定义，有则判断是否相等
./test.sh -f config.conf -v --prefix=/home
我们称-f为选项，它需要一个参数，即config.conf，-v也是一个选项，但它不需要参数。
--prefix我们称之为长选项，即选项本身多余一个字符，它也需要一个参数，用等号连接，当然等号不是必须的，/home可以直接卸载--prefix后面，即--prefix/home。
# 伪目标、虚目标
伪目标不是一个真正的文件名，在执行make时可以指定这个目标执行
1. 如果我们需要书写这样一个规则：规则所定义的命令不是去创建目标文件，而是使用make指定具体的目标来执行一些特定的命令。像下边那样：
	clean:
	rm *.o temp
规则中"rm"不是创建文件"clean"的命令，只是删除当前目录下的所有.O文件和temp文件。在工作目录下不存在"clean"这个文件时，我们输入"make clean"后，"rm *.o temp"总会被执行。这是我们的初衷。
但当前工作目录下存在"clean"时情况就不一样了，在我们输入"make clean"时。规则没有依赖文件，所以目标被认为是最新的而不去执行规则所定义的命令，命令"rm"将不会被执行。这并不是我们的初衷。为了避免这个问题，我们可以将目标"clean"明确声明为伪目标。讲一个目标声明为伪目标需要将它作为特殊目标.PHONY的依赖。如下：
	.PHONY: dummy_help all dep obj cleanobj cleanint cleanout clean veryclean verycleanint install dirint dirout dirs pch cleanpch
这样dummy_help等就是一些伪目标或者说是虚目标，无论当前目录下是否存在"clean"这个文件。我们输入"make clean"之后，"rm"命令都会被执行。而且当一个目标被声明为伪目标后，make在执行规则时不会试图去查找隐含规则来创建这个目标。这样也提高了make的执行效率，同时我们也不用担心由于目标和文件名重名而使我们的期望失败。在书写伪目标规则时，首先需要声明目标是一个伪目标，之后才是伪目标的规则定义。目标"clean"书写格式应该如下：
	.PHONY: clean
	cleanint: cleanobj
	# 移除所有临时文件(目标文件、依赖文件)
        $(RM) -f $(DEPFILE_TO_DEL)
	cleanout:
	ifeq ($(ISWHAT),LIB)
	# 库
	        $(RM) -f $(TargetOutFile).*
	else
	ifeq ($(ISWHAT),EXE)
	# EXE文件
	        $(RM) -f $(TargetOutFile)
	else
	# DLL或其他
	        $(RM) -f $(TargetOutFile).*
	ifneq ($(OutDirLib),)
	        $(RM) -f $(TargetOutFileLib).*
	endif
	endif
	endif
	
	cleanpch:
	        $(RM) -f $(PchFile)
	clean: cleanint cleanout cleanpch
	        @echo ==== clean now ====

# 通配符
通配符是一种特殊语句，主要有星号(*)和问号(?)，用来模糊搜索文件
# Makefile中的wildcard用法
在Makefile规则中，通配符会被自动展开。但在变量的定义和函数引用时，通配符将失效。这种情况下如果需要通配符有效，就需要使用函数"wildcard"，它的用法是：$(wildcard PATTERN...)。在Makefile中，它被展开为已经存在的、使用空格分开的、匹配此模式的所有文件列表。如果不存在任何符合此模式的文件，函数会忽略模式字符并返回空。需要
# Linux make 保存错误日志
$mkdbg all 2>&1|tee xxx.log
这条命令是编译并保存打印信息。在Linux Shell的设备定义中，“0”表示标准输入，“1”表示标准输出，“2”表示标准出错信息输出。2>&1表示把2设备的信息重定向到1设 备；“|”是管道符号，把标准输出的信息直接传递给后面的命令；tee是创建文件并保存信息的工具；xxx.log是文件名。
这种管道的用法在Linux Shell命令中使用非常普遍。编译过程中都可以使用这个方法，生成日志文件，保存到buildlogs目录下。
  1 # 编译模式名
  2 ConfigurationName               := debug
  3 # 用于体现该编译模式的文件标记
  4 # (对于debug版来说，比如目标文件后缀就会变成._do)
  5 CfgFileTag                              := _d
  6 # 编译选项
  7 #CCFlag                                 := -Wall -g -D_DEBUG -march=native
  8 CFlag                                   := -Wall -Wno-multichar -g -D_DEBUG -DPG_MEMORY_DISABLE_POOL -march=core2 -m32 -rdynamic
  9 CCFlag                                  := $(CFlag) -std=c++0x
 10
 11 #CCFlag                                 := -finput-charset=gb2312 -Wall -g -D_DEBUG
 12 # 连接选项
 13 #LNFlag                                 := -lpthread -march=native
 14 LNFlag                                  := -lpthread -ldl -lrt -march=prescott
