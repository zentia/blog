---
title: Git 服务器搭建
date: 2017-10-13 15:43:25
tags:
  - Git
---
首先要明确现在代码的分布情况，开发者电脑上的本地仓库，git服务器上的远端仓库，web服务器上的另一个本地仓库，我们浏览器访问的就是这里的代码。其实自动部署就是要当开发者本地仓库提交代码到远端仓库的时候，自动把代码部署到web服务器的本地仓库，实现开发着本地仓库和web服务器的本地仓库同步。（即通过GitHook中的post-receive脚本文件）
# 1、安装Git
```
yum install curl-devel expat-devel gettext-devel openssl-devel zlib-devel perl-devel
yum install git
```
接下来我们创建一个git用户组和用户，用来运行git服务：
```
groupadd git
adduser git -g git
```

# 3、初始化Git仓库
```
$ git init --bare runoob.git
Initialized empty Git repository in /home/gitrepo/
```
git init 使用当前目录作为Git仓库,该命令执行完后回生成一个.git目录
git init zentia 指定目录创建Git仓库

修改/etc/ssh/sshd_config文件，将其中的PermitRootLogin no修改为yes,PublicAuthentication yes修改为no,AuthorizedKeysFile .ssh/authorized_keys前面加上的#屏蔽掉,PasswordAuthentication no修改为yes就可以了
ssh -l username hostname

在服务器checkout出一份代码
```
# In remote server
cd ~
git clone sample.git/ my_repo
```
这个时候就会出现my_repo文件夹，里面就会有你的代码。同时，这个代码文件夹的默认origin远端（remote）就会变成my_repo文件夹。当你做git fetch git pull等动作的时候就会从my_repo取信息。
3.配置Git Hook
进入到/home/gitrepo/sample.git文件夹，使用vi post-receive创建一个脚本，当你在本地仓库执行git push后就会触发post-receive。

/usr/games/project.git/
git clone git@ec2-18-191-158-223.us-east-2.compute.amazonaws.com:/usr/games/project.git/
添加到本地仓库

    git add .

添加提交描述

    git commit -m ‘内容’

提交前先从远程仓库🐖分支中拉取请求

    git pull origin master

把本地仓库代码提交

    git push -u origin master

查看远程仓库地址

    git remote -v