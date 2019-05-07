---
title: Unity打包详细流程
date: 2018-03-13 18:00:36
tags:
	Unity
---

打包策略：按照目录划分
目录进行分类
配置文件如下：
```
{
	"AssetBundleInfo": [
		{
			"assetBundleName": "ui.bytes",
			"dirPrefix": [
				"Art/ResExport/GUI/",
				"Art/GUI/Atlas/",
				"res/GUI/res/",
				"res/GUI/"
			]
		},
		{
			"assetBundleName": "ui_font.bytes",
			"dirPrefix": [
				"res/GUI/res/Font/"
			]
		},
		{
			"assetBundleName": "particles.bytes",
			"dirPrefix": [
				"res/Particles/",
				"Art/Particles/"
			]
		},
		{
			"assetBundleName": "gfx.bytes",
			"dirPrefix": [
				"res/FX/",
				"res/SkillWarning/",
				"res/Shader/",
				"PluginsScripts/NGUI/Resources/Shaders/",
				"Art/Graphics/shader/"
			]
		},
		{
			"assetBundleName": "misc.bytes",
			"dirPrefix": [
				"Art/",
				"res/",
				"Resources/",
				"Plugins/",
				"PluginsScripts/",
				"Models/"
			]
		},
		{
			"assetBundleName": .bytes",
			"dirPrefix": [
				"Scenes/Assets Skyboxes/"
			]
		},
		{
			"assetBundleName": "scenes.bytes",
			"dirPrefix": [
				"res/Scenes/",
				"Scenes/"
			]
		}
	],
	"PackageInfo": [
		{
			"pkgName": "cfg",
			"dirPrefix": [
				"cfg/"
			]
		},
		{
			"pkgName": "lang",
			"dirPrefix": [
				"Assets/Art/lang"
			]
		},
		{
			"pkgName": "code",
			"dirPrefix": [
				"cfg/",
				"mb/",
				"Lua/"
			]
		},
		{
			"pkgName": "gfx",
			"dirPrefix": [
				"FX/",
				"MaterialEx/",
				"Shader/",
				"SkillWarning/",
				"Textures/",
				"Still/",
				"bones/",
				"weapons/",
				"Equips/"
			]
		},
		{
			"pkgName": "gui",
			"dirPrefix": [
				"GUI/"
			]
		},
		{
			"pkgName": "particles",
			"dirPrefix": [
				"Particles/",
				"Assets/Art/Particles/"
			]
		},
		{
			"pkgName": "scenes",
			"dirPrefix": [
				"Scenes/",
				"scn/"
			]
		},
		{
			"pkgName": "sounds",
			"dirPrefix": [
				"Sounds/",
				"Assets/Art/Sounds/"
			]
		},
		{
			"pkgName": "animations",
			"dirPrefix": [
				"Animations/"
			]
		},
		{
			"pkgName": "scenes_mesh",
			"dirPrefix": [
				"Assets/Art/Scene/_Mesh/"
			]
		},
		{
			"pkgName": "scenes_assets",
			"dirPrefix": [
				"Assets/Scenes/"
			]
		},
		{
			"pkgName": "models",
			"dirPrefix": [
				"Assets/Models/"
			]
		},
		{
			"pkgName": "misc",
			"dirPrefix": [
				"Assets/"
			]
		},
		{
			"pkgName": "pandora",
			"dirPrefix": [
				"pandora/"
			]
		}
	]
}
```
不使用Resources的原因是，打包程序的时候会将Resources目录下的全部文件都加密压缩达到包内，这样如果我们想使用assetbundle方式打小包就不行了（比如更新包）。
1.多语言文件先导入
2.根据配置文件生成包信息，AssetBundleInfo,AssetBundleIndex
3.处理Res目录资源
4.AssetDatabase.GetDependencies
```CS
string nameParam = new string[1];
string[] GetDirectDeps(string uri)
{
	DepInfo depInfo = new DepInfo();
	nameParam[0] = uri;
	var deps = AssetDatabase.GetDependenices(nameParem, true);//查找这个资源引用了哪些资源,cs和js文件不用处理
	depInfos[uri] = depInfo;//global
	return deps;
}
string[] MakeDepends(string uri)
{
	var deps = GetDirectDeps(uri);
}
static void addPkgFile(string filePath, bool isCompress, string accessPath, byte fileType, byte loadType)
{
	accessPath = accessPath.ToLower();
	PkgInfo.FileInfo fileInfo = NewPkgFileInfo(filePath, isCompress, accessPath, )
}
public IEnumerator ProcessSounds()
{
	titleText = "处理音频";
	yield return null;

	string soundsDir = Application.dataPath + "/Art/Sounds";
	if (System.IO.Exists(soundsDir))
	{
		int rootSize = Application.dataPath.Length - 6;//Assets
		string[] file = System.IO.Directory.GetFiles(soundsDir, "*.bank", SearchOption.AllDirectories);
		foreach (var file in files)
		{
			string path = file.SubString(rootSize).ToLower().Replace('\\', '/');
			addPkgFile(path, false, 0, 0);
			addFileProcessed.Add(path);
		}
	}
}
public IEnumerator DoProcess()
{
	yield return ProcessRes(resUri);
	if (!isRuning) yield return;

	yield return ProcessSounds();//smod,因为unity自带的容易崩溃
}
```