---
title: Apache
date: 2017-10-27 10:08:03
tags:
  - server
---

```
[1] 安装 httpd.
[root@linuxprobe ~]# yum -y install httpd
# 删除默认欢迎页面
[root@linuxprobe ~]# rm -f /etc/httpd/conf.d/welcome.conf
[2] 配置httpd，将服务器名称替换为您自己的环境
[root@linuxprobe ~]# vim /etc/httpd/conf/httpd.conf
# line 86: 改变管理员的邮箱地址
ServerAdmin root@linuxprobe.org
# line 95: 改变域名信息
ServerName www.linuxprobe.org:80
# line 151: none变成All
AllowOverride All
# line 164: 添加只能使用目录名称访问的文件名
DirectoryIndex index.html index.cgi index.php
# add follows to the end
# server's response header（安全性）
ServerTokens Prod
# keepalive is ON
KeepAlive On
[root@linuxprobe ~]# systemctl start httpd
[root@linuxprobe ~]# systemctl enable httpd
[3] 如果Firewalld正在运行，请允许HTTP服务。，HTTP使用80 / TCP
[root@linuxprobe ~]# firewall-cmd --add-service=http --permanent
success
[root@linuxprobe ~]# firewall-cmd --reload
success
[4] 创建一个HTML测试页，并使用Web浏览器从客户端PC访问它。如果显示以下页面，是正确的
[root@linuxprobe ~]# vi /var/www/html/index.html
<html>
<body>
<div style="width: 100%; font-size: 40px; font-weight: bold; text-align: center;">
Welcome access LinuxProbe.org,This is Test Page!
</div>
</body>
</html>
```
http://blog.csdn.net/wh211212/article/details/52982917