---
title: Hexo Manual
date: 2017-10-12 16:58:15
tags:
  - Hexo
---
# 安装

	npm install -g hexo-cli

如果提示timeout的话，使用下面指令

	npm config delete proxy

# 关闭背景动画
背景动画基于canvas，个人感觉比较耗，毕竟i3，所以关闭，在\themes\next\_config.yml中修改字段
```javascript
three_waves: false
```

# 嵌入PDF
npm install --save hexo-pdf
添加

	hexo deploy -- 发布
编辑主题的 source/css/_variables/custom.styl 文件，新增变量：
```
// 修改成你期望的宽度
$content-desktop = 700px

// 当视窗超过 1600px 后的宽度
$content-desktop-large = 900px
```

发布：

	hexo d -g

# Insert Image
```
{% asset_img example.jpg This is an example image %}
```

# Font

```
<font size = 4 color=red></font> # 字体大小颜色设置
```

- **列表嵌套**
   + 上一级和下一级之间敲三个空格即可

用pass-by-refrence-to-const替换pass-by-value
内置类型，STL迭代器，和函数对象采用pass-by-value
不要返回pointer或者reference指向一个on stack对象（被析构）
不要返回pointer或者reference指向一个on heap对象（需要用户delete）
不要返回pointer或者reference指向local static对象
切记将成员变量声明为private
protected并不比public更有封装性（其实我不是很喜欢封装这个东西）
多一个成员函数，就多一分破坏封装性
若所有参数都需要类型转换，采用non-member函数（member类型转换不行吗？）

|||
|--|--|--|
|腾讯开发者平台|http://zentia.coding.me|git@git.dev.tencent.com:Zentia/Hexo.git   |
|github        |http://zentia.github.io     |git@github.com:zentia/zentia.github.io.git|
|码云          |http://zentia.gitee.io      |git@gitee.com:zentia/zentia.git           |

# 修复图片展示
新建博文，设置type: "picture"，使用{\% gp x-x \%} ... {\% endgp \%}标签引用要展示的图片地址，如下所示：
```
---
title: Naruto-Pictures
categories: [图片]
tags: [picture,naruto]
date: 2016-09-02 14:36:04
keywords: picture,naruto
type: "picture"
top: 999
---
{% gp 5-3 %}
![](https://cdn.ehlxr.top/post/18210.jpg)
![](https://cdn.ehlxr.top/post/196232.jpg)
![](https://cdn.ehlxr.top/post/224147.jpg)
![](https://cdn.ehlxr.top/post/199301.jpg)
![](https://cdn.ehlxr.top/post/213318.jpg)
{% endgp %}
```
{\% gp 5-3 \%}：设置图片展示效果，参考 theme/next/scripts/tags/group-pictures.js 注释示意图。
主题目前首页可以正常显示步骤 8.2 设置的图片效果，但是点击进入后显示效果丢失，所以需要修改一下文件 themes\next\source\css\_common\components\tags\group-pictures.styl 中的以下样式：
```
.page-post-detail .post-body .group-picture-column {
  // float: none;
  margin-top: 10px;
  // width: auto !important;
  img { margin: 0 auto; }
}
```

# algolia
由于zentia.site这个域名没有备案，导致一直无法使用，然后只能使用zentia.github.io。然后一直没有更新algolia导致搜索还是访问之前的源，这个问题痛苦了好久。然后今天解决

    hexo algolia

可能会提示"ERROR [hexo-algolia] Please set an `HEXO_ALGOLIA_INDEXING_KEY` environment varia"这个问题，然后"export HEXO_ALGOLIA_INDEXING_KEY='f4ca7ae2408d2e27ffc8269ad8d34273'"这样就解决了。