---
title: Unity打包详细流程
date: 2018-03-13 18:00:36
categories:
- Engine
- Unity
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

# 打包常见问题整理
```
CommandInvokationFailure: Gradle build failed. 
C:\Program Files\Unity_2018.3.0f2\Editor\Data\PlaybackEngines\AndroidPlayer/Tools\OpenJDK\Windows\bin\java.exe -classpath "C:\Program Files\Unity_2018.3.0f2\Editor\Data\PlaybackEngines\AndroidPlayer\Tools\gradle\lib\gradle-launcher-4.6.jar" org.gradle.launcher.GradleMain "-Dorg.gradle.jvmargs=-Xmx4096m" "assembleRelease"

stderr[

FAILURE: Build failed with an exception.

* What went wrong:
A problem occurred configuring root project 'gradleOut'.
> Could not resolve all artifacts for configuration ':classpath'.
   > Could not download bcpkix-jdk15on.jar (org.bouncycastle:bcpkix-jdk15on:1.56)
      > Could not get resource 'https://jcenter.bintray.com/org/bouncycastle/bcpkix-jdk15on/1.56/bcpkix-jdk15on-1.56.jar'.
         > Could not GET 'https://jcenter.bintray.com/org/bouncycastle/bcpkix-jdk15on/1.56/bcpkix-jdk15on-1.56.jar'.
            > Connection reset
   > Could not download bcprov-jdk15on.jar (org.bouncycastle:bcprov-jdk15on:1.56)
      > Could not get resource 'https://jcenter.bintray.com/org/bouncycastle/bcprov-jdk15on/1.56/bcprov-jdk15on-1.56.jar'.
         > Could not GET 'https://jcenter.bintray.com/org/bouncycastle/bcprov-jdk15on/1.56/bcprov-jdk15on-1.56.jar'.
            > Connection reset
   > Could not download fastutil.jar (it.unimi.dsi:fastutil:7.2.0)
      > Could not get resource 'https://jcenter.bintray.com/it/unimi/dsi/fastutil/7.2.0/fastutil-7.2.0.jar'.
         > Could not GET 'https://jcenter.bintray.com/it/unimi/dsi/fastutil/7.2.0/fastutil-7.2.0.jar'.
            > Connection reset
   > Could not download jimfs.jar (com.google.jimfs:jimfs:1.1)
      > Could not get resource 'https://jcenter.bintray.com/com/google/jimfs/jimfs/1.1/jimfs-1.1.jar'.
         > Could not GET 'https://jcenter.bintray.com/com/google/jimfs/jimfs/1.1/jimfs-1.1.jar'.
            > Connection reset
   > Could not download guava.jar (com.google.guava:guava:23.0)
      > Could not get resource 'https://jcenter.bintray.com/com/google/guava/guava/23.0/guava-23.0.jar'.
         > Could not GET 'https://jcenter.bintray.com/com/google/guava/guava/23.0/guava-23.0.jar'.
            > Connection reset
   > Could not download gson.jar (com.google.code.gson:gson:2.8.0)
      > Could not get resource 'https://jcenter.bintray.com/com/google/code/gson/gson/2.8.0/gson-2.8.0.jar'.
         > Could not GET 'https://jcenter.bintray.com/com/google/code/gson/gson/2.8.0/gson-2.8.0.jar'.
            > Connection reset
   > Could not download httpclient.jar (org.apache.httpcomponents:httpclient:4.5.2)
      > Could not get resource 'https://jcenter.bintray.com/org/apache/httpcomponents/httpclient/4.5.2/httpclient-4.5.2.jar'.
         > Could not GET 'https://jcenter.bintray.com/org/apache/httpcomponents/httpclient/4.5.2/httpclient-4.5.2.jar'.
            > Connection reset
   > Could not download httpcore.jar (org.apache.httpcomponents:httpcore:4.4.5)
      > Could not get resource 'https://jcenter.bintray.com/org/apache/httpcomponents/httpcore/4.4.5/httpcore-4.4.5.jar'.
         > Could not GET 'https://jcenter.bintray.com/org/apache/httpcomponents/httpcore/4.4.5/httpcore-4.4.5.jar'.
            > Connection reset
   > Could not download kotlin-reflect.jar (org.jetbrains.kotlin:kotlin-reflect:1.2.0)
      > Could not get resource 'https://jcenter.bintray.com/org/jetbrains/kotlin/kotlin-reflect/1.2.0/kotlin-reflect-1.2.0.jar'.
         > Could not GET 'https://jcenter.bintray.com/org/jetbrains/kotlin/kotlin-reflect/1.2.0/kotlin-reflect-1.2.0.jar'.
            > Connection reset
   > Could not download antlr4.jar (org.antlr:antlr4:4.5.3)
      > Could not get resource 'https://jcenter.bintray.com/org/antlr/antlr4/4.5.3/antlr4-4.5.3.jar'.
         > Could not GET 'https://jcenter.bintray.com/org/antlr/antlr4/4.5.3/antlr4-4.5.3.jar'.
            > Connection reset
   > Could not download juniversalchardet.jar (com.googlecode.juniversalchardet:juniversalchardet:1.0.3)
      > Could not get resource 'https://jcenter.bintray.com/com/googlecode/juniversalchardet/juniversalchardet/1.0.3/juniversalchardet-1.0.3.jar'.
         > Could not GET 'https://jcenter.bintray.com/com/googlecode/juniversalchardet/juniversalchardet/1.0.3/juniversalchardet-1.0.3.jar'.
            > Connection reset
   > Could not download kotlin-stdlib.jar (org.jetbrains.kotlin:kotlin-stdlib:1.2.20)
      > Could not get resource 'https://jcenter.bintray.com/org/jetbrains/kotlin/kotlin-stdlib/1.2.20/kotlin-stdlib-1.2.20.jar'.
         > Could not GET 'https://jcenter.bintray.com/org/jetbrains/kotlin/kotlin-stdlib/1.2.20/kotlin-stdlib-1.2.20.jar'.
            > Connection reset
   > Could not download proguard-base.jar (net.sf.proguard:proguard-base:6.0.3)
      > Could not get resource 'https://jcenter.bintray.com/net/sf/proguard/proguard-base/6.0.3/proguard-base-6.0.3.jar'.
         > Could not GET 'https://jcenter.bintray.com/net/sf/proguard/proguard-base/6.0.3/proguard-base-6.0.3.jar'.
            > Connection reset
   > Could not download auto-value.jar (com.google.auto.value:auto-value:1.5.2)
      > Could not get resource 'https://jcenter.bintray.com/com/google/auto/value/auto-value/1.5.2/auto-value-1.5.2.jar'.
         > Could not GET 'https://jcenter.bintray.com/com/google/auto/value/auto-value/1.5.2/auto-value-1.5.2.jar'.
            > Connection reset
   > Could not download protobuf-java.jar (com.google.protobuf:protobuf-java:3.4.0)
      > Could not get resource 'https://jcenter.bintray.com/com/google/protobuf/protobuf-java/3.4.0/protobuf-java-3.4.0.jar'.
         > Could not GET 'https://jcenter.bintray.com/com/google/protobuf/protobuf-java/3.4.0/protobuf-java-3.4.0.jar'.
            > Connection reset
   > Could not download jdom2.jar (org.jdom:jdom2:2.0.6)
      > Could not get resource 'https://jcenter.bintray.com/org/jdom/jdom2/2.0.6/jdom2-2.0.6.jar'.
         > Could not GET 'https://jcenter.bintray.com/org/jdom/jdom2/2.0.6/jdom2-2.0.6.jar'.
            > Connection reset
   > Could not download commons-compress.jar (org.apache.commons:commons-compress:1.12)
      > Could not get resource 'https://jcenter.bintray.com/org/apache/commons/commons-compress/1.12/commons-compress-1.12.jar'.
         > Could not GET 'https://jcenter.bintray.com/org/apache/commons/commons-compress/1.12/commons-compress-1.12.jar'.
            > Connection reset
   > Could not download commons-codec.jar (commons-codec:commons-codec:1.9)
      > Could not get resource 'https://jcenter.bintray.com/commons-codec/commons-codec/1.9/commons-codec-1.9.jar'.
         > Could not GET 'https://jcenter.bintray.com/commons-codec/commons-codec/1.9/commons-codec-1.9.jar'.
            > Connection reset
   > Could not download jaxb-runtime.jar (org.glassfish.jaxb:jaxb-runtime:2.2.11)
      > Could not get resource 'https://jcenter.bintray.com/org/glassfish/jaxb/jaxb-runtime/2.2.11/jaxb-runtime-2.2.11.jar'.
         > Could not GET 'https://jcenter.bintray.com/org/glassfish/jaxb/jaxb-runtime/2.2.11/jaxb-runtime-2.2.11.jar'.
            > Remote host closed connection during handshake
   > Could not download FastInfoset.jar (com.sun.xml.fastinfoset:FastInfoset:1.2.13)
      > Could not get resource 'https://jcenter.bintray.com/com/sun/xml/fastinfoset/FastInfoset/1.2.13/FastInfoset-1.2.13.jar'.
         > Could not GET 'https://jcenter.bintray.com/com/sun/xml/fastinfoset/FastInfoset/1.2.13/FastInfoset-1.2.13.jar'.
            > Connection reset

* Try:
Run with --stacktrace option to get the stack trace. Run with --info or --debug option to get more log output. Run with --scan to get full insights.

* Get more help at https://help.gradle.org

BUILD FAILED in 5m 15s
]
stdout[
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/gradle/3.2.0/gradle-3.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/lint/lint-gradle-api/26.2.0/lint-gradle-api-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/androidx/databinding/databinding-compiler-common/3.2.0/databinding-compiler-common-3.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/transform-api/2.0.0-deprecated-use-gradle-api/transform-api-2.0.0-deprecated-use-gradle-api.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/builder/3.2.0/builder-3.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/gradle-api/3.2.0/gradle-api-3.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/analytics-library/shared/26.2.0/shared-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/bundletool/0.5.0/bundletool-0.5.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/jetifier/jetifier-core/1.0.0-alpha10/jetifier-core-1.0.0-alpha10.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/jetifier/jetifier-processor/1.0.0-alpha10/jetifier-processor-1.0.0-alpha10.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/analytics-library/crash/26.2.0/crash-26.2.0.pom
Download https://jcenter.bintray.com/org/ow2/asm/asm-analysis/6.0/asm-analysis-6.0.pom
Download https://jcenter.bintray.com/org/ow2/asm/asm/6.0/asm-6.0.pom
Download https://jcenter.bintray.com/org/jetbrains/kotlin/kotlin-stdlib-jre8/1.2.0/kotlin-stdlib-jre8-1.2.0.pom
Download https://jcenter.bintray.com/net/sf/jopt-simple/jopt-simple/4.9/jopt-simple-4.9.pom
Download https://jcenter.bintray.com/org/ow2/asm/asm-commons/6.0/asm-commons-6.0.pom
Download https://jcenter.bintray.com/net/sf/proguard/proguard-gradle/6.0.3/proguard-gradle-6.0.3.pom
Download https://jcenter.bintray.com/org/ow2/asm/asm-util/6.0/asm-util-6.0.pom
Download https://jcenter.bintray.com/org/ow2/asm/asm-parent/6.0/asm-parent-6.0.pom
Download https://jcenter.bintray.com/net/sf/proguard/proguard-parent/6.0.3/proguard-parent-6.0.3.pom
Download https://jcenter.bintray.com/org/ow2/ow2/1.3/ow2-1.3.pom
Download https://jcenter.bintray.com/org/sonatype/oss/oss-parent/7/oss-parent-7.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/sdk-common/26.2.0/sdk-common-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/ddms/ddmlib/26.2.0/ddmlib-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/common/26.2.0/common-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/analytics-library/tracker/26.2.0/tracker-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/analytics-library/protos/26.2.0/protos-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/manifest-merger/26.2.0/manifest-merger-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/sdklib/26.2.0/sdklib-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/builder-model/3.2.0/builder-model-3.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/apkzlib/3.2.0/apkzlib-3.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/apksig/3.2.0/apksig-3.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/builder-test-api/3.2.0/builder-test-api-3.2.0.pom
Download https://jcenter.bintray.com/org/bouncycastle/bcpkix-jdk15on/1.56/bcpkix-jdk15on-1.56.pom
Download https://jcenter.bintray.com/org/ow2/asm/asm-tree/6.0/asm-tree-6.0.pom
Download https://jcenter.bintray.com/it/unimi/dsi/fastutil/7.2.0/fastutil-7.2.0.pom
Download https://jcenter.bintray.com/org/bouncycastle/bcprov-jdk15on/1.56/bcprov-jdk15on-1.56.pom
Download https://jcenter.bintray.com/com/googlecode/json-simple/json-simple/1.1/json-simple-1.1.pom
Download https://jcenter.bintray.com/com/squareup/javawriter/2.5.0/javawriter-2.5.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/annotations/26.2.0/annotations-26.2.0.pom
Download https://jcenter.bintray.com/com/google/guava/guava/23.0/guava-23.0.pom
Download https://jcenter.bintray.com/com/google/code/gson/gson/2.8.0/gson-2.8.0.pom
Download https://jcenter.bintray.com/com/google/code/gson/gson-parent/2.8.0/gson-parent-2.8.0.pom
Download https://jcenter.bintray.com/com/google/guava/guava-parent/23.0/guava-parent-23.0.pom
Download https://jcenter.bintray.com/org/apache/httpcomponents/httpclient/4.5.2/httpclient-4.5.2.pom
Download https://jcenter.bintray.com/org/apache/httpcomponents/httpcore/4.4.5/httpcore-4.4.5.pom
Download https://jcenter.bintray.com/org/apache/httpcomponents/httpmime/4.5.2/httpmime-4.5.2.pom
Download https://jcenter.bintray.com/org/apache/httpcomponents/httpcomponents-client/4.5.2/httpcomponents-client-4.5.2.pom
Download https://jcenter.bintray.com/org/apache/httpcomponents/httpcomponents-core/4.4.5/httpcomponents-core-4.4.5.pom
Download https://jcenter.bintray.com/org/apache/httpcomponents/project/7/project-7.pom
Download https://jcenter.bintray.com/org/apache/apache/13/apache-13.pom
Download https://jcenter.bintray.com/org/jetbrains/kotlin/kotlin-reflect/1.2.0/kotlin-reflect-1.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/databinding/baseLibrary/3.2.0/baseLibrary-3.2.0.pom
Download https://dl.google.com/dl/android/maven2/androidx/databinding/databinding-common/3.2.0/databinding-common-3.2.0.pom
Download https://jcenter.bintray.com/commons-io/commons-io/2.4/commons-io-2.4.pom
Download https://jcenter.bintray.com/com/squareup/javapoet/1.8.0/javapoet-1.8.0.pom
Download https://jcenter.bintray.com/org/antlr/antlr4/4.5.3/antlr4-4.5.3.pom
Download https://jcenter.bintray.com/com/googlecode/juniversalchardet/juniversalchardet/1.0.3/juniversalchardet-1.0.3.pom
Download https://jcenter.bintray.com/org/apache/commons/commons-parent/25/commons-parent-25.pom
Download https://jcenter.bintray.com/org/antlr/antlr4-master/4.5.3/antlr4-master-4.5.3.pom
Download https://jcenter.bintray.com/org/sonatype/oss/oss-parent/9/oss-parent-9.pom
Download https://jcenter.bintray.com/org/apache/apache/9/apache-9.pom
Download https://jcenter.bintray.com/org/jetbrains/kotlin/kotlin-stdlib-jre7/1.2.0/kotlin-stdlib-jre7-1.2.0.pom
Download https://jcenter.bintray.com/org/jetbrains/kotlin/kotlin-stdlib/1.2.0/kotlin-stdlib-1.2.0.pom
Download https://jcenter.bintray.com/net/sf/proguard/proguard-base/6.0.3/proguard-base-6.0.3.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/build/aapt2-proto/0.3.1/aapt2-proto-0.3.1.pom
Download https://jcenter.bintray.com/com/google/errorprone/error_prone_annotations/2.2.0/error_prone_annotations-2.2.0.pom
Download https://jcenter.bintray.com/com/google/auto/value/auto-value/1.5.2/auto-value-1.5.2.pom
Download https://jcenter.bintray.com/com/google/protobuf/protobuf-java/3.4.0/protobuf-java-3.4.0.pom
Download https://jcenter.bintray.com/com/google/protobuf/protobuf-java-util/3.4.0/protobuf-java-util-3.4.0.pom
Download https://jcenter.bintray.com/com/google/auto/auto-parent/3/auto-parent-3.pom
Download https://jcenter.bintray.com/com/google/protobuf/protobuf-parent/3.4.0/protobuf-parent-3.4.0.pom
Download https://jcenter.bintray.com/com/google/errorprone/error_prone_parent/2.2.0/error_prone_parent-2.2.0.pom
Download https://jcenter.bintray.com/com/google/google/1/google-1.pom
Download https://jcenter.bintray.com/org/jdom/jdom2/2.0.6/jdom2-2.0.6.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/dvlib/26.2.0/dvlib-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/layoutlib/layoutlib-api/26.2.0/layoutlib-api-26.2.0.pom
Download https://dl.google.com/dl/android/maven2/com/android/tools/repository/26.2.0/repository-26.2.0.pom
Download https://jcenter.bintray.com/org/apache/commons/commons-compress/1.12/commons-compress-1.12.pom
Download https://jcenter.bintray.com/org/apache/commons/commons-parent/39/commons-parent-39.pom
Download https://jcenter.bintray.com/org/apache/apache/16/apache-16.pom
Download https://jcenter.bintray.com/javax/inject/javax.inject/1/javax.inject-1.pom
Download https://jcenter.bintray.com/net/sf/kxml/kxml2/2.3.0/kxml2-2.3.0.pom
Download https://jcenter.bintray.com/com/google/code/findbugs/jsr305/1.3.9/jsr305-1.3.9.pom
Download https://jcenter.bintray.com/commons-logging/commons-logging/1.2/commons-logging-1.2.pom
Download https://jcenter.bintray.com/commons-codec/co<message truncated>
```