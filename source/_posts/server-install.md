---
title: 服务器安装说明
date: 2017-10-24 12:53:24
tags:
  - server
---
1. 解压服务器执行文件xx_svr_binfull_XXXX.tgz到指定文件夹。
2. 解压配置文件xx_svr_cfgfull_XXXX.tgz到相同的文件夹。
3. 解压配置文件xx_svr_resfull_XXXX.tgz到相同的文件夹。
4. 将我们提供的key文件修改文件名为auth-info.key放在XCAAFS和XCLS文件夹下
5. XGMS目录下创建filecache目录，并给予执行服务器帐号写权限。
6. XLBA目录下创建siglog目录，并给予执行服务器帐号写权限。
7. 确认pid文件目录/tmp有写权限，确认/data日志目录有写权限。
8. 创建数据库，执行SQL下ca.sh，命令为：
./ca.sh 版本后缀 地址
如：
./ca.sh sy xxsy.cxebfhyvgemb.us-east-2.rds.amazonaws.com
9. 修改配置文件cmncfg.txt，主要修改如下：
SVRGRPID	大区ID，如果需要在一个集群建立多个大区，则大区ID不可重复
VER_EXT		版本后缀，同一台机器配置多组大区的时候必须不同
ONEOUTERIP	对外IP，客户端主要从此IP连接服务器
ONEINNERIP	服务器组内部IP，服务器间通信通过此IP
MYSQLIP		数据库IP
GDB_PORT	数据库端口
DB_USER		数据库帐号
DB_PASSWORD	数据库密码
PORTSHIFT	同一台机器上启动多组服务器必须不同
SHMSHIFT	同上
注：如分开多台服务器架设还需要修改指定服务器地址
10. 按照《服务器搭建和日常维护》文档第2节修改共享内存限制和tcp相关配置。
11. runGlog.sh启动日志服务器，一共15个进程。
12. 执行run.sh启动服务器组。


共有IP:18.219.246.109
私有IP:172.31.29.118