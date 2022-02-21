---
title: ssh
date: 2017-10-13 14:42:00
tags:
  - ssh
---
ssh中，有两个钥匙：***公钥、私钥***，公钥主要是用于对一些敏感信息进行加密，私钥适用于解密。
ssh的文件都存在于机器~/.ssh中。

- 客户端：id_rsa(私钥)、id_rsa.pub(公钥)、known_hosts(已知远程主机)
- 服务端: authorized_keys(验证过的公钥列表)、sshd_config(ssh配置文件)

# 设置ssh连接不断开

	vim /etc/ssh/sshd_config
	# 客户端每隔多少秒向服务器发送一个心跳数据
	ClientAliveInterval 30
	# 客户端多少秒没有响应，服务器自动断掉连接
	ClientAliveCountMax 1800
	service sshd restart