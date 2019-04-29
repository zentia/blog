---
title: Skinned Mesh原理解析
date: 2018-03-23 11:41:53
tags:
	
---

# 一、3D模型动画基本原理和分类

3D模型动画的基本原理是让模型中各定点的位置随时间变化。主要种类由Morph动画，关节动画和骨骼蒙皮动画(Skinned Mesh)。从动画数据的角度来说，三者一般都采用关键帧技术，即只给出关键帧的数据，其他帧的数据使用插值得到。但由于这三种技术的不同，关键帧的数据是不一样的。

Morph(渐变，变形)动画是直接指定动画每一帧的顶点位置，其动画关键中存储的是Mesh所有顶点在关键帧对应时刻的位置。

关节动画的模型不是一个整体的Mesh，而是分成很多部分(Mesh),通过一个父子层级结构将这些分散的Mesh组织在一起，父Mesh带动其子Mesh运动，各mesh中的顶点坐标定义在自己的坐标系中，这样各个Mesh是作为一个整体参与运动的。动画帧中设置各子Mesh相对于Mesh的变化（主要是旋转，当然也可以包括移动和缩放），通过子到父，一级级的变化累加（当然从技术上，如果是矩阵操作是累乘）得到该Mesh在整个动画模型所在的坐标空间中的变换（从本文的视角来说就是世界坐标系了，下同），从而确定每个Mesh在世界坐标系中的位置和方向，然后以Mesh为单位渲染即可。关节动画的问题是，各部分Mesh中的顶点时固定在其Mesh坐标系中的，这样在两个mesh结合处就可能产生裂缝。

第三类就是骨骼蒙皮动画即Skinned Mesh了，骨骼蒙皮动画的出现解决了关节动画的裂缝问题，而且效果非常酷，骨骼动画的基本原理可概括为：在骨骼控制下，通过顶点混合动态计算蒙皮网格的顶点，而骨骼的运动相对于其父骨骼，并由动画关键帧数据驱动。一个骨骼动画通常包括骨骼层次结构数据，网格(Mesh)数据，网格蒙皮数据(skin info)和骨骼的动画(关键帧)数据。

# 二、Skinned Mesh 原理和结构分析

Skinned Mesh中文一般乘坐骨骼蒙皮动画，这种动画包含骨骼(Bone)和蒙皮(Skinned Mesh)两个部分，Bone的层次结构和关节动画类似，Mesh则和关节动画不同：关节动画中时使用多个分散的Mesh,而Skinned Mesh中的Mesh是一个整体，也就是说只有一个mesh,实际上如果没有骨骼让Mesh运动变形，Mesh就和静态模型一样了，Skinned Mesh技术的精华在于蒙皮，所谓的皮并不是模型的贴图，而是mesh本身，蒙皮是将mesh中的顶点附着（绑定）在骨骼之上，而且每个顶点可以被多个骨骼所控制，这样的关节处的顶点由于同时收到了父子骨骼的拉扯而改变位置就消除了裂缝。

## 1.理解骨骼和骨骼层次结构(Bone Hierarchy)

首先要明确一个观念：骨骼决定了模型整体在世界坐标系中的位置和朝向。
先看看静态模型吧，静态模型没有骨骼，我们在世界坐标系中放置静态模型时，只要指定模型自身坐标系在世界坐标系中的位置和朝向。在骨骼动画中，不是把Mesh直接放到世界坐标系中，Mesh只是作为Skin使用的，是依附骨骼的，真正决定模型在世界坐标系中的位置和朝向的是骨骼。在渲染静态模型时，由于模型的顶点都是定义在模型坐标系中的，所以各顶点只要经过模型坐标系到世界坐标系的变换就可以进行渲染。而对于骨骼动画，设置模型的位置和朝向，实际实在设置根骨骼的位置和朝向，然后根据骨骼层级结构中父子骨骼之间的变换关系计算出各个骨骼的位置和朝向，然后根据骨骼堆Mesh中顶点的绑定计算出顶点在世界坐标系中的坐标，从而堆顶点进行渲染，要记住，在骨骼动画中，骨骼才是模型主题，Mesh不过是一层皮，一件衣服。
如何理解骨骼？请看第二个管娘：骨骼可理解为一个坐标空间。
在一些文章中往往会提到关节和骨骼，那么关节是什么？骨骼有事什么？下图是一个手臂的骨骼层次的实例。
<img src="http://oxcvfpext.bkt.clouddn.com/Unity-Skinned-Mesh-1.png">
骨骼只是一个形象的说法，实际上骨骼可理解为一个坐标空间，关节可理解为骨骼坐标空间的远点。关节的位置由它在父骨骼坐标空间中描述，上图中由三块骨骼，分别是手臂，前臂和两个手指。Clavicle(骨锁)是一个关节，它是上臂的原点，同样肘关节(elbow joint)是前臂的原点，腕关节(wrist)是手指骨骼的远点。关节既决定了骨骼空间的位置，旋转和缩放分量决定了骨骼空间的旋转和缩放。我们来看前臂这个骨骼，其原点位置位于上臂上某处的，对于上臂来说，它知道自己的坐标空间某处(即肘关节所在的位置)有一个子空间，那就是前臂，至于前臂里面是啥就不考虑了。当前臂绕肘关节旋转时，实际是前臂坐标空间在旋转，从而其中包含的子空间也在绕肘关节旋转，这个例子中是finger骨骼。和实际生物骨骼不同的是，我们这里的骨骼并没有实质的骨头，所以前臂旋转时，他自己没啥可转的，改变的只是坐标空间的朝向。你可以说上图蓝线在转，但实际蓝线并不存在，蓝线只是画上去表示骨骼之间关系的，真正转的是骨骼空间，我们能看到在转的是wrist joint,也就是两个finger骨骼的坐标空间，因为他们是子空间，会跟随父空间运动，就好比跟着地球转一样。

骨骼就是坐标空间，骨骼层次就是嵌套的坐标空间。关节只是描述骨骼的位置即骨骼自己的坐标空间原点再其父空间中的位置，绕关节旋转是指骨骼坐标空间(包括所有子空间)自身的旋转。但还有两个可能的疑问，一是骨骼的长度，由于骨骼是坐标空间没有所谓的长度和宽度限制，我们看到的长度一方面是蒙皮后的结果，而另一方面子骨骼的原点(也就是关节)的位置往往决定了视觉上父骨骼的长度，比如这里upper arm线段的长度实际是由elbow joint的位置决定的。第二个问题，实际上我们的例子中手指没有子骨骼，所以那个端点并不存在，那是为了方便演示画上去的。实际问题中总有最下层的骨骼，他们不能决定其他骨骼了，他们的作用只剩下控制Mesh顶点了。对了，那么手指的长度如何确定？我们看到的长度应该是手指部分的顶点和蒙皮决定的，也就是由Mesh中属于手指的那些点离腕关节的距离决定。

经过一段长篇大论，我们终于弄清楚骨骼和骨骼层次结构了，但是为什么要将骨骼组织成层次结构呢？答案是为了做动画方便，设想如果只有一块骨骼，那么让他动起来太简单了，动画每一帧直接指定他的位置即可，如果是n块呢？通过组成一个层次结构，就可以通过父骨骼控制子骨骼的运动，牵一发而动全身，改变某骨骼时并不需要设置其子骨骼的位置，子骨骼的位置会通过计算自动得到，上问已经说过，父子骨骼之间的关系可以理解为，子骨骼位于父骨骼的坐标系中，我们知道物体在坐标系中可以平移变换，以及自身的旋转和缩放。子骨骼在父骨骼的坐标系中也可以做这些变换来改变自己在其父骨骼坐标系中的位置和朝向等，那么如何表示呢？由于4x4矩阵可以同时表示上述三种变换，所以一般描述骨骼在其父骨骼坐标系时使用一个举证，也就是DirectX SkinnedMesh中的FramrTrnsformMatrix。实际上这不是唯一的方法，但应该是公认的方法因为矩阵不光可以同时表示多种变换还可以方便的通过连乘进行变换的组合，这在层次结构中非常方便。在本文的例子-最简单的skinned mesh示例中，我只演示了评议变换，所以只用一个3d坐标就可以表示子骨骼在父骨骼的位置。下面是Bone Class最初的定义：
```C++
class Bone
{
	public:
	float m_x, m_y, m_z;//这个坐标是定义在父骨骼坐标系中
}
```

同时增加了一组坐标，存放计算好的世界坐标系。
将各个骨骼相关于相对于父骨骼摆放好，就形成了一个骨骼层次结构的初始坐标，所谓初始是指定义骨骼层次时，后来动画改变了骨骼的相对位置，准确的说一般是改变了骨骼自身的旋转而位置保持不变。

假设我们通过某种方法建立了骨骼层次结构，那么每一块骨骼的位置都依赖于其父骨骼的位置，而跟骨骼没有父，他的位置就是整个骨骼体系在世界坐标系中的位置。可以认为root的父就是世界坐标系。但是初始位置时，跟骨骼一般不是世界原点的，比如使用3d max character studio创建的biped骨架时，一般两脚之间时世界原点，而根骨骼-骨盆位于原点上方(+z轴上)。这有什么关系呢，其实也没什么大不了的，只是我们在指定骨骼动画模型整体坐标时，比如设定坐标(0,0,0),则根骨骼-骨盆被置于世界原点，假如xy平面时地面，那么人下个身子到地面下了。我们想让两脚之间算作人的原点，这样设定(0,0,0)的坐标时人就站在地面上了，所以可以在两脚之间设定一个额外的根骨骼放到世界原点上，或者这个骨骼并不需要真实存在，只是在你的骨骼模型结构中保存骨盆骨骼到世界原点的变换矩阵。在微软X文件中，一般有一个Scene_Root节点，这算一个额外的骨骼吧，他的变换矩阵为单位阵，表示他初始位于世界原点，而真正骨骼的跟Bip01,作为Scene_root的子骨骼，其变换矩阵表示相对于root的位置。说这么多其实我只是像解释下，为什么要存在Scenen_Root这种额外的骨骼，以及加深理解骨骼定位骨骼动画模型整体的世界坐标的作用。

有了骨骼类，现在让我们看一下建立骨骼层次的代码，在bone class中增加一个构造函数和两个成员函数：

```C++
class Bone
{
public:
	Bone(float x, float y, float z): m_pSibling(NULL),m_pFirstChild(NULL),m_pFather(NULL),
	m_x(x),m_y(y),m_z(z){}

	void SetFirstChild(Bone *pChild)
	{
		m_pFirstChild = pChild;
		m_pFirstChild->m_pFather = this;
	}
	void SetSibling(Bone *pSibling)
	{
		m_pSibling = pSibling;
		m_pSibling->m_pFather = m_pFather;
	}
}
```

注意我增加了一个成员变量，Bone* m_pFather,这是指向父骨骼的指针，在这个例子中计算骨骼动画时本不需要这个指针，但我为了画一条从父骨骼关节到子骨骼关节的连线，增加了它，因为每个骨骼只有第一子骨骼的指针，绘制父骨骼时从父到子画线就只能画一条，所以记录每个骨骼的父，在胡子hi子骨骼时画这根线。

有了这个函数，就可以创建骨骼层次了，例如：
```C++
Bone * g_boneRoot;
Bone * g_bone1, *g_bone32, *g_bone22;

void buildBones()
{
	g_boneRoot = new Bone(0,0,0);
	g_bone1 = new Bone(0.1,0,0);
	g_bone21 = new Bone(0,0,0.1,0);
	g_bone22 = new Bone(0.1, 0.0,0);

	g_boneRoot->SetFirstChild(g_bone1);
	gbone1->SetFirstChild(g_bone21);
	g_bone21->SetSibling(g_bone22);
}
```
接下来是骨骼层次中最核心的部分，更新骨骼！由于动画的作用，某个骨骼的变化(TransformMatrix)变了，这时就要根据新的变化来计算，所以这个过程一般乘坐UpdateBoneMatrix。因为骨骼的变化都是相对父的，要变换顶点必须使用世界变换矩阵，所以这个过程根据更新了某些骨骼的骨骼变换矩阵(TransformMatrix)计算出所有骨骼的世界变换矩阵(CombinedMatrix)。在本文的例子中，骨骼只能平移，甚至我们没有用矩阵，所以当有骨骼变动时要做的只是直接计算骨骼的世界坐标，因此函数命名为ComputeWorldPos,相当于UpdateBoneMatrix后再用顶点相乘CombinedMatrix。
```C++
class Bone
{
	//give father's world pos, compute the bone's world pos
	void ComputeWorldPos(float fatherWX, float fatherWY, float WZ)
	{
		m_wx = fatherWX + m_x;
		m_wy = fatherWY + m_y;
		m_wz = fatherWZ + m_z;
		if (m_pSibling != NULL)
			m_pSibling->ComputeWorldPos(fatherWX, fatherWY, fatherWZ);

		if (m_pFirstChild != NULL)
			m_pFirstChild->ComputreWorldPos(m_wx, m_wy, m_wz);
	}
}
```

其中的递归调用使用了微软例子的思想。

有了上述函数，当某骨骼运动时就可以让其子骨骼跟随运动了，但是怎么让骨骼运动呢？这就是动画问题了，我不打算在这个简单的例子中使用关键帧动画，而只是通过程序每帧改变某些骨骼的位置，DEMO中的animateBones就是做这个的，你可以在里面改变不同的骨骼看看效果。在本文下面会对骨骼的关键帧动画做简单的讨论。

## 2.蒙皮信息和蒙皮过程

### 2-1 Skin info的定义

上文曾讨论过，Skinned Mesh中Mesh时作为皮肤使用，蒙在骨骼之上的。为了让普通的Mesh具有蒙皮的功能，必须添加蒙皮信息，即Skin info。我们知道mesh是由顶点构成的，建模时顶点是定义在模型自身坐标系的，即相对于Mesh原点的，而骨骼动画中决定模型顶点最终世界坐标的是骨骼，所以要让骨骼决定顶点的世界坐标，这就要将顶点和骨骼联系起来，Skin info正是起了这个作用。下面是DEMO中顶点类的定义的代码片段：

```C++
#define MAX_BONE_PER_VERTEX 4
class Vertex
{
	float m_x, m_y, m_z;//local pos in mesh space
	float m_wX, m_wY, m_wZ; // blended vertex pos, in world space

	// skin info
	int m_boneNum;
	Bone * m_bones[MAX_BONE_PER_VERTEX];
	float m_boneWeights[MAX_BONE_PER_VERTEX];
}
```

顶点的Skin info包含影响该顶点的骨骼数目，只想这些骨骼的指针，这些骨骼作用于该顶点的权重(Skin weight)。由于只是一个简单的例子，这儿没有考虑优化，所以用静态数组存放骨骼指针和权重，且实际引擎中Skin info的定义方式不一定是这样的，但基本原理一致。

MAX_BONE_PER_VERTEX在这儿用来设置可同时影响顶点的最大骨骼数，实际上由于这个DEMO是手工进行Vertex Blending并且没有硬件加速，以及为了确保速度，一般会定义最大骨骼数。另外在本Demo中，Skin info是手工设定的，而实际项目中，一般是在建模软件中生成这些信息并导出。

Skin info 的作用是使用各个骨骼的变换矩阵对顶点进行变换并乘以权重，这样某块骨骼只能对该顶点产生部分影响。各骨骼权重之和应该为1.

Skin info是针对顶点的，然后在使用Skin info前我们必须要使用Bone Offset Matrix对顶点进行变换，下面具体讨论Bone offset Matrix。（写下这句话的时候我感觉不妥，因为实际是先将所有的矩阵相乘最后在用作顶点，这儿是按照理论上的顺序进行讲述吧，请不要与实际情况混肴，其实他们也并不矛盾。而且在我们的DEMO中由于没有使用矩阵，所以变换的顺序和理论顺序是一致的）

### 2-2 Bone Offset Matrix的含义和计算方法

上文已经说过：“骨骼动画中决定模型顶点最终世界坐标的是骨骼，所以要让骨骼决定顶点的世界坐标”，现在让我们看下顶点受一块骨骼的作用的坐标变换过程：
	mesh vertex(defined in mesh space)---->Bone space---->world 
从这个过程中可以看出，需要首先将模型顶点从模型空间变换到某块骨骼自身的骨骼空间，然后才能利用骨骼的世界变换计算顶点的世界坐标。Bone Offset Matrix的作用正是将模型从顶点空间变换到骨骼空间。那么Bone Offset Matrix如何得到呢？下面具体分析：

Mesh space是建模时使用的空间，mesh中顶点的位置相对于这个空间的原点定义。比如在3dmax中建模时(视xy平面为地面，+z朝上)，可将模型两脚之间的中点作为Mesh空间的原点，并将其放置在世界原点，这样左脚上某一顶点坐标是(10,10,2),右脚上堆成的一点坐标是(-10,10,2)，头顶的坐标是(0,0,170)。由于此时Mesh空间和世界空间重合，上述坐标即在Mesh空间也是世界空间，换句话说，此时实际是以世界空间作为Mesh空间了。在骨骼动画中，在世界中放置的是骨骼而不是Mesh，所以这个区别并不重要。在3d max中添加骨骼的时候，也是将骨骼放入世界空间中，并调整骨骼的相对位置是得和mesh相吻合（即设置骨骼的TransformMatrix),得到股价的初始姿势以及相应的Transform Matrix(按惯例模型做成两臂侧平举直立，骨骼也要适合这个姿态)。由于骨骼的Transform Matrix(作用是将顶点从骨骼空间变换到上层空间)是基于其父骨骼空间的，只有根骨骼的Transform是基于世界空间的，所以要通过自下而上一层层Tranform变换(如果使用行向量右乘矩阵，这个Transform的累计过程就是C=MboneMfatherMgrandpar..Mroot),得到该骨骼在世界空间上的变换矩阵-Combined Transform Matrix,即通过这个矩阵可将该顶点从骨骼空间变换到世界空间。那么这个矩阵的逆矩阵就可以将世界空间中的顶点变换到某块骨骼的骨骼空间。由于Mesh实际上就是定义在世界空间了，所以这个逆矩阵就是Offset Matrix。即OffsetMatrix就是骨骼在初始位置（没有经过任何动画改变）时将bone变换到世界空间的矩阵(CombinedTransformMatrix)的逆矩阵，有一些资料称之为InverseMatrix。在几何流水线中，是通过变换矩阵将顶点变换到上层空间，最终得到世界坐标，逆矩阵则作相反的事，所以Inverse这种提法也符合惯例。那么Offset这种提法从字面上怎么理解呢？Offset即骨骼相对世界原点的偏移，世界原点加上这个偏移就变成骨骼空间的原点，同样定义在世界空间中的点经过这个偏移矩阵的作用也被变换成骨骼空间了。从另一角度理解，在动画中模型中顶点的位置是根据骨骼位置动态计算的，也就是说顶点跟着骨骼动，但首先必须确定顶点和骨骼之间的相对位置（即顶点在该骨骼坐标系中的位置），一个骨骼可能对应很多顶点，如果要保存这个相对位置每个顶点对于每块受控制的骨骼都要保存，这样就要保存太多的矩阵了。。所以只保存mesh空间到骨骼空间的变换（即OffsetMatrix),然后通过这个变换计算每个顶点在该骨骼空间中的坐标，所以OffsetMatrix也反应了mesh和每块骨骼的相对位置，只是这个位置是间接的通过和世界坐标空间的关系表达的，在初始位置将骨骼按照模型的形状摆好的关键之处。

以上的分析是通过mesh space和world space重合得到Offset Matrix的计算方法。那么如果他们不重合呢？那就要先计算顶点从mesh space变换到world space的变换矩阵，并乘上（还是右乘为例）Combined Matrix的Inverse Matrix从而得到Offset Matrix。但是这不是找麻烦吗，因为Mesh的原点在哪里并不重要，为啥不让他们重合呢？

还有一个问题是，既然Offset Matrix可以计算出来，为啥还要在骨骼动画文件中同时提供TransformMatrix和OffsetMatrix呢？实际上文件中确实可以不提供OffsetMatrix,而只在载入时计算，但TransformMatrix不可缺少，动画关键帧数据一般只存储骨骼的旋转和跟骨骼的位置，骨骼见的相对位置还是要靠TransformMatrix提供。在微软的X文件结构中提供了OffsetMatrix,原因是什么呢？我不知道。我猜想一个可能的原因是为了兼容性和灵活性，比如mesh并没有定义在世界坐标系，而是作为一个object放置在3d max中，在导出骨骼动画时不能简单的认为mesh的顶点坐标时相对于世界远点的，还要把这个object的位置考虑进去，于是导出插件要计算OffsetMatrix并保存在x文件中以避免兼容性问题。

关于OffsetMatrix和TransformMatrix含有评议，旋转和缩放的讨论：

首先，OffsetMatrix取决于骨骼的初始位置(即TransformMatrix),由于骨骼动画中我们使用的动画中的位置，初始位置时什么样并不重要，所以可以在初始位置中包含平移，而旋转和缩放在动画中设置(一般也仅仅使用旋转，这也是为啥动画通常中可以用一个四元数表示骨骼的关键帧)。在这种情况下，OffsetMatrix只包含平移即可。因此一些引擎的Bone中不存放Transform矩阵，而只存放骨骼在父骨骼空间的坐标，然后旋转只在动画帧中设置，最基本的骨骼动画即可实现。但也可在Transform和Offset Matrix中包括旋转和缩放，这样可以提供创建动画时的容错性。

在本文的DEMO中，我们也没有使用矩阵保存Bone Offset，而只用了一个坐标保存偏移位置。

```C++
class BoneOffset
{
public:
	float m_offx, m_offy, m_offz;
;

//在Bone class中，有一个方法用来计算Bone Offset
class Bone
{
public:
	BoneOffset m_boneOffset;
	//called after ComputeWorldPos() when bone loaded but not animated
	void ComputeBoneOffset()
	{
		m_boneOffset.m_offx = -m_wx;
		m_boneOffset.m_offy = -m_wy;
		m_boneOffset.m_offz = -m_wz;

		if (m_pSibling != NULL)
			m_pSibing->ComputeBoneOffset();
		if (m_pFirstChild != NULL)
			m_pFirstChild->ComputeBoneOffset();
	}
};
```
在ComputeBoneOffset()中，使用计算好的骨骼的世界坐标来计算bone offset,这儿的计算只是取一个负数，在实际引擎中，如果bone offset是一个矩阵，这儿就应该是求逆矩阵，其实由于旋转矩阵是正交的，只要求出旋转矩阵的转置矩阵。注意由于我们计算Bone offset时是使用计算好的世界坐标，所以在这之前必须在初始位置时对根骨骼调用ComputeWorldPos()以计算出各个骨骼在初始位置时的世界坐标。

### 2-3 最终：顶点混合(vertex blending)

现在我们有了Skin info,有了Bone Offset。现在开始做顶点混合，这是骨骼动画的精髓所在，正是这个技术消除了关节处的裂缝。顶点混合后得到了顶点新的世界坐标，对所有的顶点执行vertex blending后，从Mesh的角度看，Mesh deform(变形)了，变成动画需要的形状了。

首先，让我们看看使用单块骨骼对顶点进行作用的过程以下是DEMO中的相关代码：
```C++
class Vertex
{
public:
	void ComputerWorldPosByBone(Bone *pBone, float &outx, float &outY, float &outZ)
	{
		//step1: transform vertex from mesh space to bone space
		outX = m_x + pBone->m_boneOffset.m_offx;
		outY = m_y + pBone->m_boneOffset.m_offy;
		outZ = m_z + pBone->m_boneOffset.m_offz;

		//step2: transform vertex bone space to world space
		outX += pBone->m_wx;
		outY += pBone->m_wy;
		outZ += pBone->m_wz;
	}
};
```
这个函数使用一块骨骼对顶点进行变换，将顶点Mesh坐标系变换到世界坐标系，这儿使用了骨骼的Bone Offset Matrix和Combined Transform Matrix

对于多块骨骼，对每块骨骼执行这个过程并将结果根据权重混合(即vertex blending)就得到顶点最终的世界坐标。进行vertex blending的代码如下：
```C++
class Vertex
{
	void BlendVertex()
	{
		//do the vertex blending,get the vertex's pos in world space
		m_wX = 0;
		m_wY = 0;
		m_wZ = 0;

		for (int i = 0; i < m_boneNum; ++i)
		{
			float tx, ty, tz;
			ComputeWorldPosByBone(m_bones[i], tx, ty, tz);
			tx *= m_boneWeights[i];
			ty *= m_boneWeights[i];
			tz *= m_boneWeights[i];

			m_wX += tx;
			m_wY += ty;
			m_wZ += tz;
		}
	}
}
```
这些函数我都放在Vertex类中了，因为只是一个简单DEMO所以没有特别考虑类结构问题，在BlendVertex()中，遍历影响该顶点的所有骨骼，用每块骨骼计算出顶点的世界坐标，然后使用Skin Weight对这些坐标进行加权平均。tx,ty,tz是某块骨骼作用后顶点的世界坐标乘以权重后的值，这些值相加后就是最终的世界坐标了。

现在让我们用一个工时回顾以下Vertex blending的整个过程(使用矩阵变换)

```C++
Vworld = Vmesh * BoneOffsetMatrix1 * CombindMatrix1 * Weight1
+ Vmesh* BoneOffsetMatrix2 * CombineMatrix2*Wright2
+ ...
+ Vmesh * BoneOffsetMatrixn * CombindMatrixN * WeightN
```

## 4 总结

从结构上看，SkinnedMesh包括：动画数据，骨骼数据，包括Skin info的Mesh数据，以及Bone Offset Matrix。

从过程上看，载入阶段：载入并建立骨骼层次结构，计算或载入Bone Offset Matrix,载入Mesh数据和Skin info(具体的实现不同的引擎可能都不一样)。运行阶段：根据时间从动画数据中获取骨骼当前时刻的Transfrom Matrix，调用UpdateBoneMatrix计算出各骨骼的CombineMatrix,对于每个顶点根据Skin info进行Vertex Blending计算出顶点的世界坐标，最终进行模型的渲染。

# 三、关于本文的例子
这个例子做了尽可能的简化，只包含一个cpp文件，使用OpenGL和GLUT作为渲染器和框架，仅有400多行代码。例子中手工创建了一个骨骼层次和Mesh，手工设置Skin info并自动计算BoneOffset,使用程序控制骨骼平移演示了骨骼层次的运动和骨骼影响下Mesh顶点的运动，例子中甚至没有使用矩阵。本例子仅作理解骨骼动画只用。
<img src="http://oxcvfpext.bkt.clouddn.com/Unity-Skinned-Mesh-2.png">
截图中绿色网格是模型原始形状，蓝色是骨骼，红色是动画时模型形状，DEMO中左数第二个骨骼做上下运动，最下方的骨骼做x方向平移。DEMO没有使用旋转，而实际的骨骼动画中往往没有平移只有旋转的，因为胳膊只能转不能边长，但原理一致。
(这个公式使用的是行向量左乘矩阵)
由于BoneOffsetMatrix和Combined Matrix都是矩阵，可以先相乘这样就减少了很多计算了，在实际游戏中可以VS进行硬件加速计算。
代码的执行过程为，初始化时：
```C++
buildBones();//创建骨骼层次
buildMesh();//创建mesh,设置skin info，计算bone offset 
```	
每帧运行时：
```C++
//draw original mesh
g_mesh->DrawStaticMesh(0,0,0);

//move bones
animateBones();

//update all bone's pos in bone tree
g_boneRoot->ComputeWorldPos(0,0,0);

//update vertex pos by bones,using vertex blending
g_mesh->UpdateVertices();

//draw deformed mesh
g_mesh->Draw();

//draw bone
g_boneRoot->Draw();
```
为保证文本的完整性，下面贴出所有代码。
```C++
// A simplest Skinned Mesh demo
#include <GL/glut.h>
#define NULL 0

class BoneOffset
{
public :
    //BoneOffset transform a vertex from mesh space to bone space.
    //In other words, it is the offset from mesh space to a bone's space.
    //For each bone, there is a BoneOffest.
    //If we add the offset to the vertex's pos (in mesh space), we get the vertex's pos in bone space
    //For example: if a vertex's pos in mesh space is (100,0,0), the bone offset is (-20,0,0), so the vertex's pos in bone space is (80,0,0)
    //Actually, BoneOffset is the invert transform of that we place a bone in mesh space, that is (-20,0,0) means the bone is at (20,0,0) in mesh space
    float m_offx , m_offy , m_offz ;
};

//----------------------------------------------------------------

class Bone
{
public :
    Bone () {}
    Bone ( float x , float y , float z ):m_pSibling (NULL ),m_pFirstChild (NULL ),m_pFather (NULL ),m_x (x ),m_y (y ),m_z (z ){}

    ~Bone () {}

    Bone * m_pSibling ;
    Bone * m_pFirstChild ;
    Bone * m_pFather ; //only for draw bone

    void SetFirstChild (Bone * pChild ) { m_pFirstChild = pChild ; m_pFirstChild ->m_pFather = this ; }
    void SetSibling (Bone * pSibling ) { m_pSibling = pSibling ; m_pSibling ->m_pFather = m_pFather ; }

    float m_x , m_y , m_z ; //pos in its parent's space

    float m_wx , m_wy , m_wz ; //pos in world space

    //give father's world pos, compute the bone's world pos
    void ComputeWorldPos ( float fatherWX , float fatherWY , float fatherWZ )
    {
       m_wx = fatherWX +m_x ;
       m_wy = fatherWY +m_y ;
       m_wz = fatherWZ +m_z ;

       if (m_pSibling !=NULL )
           m_pSibling ->ComputeWorldPos (fatherWX , fatherWY , fatherWZ );

       if (m_pFirstChild !=NULL )
           m_pFirstChild ->ComputeWorldPos (m_wx , m_wy , m_wz );
    }

    BoneOffset m_boneOffset ;

    //called after compute world pos when bone loaded but not animated
    void ComputeBoneOffset ()
    {
       m_boneOffset .m_offx = -m_wx ;
       m_boneOffset .m_offy = -m_wy ;
       m_boneOffset .m_offz = -m_wz ;

       if (m_pSibling !=NULL )
           m_pSibling ->ComputeBoneOffset ();
       if (m_pFirstChild !=NULL )
           m_pFirstChild ->ComputeBoneOffset ();
    }

    void Draw ()
    {
       glColor3f (0,0,1.0);
       glPointSize (4);
       glBegin (GL_POINTS );
       glVertex3f (m_wx ,m_wy ,m_wz );
       glEnd ();
       if (m_pFather !=NULL )
       {
           glBegin (GL_LINES );
              glVertex3f (m_pFather ->m_wx ,m_pFather ->m_wy ,m_pFather ->m_wz );
              glVertex3f (m_wx ,m_wy ,m_wz );
           glEnd ();
       }

       if (m_pSibling !=NULL )
           m_pSibling ->Draw ();
       if (m_pFirstChild !=NULL )
           m_pFirstChild ->Draw ();

    }
};

//--------------------------------------------------------------

#define MAX_BONE_PER_VERTEX 4

class Vertex
{
public :
    Vertex ():m_boneNum (0)
    {
    }

    void ComputeWorldPosByBone (Bone * pBone , float & outX , float & outY , float & outZ )
    {
       //step1: transform vertex from mesh space to bone space
       outX = m_x +pBone ->m_boneOffset .m_offx ;
       outY = m_y +pBone ->m_boneOffset .m_offy ;
       outZ = m_z +pBone ->m_boneOffset .m_offz ;

       //step2: transform vertex from bone space to world sapce
       outX += pBone ->m_wx ;
       outY += pBone ->m_wy ;
       outZ += pBone ->m_wz ;
    }

    void BlendVertex ()
    { //do the vertex blending,get the vertex's pos in world space

       m_wX = 0;
       m_wY = 0;
       m_wZ = 0;

       for ( int i =0; i <m_boneNum ; ++i )        
       {            
       		float tx , ty , tz ;            
       		ComputeWorldPosByBone (m_bones [i ], tx , ty , tz );            
       		tx *= m_boneWeights [i ];            
       		ty *= m_boneWeights [i ];            
       		tz *= m_boneWeights [i ];            
       		m_wX += tx ;            
       		m_wY += ty ;            
       		m_wZ += tz ;        
       	}     
    }     
    float m_x , m_y , m_z ; 
    //local pos in mesh space     
    float m_wX , m_wY , m_wZ ; 
    //blended vertex pos, in world space     
    //skin info     
    int m_boneNum ;     
    Bone * m_bones [MAX_BONE_PER_VERTEX ];     
    float m_boneWeights [MAX_BONE_PER_VERTEX ];     
    void SetBoneAndWeight ( int index , Bone * pBone , float weight )     
    {        
    	m_bones [index ] = pBone ;        
    	m_boneWeights [index ] = weight ;     
   	} 
}; 
//----------------------------------------------------------- 
class SkinMesh 
{ 
public :     
	SkinMesh ():m_vertexNum (0)
	{

	}     

	SkinMesh ( int vertexNum ):m_vertexNum (vertexNum )     
	{        
		m_vertexs = new Vertex [vertexNum ];     
	}     

	~SkinMesh ()     
	{        
		if (m_vertexNum >0)
        delete [] m_vertexs ;
    }

    void UpdateVertices ()
    {
       for ( int i =0; i <m_vertexNum ; ++i )
       {
           m_vertexs [i ].BlendVertex ();
       }
    }

    void DrawStaticMesh ( float x , float y , float z )
    {
       glColor3f (0,1.0,0);
       glPointSize (4);
       glBegin (GL_POINTS );
       for ( int i =0; i <m_vertexNum ; ++i )
           glVertex3f (m_vertexs [i ].m_x +x ,m_vertexs [i ].m_y +y ,m_vertexs [i ].m_z +z );
       glEnd ();

       glBegin (GL_LINE_LOOP );
       for ( int i =0; i <m_vertexNum ; ++i )
           glVertex3f (m_vertexs [i ].m_x +x ,m_vertexs [i ].m_y +y ,m_vertexs [i ].m_z +z );
       glEnd ();
    }

    void Draw ()
    {
       glColor3f (1.0,0, 0);
       glPointSize (4);
       glBegin (GL_POINTS );
       for ( int i =0; i <m_vertexNum ; ++i )
           glVertex3f (m_vertexs [i ].m_wX ,m_vertexs [i ].m_wY ,m_vertexs [i ].m_wZ );
       glEnd ();

       glBegin (GL_LINE_LOOP );
       for ( int i =0; i <m_vertexNum ; ++i )            glVertex3f (m_vertexs [i ].m_wX ,m_vertexs [i ].m_wY ,m_vertexs [i ].m_wZ );        glEnd ();     }     int m_vertexNum ;     Vertex * m_vertexs ; //array of vertices in mesh }; //-------------------------------------------------------------- Bone * g_boneRoot ; Bone * g_bone1 , *g_bone2 , *g_bone31 , *g_bone32 ; void buildBones () {     g_boneRoot = new Bone (0, 0, 0);     g_bone1 = new Bone (0.2, 0, 0);     g_bone2 = new Bone (0.2, 0, 0);     g_bone31 = new Bone (0.2, 0.1, 0);     g_bone32 = new Bone (0.2, -0.1, 0);     g_boneRoot ->SetFirstChild (g_bone1 );
    g_bone1 ->SetFirstChild (g_bone2 );
    g_bone2 ->SetFirstChild (g_bone31 );
    g_bone31 ->SetSibling (g_bone32 );
}

void deleteBones ()
{
    delete g_boneRoot ;
    delete g_bone1 ;
    delete g_bone2 ;
    delete g_bone31 ;
    delete g_bone32 ;
}

void animateBones ()
{
    static int dir =-1, dir2 =-1;
    //animate bones manually

    g_bone1 ->m_y +=0.00001f*dir ;

    if (g_bone1 ->m_y m_y >0.2)
       dir *=-1;

    g_bone32 ->m_x +=0.00001f*dir2 ;

    if (g_bone32 ->m_x m_x >0.2)
       dir2 *=-1;
}

SkinMesh * g_mesh ;

void buildMesh ()
{
    float _meshData []=
    { //x,y,z
       -0.1,0.05,0,
       0.1,0.05,0,
       0.3,0.05,0,
       0.45,0.06,0,
       0.6,0.15,0,
       0.65,0.1,0,

       0.5,0,0,

       0.65,-0.1,0,
       0.6,-0.15,0,
       0.45,-0.06,0,
       0.3,-0.05,0,
       0.1,-0.05,0,
       -0.1,-0.05,0,
    };

    float _skinInfo []=
    { //bone_num,bone id(0,1,2,31 or 32), bone weight 1~4,
       1,  0, -1, -1, -1,    1.0, 0.0, 0.0, 0.0,
       2,  0,  1, -1, -1, 0.5, 0.5, 0.0, 0.0,
       2,  1,  2, -1, -1,  0.5, 0.5, 0.0, 0.0,
       2,  2,  31, -1, -1, 0.3, 0.7, 0.0, 0.0,
       2,  2,  31, -1, -1, 0.2, 0.8, 0.0, 0.0,
       1,  31, -1, -1, -1, 1.0, 0.0, 0.0, 0.0,

       2,  31, 32, -1, -1, 0.5, 0.5, 0.0, 0.0,

       1,  32, -1, -1, -1, 1.0, 0.0, 0.0, 0.0,
       2,  2,  32, -1, -1, 0.2, 0.8, 0.0, 0.0,
       2,  2,  32, -1, -1, 0.3, 0.7, 0.0, 0.0,
       2,  1,  2, -1, -1,  0.5, 0.5, 0.0, 0.0,
       2,  0,  1, -1, -1, 0.5, 0.5, 0.0, 0.0,
       1,  0, -1, -1, -1,    1.0, 0.0, 0.0, 0.0,
    };

    int vertexNum = sizeof (_meshData )/( sizeof ( float )*3);
    g_mesh = new SkinMesh (vertexNum );
    for ( int i =0; i m_vertexs [i ].m_x = _meshData [i *3];
       g_mesh ->m_vertexs [i ].m_y = _meshData [i *3+1];
       g_mesh ->m_vertexs [i ].m_z = _meshData [i *3+2];
    }

    //set skin info
    for ( int i =0; i m_vertexs [i ].m_boneNum = _skinInfo [i *9];
       for ( int j =0; j <g_mesh ->m_vertexs [i ].m_boneNum ; ++j )
       {
           Bone * pBone = g_boneRoot ;
           if (_skinInfo [i *9+1+j ]==1)
              pBone = g_bone1 ;
           else if (_skinInfo [i *9+1+j ]==2)
              pBone = g_bone2 ;
           else if (_skinInfo [i *9+1+j ]==31)
              pBone = g_bone31 ;
           else if (_skinInfo [i *9+1+j ]==32)
              pBone = g_bone32 ;

           g_mesh ->m_vertexs [i ].SetBoneAndWeight (j , pBone , _skinInfo [i *9+5+j ]);
       }
    }

    //compute bone offset
    g_boneRoot ->ComputeWorldPos (0, 0, 0);
    g_boneRoot ->ComputeBoneOffset ();
}

void deleteMesh ()
{
    delete g_mesh ;
}

void myInit ()
{
    buildBones ();
    buildMesh ();
}

void myQuit ()
{
    deleteBones ();
    deleteMesh ();
}

void myReshape ( int width , int height )
{
    GLfloat h = (GLfloat ) height / (GLfloat ) width ;

    glViewport (0, 0, (GLint ) width , (GLint ) height );
    glMatrixMode (GL_PROJECTION );
    glLoadIdentity ();
//  glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
    glFrustum (-1.0, 1.0, -h , h , 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW );
    glLoadIdentity ();
    glTranslatef (0.0, 0.0, -1.0);
}

void myDisplay ( void )
{
    glClear (GL_COLOR_BUFFER_BIT );

    //draw original mesh
    g_mesh ->DrawStaticMesh (0,0,0);

    //move bones
    animateBones ();

    //update all bone's pos in bone tree
    g_boneRoot ->ComputeWorldPos (0, 0, 0);

    //update vertex pos by bones, using vertex blending
    g_mesh ->UpdateVertices ();

    //draw deformed mesh
    g_mesh ->Draw ();

    //draw bone
    g_boneRoot ->Draw ();

    glFlush ();
    glutSwapBuffers ();
}

void myIdle ( void )
{
    myDisplay ();
}

int main ( int argc , char *argv [])
{
    glutInit (&argc , argv );
    glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowPosition (100, 100);
    glutInitWindowSize (640, 480);
    glutCreateWindow ( "A simplest skinned mesh DEMO, by happyfirecn@yahoo.com.cn" );

    glutDisplayFunc (myDisplay );
    glutReshapeFunc (myReshape );
    glutIdleFunc (myIdle );

    myInit ();
    glutMainLoop ();
    myQuit ();

    return 0;
}
```
//