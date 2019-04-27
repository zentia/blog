---
title: Centos7下 vim
date: 2017-10-14 12:31:04
tags:
  - vi
---
```javascript
yum -y install vim
```
# 环境配置
1. /etc/vimrc 全局
2. vim ~/.vimrc 新建
```javascript
set nu // 行号
set showmode //显示当前模式
set ruler //显示行数信息
set autoindent //回车对齐
syntax on //语法检测
set encoding=urf-8 fileencodings=ucs-born,utf-8,cp936 //自动识别编码，如果没有找到，用latin-1(ASCII)编码打开(解决乱码问题)
set ts=4 //四个空格
```
vi查看文件编码格式
	:set fileencoding
:edit ++enc=utf-8

最后来看看处理中文文档最典型的几种情况和设置方式：

1，系统locale是utf-8（很多linux系统默认的locale形式），编辑的文档是GB2312或GBK形式的（Windows记事本 默认保存形式，大部分编辑器也默认保存为这个形式，所以最常见），终端类型utf-8（也就是假定客户端是putty类的unicode软件）

则vim打开文档后，encoding=utf-8（locale决定的），fileencoding=latin1（自动编码判断机制不准导致的），termencoding=空（默认无需转换term编码），显示文件为乱码。

解决方案1：首先要修正fileencoding为cp936或者euc-cn（二者一样的，只不过叫法不同），注意修正的方法不是:set fileencoding=cp936，这只是将文件保存为cp936，正确的方法是重新以cp936的编码方式加载文件为:edit ++enc=cp936，可以简写为:e ++enc=cp936。

解决方案2：临时改变vim运行的locale环境，方法是以LANG=zh_CN vim abc.txt的方式来启动vim，则此时encoding=euc-cn（locale决定的），fileencoding=空（此locale下文件 编码自动判别功能不启用，所以fileencoding为文件本身编码方式不变，也就是euc-cn），termencoding=空（默认值，为空则等 于encoding）此时还是乱码的，因为我们的ssh终端认为接受的数据为utf-8，但vim发送数据为euc-cn，所以还是不对。此时再用命令: set termencoding=utf-8将终端数据输出为utf-8，则显示正常。

2，情况与1基本相同，只是使用的ssh软件为secure CRT类ansi类软件。

vim打开文档后，encoding=utf-8（locale决定的），fileencoding=latin1（自动编码判断机制不准导致的），termencoding=空（默认无需转换term编码），显示文件为乱码。

解决方案1：首先要保证运行secure CRT的windows机器的默认代码页为CP936，这一点中文windows已经是默认设置了。其他的与上面方案1相同，只是要增加一步，:set termencoding=cp936

解决方案2：与上面方案2类似，不过最后一步修改termencoding省略即可，在此情况下需要的修改最少，只要以locale为zh_CN开 启vim，则encoding=euc-cn，fileencoding和termencoding都为空即为encoding的值，是最理想的一种情 况。

可见理解这3个关键点和3个参数的意义，对于编码问题有很大助力，以后就可以随心所欲的处理文档了，同时不仅仅是应用于vim，在其他需要编码转换的环境里，都可以应用类似的思路来处理问题解决问题。

# 行跳转
1. 12gg / 12G
2. :12
3. 打开文件时输入vim +12 filename

# vim 精确匹配查找单词
精确匹配查找单词
如果你输入 "/the"，你也可能找到"there"。要找到以"the"结尾的单次，可以用: /the\> "\>"是一个特殊的记号，表示只匹配单词末尾。类似的，"\<" 只匹配单词的开头。
这样要匹配一个完整的单词"the"，只需要: /\<the\>

:e 刷新文件内容