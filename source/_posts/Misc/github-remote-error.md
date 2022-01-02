---
layout: _post
title: fatal:remote error You can't push to git 的解决办法
date: 2017-10-13 13:34:04
tags:
  - Git
---
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
