---
title: 服务器配置文件说明
date: 2017-10-12 20:56:07
tags:
  - server
---
***摘要：***本文档描述手游服务器的各个配置文件的使用和配置参数说明。
关键字：服务器 维护 配置

# 1.概述

请先参考《游戏系统架构》以了解手游的服务器架构状况。
手游的所有配置文件都采用相同的格式。运维人员会经常用到的配置文件特性有：注释，文件包含include、宏定义、条件读取。

# 2.各个服务器主要配置参数说明

## 2.1 公共配置文件

文件名：cmncfg.txt
该文件主要用来被所有的配置文件包含，可以定义一些全局性的信息（比如在不同的服务器配置信息不同的地方就可以放在这个文件里，这样不同的服务器只要修改这一个文件即可。）
它一般放在所有服务器程序目录的上层目录，命名为cmncfg.txt
典型内容如下：
```javascript
////////////////////////////////////////////
// 经常要变的部分
////////////////////////////////////////////
// 大区的ID（这个配置很重要，如果配置错误会导致所有的角色和物品ID错误，影响合服）
#defene SVRGRPID 2
// 代表版本字串
#define VERMAIN 0.7100
// 各类服务器IP定义
// 日志服务器的地址
#define LOGSVR 192.168.0.49
// GMS4Chat
#define GMS4CHATINNERIP 192.168.0.49
// CAAFS4CHAT
#define CAAFS4CHATOUTTERIP 192.168.0.49
// GMS4Game
#define GMS4GAMEINNERIP 192.168.0.49
// DBS
#define DBSINNERIP 192.168.0.49
// P2PSERVER
#define P2POUTTERIP 192.168.0.49
// XGMS
#define XGMSINNERIP 192.168.0.49
// TTYGZS
#define TTYGZSINNERIP 192.168.0.49
// XCAAFS
#define XCAAFSOUTTERIP 192.168.0.49
// XCLS
#define XCLSOUTERIP 192.168.0.49
// 本组配置的端口增量（主要用于在同一台机器上启动多组服务器）
#define PORTSHIEF 0
// 共享内存增量
#define SHMSHIFT 0
// 日志的跟路径（注意这个配置，如果在后面加上了"/"则会导致目录创建失败，因为字串中后来形成的//会被当成注释）
#define LOGPATH /ITC/LOG
// 数据库相关的定义
#define DB_HOST localhost
// 如果DB_HOST是localhost则需要填写这个，默认值是/tmp/mysql.sock
#define DB_SOCKET /TMP/mysql.sock
#define DB__USER root
#define DB_PASSWORD zentiadb2017
#define DB_CHARSET utf8
// 用户数据库
#define PLAYERDB zentia0
// 全局总控相关数据库（GMS4Game使用的）
#define IMSDB ims0

////////////////////////////////////////////////////
// 不经常变的部分
////////////////////////////////////////////////////
// 表面这是debug版本
#define DEBUG
// 表明这是内部调试用的，可以放宽一些限制（正常运营时应该注视掉）
// #define INNERDEBUG
// 表明客户端超时不要太长（在内部测试的时候可能有用，因为有时候需要长时间不断，有时候需要正常超时断）
// define QUICKDROP

/////////////////////////////////////////////////////
// 日志服务定义
/////////////////////////////////////////////////////
// 日志服务的描述串（根据各个服务器需求的不同分为下面三个等级）
#define LOGADDRSTR_0 	0,$(LOGSVR):2000+$(PORTSHIFT), 	1,$(LOGSVR):2001+$(PORTSHIFT),	2,$(LOGSVR):2002+$(PORTSHIFT)
#define LOGADDRSTR_1	0,$(LOGSVR):2000+$(PORTSHIFT),	1,$(LOGSVR):2001+$(PORTSHIFT),	2,$(LOGSVR):2002+$(PORTSHIFT),	3,$(LOGSVR):2003+$(PORTSHIFT),	4,$(LOGSVR):2004+$(PORTSHIFT),	5.$(LOGSVR):2005+$(PORTSHIFT),	6,$(LOGSVR):2006+$(PORTSHIFT)
#define LOGADDRSTR_1	0,$(LOGSVR):2000+$(PORTSHIFT),	1,$(LOGSVR):2001+$(PORTSHIFT),	2,$(LOGSVR):2002+$(PORTSHIFT),	3,$(LOGSVR):2003+$(PORTSHIFT),	4,$(LOGSVR):2004+$(PORTSHIFT),	5.$(LOGSVR):2005+$(PORTSHIFT),	6,$(LOGSVR):2006+$(PORTSHIFT),	7,$(LOGSVR):2007+$(PORTSHIFT),	8,$(LOGSVR):2008+$(PORTSHIFT),	9,$(LOGSVR):2009+$(PORTSHIFT),	10,$(LOGSVR):2010+$(PORTSHIFT),	11,$(LOGSVR):2011+$(PORTSHIFT),	12.$(LOGSVR):2012+$(PORTSHIFT),	13,$(LOGSVR):2013$(PORTSHIFT),	14,$(LOGSVR):2014$(PORTSHIFT)
// 本大区最多支持的同时在线用户数量
#define MAXPLAYER 16000
// 服务器间tcp通讯的消息缓冲设置
#define MSGBUFFLEN 10*1024*1024
// debug版的文件后缀
#ifdef DEBUG
#define CFGEXT
#else
#define CFGEXT
#endif
// 这个只有配置caafs4chat的时候才需要，正常的配置不需要，这个一般不变的，因为CAAFS4Chat一般就起一个即可
#define CAAFS4CHATID 1

// 是否在GSZ宕掉之后重新拉起自动载入用户
#define GZSDOWNRESTOREPLAYER true

// CNL网络超时（和客户端的断线超时）
#ifdef INNERDEBUG
#define DROPTIMEOUT 2000000
#else
#define DROPTIMEOUT 100000
#endif
#ifdef QUICKDROP
#define DROPTIMEOUT 600000
#endif

```

## 2.2 GMS4Game

配置文件：gms4game_cfg.txt
典型内容如下：
```javascript
#include ../cmncfg.txt

CMN
{
	stream_size			= $(MSGBUFFLEN)
	// 本地监听的地址和端口（供其他服务器连接）
	tcp_listen_host 	= $(MYOUTERIP):5000+$(PORTSHIFT)
	// 和其他服务器TCP连接的超时设置
	keepalive_idle		= 60000
	keepalive_interval	= 2000
	// MySQL配置
	db_host 			= $(DB_HOST)
	db_user 			= $(DB_USER)
	db_password 		= $(DB_PASSWORD)
	db_database 		= $(IMSDB)
	db_charset 			= $(DB_CHARSET)
	db_socket 			= $(DB_SOCKET)
	// 日志服务（这个只需要比较少的日志种类）
	logger_addr 		= $(LOGADDRSTR_0)
	// szPIDFile 		= /tmp/GMS4Game-$(SVRGRPID).pid
}
```

# 2.3 XLBA
配置文件：lba_cfg.txt
典型内容如下（基于XCMNSVR的配置）:
gzs_cfg1.txt
```javascript

```
# 附录1.基于XCMNSVR的配置
  XCMNSVR是PNGS框架中的一种标准服务器程序结构。
  通过该结构配置的服务器由一个主程序框架：XCMNSVR和一系列的动态连接库插件构成。
  其配置文件一般都是下面的样子：
```
CMN
{
	// 用于记录日志中表示程序名和在windows下显示窗口标题栏上
	szSvrName		= 服务器名
	// 是否运行为后台守护进程（这个选项一般在GDB启动程序进行调试比较有用）
	bDaemon			= true
	// 日志服务地址
	szLoggerAddr	= $(LOGADDRSTR)
	// 每个逻辑循环的最大时间（其大小会影响服务器在没有网络输入输出的情况下的逻辑tick精度）
	nSelectInterval = 50
}
PlugIn
{
	// 各个插件的载入和创建参数
	// 注意：插件的文件名必须是带路径的。即便是当前路径下也需要写"./"前缀
	DLL		= 0,/TCPReceiver$(CFGEXT).so,TCPReceiver_Create
	DLL		= 0,/TCPReceiver$(CFGEXT).so,LBACDTest_Create
}
// 后面跟着各个插件自己需要的参数配置
```
# 附录2.CNTRSVR的配置

# 附录3.GSMEM的配置

# 附录4.IP的允许禁止文件的配置