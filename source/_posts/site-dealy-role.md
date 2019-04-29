---
layout: _post
title: 网站部署规则
date: 2017-10-13 14:05:21
tags:
---
网站发布只在自己笔记本电脑发布，公司电脑只能hexo s --debug,不能 hexo d -g
本地内容修改完成之后记得
```javascript
git add . 
git commit -am "m"
git push origin master
```