---
title: 为啥Unity构建会很慢？
categories:
- Engine
- Unity
---
由于第一天出包太慢了，从周五中午到周六下午，还在出包，然后开始检查。
当前是卡在了这里
```C++
static bool CompressWithPVRTexToolParallel(ExternalToolCompressionParams params)
{
    Assert(IsCompressedETCTextureFormat(params.format) || IsCompressedETC2TextureFormat(params.format));

    const int blockRowsPerJob = 8; // 这里原来是16的，我改成了8，本地的CPU根本用不完啊。
    const int pixelRowsPerJob = blockRowsPerJob * 4;

    std::vector<PVRTexToolJob> jobs((params.height + pixelRowsPerJob - 1) / pixelRowsPerJob);
    if (jobs.empty())
        return true;

    const int fullBlockRowCompressedBytes = CalculateImageSize(params.width, pixelRowsPerJob, params.format);
    const int fullBlockRowUncompressedBytes = params.width * pixelRowsPerJob * 4;
    UInt8* srcBuffer = params.inBuffer;
    UInt8* dstBuffer = params.outBuffer;
    for (size_t i = 0; i < jobs.size(); ++i, srcBuffer += fullBlockRowUncompressedBytes, dstBuffer += fullBlockRowCompressedBytes)
    {
        ExternalToolCompressionParams blockParams = params;
        blockParams.height = std::min(pixelRowsPerJob, params.height - pixelRowsPerJob * static_cast<int>(i));
        blockParams.inBuffer = srcBuffer;
        blockParams.outBuffer = dstBuffer;

        PVRTexToolJob j = { blockParams, false };
        jobs[i] = j;
    }

    JobFence fence;
    // 下面两个函数，job函数
    ScheduleJobForeach(fence, PVRTexToolJob::Dispatch, &jobs[0], jobs.size());
    SyncFence(fence);

    for (size_t i = 0; i < jobs.size(); ++i)
    {
        if (!jobs[i].success)
            return false;
    }

    return true;
}
```
当我把blockRowsPerJob改成8的时候，Unity终于可以让我的CPU利用率上70%了,有的时候还会上100%。虽然是无奈之举，但是也比之前好多了。
然后我们在打开当占用100%的图，他妈的，3M的Mask图，3M的法线图，3M的Detail图。。。这种图在什么手机上也进不去游戏啊，资源检查应该要预警和提示的。
而从昨天下午到今天一直在处理这些图片，还是要拉起一个exe然后走IO。。
然后我们在统一一下超过3M的图片大小吧。
我们随便找一个sh脚本，然后给他起名叫`折磨人.sh`
然后实现如下：
```sh
#!/bin/bash

# 统计一个目录下指定类型或限制某种类型以外的文件的大小，包括子目录

help() {
  echo "Description: 用于查找某个目录下特定类型文件的数量和总的大小，或者查找某种类型以外的文件数量和总的大小"
  echo "Author: cocowool <cocowool@qq.com>, Blog: http://www.edulinks.cn"
  echo "Usage: sh count_file.sh -p folder_path [ -t jpg ] [ -x html ] "
  echo "       	-p : 需要查找的文件路径"
  echo "       	-t : 需要查找的文件类型"
  echo "       	-x : 需要排除的文件类型"
  echo "		-s : 需要查找的文件的最小容量"
  exit 0
}

if [[ $# == 0 ]] || [[ "$1" == "-h" ]]; then
  help
  exit 0
fi

# INCLUDE_FILE_TYPE=""
# EXCLUDE_FILE_TYPE=""

# echo $*
while getopts "p:t:x:" opt
do
  case "$opt" in
    p) 
      FOLDER_PATH=$OPTARG ;;
    t) 
      # echo "Found t option"
      INCLUDE_FILE_TYPE=$OPTARG ;;
    x)
      # echo "Found x option"
      EXCLUDE_FILE_TYPE=$OPTARG ;;
    # getopts doesn't support long option, such as --option
    # debug)
    #   echo "Found debug option"
    #   echo $OPTARG ;;
	s)
		echo $OPTARG
		size=$OPTARG
    *) echo "$opt is invalid option" ;;
  esac
done

# FOLDER_PATH="/Users/shiqiang/Projects/edulinks-blog/public"
# 解决文件中含有空格的问题
IFS=$(echo "\n\b")

echo "查找的文件路径为：$FOLDER_PATH"

if [ -n $FOLDER_PATH ]; then
  #list=`find $FOLDER_PATH | grep "jpg"`
  if [ -n "$INCLUDE_FILE_TYPE" ]; then
    echo "查找的文件后缀为：$INCLUDE_FILE_TYPE."
    list=`find $FOLDER_PATH -type f -name "*.$INCLUDE_FILE_TYPE"`
  elif [ -n "$EXCLUDE_FILE_TYPE" ]; then
    echo "查找文件后缀不是：$EXCLUDE_FILE_TYPE 的文件"
    list=`find $FOLDER_PATH -type f ! -name "*.$EXCLUDE_FILE_TYPE"`
  fi

  for i in $list
    do
    # echo $i
    fileSize=$(du -k "${i}" | cut -f1)
	if [$size lt $fileSize]
		# echo $fileSize
		((totalSize=fileSize+totalSize))
		((totalNum=totalNum+1))
	fi
  done

  echo "文件总大小为：$((totalSize/1024))M"
  echo "文件总数量为：$((totalNum))"
fi

```
写好之后我们测试一下

下面这个函数是在Dispatch中执行的，也巨垃圾，本来应该跑内存，非要走IO，这能不慢吗？难道PVRTexTool没有源码？果真没有找到源码，不过又好奇，上次我在讲PPT的时候，我们现在完全可以都用ASTC了，为什么还会跑PRVTC呢？
```C++
static bool CompressWithPVRTexTool(ExternalToolCompressionParams params)
{

}
```

然后我们顺藤摸瓜找到这个函数
```C++
static void CompressSingleImageTexture()
{
    ...
    if (shouldUseAllegorithmic)
        compressFunc = &CompressWithAllegorithmicTool;
    else if (isPVRTC)
        compressFunc = &CompressWithPVRTexTool;
    else
        compressFunc = &CompressWithPVRTexToolParallel;
    ...
}
```
由此可得，CompressWithPVRTexToolParallel是一个CompressWithPVRTexTool的一个多线程版本。而CompressWithAllegorithmicTool是另外一个exe工具，unity源码的代码说实在的越看越感觉垃圾，似乎一直在说服我，有一个能跑就可以了。
反正这里不管咋样也不会快了。爱用不用吧你。
未完待续。
