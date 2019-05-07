---
layout: _post
title: 初始化云服务器
date: 2017-10-11 00:31:02
tags:
  - Server
---

# 搭建git服务器
yum install -y git
adduser git //创建用户git
git clone git@10.173.32.7:/home/git/sample.git //克隆远程仓库
# clone git仓库
# 上传代码
git commit -am "<message>", 将所有修改，但未进stage的改动加入stage,并记录commit信息（某种程度上相当于git add 和 git commit -m的组合,前提是被改动文件已经是tracked)
```javascript
find ./ -type d -name "del_dir" -exec rm -r {} \;
```
注意中间的空格
