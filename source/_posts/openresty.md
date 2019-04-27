---
title: openresty
date: 2018-03-18 14:03:37
tags:
	- openresty
---
/usr/local/openresty/luajit:luajit环境，luajit类似于java的jit,即即时编译，lua是一种解释语言，通过luajit可以即时编译lua代码到机器代码，得到很好的性能。
/usr/local/openresty/lualib:要使用的lua库，里边提供了一些默认的lua库，如redis,json库等，也可以把自己开发的或者第三方放在这；
/usr/local/openresty/nginx:安装的nginx

/usr/local/openresty/nginx/sbin/nginx -V 	查看nginx版本和安装的模块

#启动 nginx
	nginx -p `pwd`/ -c conf/nginx.conf

#重启nginx
	/usr/local/openresty/nginx/sbin/nginx -s reload

# nginx.pid no file
nginx -c /home/www/conf/nginx.conf

# add_header directive is not allowed here in
