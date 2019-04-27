---
title: 大规模场景的资源拆分合动态加载
date: 2017-10-04 20:31:13
tags:
  - Unity
categories: Unity
---

# 概要
- ***资源拆分***	
	- 地形，地表
- ***动态加载/卸载***
- ***打包策略***
# 资源拆分
- 地形资源拆分
	- Unity原始地形有网格尺寸的限制（4096）
	- Mesh地形有面片数限制
	- 降低内存占用，仅需载入部分地形数据
	- 降低渲染面片数，视域体剔除
	- 制作大规模地形
		- Terrain Composer 2
		- T4M
		- 建模软件...
	- 拆分现有地形
		- Terrain Slicing & Dynamic Loading Kit
		- 地形数据
			
----------

| filed 					| meaning |
| ------------------------- |:------------------------------------------------------------------------------------------------:| 
| basemapDistance      	| Heightmap patches beyond basemap distance will use a preomputed low res basemap. | 
| castShadows      		| Should terrain cast shadows?      | 
| ollectDetailPatches | Collect Detail patches from memory.      | 
| detailObjectDensity | Density of detail objects. |
| detailObjectDistance | Detail objets will be displayed up to this distance. |
| heightmapMaxmumLOD | Lets you essentially lower the heightmap resolution used for rendering. |
| heightmapPixelError | An approximation of how many pixels the terrain will pop in the worst case when switching lod. |
| lightmapindex | The index of the lightmap applied to this renderer. |
| terrainData 				| The Terrain Data that stres htightmaps,terrain textures,detail meshes and trees. |
| treeBillboardDistance | Distance from the camera where trees will be rendered as billboards only. |
| terrCrossFadeLength | Total distance delta that trees will use to transition from billboard orientation to to mesh orientation. |
| treeDistance 				| The maximum distance at which trees are rendered. |
| treeMaximumFullLODCount 	| Maximum number of trees rendered at full LOD. |

- Lightmap
	- 拆分后重新烘培
	- 切分烘培好的Lightmap
		- exr 格式，FreeImage
- 切割跨地形的大模型
- 按地形块分组
# 动态加载
- 关键在于：流畅
	- 卡顿分析
	- Instantiate 前自动加载未加载的引用资源
		- Shader(Fallback)
		- Texture
		- Mesh
		- AnimationClip
	- 预加载资源
		- Shader(Fallback)
		- Texture
	- 预加载资源异步加载
		- Resources.LoadAsync
		- AssetBundle.LoadAsync
	- 实例化引起的序列化操作(Loading.LoadFileHeaders)
		- 避免一次性实例化过多的粒子系统（预加载）
		- 避免层级复杂，组件Awake过多
		- 尝试拆分Prefab，流失Instantiate
	- 加载策略(Assetbundle)
		- Shared包常驻内存
			- 大纹理等资源采用LoadFromCacheOrDownload
			- Material/Mesh等可采用new WWW
	- 卸载策略（AssetBundle）
		- Prefab 包
			- GameObject可通过Destroy来销毁
			- TerrainData,Object等可通过Resources.UnloadAsset来进行卸载
		- Shared包
			- 建议在确认不适用或切换场景时进行卸载
			- 切换场景时调用UnloadUnusedAssets来卸载Texture，Mesh等加载的共享资源
	- 注意事项
		- 加载方式
			- 九宫格
				- 适用于非自由视角
			- 建立缓冲池，防止反复实例化。
			- Load vs LoadAsync
			- 进场景预加载，推荐Load
			- 游戏中，推荐LoadAsync
		- Lightmap动态加载
			- LightmapSettings
			- lightmapindex/lightmapScaleOffset
			- Shader Stripping
		- Terrain动态加载
			- Lightmapld
			- Terrain.SetNeighbors
		- 动态 Static Batching (StaticBatchingUtility)
			- 运行时CPU/堆内存开销较大
			- 优先推荐手动拼合
			- 推荐分组拼合
		- 防止资源泄露
			- 运行时创建
				- new Material,Mesh
				- Material set
			- AssetBundle重复加载
				- 共享资源
					- Texture,Mesh,Font
					- Prefab
			- 隐形资源
				- TerrainData/AlphaMap
# 打包策略
	- 地形资源
	```javascript
	List<Object> groundObjs = new List<Object>();
	groundObjs.AddRange(Resources.LoadAll(BundleConfig.GroundObjectsPath, typeof(Texture2D)));
	groundObjs.AddRange(Resources.LoadAll(BundleConfig.GroundObjectsPath, typeof(GameObject)));

	BuildPipeline.PushAssetDependencies();
	
	```
	- 地表资源
		- 复杂资源拆分
			- "流失"实例化
		- 按地形块分组打包
