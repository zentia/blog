---
title: Dictionary
mathjax: true
date: 2019-04-09 20:57:21
tags:
    - CSharp
categories: CSharp
---
# Entry结构体
这是Dictionary存放数据的最小单位，调用Add(Key,Value)方法添加的元素都会被封装在这样的一个结构体中。
```CSharp
private struct Entry
{
    public int hashCode; // 除符号位以外的31位hashCode值，如果该Entry没有被使用，那么为-1
    public int next;     // 下一个元素的下标索引，如果没有下一个就为-1
    public TKey key;     // 存放元素的键
    public TValue value; // 存放元素的值
}
```
# 其它关键私有变量
```CSharp
private int[] buckets;          // Hash桶
private Entry[] entries;        // Entry数组，存放元素
private int count;              // 当前entries的index位置
private int version;            // 当前版本，防止迭代过程中集合被更改
private int freeList;           // 被删除Entry在entries中下标index，这个位置是空闲的
private int freeCount;          // 有多少个被删除的Entry，有多少个空闲的位置
private IEqualityComparer<TKey> comparer; // 比较器
private KeyCollection keys;     // 存放key的集合
private ValueCollection values; // 存放Value的集合
```
# Add操作
首先我们用图的形式来描述一个Dictionary的数据结构，其中只画出了关键的地方。桶大小为4以及Entry大小也为4的一个数据结构。
{% asset_img 1.png %}
假设需要执行一个Add操作，dictionary.Add("a","b")，其中key="a"，value="b"。
1. 根据key的值，计算出它的hashCode。我们假设"a"的hash值为6(GetHashCode("a")=6)。
2. 通过对hashCode取余运算，计算出该hashCode落在哪一个buckets桶中。现在桶的长度(buckets.Length)为4，那么就是6%4最后落在index为2的桶中，也就是buckets[2]。
3. 接下来将hashCode、key、value等信息存入entries[count]中，因为count位置是空闲的；继续count++指向下一个空闲位置。上图中第一个位置，index=0就是空闲的，所以就存放在entries[0]的位置。
4. 将Entry的下标entryIndex赋值给buckets中对应下标的bucket。步骤3中存放在entries[0]的位置，所以buckets[2]=0。
5. 最后version++，集合发生了变化，所以版本需要+1。只有增加、替换和删除元素才会更新版本。
-----------------------------------------------------------------------------------------------------------------------------------
完成上面Add操作后，数据结构更新成了下图这样的形式。
{% asset_img 2.png %}
这样是理想情况下的操作，一个bucket中只有一个hashCode没有碰撞的产生，但是实际上是会经常产生碰撞；那么Dictionary类中又是如何解决碰撞的呢。
我们继续执行一个Add操作，dictionary.Add("c","d")，假设GetHashCode("c")=6，最后6%4=2。最后桶的index也是2，按照之前的步骤1~3是没有问题的，执行完后数据结构如下图所示。
{% asset_img 3.png %}
如果继续执行步骤4那么bucket[2] = 1，然后原来的buckets[2]=>entries[0]的关系就会丢失，这是我们不愿意看到的。现在Entry中的next就发挥大作用了。

-----------------------------------------------------------------------------------------------------------------------------------
如果对应的buckets[index]有其它元素已经存在，那么会执行以下两条语句，让新的entry.next指向之前的元素，让buckets[index]指向现在的新元素，就构成了一个单链表。
```CSharp
entries[index].next = buckets[targetBucket];
...
buckets[targetBucket] = index;
```
实际上步骤4也就是做一个这样的操作，并不会去判断是不是有其它元素，因为buckets中桶初始值就是-1，不会造成问题。

-----------------------------------------------------------------------------------------------------------------------------------
经过上面的步骤以后，数据结构就更新成了下图这个样子

{% asset_img 4.png %}