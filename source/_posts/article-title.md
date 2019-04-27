---
title: scene和prefab的区别
date: 2017-09-17 13:08:18
tags:
  - Unity
---
如果一个场景有多个相同的pfb(指引用相同的pfb，具体每个pfb可能有参数不同），那么Scene里每个pfb都是独立的对象，加载时每个对象都要走全套的流程: 文件I/O ->反序列化 (解析，new obj + ctor) -> awake -> shader\texture\vbo create\upload，如果走pfb+Instantiate，那么前面的流程(文件I/O，反序列化）只用走一遍，直接Instantiate复制内存中现有的对象就行.
另外pfb可以在合适的时候提前异步预加载，这样切场景的时候就更快了（场景也可以异步预加载，不过awake\upload to gpu这些必须在主线程做，所以会卡一下），pfb也便于做细粒度的热更. 但pfb对美术日常调整场景不友好，除非有个一键生成场景+一键存pfb. 
单论切换速度，什么都比不过预加载 + 大内存永驻

CentOS
1.yum install subversion
2.创建svn版本目录: mkdir -p /var/svn/svnrepos
3.创建版本库: svnadmin create /var/svn/svnrepos
4.cd /var/svn/svnrepos/conf (authz文件是权限控制文件;passwd是帐号密码文件;svnserve.conf SVN服务器配置文件)
5.设置帐号密码
	vi passwd
	在[users]块中添加用户和密码，格式：帐号=密码，如zentia=zentia
6.设置权限
vi authz
在末尾添加如下代码
[/]
zentia=rw
liyanfeng=r
意思是版本库的根目录zentia对其有读写权限，liyanfeng只有读权限。
7.修改svnserve.conf文件
打开下面几个注释：
anon-access=read #匿名用户可读
auth-access = write #授权用户可写
password-db = passwd #使用哪个文件作为帐号文件
authz-db = authz #使用哪个文件作为权限文件
realm = /var/svn/svnrepos #认证空间名，版本库所在目录
8.启动svn版本库
svnserve -d -r /var/svn/svnrepos 
ps aux | grep svnserve #查看服务是否启动
# SVN客户端搭建
1.修改host文件
	10.173.32.4 zentiasvr
