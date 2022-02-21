---
title: Shell
date: 2017-11-02 23:31:18
tags:
  - Linux
---
${}用来做变量替换。
一般情况下，$var与${var}并没啥不一样
但是用${}会比较精确的界定变量名称的范围，比方说：
$ A=B
$ echo $AB
g++ -DLINUX -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -Wall -Wno-multichar -g -D_DEBUG -DPG_MEMORY_DISABLE_POOL -march=core2 -m32 -rdynamic -std=c++0x -o ../../bin/debug/xxsy/ScnSvr_AllInOne/ScnSvr_AllInOne_d ../../temp/debug/xxsy/ScnSvr_AllInOne/ScnSvr_AllInOne.o_d  /root/work_code/src/lib/libGSDB_Lib_d.a /root/work_code/src/lib/libScnSvr_d.a /root/work_code/src/lib/libGZSGamePlay_Lib_d.a /root/work_code/src/lib/libMCE4Chat_d.a /root/work_code/src/lib/libMCE_d.a /root/work_code/src/lib/libDIA4DBI_d.a /root/work_code/src/lib/libDIA_d.a /root/work_code/src/lib/libPNGS_d.a /root/work_code/src/lib/libWHNET_d.a /root/work_code/src/lib/libWHCMN_d.a /root/work_code/src/lib/libxxsy_cmn_d.a /root/work_code/src/lib/libpgcmn_d.a -lpathfind_d -lcJSON_d -lcryptlib_d -llua_d -lzlib_d -ltss_sdk_d  /root/work_code/3rd/lib/libqos_client.a -lz -ldl -lssl -lpthread -ldl -lrt -march=prescott  -L/root/work_code/3rd/lib -L/root/work_code/src/lib -pthread
/usr/bin/ld: skipping incompatible /usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib/libm.so when searching for -lm

	#0 这个是shell的执行名字
	#n 这个是shell的第n个参数值，n=1..9
	$* 这个是shell的所有参数
	$# 表示参数的个数
	$$ shell的PID
	$! 执行上一个的PID
	$? 执行上一个指令的返回值（显示最后的退出状态，0表示没有错误，其它任何值表明有错误）
	$- 显示shell使用的当前选项，与set命令功能相同
	$@ 跟$* 类似，但是可以当作数组用

	-eq 等于
	-ne 不等于
	-le 小于等于
	-ge 大于等于
	-lt 小于
	-gt 大于

= 	就是赋值运算
:=	就是当冒号前面的变量不存在或者值为空时，就把等号后的值赋值给变量

# 删除十天前的文件lt

	find ./ -mtime +10 -name "*.*" -exec rm -rf {} \;

# 文件传输（Windows向Linux传输）

## 安装

	yum install openssh-clients -y

## 传输文件

	scp -p ./file/yum.log root@192.168.199.150:/tmp/yum.log

## 传输目录

	scp -rp ./file/ root@192.168.199.150:/tmp/

查看那个共享内存被哪些进程使用着

	lsof | grep "shmid"

将静态库文件解析成.o文件

	ar -x libexpat.a

查看归档文件的函数名

	nm -s xmltok.o

查看汇编代码

objdump -j .text -Sl tbus.o | more