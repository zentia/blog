---
title: Unity-2018
mathjax: true
date: 2019-03-30 22:42:25
tags:
	- Unity2018
categories: Unity2018
---
Shader中使用"UsePass"引用同一个Shader的另外一个SubShader的Pass会导致崩溃，这个是Bug还是就是不能这么写？我使用的版本是2017.4.6f1。例如这样：
```C
Shader "Unlit/NewUnlitShader 1"
{
	Properties
	{
		_MainTex("Texture". 2D) = "white"{}
	}
	SubShader
	{
		Tags{"RenderType"="Oparque"}
		LOD 100

		UsePass "Unlit/NewUnlitShader 1/TEST"

		Pass
		{
			Name "test"
		}
	}
}
```
UsePass主要是用来实现不同的Shader中的代码复用的，查看文档的话也可以看到相关描述：
Some of the shaders conld reuse existing passes from other shaders, reducing code duplication
所以不要用UsePass引用同一个Shader中的Pass
