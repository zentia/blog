---
title: git笔记
date: 2017-10-12 14:58:01
tags:
  - Git
---

# warning:LF will be replaced by CRLF问题解决方法

CRLF -- Carriage-Return Line-Feed回车(ASCLL 13, \r)换行(LF, ASCLL 10, \n)。
解决方法
```javascript
git config --global core.autocrlf false
```