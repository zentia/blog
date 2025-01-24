Unity有两种动态加载机制：Resources.Load和AssetBundle，二者本质并无区别。Resources.Load就是从一个缺省打进程程序包的AssetBundle(Resourece)里加载资源，而一般AssetBundle文件需要你自己创建，运行时加载。

# AssetBundle加载基础

通过AssetBunlde加载资源，分为两步，第一步是获取AssetBundle对象，第二步是通过该对象加载需要的资源。而第一步又分为两种方式，下文中将结合常用的API进行详细地描述。

## 第一步，获取AssetBundle对象常用地API

### 方式一，先获取WWW对象，再通过WWW.assetBundle获取AssetBundle对象：

	public WWW(string uri);

加载Bundle文件并获取WWW对象，完成后会在内存中创建较大地WebStream(解压后的内容，通常为原Bundle文件的4~5倍大小，纹理资源比例可能更大),因此后续的AssetBundle.Load可以直接在内存中进行。

	public static WWW LoadFromCacheOrDownload(string uri, int version, unit crc = 0);

加载Bundle文件并获取WWW对象，同时将解压形式的Bundle内容存入磁盘中作为缓存（如果该Bundle已在缓存中，则省去这一步），完成后只会在内存中创建较小的SerializedFile，而后续的AssetBundle.Load需要通过IO从磁盘中的缓存获取。

	public AssetBundle assetBundle;

通过之前两个接口获取WWW对象后，即可通过WWW.assetBundle获取AssetBundle对象。

### 方式二，直接获取AssetBundle:

	public static AssetBundle LoadFromFile(string path); 
	public static AssetBundle LoadFromFileAsync(string path);

通过未压缩的Bundle文件，同步创建AssetBundle对象，这是最快的方式。创建完成后只会在内存中创建较小的SerializedFile，而后续的AssetBundle.Load需要通过IO从磁盘中获取。
如果AssetBundle是未压缩，或者是数据块形式（LZ4算法压缩）的，LoadFromFile将从磁盘中直接加载它。如果AssetBundle是高度压缩(LZMA算法压缩)的，再将它加载进入内存前，会首先将它解压。

```C#
void LoadAssetBundles(string baseDir)
{
	if (assetBundleInfos != null)
	{
		for (int i = 0; i < assetBundleInfos.Length; i++)
		{
			var bInfo = assetBundleInfos[i];
			if (bInfo.assetBundle != null)
			{
				bInfo.assetBundle.Unload(false);
			}
		}
		assetBundleInfos = null;
	}

	var abInfos = new List<AssetBundleInfo>(50);

	var files = Directory.GetFiles(m_dir_update, "*.bytes");
	for (int i = 0; i < files.Length; i++)
	{
		string f = files[i];
		string n = Path.GetFileName(f);
		if (!CheckABName(abInfos, n)) contine;

		abInfos.Add(new AssetBundleInfo{name = n, uri = f});
	}

	if (baseDir != null)
	{
		files = Directory.GetFiles(baseDir, "*.bytes");
		for (int i = 0; i < files.Length; i++)
		{
			string f = files[i];
			string n = Path.GetFileName(f);
			if (!CheckABName(abInfos, n)) continue;

			abInfos.Add(new AssetBundleInfo{name = n, uri = f});
		}
	}

	files = LoadConfigLines("res_idx");
	for (int i = 0; i < files.Length; i++)
	{
		string n = files[i].Trim();
		if (!CheckABName(abInfos, n)) continue;

		string f = streamAssetsPath + n;
		abInfos.Add(new AssetBundleInfo{name = n, uri = f});
	}

	if (abInfos.Count == 0) return;

	string prefix = "assets/res/";
	int cprefix = prefix.Length;

	for (int i = 0; i < abInfos.Count;)
	{
		var abInfo = abInfos[i];
		var ab = AssetBundle.LoadFromFile(abInfo.uri);
		if (ab == null)
		{
			abInfos.RemoveAt(i);
			sbError.Append("\nfail ab : ");
			sbError.Append(abInfo)
		}
	}
}
```

	public static AssetBundle LoadFromMemory(byte[] binary)

通过Bundle的二进制数据，异步创建AssetBundle对象，完成后会在内存中创建较大的WebStream。调用时，Bundle的解压是异步进行的，因此对于未压缩的Bundle文件，该接口于LoadFromMemoryAsync是等价的。

	public static AssetBundle LoadFromMemoryAsync

该接口是 CreateFromMemory 的同步版本，这个方法的参数是包含了AssetBundle数据的字节数组。如果需要的话，你还可以传入一个CRC(循环冗余校验码)参数。如果AssetBundle使用了LZMA算法压缩，那么AssetBundle在加载的时候会被解压。如果AssetBundle使用了LZ4算法压缩，它将直接以压缩形式被加载。

```C#
IEnumerator LoadFromMemoryAsync(string path)
{
	AssetBundleCreateRequest createRequest = AssetBundle.LoadFromMemoryAsync(File.ReadAllBytes(path));
	yield return createRequest;
	AssetBundle bundle = createRequest.assetBundle;
	var prefab = bundle.LoadAsset<GameObject>("MyObject");
	Instantiate(prefab);
}
```

## 第二步，从AssetBundle加载资源的常用API

	public Object Load(string name, Type type);

通过给定的名字和资源类型，加载资源。加载时会自动加载其依赖的资源，即Load一个Prefab时，会自动Load其引用的Texture资源。

	public Object[] LoadAll(Type type);
	public AssetBundleRequest LoadAllAssetsAsync();

一次性加载Bundle中给定资源类型的所有资源

	public AssetBundleRequest LoadAsync(string name, Type type)

### AssetBundle的压缩类型

Unity3D引擎为我们提供了三种压缩策略来处理AssetBundle的压缩，即：

- LZMA格式
- LZ4格式
- 不压缩

>LZ4是块压缩(chunk-based)，LZMA是流压缩(stream-based)。流压缩(LZMA)在处理整个数据块时使用同一个字典，它提供了最大可能的压缩率但只支持顺序读取。块压缩(LZ4)指的是原始数据被分成大小相同的子块并单端压缩。如果你想要实时解压/随机读取开销小，则应该使用这种。

>LZMA压缩方式的优点在于使用同一个字典压缩率较高，但只能顺序读取意味着加载任意一个资源时，都需要将整个AssetBundle解压，造成卡顿和额外内存占用。LZ4基于快压缩率较低（测试LZMA换LZ4：86.9M->108M），但只需解压需要块即可，不会有大的卡顿和额外内存占用。

#### LZMA(stream-based)

在默认情况下，打包生成的AssetBundle都会被压缩。在U3D中，AssetBundle的标准压缩格式便是LZMA（LZMA是一种序列化流文件），因此在默认情况下，打出的AssetBundle包处于LZMA格式的压缩状态。

#### LZ4(chunk-based)

Unity 5.3之后的版本增加了LZ4格式压缩，是一种块压缩方式，由于LZ4的压缩比一般，因此经过压缩之后的AssetBundle包体的体积较大（该算法基于chunk）。但是，使用LZ4格式的好处在于解压缩的时间相对要短。

使用LZ4格式压缩，需要打包设置

	BuildPipeline.BuildAssetBundles(Application.streamingAssetPath, BuildAssetBundleOptions.ChunkBasedCompression);

#### 不压缩

当然，我们也可以不对AssetBundle进行压缩。没有经过压缩的胞体系最大，但是访问速度最快。

若要使用不压缩的策略，只需要在打包的时候开启

	BuildPipeline.BuildAsetBundles(Application.streamingAssetPath, BuildAssetBundleOptions.UncompressedAssetBundle);
	


# 五 AssetBundle原理分析

## 5.2 AssetBundle及Assets的卸载

在AssetBundle的下载和加载过程中，以及Assets加载和实例化过程中，AssetBundle以及加载的Assets都会占用内存。

1. AssetBundle的卸载采用Assetbundle.Unload(bool)接口。
2. Assets的卸载有两种方式:
	- AssetBundle.Unload(true); // 这会强制卸载掉所有从AssetBundle加载的Assets。
	- Resource.UnloadUnusedAssets()和Resources.UnloadAsset。这会卸载掉所有没有用到的Assets。需要注意的是，该接口作用于整个系统，而不仅仅是当前的AssetBundle，而且不会卸载从当前AssetBundle文件中加载并仍在使用的Assets。
3. 对于实例化出来的对象，可以使用GameObject.Destroy活GameObject.DestroyImmediate。注意的是：官方说法是这样的，如果使用GameObject.Destroy接口，Unity会将真正的删除操作延后到一个合适的时机统一进行处理，但会在渲染之前。

对于WWW对象，可以使用www=null或www.dispose。
这两者是由区别的，www=null不会立即释放内存，而是系统自动回收机制启动时回收。www.dispose则会立即调用系统的回收机制来释放内存。当WWW对象被释放后，其对于Web Stream数据的引用计数也会相应减1。

对于Web Stream数据，它所占用的内存会在其引用计数为0时，被系统自动回收。例如：当上图中的AssetBundle对象和WWW对象被释放后，Web Stream数据所占内存也会被系统自动回收。

# AssetBundle依赖加载

如果一个或者多个UnityEngine.Objects引用了其他AssetBundle中的UnityEngine.Object，那么AssetBundle之间就产生了依赖关系了。如果UnityEngine.ObjectA所引用的UnityEngine.ObjectB不是其他的AssetBundle中的，那么依赖就不会产生。
如果产生依赖，被依赖对象(UnityEngine.ObjectB)将被拷贝进你创建的AssetBundle(指包含UnityEngine.ObjectA的AssetBundle)

更近一步，如果有多个对象(UnityEngine.ObjectA1、UnityEngine.ObjectA2、UnityEngine.ObjectA3...)引用了同一个被依赖对象(UnityEngine.ObjectB)，那么被依赖对象将被拷贝多份，打包进各个对象各自的AssetBundle。

如果一个AssetBundle存在依赖性，那么要注意的是，那些包含了被依赖对象的AssetBundles，需要在你想要实例化的对象的加载之前加载。Unity不会自动帮你加载这些依赖。

