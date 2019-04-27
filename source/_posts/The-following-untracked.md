---
title: The following untracked working tree files would be overwritten by merge
date: 2017-10-12 14:53:23
tags:
  - git
---
git pull的时候遇到的问题。
解决办法：
git clean -d -fx
note:会删除没有add到仓库的文件，操作记得慎重，以免改动文件的丢失。

