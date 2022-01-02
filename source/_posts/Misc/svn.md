---
title: svn
date: 2017-10-15 11:17:03
tags:
  - svn
---
# svn: E200002: line 19: Option expected错误解决
这是因为修改svnserve.conf时，打开注释时，配置的前面有空格，应该顶格写。
修改后即可。
# 解决svn Authorization failed错误
出现该问题基本都是三个配置文件的问题，下面把这个文件列出来
svnserve.conf:
[general]
anon-access = read
auth-access = write
password-db = passwd
authz-db = authz
passwd:
[users]
admin=123
authz:
[groups]
[/]
admin= rw
出现authorization failed异常，一般都是authz文件里，用户组或者用户权限没有配置好，只要设置[/]就可以，代表根目录下所有的资源，如果要限定资源，可以加上 子目录即可。
# checkout
svn checkout path（path是服务器上的目录）
   例如：svn checkout svn://192.168.171.128/zentia/trunk/xxprj/server
# revert
	svn revert [-R] dir
# CentOS下svn迁移备份的三种方法
一般采用三种方式：
1. svnadmin dump
2. svnadmin hotcopy
3. svnsync
注意，svn备份不宜采用普通的文件拷贝方式（除非你很懂svn，并且将库暂停），如copy、rsync命令。
svnadmin dump是官方推荐的备份方式，优点是比较灵活，可以全量备份也可以增量备份，并提供了版本恢复机制。
缺点：如果版本比较大，如版本数增长到数万、数十万，那么dump的过程将非常慢；备份耗时，恢复更耗时；不利于快速进行灾难恢复。
个人建议在版本数比较小的情况下使用这种备份方式。
svnadmin hotcopy原设计目的估计不是用来备份的，只能进行全量拷贝，不能进行增量备份；
优点是：备份过程较快，灾难恢复也很快；如果备份机上已经搭建了svn服务，甚至不需要恢复，只需要进行简单配置即可切换到备份库上工作。
缺点是：比较消耗硬盘，需要有较大的硬盘支持。
svnsync实际上是
# CentOS下SVN服务的启动语关闭
svnserve -d -r /home/svn
/home/svn 为版本的根目录
关闭SVN服务：
ps aux | grep svn
kill -s 9 64434
64434为进程ID

或者 killall svnserve

# linux下SVN中改变执行权限
本地文件在commit到仓库之前若没有chmod+x权限的话，拿在svn仓库里的文件将会保持当前无可执行属性状态，我们可以做svn命令修改。
SVN中，如果我们在linux下提交权限为755文件，就会在svn的属性中存在一个svn:executable。
只在本地chmod 644 ，是不能造成修改的。
svn propset svn:executable on * （注意，如果包含目录会执行失败）
svn commit
# 前一个操作被卡住的问题
去官网下载win32那个包就可以了。
sqlite3.exe .svn/wc.db "delete from wc_lock"
sqlite3.exe .svn/wc.db "delete from work_queue"
/OUT:"F:\zentia\server\bin\vc120debugs\glogger_d.exe" /MANIFEST /NXCOMPAT /PDB:"F:\zentia\server\bin\vc120debugs\glogger_d.pdb" /DYNAMICBASE:NO "WHCMN_d.lib" "WHNET_d.lib" "PNGS_d.lib" "cryptlib_d.lib" "zlib_d.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /FIXED /LARGEADDRESSAWARE /IMPLIB:"F:\zentia\server\bin\vc120debugs\glogger_d.lib" /DEBUG /MACHINE:X86 /OPT:NOREF /SAFESEH:NO /INCREMENTAL /PGD:"F:\zentia\server\bin\vc120debugs\glogger_d.pgd" /SUBSYSTEM:CONSOLE",5.01" /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /ManifestFile:"F:\zentia\server\temp\vc120\glogger2010\debug\glogger_d.exe.intermediate.manifest" /OPT:NOICF /ERRORREPORT:PROMPT /NOLOGO /LIBPATH:"F:\zentia\server\src\svr\glogger\/lib/win32" /LIBPATH:"F:\zentia\server\lib\vc120lib\" /LIBPATH:"F:\zentia\server\3rd\lib\vc120\" /LIBPATH:"F:\zentia\server\3rd\lib" /LIBPATH:"%(AdditionalLibraryDirectories)" /TLBID:1 
g++ -DLINUX -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -Wall -Wno-multichar -g -D_DEBUG -DPG_MEMORY_DISABLE_POOL -march=core2 -m32 -rdynamic -std=c++0x -o ../../bin/debug/svr/GBNMS/GBNMS_d.so ../../temp/debug/svr/GBNMS/GBNMS.s.o_d  /root/work_code/src/lib/libGBNMS_Lib_d.s.a /root/work_code/src/lib/libDIA_d.s.a /root/work_code/src/lib/libPNGS_d.s.a /root/work_code/src/lib/lib    WHNET_d.s.a /root/work_code/src/lib/libWHCMN_d.s.a  /usr/lib64/mysql/libmysqlclient_r.a -lz -lpthread -ldl -lrt -march=prescott -shared  -L/root/work_code/3rd/lib -L/root/work_code/src/lib
# 拉分支

svnserve -d -r /var/svn
启动svn服务器，但是客户端还是一直连接不上

svn info 查看库版本路径

客户端一直连接不上服务器，最后发现是防火墙的问题，最暴力直接关掉防火墙就好啦。

post-commit.tmpl 
它在事务外城后运行，创建一个新的修订版本。大多数人用这个钩子来发送关于提交的描述性邮件，或者作为版本库的备份。版本库传给程序两个参数：到版本库的路径和被创建的新的修订版本号。退出程序时被忽略。

关闭防火墙
重启后生效
开启: chkconfig iptables on
关闭: chkconfig iptables off
即时生效，重启后失效
开启：service iptables start 
关闭：service iptables stop

查看磁盘空间
df -h

## 创建版本库
```shell
	mkdir /var/svn

	# 进入svn目录
	cd /var/svn  
	# 用svn管理员创建bigsvr
	svnadmin create /var/svn/bigsvr
	# 进入库
	cd bigsvr
```
subversion目录说明
db		所有版本控制的数据存放目录
hooks	放置hook脚本
locks 	用户操作的锁文件
format	记录当前文件库的版本号
conf	配置文件

# Svn命令行总结

1. 上传项目到SVN服务器上

	svn import project_dir(本地项目全路径)

2. 下载checkout svn项目到本地

	svn checkout 简写：svn co

3. 添加新文件

	svn add file(文件名)
	svn add \*.php(添加当前目录下所有的php文件)

4. 提交到版本库

	svn commit -m "LogMessage" [-N] [--no-unlock] PATH(如果选择了保持锁，就使用--no-unlock开关)

5. 更新文件

	svn update
	svn update -r 修正版本 文件名
	svn update (后面没有目录，默认将当前目录以及子目录下的所有文件都更新到最新版本)
	svn update -r 200 test.cpp (将版本库中的文件test.cpp还原到修正版本(revision)200)
	svn update test.php (更新于版本库同步)

6. 设置忽略

	svn propset svn:ignore Temp .

7. relocate

	svn switch --relocate svn://18.223.184.177/project/ss/ss/bin/data svn://18.223.184.177/project/ss/ss/bin/data