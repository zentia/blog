---
title: git笔记
date: 2017-10-12 14:58:01
categories:
- Git
---

# warning:LF will be replaced by CRLF问题解决方法

CRLF -- Carriage-Return Line-Feed回车(ASCLL 13, \r)换行(LF, ASCLL 10, \n)。
解决方法
```javascript
git config --global core.autocrlf false
```

# 切换http

	git remote set-url origin https://github.com/zentia/blog.git

branch正确的情况下，不知为什么，在push的时候出现:
```javascript
fatal: remote error:
You can't push to git://github.com/zentia/hexo.git
Use git@github.com:zentia/hexo.git
```
解决办法:
```javascript
git remote rm origin
git remote add origin git@github.com:zentia/hexo.git
git push origin master
```
