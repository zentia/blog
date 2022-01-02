---
title: 日记
date: 2017-10-30 16:36:35
tags:
  - 日记
---

SetUseAuth
由于个人喜欢sublime也在使用，遇到不支持ANSI编码，特标注：ConvertToUtf8
	import urllib.request,os; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); open(os.path.join(ipp, pf), 'wb').write(urllib.request.urlopen( 'http://sublime.wbond.net/' + pf.replace(' ','%20')).read())
上面的是安装package controls

	grep key1 * | grep key2 并所有文件
	find ./ -name "*.ext" | xargs -i rm -rf {} # 查询某个后缀名的所有文件，然后移除，-i表示对文件操作

解决树冲突：

	svn resolve --accept working dir

https://github.com/Unity-Technologies/UnityCsReference

README.md
Unity 2018.1.0b12 C# reference source code

The C# part of the Unity engine and editor source code.May be used for reference purposes only.

https://github.com/duanjiahao/UnityDecompiled

https://github.com/MattRix/UnityDecompiled

https://github.com/jameslinden/unity-decompiled


|IP				|Username 	|Password		|Environment|
|---------------|-----------|---------------|-----------|
|106.12.98.12	|root		|lyf8068353A@	|Outter net	|
虚拟机Centos的密码都是123456789
Mac的密码都是123456
PC的密码是123456