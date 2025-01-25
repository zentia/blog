# AssetBundle工作流程
要开始使用AssetBundles，请按照以下步骤操作。有关每个工作流程的更多详细信息，请参阅本文的其他页面。
## 将资产分配给AssetBundles
分配给定资产Assetbundle的，请按照下列步骤操作：
1. 从项目视图中选择要分配给捆绑包的资产
2. 检查检查器中的对象
3. 在检查器的底部，您应该看到一个分配AssetBundles和Variants的部分
4. 左侧下拉分配AssetBundle，而右侧下拉分配变量
5. 单击左侧下拉列表，显示"无"，以显示当前已注册的AssetBundle名称
6. 单击"新建..."以创建新的AssetBundle
7. 输入所需的AssetBundle名称。请注意，AssetBundle名称确实支持一种文件夹结构，具体取决于您键入的内容。要添加子文件夹，请用"/"分割文件夹名称。例如：AssetBundle名称"environment/forest"将在环境子文件夹下创建名forest的包
8. 一旦选择或创建了AssetBundle名称，您可以重复此过程以右手下拉以分配或创建变体名称（如果需要）。构建AssetBundle不需要变体名称

# 构建AssetBundles
在Asset文件夹中创建一个名为Editor的文件夹，并将包含以下内容的脚本放在该文件夹中：
```
using UnityEditor;
using UnityEngine;
using System.IO;

public class CreateAssetBundles
{
    [MenuItem("Assets/Build AssetBundles")]
    static void BuildAllAssetBundles()
    {
        if (!Directory.Exists(Application.streamingAssetsPath))
        {
            Directory.CreateDirectory(Application.streamingAssetsPath);
        }
        BuildPipeline.BuildAssetBundles(Application.streamingAssetsPath, BuildAssetBundleOptions.None, BuildTarget.Android);
        AssetDatabase.Refresh();
    }
}
```
此脚本将在Assets菜单底部创建一个名为"Build AssetBundles"的菜单项，该菜单项将执行与该标记关联的函数中的代码。单击"Build AssetBundles"时，将显示一个带有构建对话框的进度条。这将获取您使用AssetBundle名称标记的所有资产，并将它们放在assetBundleDirectory定义的路径中的文件夹中。
# 将AssetBundles上传到场外存储
此步骤对每个用户都是唯一的，而不是Unity可以告诉您如何操作的步骤。如果您打算将AssetBundles上传到第三方托管网站，请在此执行此操作。如果您正在进行严格的本地开发并打算将所有AssetBundle都放在磁盘上，请跳到下一步。
# 加载AssetBundle和Assets
对于打算从本地存储加载的用户，您将对AssetBundles.LoadFromFile API感兴趣。如下：
```
public class LoadFromFileExample : MonoBehaviour 
{
    void Start()
    {
        var myLoaderAssetBundle = AssetBundle.LoadFromFile(Path.Combine(Application.streamingAssetsPath, "myassetBundle"));
        if (myLoaderAssetBundle == null) 
        {
            Debug.Log("Failed to load AssetBundle!");
            return;
        }
        var prefab = myLoaderAssetBundle.LoadAsset<GameObject>("MyObject");
        Instantiate(prefab);
    }
}
```
LoadFromFile 获取bundle文件的路径。
如果您自己托管AssetBundles并需要将它们下载到游戏中，您将对UnityWebRequest API感兴趣。这是一个例子：
```
IEnumerator InstantiateObject()
{
    string uri = "file:///"
}
```