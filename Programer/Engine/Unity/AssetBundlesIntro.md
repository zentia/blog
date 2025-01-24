# AssetBundles

AssetBundle是一个包含模型，纹理，预制，音频剪辑，甚至整个场景的可以在运行中加载的档案文件。AssetBundles可以表达彼此之间的依赖关系；例如，AssetBundle A中的Matrial可以引用AssetBundle B中的纹理。为了通过网络进行有效传递，可以根据用例需求使用内置算法选择压缩AssetBundle。
AssetBundles可用于：可下载的内容更新（DLC）、降低初始安装包大小、针对用户平台进行最优资源加载，以及降低运行时内存压力。

## 压缩格式(BuildAssetBundleOptions)

|||
|--|--|
|None                       |无需任何特殊选项即可构建assetBundle     |
|UncompressedAssetBundle    |创建ab包时不压缩数据                    |
|DisableWriteTypeTree       |不再ab中包含类型信息                    |
|DeterministicAssetBundle   |使用存储在asset中的id的hash构建ab       |
|ForceRebuildAssetBundle    |强制rebuild ab                          |
|IgnoreTypeTreeChanges      |执行增量构建检查时忽略类型树的改变      |
|AppendHashToAssetBundleName|将hash附加到assetBundle名称             |
|ChunkBasedCompression      |创建AssetBundle时使用基于块LZ4压缩      |
|StricMode                  |如果在其中报告任何错误，则不允许构建成功|
|DryRunBuild                |只编译                                  |
|DisableLoadAssetByFileName          |禁用掉通过文件名加载ab         |
|DisableLoadAssetByFileNameWithExtension|禁用掉通过文件名后缀加载ab  |

Unity5的BuildAssetBundleOptions有CollectionDependencies但是在Unity2018.3中已经废弃了，原因是2018.3默认收集资源的依赖资源。
这个特性十分的方便，我们只需要将所有东西都做成prefab，将prefab作为资源指定给AssetBundle，由Unity自己去收集所用到的资源就好了。但实际项目中仅仅如此是不够的，原因在复用的资源上。
如果多个prefab使用了同一个资源，会出现什么问题呢？我们来试验一下。

而image下面的Dependencies是空的，也就是它没有任何依赖了。

然后此时我在将资源也打包，此时的Dependencies就出来了，但是路径是绝对路径。
这其实就是AssetBundle的链式结构和增量打包了。一个小的部分改变了，它将会改变的只有总的AssetBundle.manifest，还有直接依赖它本身的manifest。其他不依赖的部分是不需要重新打包的。
还有一点需要注意的地方是，除了manifest文件以外，还有一个没有后缀名称的AssetBundle文件。这个文件其实才是包含了所有依赖关系的总的依赖关系的配置文件，刚才我们能用txt打开的manifest文件，都只是用来做本地依赖关系和增量打包的时候用的。我们加载AssetBundle的时候，完全不需要加载那么manifest文件的，只需要那个没有后缀名称的AssetBundle文件（具体名字和你导出的文件夹有关）就行了。它代表的是该项目的所有AssetBundle依赖关系。
所以，刚才我们看到的manifest里面用的都是本地绝对路劲，那是针对你本地打包时用的，和加载无关。

