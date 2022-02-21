---
title: netstat
date: 2017-10-26 20:45:23
tags:
  - linux
---
1. netstat -tln 查看端口占用情况\
p 可以显示进程
每一列含义
协议类型  接受流量 发送流量 本机地址 目的地址 状态 
自己的IP地址就是本地地址，需要连接的地址的就是目的地址

nmap 127.0.0.1 查看本机开放的端口
0.0.0.0 表示网络的所有主机
[{"type":0,"pid":-1,"Attr1":0,"id":1,"Attr2":0,"name":"PC主支","tag":0},{"type":0,"pid":1,"Attr1":0,"id":2,"Attr2":0,"name":"策划大区","tag":1},{"type":0,"pid":1,"Attr1":0,"id":3,"Attr2":0,"name":"程序大区","tag":1},{"type":0,"pid":1,"Attr1":0,"id":4,"Attr2":0,"name":"3.0.90.0","tag":1},{"type":0,"pid":1,"Attr1":0,"id":22,"Attr2":0,"name":"4.0.30.0","tag":1},{"type":0,"pid":1,"Attr1":0,"id":28,"Attr2":0,"name":"云服务器","tag":1},{"type":0,"pid":1,"Attr1":0,"id":34,"Attr2":0,"name":"4.0.0.0","tag":1},{"type":0,"pid":1,"Attr1":0,"id":35,"Attr2":0,"name":"3.0.60.0","tag":1},{"type":0,"pid":1,"Attr1":0,"id":41,"Attr2":0,"name":"合服","tag":1},{"type":1,"pid":2,"id":381,"RoleCount":0,"RoleInfos":{},"name":"[1]策划主支","Attr1":4,"Flag":64,"Url":"192.168.3.6:16400","Attr2":0,"tag":3},{"type":1,"pid":2,"id":382,"RoleCount":0,"RoleInfos":{},"name":"[2]策划主支","Attr1":3,"Flag":64,"Url":"192.168.6.113:16400","Attr2":0,"tag":1},{"type":1,"pid":3,"id":256,"RoleCount":0,"RoleInfos":{},"name":"[1]程序主支","Attr1":2,"Flag":64,"Url":"192.168.3.6:26400","Attr2":0,"tag":3},{"type":1,"pid":3,"id":257,"RoleCount":0,"RoleInfos":{},"name":"[2]程序主支","Attr1":1,"Flag":64,"Url":"192.168.6.113:26400","Attr2":0,"tag":1},
{"type":1,"pid":3,"id":259,"RoleCount":0,"RoleInfos":{},"name":"yhy-1","Attr1":0,"Flag":32,"Url":"192.168.7.13:3100 192.168.7.13:3110","Attr2":0,"tag":1},
{"type":1,"pid":3,"id":53,"RoleCount":0,"RoleInfos":{},"name":"yhy-2","Attr1":0,"Flag":32,"Url":"192.168.7.13:13100","Attr2":0,"tag":1},
{"type":1,"pid":3,"id":19,"RoleCount":0,"RoleInfos":{},"name":"218(zy)","Attr1":0,"Flag":32,"Url":"192.168.5.106:3100","Attr2":0,"tag":1},
{"type":1,"pid":3,"id":20,"RoleCount":0,"RoleInfos":{},"name":"190(zmq)","Attr1":0,"Flag":32,"Url":"192.168.5.228:26400","Attr2":0,"tag":1},
{"type":1,"pid":3,"id":21,"RoleCount":0,"RoleInfos":{},"name":"230(hjz)","Attr1":0,"Flag":32,"Url":"10.96.205.230:26400","Attr2":0,"tag":1},
{"type":1,"pid":3,"id":310,"RoleCount":0,"RoleInfos":{},"name":"测试专服","Attr1":0,"Flag":64,"Url":"192.168.6.113:7100","Attr2":0,"tag":1},
{"type":1,"pid":3,"id":268,"RoleCount":0,"RoleInfos":{},"name":"主支跨服大区","Attr1":0,"Flag":64,"Url":"192.168.3.6:10010","Attr2":0,"tag":1},
{"type":1,"pid":4,"id":93,"RoleCount":0,"RoleInfos":{},"name":"[1]3.0.90.0","Attr1":0,"Flag":64,"Url":"192.168.3.6:51100","Attr2":0,"tag":3},
{"type":1,"pid":4,"id":94,"RoleCount":0,"RoleInfos":{},"name":"[2]3.0.90.0","Attr1":0,"Flag":64,"Url":"192.168.6.113:51100","Attr2":0,"tag":1},
{"type":1,"pid":22,"id":95,"RoleCount":0,"RoleInfos":{},"name":"[1]4.0.30.0","Attr1":0,"Flag":64,"Url":"192.168.3.6:43100","Attr2":0,"tag":1},
{"type":1,"pid":22,"id":96,"RoleCount":0,"RoleInfos":{},"name":"[2]4.0.30.0","Attr1":0,"Flag":64,"Url":"192.168.6.113:43100","Attr2":0,"tag":1},{"type":1,"pid":28,"id":5999,"RoleCount":0,"RoleInfos":{},"name":"IOS游客","Attr1":0,"Flag":64,"Url":"139.199.37.182:48100","Attr2":0,"tag":1},{"type":1,"pid":28,"id":4999,"RoleCount":0,"RoleInfos":{},"name":"IOS微信","Attr1":0,"Flag":64,"Url":"139.199.37.182:53100","Attr2":0,"tag":1},{"type":1,"pid":28,"id":3999,"RoleCount":0,"RoleInfos":{},"name":"IOS手Q","Attr1":0,"Flag":64,"Url":"139.199.37.182:33100","Attr2":0,"tag":1},{"type":1,"pid":28,"id":1999,"RoleCount":0,"RoleInfos":{},"name":"安卓手Q","Attr1":0,"Flag":64,"Url":"139.199.37.182:23100","Attr2":0,"tag":1},{"type":1,"pid":28,"id":2999,"RoleCount":0,"RoleInfos":{},"name":"安卓微信","Attr1":0,"Flag":64,"Url":"139.199.37.182:28100","Attr2":0,"tag":1},{"type":1,"pid":34,"id":97,"RoleCount":0,"RoleInfos":{},"name":"[1]4.0.0.0","Attr1":0,"Flag":64,"Url":"192.168.3.6:20400","Attr2":0,"tag":1},{"type":1,"pid":34,"id":98,"RoleCount":0,"RoleInfos":{},"name":"[2]4.0.0.0","Attr1":0,"Flag":64,"Url":"1921.68.6.113:20400","Attr2":0,"tag":1},{"type":1,"pid":35,"id":90,"RoleCount":0,"RoleInfos":{},"name":"[1]3.0.60.0","Attr1":6,"Flag":64,"Url":"192.168.3.6:6400","Attr2":0,"tag":1},{"type":1,"pid":35,"id":91,"RoleCount":0,"RoleInfos":{},"name":"[2]3.0.60.0","Attr1":5,"Flag":64,"Url":"192.168.6.113:6400","Attr2":0,"tag":1},{"type":1,"pid":41,"id":311,"RoleCount":0,"RoleInfos":{},"name":"合服后(3014+3016)","Attr1":0,"Flag":64,"Url":"192.168.6.113:7100","Attr2":0,"tag":9},{"type":1,"pid":41,"id":312,"RoleCount":0,"RoleInfos":{},"name":"3014","Attr1":0,"Flag":64,"Url":"192.168.6.113:23100","Attr2":0,"tag":9},{"type":1,"pid":41,"id":313,"RoleCount":0,"RoleInfos":{},"name":"3016","Attr1":0,"Flag":64,"Url":"192.168.6.113:37100","Attr2":0,"tag":9}]
