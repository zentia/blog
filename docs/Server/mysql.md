---
title: Mysql
date: 2017-10-14 12:01:59
tags:
- Server
categoires:
- Server
---
# 修改mysql密码

```
mysql -uroot -p
use mysql;
UPDATE user SET password=password("123456") WHERE user='root'; 
flush privileges;
exit;
```

```
SHOW DATABASES;
```

首先CentOS7 已经不支持mysql，因为收费了你懂得，所以内部集成了mariadb，而安装mysql的话会和mariadb的文件冲突，所以需要先卸载掉mariadb，以下为卸载mariadb，安装mysql的步骤。

# 列出所有被安装的rpm package 
rpm -qa | grep mariadb

# 卸载
rpm -e mariadb-libs-5.5.37-1.el7_0.x86_64
错误：依赖检测失败：
libmysqlclient.so.18()(64bit) 被 (已安裝) postfix-2:2.10.1-6.el7.x86_64 需要
libmysqlclient.so.18(libmysqlclient_18)(64bit) 被 (已安裝) postfix-2:2.10.1-6.el7.x86_64 需要

#强制卸载，因为没有--nodeps
rpm -e --nodeps mariadb-libs-5.5.37-1.el7_0.x86_64

# 安装mysql依赖
yum install vim libaio net-tools

其他情况：

1、centos下yum暂时没有mysql-server直接安装包；
MariaDB是MySQL社区开发的分支，也是一个增强型的替代品;

2、安装MariaDB（目前还是先不要安装了）
yum -y install mariadb-server mariadb mariadb-devel
systemctl start mariadb
systemctl enable mariadb
mysql_secure_installation
firewall-cmd --permanent --add-service mysql
systemctl restart firewalld.service
iptables -L -n|grep 3306

CentOS7的yum源中默认好像是没有mysql的。为了解决这个问题，我们要先下载mysql的repo源。

1. 下载mysql的repo源

$ wget http://repo.mysql.com/mysql-community-release-el7-5.noarch.rpm

2. 安装mysql-community-release-el7-5.noarch.rpm包

$ sudo rpm -ivh mysql-community-release-el7-5.noarch.rpm

安装这个包后，会获得两个mysql的yum repo源：/etc/yum.repos.d/mysql-community.repo，/etc/yum.repos.d/mysql-community-source.repo。

3. 安装mysql

$ sudo yum install mysql-server
根据步骤安装就可以了，不过安装完成后，没有密码，需要重置密码。
4. 重置密码
重置密码前，首先要登录
$ mysql -u root
登录时有可能报这样的错：ERROR 2002 (HY000): Can‘t connect to local MySQL server through socket ‘/var/lib/mysql/mysql.sock‘ (2)，原因是/var/lib/mysql的访问权限问题。下面的命令把/var/lib/mysql的拥有者改为当前用户：
$ sudo chown -R openscanner:openscanner /var/lib/mysql
然后，重启服务：
$ service mysqld restart

5. 开放3306端口
$ sudo vim /etc/sysconfig/iptables
添加以下内容：
-A INPUT -p tcp -m state --state NEW -m tcp --dport 3306 -j ACCEPT
保存后重启防火墙：
$ sudo service iptables restart
这样从其它客户机也可以连接上mysql服务了。
MYSQL启动后报：ERROR! The server quit without updating PID file错误的问题解决
MYSQL日志：Can't find file: './mysql/plugin.frm' (errno: 13 - Permission denied)

1. 权限不够：chown -R mysql:mysql /home/mysql/data”  chmod -R 755 /home/mysql/data
2. centos7的selinux问题：打开/etc/selinux/config，把SELINUX=enforcing改为SELINUX=disabled后存盘退出重启机器。
查看存储过程的创建代码
show create procedure proc_name;

# ERROR 1728 (HY000): Cannot load from mysql.proc. The table is probably corrupted
【错误过程】：MySQL从5.1升级至5.5后在调用存储过程时报出“Cannot load from mysql.proc. The table is probably corrupted。”
【造成原因】：MySQL升级完成后未对相关数据库执行升级.
【解决办法】：在命令行中执行mysql_upgrade -uroot -p 即可~
# 查看Mysql端口
	show global variables like 'port';
# 字符集
## 系统变量
- character_set_server          默认的内部操作字符集
- character_set_client          客户端来源数据使用的字符集
- character_set_connection      连接层字符集
- character_set_results         查询结果字符集
- character_set_database        当前选中数据库的默认字符集
- character_set_system          系统元数据（字段名等）字符集

## 查看

	show variables like 'character_set_database';
    show variables like '%character%';
    show variables like 'collation%';
## 修改

### 临时修改
	alter database xxx CHARACTER SET gb2312; // 修改库的字符集
    SET character_set_client = utf8;

### 永久修改
修改my.cnf 位置在/etc/my.cnf
在[mysqld]上面加下面两句话
    
    [client]
    default-character-set=utf8

在[mysqld]最下面加入下面几句话

    default-storage-engine=INNODB
    character-set-server=utf8
    collation-server=utf8_general_ci

然后保存

重启Mysql

    service mysqld restart

## 查看MYSQL所支持的字符集

    show charset;

## 查看库的字符集

    show create database db_xxsy_game_1004\G;

## 查看表的字符集

    show table status from db_xxsy_game_1004 like 'common_accounts';
# CentOS7 开放3306端口访问
CentOS 7.0默认使用的是firewall作为防火墙，这里改为iptables防火墙。
1、关闭firewall：
systemctl stop firewalld.service
systemctl disable firewalld.service
systemctl mask firewalld.service

2、安装iptables防火墙
yum install iptables-services -y

3.启动设置防火墙

	# systemctl enable iptables
	# systemctl start iptables

4.查看防火墙状态

systemctl status iptables

5编辑防火墙，增加端口
vi /etc/sysconfig/iptables #编辑防火墙配置文件
-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 3306 -j ACCEPT
:wq! #保存退出

3.重启配置，重启系统
systemctl restart iptables.service #重启防火墙使配置生效
systemctl enable iptables.service #设置防火墙开机启动
# Host '192.168.171.128' is not allowed to connect to this MySQL server

# ERROR 1044 (42000):Access denied for user ''@'localhost' to database 'mysql'
在mysql数据库的user表里，存在用户名为空的账户即为匿名账户，导致登陆的时候虽然用的时root，但实际时匿名登陆的。
解决办法：

1. 关闭mysql
service mysqld stop
2. 屏蔽权限
mysqld_safe --skip-grant-table
3. 新开一个终端
mysql -u root mysql
delete from user where user='';
flush privileges;

# mysql.h no found file or directory
yum install mysql-devel

|数据类型|解释|存储范围|字节|
|:--|:--|:--|:--|
|TINYINT|有符号值:-128到127<br>unsigned : [0 255]|1|
|SMALLINT|signed[-32768,32767]<br>unsigned [0,65535]|2|
|BINARY|用来表示二进制数据||
|VARBINARY|

大小写忽略

### CHAR和VARCHAR类型

都用来存储字符串
VARCHAR 可以存储可变长度的字符串
CHAR    存储定长字符串
定义如下
CHAR(M)或者VARCHAR(M)
M指的是字符串的最大长度
比如
插入值'ab'
CHAR(4) 需要4个字节
VARCHAR(4) 需要3个字节
当数据为CHAR(M)类型时，不管插入值的长度是多少，所占用的存储空间都是M个字节。
当为VARCHAR(M)类型时，所占用的字节数为实际长度加1。

### BINARY和VARBINARY类型

类似于CHAR和VARCHAR
用来表示二进制数据
定义如下
BINARY(M)或者VARBINARY(M)
M指的是二进制数据的最大字节长度
注意
BINARY类型的长度是固定的，如果数据的长度不足最大长度，在后面用"\0"补齐。
比如，数据类型为BINARY(3)，当插入a时，实际存储的时"a\0\0"


# Centos7安装Mysql

    yum install mysql
    yum install mysql-devel
    yum install mariadb-server mariadb
    systemctl start mariadb

