---
title: DirectX 9 3D游戏设计入门
date: 2019-03-24 10:01:00
mathjax: true
tags:
  - 图形学
---
# 三维空间中的向量
　　几何学中，我们用有向线段表示向量，如图1。向量的两个属性是他的长度和他的顶点所指的方向。因此，可以用向量来模拟既有大小又有方向的物理模型。例如，以后我们要实现的粒子系统。我们用向量来模拟粒子的速度和加速度。在3D计算机图形学中我们用向量不仅仅模拟方向。例如我们常常想知道光线的照射方向，以及在3D世界中的摄象机。向量为在3维空间中表示方向的提供了方便。
　　
　　{% asset_img 1.png%}
　　向量与位置无关。有同样长度和方向的两个向量是相等的，即使他们在不同的位置。观察彼此平行的两个向量，例如在图1中u和v是相等的。
　　我们继续学习左手坐标系。图2显示的是左手坐标系和右手坐标系。两者不同的是Z轴的方向。在左手坐标系中Z轴是向书的里面去的，而右手坐标系是向书的外边去的。
　　　　{% asset_img 2.png%}
　　图2
　　因为向量的位置不能改变它的性质，我们可以把所有向量平移使他们的尾部和坐标系的原点重合。因此，当一个向量在标准位置我们能通过头点来描述向量。图3显示的是图1中的向量在标准位置的样子。
　　{% asset_img 3.png%}
　　图3
　　我们通常用小写字母表示一个向量，但有时也用大写字母。如2、3和4维向量分别是：
　　　　　u = (ux, uy), 
　　　　　N = (Nx, Ny, Nz), 
　　　　　c = (cx, cy, cz, cw)。
　　我们现在介绍4个特殊的3D向量，就象图4显示的。首先是都由含有0的零向量；它被表示成加粗的0 = (0, 0, 0)。接下来3个特殊的向量标准基向量。它们被叫做i, j和k向量，分别沿着坐标系的x轴,y轴和z轴，并且有1的单位长：i = (1, 0, 0), j = (0, 1, 0), and k = (0, 0, 1)。
注意：只有1个单位长度的向量叫做单位向量（模长为1的向量）。
　　{% asset_img 4.png%}
　　图4
在D3DX库中，我们能用D3DXVECTOR3类表示3维空间中的向量。它的定义是：
```C++
typedef struct D3DXVECTOR3 : public D3DVECTOR
{
public:
    D3DXVECTOR3() {};
    D3DXVECTOR3( CONST FLOAT * );
    D3DXVECTOR3( CONST D3DVECTOR& );
    D3DXVECTOR3( CONST D3DXFLOAT16 * );
    D3DXVECTOR3( FLOAT x, FLOAT y, FLOAT z );

    // casting
    operator FLOAT* ();
    operator CONST FLOAT* () const;

    // assignment operators
    D3DXVECTOR3& operator += ( CONST D3DXVECTOR3& );
    D3DXVECTOR3& operator -= ( CONST D3DXVECTOR3& );
    D3DXVECTOR3& operator *= ( FLOAT );
    D3DXVECTOR3& operator /= ( FLOAT );

    // unary operators
    D3DXVECTOR3 operator + () const;
    D3DXVECTOR3 operator - () const;

    // binary operators
    D3DXVECTOR3 operator + ( CONST D3DXVECTOR3& ) const;
    D3DXVECTOR3 operator - ( CONST D3DXVECTOR3& ) const;
    D3DXVECTOR3 operator * ( FLOAT ) const;
    D3DXVECTOR3 operator / ( FLOAT ) const;

    friend D3DXVECTOR3 operator * ( FLOAT, CONST struct D3DXVECTOR3& );

    BOOL operator == ( CONST D3DXVECTOR3& ) const;
    BOOL operator != ( CONST D3DXVECTOR3& ) const;

} D3DXVECTOR3, *LPD3DXVECTOR3;
```
注意D3DXVECTOR3是从D3DVECTOR继承的。它的定义是： 
```C++
typedef struct _D3DVECTOR {
    float x, y, z;
} D3DVECTOR;
```
　　向量有它们自己的算法，就像你在D3DXVECTOR3定义中看到的数学运算。现在你不需要知道它们怎么使用。以后介绍这些向量运算以及一些有用的函数和关于向量的重要的详细资料。
注意：在3D图形程序中，虽然我们主要关心3D向量，但有时也会用到2D和4D向量。在D3DX库中提供了D3DXVECTOR2和D3DXVECTOR4类来分别表现2D和4D向量。不同维数的向量有着和3D向量一样的性质，也就是它们描述大小和方向，仅仅是在不同的维数中。所有这些向量的数学运算对于不同维数向量都有效只是有一个除外，就是向量积。这些运算我们可通过论述3D向量扩展到2D, 4D甚至n维向量。

## 向量相等
　　几何学上，有同样方向和长度的两个向量相等。数学上，我们说有同样维数和分量的向量相等。例如：如果ux = vx, uy = vy, 且 uz = vz.那么(ux, uy, uz) = (vx, vy, vz)。在代码中我们能够用“==”判断两个向量相等。
```C++
D3DXVECTOR u(1.0f, 0.0f, 1.0f);
D3DXVECTOR v(0.0f, 1.0f, 0.0f);
if( u == v ) return true;
```
同样的，我们也能用“！=”判断两个向量不相等。
```C++
if( u != v ) return true;
```
注意：当比较浮点数时，必须注意。因为浮点数不是精确的，我们认为相等的两个浮点数是有细微差别的；因此，我们测试它们近似相等。我们定义一个常数EPSILON，把它当作非常小的“buffer”。假如两个数和EPSILON相差很小我们说它们近似相等。换句话说，EPSILON让浮点数有一定的精度。接下来的实例函数是怎样用EPSILON比较两个浮点数相等。
```C++
bool Equals(float lhs, float rhs)
{
	// if lhs == rhs their difference should be zero
	return fabs(lhs - rhs) < EPSILON ? true : false;
}
```
   当我们用D3DXVECTOR3类时不必担心，因为它已经帮我们处理了，但是在一般情况下适当注意比较两个浮点数是很重要的。
## 计算向量大小（向量的模）
　　几何学上，向量的大小是有向线段的长度。知道向量的分量，利用下面的公式我们就能计算出向量的大小。
<font size="4">$||u||=\sqrt{u_{x}^2+u_{y}^2+u_{z}^2}$</font>
‖u‖表示向量u的长度。例如：计算向量u = (1, 2, 3)和v = (1, 1)的大小。
根据公式（1），我们得到：
　　<font size="4">$||u||=\sqrt{1^2+2^2+3^2} = \sqrt{1+4+9}=\sqrt{14}$</font>
　　
我们利用D3DX库中下面的函数便能计算向量的大小。
```C++
FLOAT D3DXVec3Length( // Returns the magnitude.
　　CONST D3DXVECTOR3* pV // The vector to compute the length of.
);
D3DXVECTOR3 v(1.0f, 2.0f, 3.0f);
float magnitude = D3DXVec3Length( &v ); // = sqrt(14)
```
## 标准化向量
　　<font color="red">标准化向量是让向量的大小等于1，即被叫作单位向量。</font>我们能利用向量大小以及各个分量把一个向量标准化，就像这样：
　　$\hat{u} ={u \over ||u||} = {({u_x \over ||u||}, {u_y \over ||u||}, {u_z \over ||u||})}$
我们这样表示单位向量$\hat{u}$。如：标准化向量u = (1, 2, 3) 和 v = (1, 1)。
解答方法：根据(2)和(3)我们得到‖u‖=√14 和 ‖v‖=√2,因此：
　　
我们利用D3DX库中下面的函数能标准化向量。
```C++
D3DXVECTOR3 *D3DXVec3Normalize(
    D3DXVECTOR3* pOut, // Result.
    CONST D3DXVECTOR3* pV // The vector to normalize.
);
```
注意：这个函数返回一个指针，因此它可以作为一个参数传递给另一个函数。大数情况下，除非另作说明，D3DX数学函数返回的结果是一个指针。但不是所有函数都这样。
## 向量相加
　　我们能够通过分别把两个向量的各个分量相加得到向量之和，注意在相加之前必须保证它们有相同的维数。
$u + v = ({u_x + v_x, u_y + v_y, u_z + v_z})$
图5显示的是几何学上的向量相加。
{% asset_img 5.png 图5 %}

两个向量相加的代码，我们使用重载的加法操作符：
```C++
D3DXVECTOR3 u(2.0f, 0.0f, 1.0f);
D3DXVECTOR3 v(0.0f, -1.0f, 5.0f);

// (2.0 + 0.0,  0.0 + (-1.0),  1.0 + 5.0)
D3DXVECTOR3 sum = u + v; // = (2.0f, -1.0f, 6.0f)
```
## 向量相减
　　和加法类似，通过分别把两个向量的各个分量相减得到向量之差。再次重声两个向量必须是相同维数。
　　$u + v = ({u_x - v_x, u_y - v_y, u_z - v_z})$
图6显示的是几何学上的向量相减。
{% asset_img 5.png 图6 %}
两个向量相减的代码，我们使用重载的减法操作符：
```C++
D3DXVECTOR3 u(2.0f, 0.0f, 1.0f);
D3DXVECTOR3 v(0.0f, -1.0f, 5.0f);
D3DXVECTOR3 difference = u - v; // = (2.0f, 1.0f, -4.0f)
```
　　图6显示，向量减法得到一个从v向量终点到u向量终点的向量。假如我们解释u和v的分量，我们能用向量相减找到从一个点到另一个点的向量。这是非常方便的操作，因为我们常常想找到从一个点到另一个点的方向向量。
## 标量与向量的乘积
我们能用一个标量与向量相乘，就象名字暗示的一样，向量按比例变化。这种运算不会改变向量的方向，除非标量是负数，这种情况向量方向相反。
　　$ku=(ku_x,ku_y,ku_z)$
D3DXVECTOR3类提供了向量与标量乘法的操作符。
```C++
D3DXVECTOR3 u(1.0f, 1.0f, -1.0f);
D3DXVECTOR3 scaledVec = u * 10.0f; // = (10.0f, 10.0f, -10.0f)
```
## 点积
　　数学上定义点积是两个向量的乘积。按下面等式计算：
　　$u \cdot v = u_{x}v_x+u_{y}v_y+u_{z}v_z = S$
　　上面的等式不能很明显的体现几何上的意义。利用余弦定律，我们能够发现它们的关系。
　　u · v =|u||v|cosθ，表示两个向量的点积是它们的模和夹角的余弦之积。因此，如果u和v都是单位向量，那么u·v就是它们夹角的余弦。
一些点积中有用的特性
　　■　假如u · v = 0，那么u⊥v。
　　■　假如u · v > 0，那么两个向量的角度θ小于90度。
　　■　假如u · v < 0，那么两个向量的角度θ大于90度。
　　我们使用下面的D3DX函数计算两个向量的点积：
```C++
FLOAT D3DXVec3Dot( // Returns the result.
　　CONST D3DXVECTOR3* pV1, // Left sided operand.
　　CONST D3DXVECTOR3* pV2 // Right sided operand.
);
D3DXVECTOR3 u(1.0f, -1.0f, 0.0f);
D3DXVECTOR3 v(3.0f, 2.0f, 1.0f);
// 1.0*3.0 + -1.0*2.0 + 0.0*1.0
// = 3.0 + -2.0
float dot = D3DXVec3Dot( &u, &v ); // = 1.0
```
## 叉积
　　第二种乘法在向量数学中叫叉积。**不像点积，结果值是一个标量，叉积的结果值是另一个向量。**通过把两个向量u和v相乘得到另一的向量p，向量p垂直于u和v。也就是说向量p垂直于u并且垂直于u。
十字相乘就像这样计算：
$p=u \times v=[(u_{y}v_z-u_{z}v_{y}),(u_{z}v_x-u_{x}v_{z}),(u_{x}v_y-u_{y}v_{x})]$
$p_x = (u_{y}v_z-u_{z}v_{y})$
$p_y = (u_{z}v_x-u_{x}v_{z})$
$p_z = (u_{x}v_y-u_{y}v_{x})$
　　
如：发现j = k×i = (0, 0, 1)×(1, 0, 0) 并且j同时垂直于k和i.
因此，j = (0, 1, 0)。回忆一下上节的标题“叉积”，是说如果u · v = 0，那么u⊥v。同样的如果j · k = 0并且j · i = 0那么我们便能知道j既垂直于k又垂直于i的。
我们使用下面的D3DX函数计算两个向量的叉积：
```C++
D3DXVECTOR3 *D3DXVec3Cross(
    D3DXVECTOR3* pOut, // Result.
    CONST D3DXVECTOR3* pV1, // Left sided operand.
    CONST D3DXVECTOR3* pV2 // Right sided operand.
);
```
　　从下图很明显，向量-p与u和v也都相互垂直。我们执行叉积的命令，确定得到的的结果不管是p或者-p。换句话说，u×v = -(v×u)。这说明叉积是不可交换的。你能通过左手法则确定叉积返回的向量。按照第一个向量指向第二个向量弯曲你的左手，这时拇指所指的方向就是向量所指的方向。
{% asset_img 7.png 图7 %}　
## 矩阵
在这一部分我们关注的焦点是数学中的矩阵。它们在3D图形学中的应用将在下一部分讲解。
一个m×n的矩阵是由m行和n列的数字组成的矩阵列。行和列的数字就是这个矩阵的维数。我们通过写在下方的数字识别矩阵清单，数字中的第一个表示行第二个表示列。例如下边的M是3×3矩阵，B是2×4矩阵, C是3×2矩阵。
$M=\begin{bmatrix} m_{11} & m_{12} & m_{13} \\ m_{21} & m_{22} & m_{23} \\ m_{31} & m_{32} & m_{33} \\\end{bmatrix}$ $B=\begin{bmatrix} b_{11} & b_{12} & b_{13} b_{14} \\ b_{21} & b_{22} & b_{23} b_{24} \\\end{bmatrix}$ $C=\begin{bmatrix} c_{11} & c_{12} \\ c_{21} & c_{22} \\c_{31} & c_{32} \\\end{bmatrix}$
我们使用加粗的大写字母表示矩阵。有时一个矩阵只包含一行或者一列。我们用行矩阵和列矩阵这个特殊的名称来称呼。例如下边就是行和列矩阵：
$V = \begin{bmatrix} v_{1} & v_{2} & v_{3} v_{4} \\\end{bmatrix}$ $U = \begin{bmatrix} u_{x} \\ v_{y} \\ v_{z} \\\end{bmatrix}$
当使用行或列矩阵时，我们只用一个下标，有时我们还用字母表示。
## 相等、数乘矩阵以及相加
这部分我们将用到下边4个矩阵：
　　$A = \begin{bmatrix} 1 & 5 & -2 3 \\\end{bmatrix}$ $B = \begin{bmatrix} 6 & 2 & 5 -8 \\\end{bmatrix}$ $C = \begin{bmatrix} 1 & 5 & -2 3 \\\end{bmatrix}$ $D = \begin{bmatrix} 1 & 2 & -1 & 3 \\ -6 & 3 & 0 & 0 \\\end{bmatrix}$
　■假如两个矩阵维数和成员都相同那么它们就相等。例如，A = C 因为A和C有同样的维数并且他们的成员都相等。A≠B同时A≠D因为他们的成员或者维数是不相同的。
　■我们能通过标量与矩阵的每个成员相乘得到标量与矩阵相乘。如矩阵D与k相乘：
　　$kD = \begin{bmatrix} k(1) & k(2) & k(-1) & k(3) \\ k(-6) & k(3) & k(0) & k(0) \\\end{bmatrix}$
假如k = 2,那么：
　　$kD = 2D = \begin{bmatrix} 2(1) & 2(2) & 2(-1) & 2(3) \\ 2(-6) & 2(3) & 2(0) & 2(0) \\\end{bmatrix} = \begin{bmatrix} 2 & 4 & -2 & 6 \\ -12 & 6 & 0 & 0 \\\end{bmatrix}$
　■当两个矩阵的维数相同时才能把它们相加。和是把两个矩阵相应的成员相加得到。如：
　　$A+B=\begin{bmatrix} 1 & 5 \\ -2 & 2(3) & 2(0) & 2(0) \\\end{bmatrix} + \begin{bmatrix} 2(1) & 2(2) & 2(-1) & 2(3) \\ 2(-6) & 2(3) & 2(0) & 2(0) \\\end{bmatrix}$
　■矩阵有加法当然也就有减法，前提是有相同的维数。矩阵减法如图所示：

## 乘法
　　矩阵相乘在3D计算机图形学中是非常重要的运算。通过矩阵相乘，我们能变换向量并且，将不同向量转换到一起。变换是下一节的内容。
　　为了得到矩阵之积AB，A的列数必须等于B的行数。假如这个条件不满足，就不能相乘。考虑下边两个矩阵，A 和 B，分别是2×3 和 3×3，如：
　　
　　我们看乘积AB是可以计算的，因为A的列数等于B的行数。注意乘积BA,它们是不能计算的，因为B的列数不等于A的行数。由此说明：一般情况下矩阵乘法不满足乘法交换律（也就是, AB≠BA）。我们说“一般不可交换”因为有一些矩阵乘法的实例还是可以的。
　　知道了矩阵乘法的计算方法，现在我们就能给出精确的定义：假如A是一个m×n的矩阵，B是一个n×p的矩阵，那么它们之积AB可计算并且是一个m×p 的矩阵C, C的成员ij 等于A的第i个与B的第j个相乘：
　　
例如，求解：
　　
　　我们检查知道乘法是可计算的，因为A的列数等于B的行数。也知道计算的结果是一个2×2的矩阵。根据公式（4），我们得到：
　　
作为练习，检查AB≠BA。
更一般的例子：
　　
## 单位矩阵
　　有一种特殊矩阵叫做单位矩阵。单位矩阵是除了对角（左上到右下）以外所有成员都是0，对角都是1的方矩阵。例如，下边是2×2, 3×3, 和 4×4的单位矩阵：

单位矩阵有如下特性：
MI = IM=M
即，用单位矩阵乘以矩阵不会改变矩阵。此外，这是一个特例：用单位矩阵进行乘法运算满足乘法交换律。单位矩阵可以看作矩阵运算中的数字“1”。

例如：验证2×2矩阵M与单位矩阵相乘得到的结果是M。

## 逆转
下面列举了关于逆矩阵的重要信息。
* 只有正方形的矩阵（方阵）才能求逆，因此当我们说矩阵求逆，那么它就是方矩阵。
* n×n矩阵M的逆矩阵是一个n×n矩阵表示为M–1
* 不是每个方矩阵都有逆矩阵
* 矩阵和他的逆矩阵相乘得到一个单位矩阵：M M–1 = M–1M = I。注意当我们进行这样的操作时矩阵是可交换的。
   逆矩阵用来解决与其他矩阵相等是非常有用的。例如，考虑等式p’= pR 并且假设我们知道p’和R想求p。首先找到R–1，一旦求得R–1，我们便能求出p，就象这样：

   求逆矩阵的方法已经超出了本书的范围，但是这在任何一本线性代数书上都有讲解。在“基本变换”一节我们给出一个特定矩阵的逆矩阵。在“D3DX 矩阵”部分我们将学习一个为我们求逆矩阵的D3DX函数。
　　我们介绍几个有用的推论：(AB) –1 = B–1 A–1。这个性质前提是：假定A和B都能求逆并且它们都是有相同维数的方矩阵。
矩阵的转置
　　矩阵的转置是相互交换矩阵的行和列。因而，m×n的矩阵的转置是一个n×m的矩阵。我们把矩阵M的转置记作MT。
例如：求下面两个矩阵的转置：
　　
　　重声一下，转置是交换矩阵的行和列。
因此：
　　
## D3DX 矩阵
　　当设计Direct3D应用程序时，使用4×4矩阵和1×4行矩阵（向量）是有代表性的。注意使用这两种矩阵意味着可以进行下列定义的矩阵乘法。
* 向量-矩阵乘法。即，假如1×4的单行矩阵V，和4×4的矩阵T，那么积VT可计算并且返回的结果是一个1×4的单行矩阵（向量）。
* 矩阵-矩阵乘法。即，假如4×4的矩阵T，和4×4的矩阵R，那么积TR和RT可计算并且两者返回的结果都是一个4×4的矩阵。注意因为矩阵乘法不满足交换律所以TR和RT不一定相等。
　　在D3DX中表示1×4的行矩阵（向量），我们用D3DXVECTOR3和D3DXVECTOR4向量类。当然D3DXVECTOR3只有3个成员，不是4个。然而，第4个成员缺省是1或0（在下一部分有更多信息）。
　　在D3DX中表示4×4的矩阵，我们用D3DXMATRIX类，定义如下：

```C++
typedef struct D3DXMATRIX : public D3DMATRIX {
public:
    D3DXMATRIX() {};
    D3DXMATRIX( CONST FLOAT * );
    D3DXMATRIX( CONST D3DMATRIX& );
    D3DXMATRIX( FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
                FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
                FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
                FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44 );
    // access grants
    FLOAT& operator () ( UINT Row, UINT Col );
    FLOAT  operator () ( UINT Row, UINT Col ) const;
    // casting operators
    operator FLOAT* ();
    operator CONST FLOAT* () const;
    // assignment operators
    D3DXMATRIX& operator *= ( CONST D3DXMATRIX& );
    D3DXMATRIX& operator += ( CONST D3DXMATRIX& );
    D3DXMATRIX& operator -= ( CONST D3DXMATRIX& );
    D3DXMATRIX& operator *= ( FLOAT );
    D3DXMATRIX& operator /= ( FLOAT );
    // unary operators
    D3DXMATRIX operator + () const;
    D3DXMATRIX operator - () const;
    // binary operators
    D3DXMATRIX operator * ( CONST D3DXMATRIX& ) const;
    D3DXMATRIX operator + ( CONST D3DXMATRIX& ) const;
    D3DXMATRIX operator - ( CONST D3DXMATRIX& ) const;
    D3DXMATRIX operator * ( FLOAT ) const;
    D3DXMATRIX operator / ( FLOAT ) const;
    friend D3DXMATRIX operator * ( FLOAT, CONST D3DXMATRIX& );
    BOOL operator == ( CONST D3DXMATRIX& ) const;
    BOOL operator != ( CONST D3DXMATRIX& ) const;
} D3DXMATRIX, *LPD3DXMATRIX;
```
D3DXMATRIX类是从单数结构D3DMATRIX继承的复数形式。D3DMATRIX的定义是：
```C++
typedef struct _D3DMATRIX {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;
        };
        float m[4][4];
    };
} D3DMATRIX;
```
　　观察D3DXMATRIX类发现有很多有用的运算符，比如对矩阵检测相等，矩阵相加和矩阵相减，标量与矩阵相乘，铸造（casting），以及非常重要的两个D3DXMATRIXs相乘。因为矩阵相乘是非常重要的，我们给出一段实例代码：
```C++
D3DXMATRIX A(…); // initialize A
D3DXMATRIX B(…); // initialize B
D3DXMATRIX C = A * B; // C = AB
```
   D3DXMATRIX类另一个重要的运算符是小括号，它允许我们非常方便的为矩阵成员赋值。注意当使用小括号时我们的下标就象C语言数组下标一样是从0开始的。例如，为一个矩阵的ij = 11 赋值，我们写成：
```C++
D3DXMATRIX M;
M(0, 0) = 5.0f; // Set entry ij = 11 to 5.0f.
```
   D3DX库也提供下列有用的函数：将D3DXMATRIX转化为单位矩阵，转置D3DXMATRIX矩阵以及求逆矩阵。
```C++
D3DXMATRIX *D3DXMatrixIdentity(
	D3DXMATRIX *pout // 将矩阵转换为单位矩阵
);

D3DXMATRIX M;
D3DXMatrixIdentity( &M ); // M = 单位矩阵

D3DXMATRIX *D3DXMatrixTranspose(
	D3DXMATRIX *pOut, // 输出的转置矩阵
	CONST D3DXMATRIX *pM // 原矩阵
);
D3DXMATRIX A(...); // 初始化矩阵A
D3DXMATRIX B;
D3DXMatrixTranspose( &B, &A ); // B = 输出的转置矩阵
```
   假如我们将不能求逆的矩阵用求逆函数，那么函数将会返回null.同样的，这本书我们忽视第二个参数，并且总是把它设置为0。
```C++
D3DXMATRIX *D3DXMatrixInverse(
	D3DXMATRIX *pOut, // 输出的逆矩阵
	FLOAT *pDeterminant, // 除非是必需的，一般设为0
	CONST D3DXMATRIX *pM // 原矩阵
);
D3DXMATRIX A(...); // 初始化矩阵
D3DXMATRIX B;
D3DXMatrixInverse( &B, 0, &A ); // B = A的逆矩阵
```

## 基本变换　　
当用Direct3D编程时，我们使用4×4矩阵来进行矩阵变换。用它的原因是：我们设置一个4×4矩阵X是为了更精确的描述矩阵变换。同样我们设置一个相匹配的点或者把向量的分量放置到一个1×4的行矩阵V中。VX的乘积返回一个新的向量V’。例如：让X沿着x轴平移10个单位同时V = [2, 6, –3, 1]，乘积VX = V’= [12, 6, –3, 1]。
　　有一些东西需要阐明。我们使用4×4矩阵是因为这样的大小能表现我们需要的所有变换。最初看来一个3×3的好象更适合3D。然而这里有很多种我们喜欢用的变换是不能用一个3×3的矩阵来表示的，比如平移、投影、反射。我们使用向量-矩阵相乘来工作，因此我们至少要通过一个矩阵乘法来完成相应的变化。增大到4×4的矩阵后，它允许我们用一个矩阵描述更多的变换，并且向量-矩阵乘法是可行的。

　　我们说过把一个相匹配的点或者一个向量的成员放置到一个1×4的行矩阵中。但是点和向量是3D的！为什么我们要用一个1×4的行矩阵呢？我们必需把3D点/向量增大为4D的单行矩阵，是为了符合向量与矩阵的乘法定义，而1×3的单行矩阵和4×4的矩阵相乘是不允许的。
　　那么，我们怎么使用第四个成员（我们用w来表示）呢？当我们把一个点放置到一个1×4的行矩阵中时，我们设置w为1。允许对点进行适当的平移。因为向量和位置无关，所以向量的平移没有被定义，如果试图这样做会返回一个无意义的向量。为了防止对向量进行平移，当在把一个向量放置到一个1×4行矩阵中时我们把w设置为0。例如：
把点p = (p1, p2, p3)放置到一个单行矩阵中就象这样：
[p1, p2, p3, 1]，
同样把向量v = (v1, v2, v3) 放置到一个单行矩阵中就象这样：
[v1, v2, v3, 0]。
　　
注意：我们设置w = 1是为了让点可以被恰当的移动，同样我们设置w = 0是为了防止向量被平移。当我们检查矩阵实际平移时这是一个非常清晰的模型。
　　有时一个矩阵变换时我们改变向量成员w的值，即w≠0 且 w≠1。考虑下边例子：
　　$p = \begin{bmatrix} p_1 & p_2 & p_3 & 1 \\\end{bmatrix} \begin{bmatrix} 1 & 0 & 0 & 0 \\ 0 & 1 & 0 & 0 \\ 0 & 0 & 1 & 1 \\ 0 & 0 & 0 & 0 \\\end{bmatrix} = \begin{bmatrix} p_1, p_2, p_3, p_3 \\\end{bmatrix} = p`$
　　因为p3≠0 且 p3≠1。
　　我们注意w =p3。当w≠0 且 w≠1时，我们说我们有一个向量在同类空间中，与3维空间中的向量是相对的。我们能通过把向量的每个分量与w相除将同类空间中的向量映射到3维空间中来。例如把同类空间中向量(x, y, z, w) 映射到3维空间中的向量x，我们这样做：
　　${x \over w}, {y \over w}, {z \over w}, {w \over w}$
　　在3D图形程序设计中，从齐次坐标空间映射到3D空间时使用**投影透视**。
矩阵平移

图8
	我们能通过与下面的矩阵相乘把向量(x, y, z, 1)沿x轴移动px个单位，沿y轴移动py 个单位，沿z轴移动pz个单位：

将矩阵平移的D3DX函数是：
D3DXMATRIX *D3DXMatrixTranslation(
	D3DXMATRIX* pOut, // 返回平移后的矩阵.
	FLOAT x, // x轴移动的单位
	FLOAT y, // y轴移动的单位
	FLOAT z // z轴移动的单位
);
　　练习：让T(p)做为一个平移变换矩阵，v = [v1, v2, v3, 0]是也个任意向量。验证vT(p) = v（即，假如w = 0，验证通过平移不会改变向量）。
　　平移矩阵求逆只需要简单的将向量p取反即可：

## 矩阵旋转

图9
	我们能用下面的矩阵把一个向量围绕x,y 和z轴旋转δ弧度。注意：当我们俯视绕轴原点时，角度是指顺时针方向的角度。

将矩阵饶着x轴旋转的D3DX函数是：
D3DXMATRIX *D3DXMatrixRotationX(
	D3DXMATRIX* pOut, // 返回旋转后的矩阵
	FLOAT Angle // Angle是旋转的弧度
);



将矩阵饶着y轴旋转的D3DX函数是：
```C++
D3DXMATRIX *D3DXMatrixRotationY(
	D3DXMATRIX* pOut, // 返回旋转后的矩阵
	FLOAT Angle // Angle是旋转的弧度
);
```
将矩阵饶着z轴旋转的D3DX函数是：
```C++
D3DXMATRIX *D3DXMatrixRotationZ(
	D3DXMATRIX* pOut, // 返回旋转后的矩阵
	FLOAT Angle // Angle是旋转的弧度
);
```
旋转矩阵R的逆矩阵等于它的转置矩阵：RT= R-1。这样的矩阵我们说它们是互相垂直的。
矩阵缩放
{% asset_img 10.png %}
图10
　　我们能通过与下面的矩阵相乘把向量沿x轴缩放qx个单位，沿y轴缩放qy 个单位，沿z轴缩放qz个单位：
　　$S(q) = \begin{bmartix}q_x & 0 & 0 & 0 \\ 0 q_y & 0 & 0 & 0 \\ 0 & 0 & q_z 0 \\ 0 & 0 & 0 & 1 \\\end{bmartix}$
将矩阵缩放的D3DX函数是：
D3DXMATRIX *D3DXMatrixScaling(
	D3DXMATRIX* pOut, // 返回缩放后的矩阵
	FLOAT sx, // x轴缩放的比例
	FLOAT sy, // y轴缩放的比例
	FLOAT sz // z轴缩放的比例.
);
　　缩放矩阵求逆只需要将每个缩放因子取倒即可：
　　
综合变换
	常常我们要对一个向量进行一系列的变换。比如，我们可能先缩放一个向量，然后旋转它，最后把它平移到指定的位置。
　　例如：先把向量p = [5, 0, 0, 1] 在所有轴上缩小为原来的1/5，然后沿着y轴旋转π/4，最后把它在x轴上移动1个单位，在y轴上移动2个单位，在z轴上移动3个单位。
　　解答：注意我们必须完成缩放，沿y轴旋转，以及移动。我们设缩放、旋转、移动的变换矩阵分别是S, Ry, T，如下：
　　
　　
　　应用缩放，旋转，以及平移一系列变换，我们得到：
　　
　　我们能用矩阵乘法把几个变换矩阵转换成一个矩阵，它是非常有益的矩阵。比如，重新考虑这部分开始的例子。通过使用矩阵相乘把3个变换矩阵合成一个矩阵。注意我们必须按实际应用的顺序来进行矩阵相乘。
　　
那么 pQ = [1.707, 2, –3.707, 1]。
　　联合变换有提高效率的能力。假如我们需要对一组数量巨大的向量（在3D图形任务中是很普遍的）进行同样的缩放，旋转以及移动变换。替换这一系列的变换，即就象等式(5)中对每一个向量的做法，我们能把所有3个变换转换到一个矩阵中，即就象在等式(6)中的做法。这样我们只需要对每一个向量进行一次乘法就可以实现3种变换。这就减少了大量的向量-矩阵乘法操作。
一些向量变换函数
　　D3DX库分别提供了下边两个对点和向量的变换函数。D3DXVec3TransformCoord函数变换点同时设置向量第4个成员为1（用于变换点向量）。D3DXVec3TransformNormal函数变换向量并且设置第4个成员为0（用于变换方向向量）。
D3DXVECTOR3 *D3DXVec3TransformCoord(
		D3DXVECTOR3* pOut, // 返回的点向量
		CONST D3DXVECTOR3* pV, // 点向量
		CONST D3DXMATRIX* pM // 变换矩阵
　　);
D3DXMATRIX T(...); // 初始化矩阵
D3DXVECTOR3 p(...); // 初始化点
D3DXVec3TransformCoord( &p, &p, &T); // 变换一个点

D3DXVECTOR3 *WINAPI D3DXVec3TransformNormal(
		D3DXVECTOR3 *pOut, //返回的方向向量
		CONST D3DXVECTOR3 *pV, // 方向向量
		CONST D3DXMATRIX *pM //变换矩阵
);
D3DXMATRIX T(...); // 初始化变换矩阵
D3DXVECTOR3 v(...); // 初始化方向向量
D3DXVec3TransformNormal( &v, &v, &T); // 变换方向向量
注意：D3DX库也提供D3DXVec3TransformCoordArray和D3DXVec3TransformNormalArray来分别变换一个点数组和向量数组
平面
   一个平面能通过一个向量n和平面上的一个点p0来描述。这个向量n垂直于平面，它被称为此平面的法向量（如图11）。

图11

在图12中我们能够发现平面上任意一点p都满足如下等式。即：假如p、p0都是平面上一点，那么向量（p - p0）垂直于平面的法向量。

图12
　　当我们通过法向量n和平面上一个已知点来描述一个平面时，等式（7）又被写成这样：
　　
　　这时d = –n·p0。
### D3DX平面
在代码中描述一个平面：仅仅需要一个法向量n和常数d就可以了。因此我们就使用一个4D向量（我们记录成(n, d)）来实现它。D3DX库中用如下的结构来定义一个平面：
```C++
typedef struct D3DXPLANE
{
#ifdef __cplusplus
public:
	D3DXPLANE() {}
	D3DXPLANE( CONST FLOAT* );
	D3DXPLANE( CONST D3DXFLOAT16* );
	D3DXPLANE( FLOAT a, FLOAT b, FLOAT c, FLOAT d );
	// casting
	operator FLOAT* ();
	operator CONST FLOAT* () const;
	// unary operators
	D3DXPLANE operator + () const;
	D3DXPLANE operator - () const;
	// binary operators
	BOOL operator == ( CONST D3DXPLANE& ) const;
	BOOL operator != ( CONST D3DXPLANE& ) const;
#endif //__cplusplus
	FLOAT a, b, c, d;
} D3DXPLANE, *LPD3DXPLANE;
```
对照等式（8）可知：这里a, b和c是平面法向量n的成员，d就是那个常数。
### 点和平面的空间关系
我们判定点和平面的关系主要是利用等式(8)来实现。例如，假设平面(n, d)，我们能判定点p和平面的关系：
	假如n·p + d = 0，那么点p与平面共面。
	假如n·p + d >0，那么点p在平面的前面且在平面的正半空间里。
	假如n·p + d <0，那么点p在平面的背面且在平面的负半空间里。

下边的D3DX函数就是利用n·p + d 来判定点和平面的关系的函数：
```C++
FLOAT D3DXPlaneDotCoord(
	CONST D3DXPLANE *pP, // 平面
	CONST D3DXVECTOR3 *pV // 点
);
// 测试点相对于平面的位置
D3DXPLANE p(0.0f, 1.0f, 0.0f, 0.0f);
D3DXVECTOR3 v(3.0f, 5.0f, 2.0f);
float x = D3DXPlaneDotCoord( &p, &v );

if( x approximately equals 0.0f ) // v在平面.上
if( x > 0 ) // v在正半空间
if( x < 0 ) // v在负半空间
```
### 创建平面
我们能通过两种方法创建平面。
第一种方法，直接用指定法线和点创建平面。假设法线n和在平面上的已知点p0,我们就能求出d：
$n \cdot P_0 + d = 0$
$n \cdot P_0 = -d$
$-n \cdot P_0 = d$
　　
D3DX库提供如下函数来完成创建平面的任务：
```C++
D3DXPLANE *D3DXPlaneFromPointNormal(
	D3DXPLANE* pOut, // Result.
	CONST D3DXVECTOR3* pPoint, // Point on the plane.
	CONST D3DXVECTOR3* pNormal // The normal of the plane.
);
```
第二种方法，我们能通过在平面上的3个点创立一个平面。
假如有点p0, p1, p2，那么我们就能得到平面上的两个向量：
$u = p_1 - p_0$
$v = p_2 - p_0$
因此我们能通过把平面上的两个向量进行十字相乘得到平面的法线。回忆左手坐标系。
$n = u \times v$
那么–(n·p0) = d.
D3DX库提供如下函数来完成通过同一平面上的3个点确定一个平面：
```C++
D3DXPLANE *D3DXPlaneFromPoints(
	D3DXPLANE* pOut, // Result.
	CONST D3DXVECTOR3* pV1, // Point 1 on the plane.
	CONST D3DXVECTOR3* pV2, // Point 2 on the plane.
	CONST D3DXVECTOR3* pV3 // Point 3 on the plane.
);
```
### 标准化平面
有时我们可能想标准化一个平面的法向量，即标准化平面。初一想，好象我们只需象标准化其他向量一样标准化平面的法向量就可以了。但是回忆在等式n·p + d = 0中的d = –n·p0。我们明白法向量的长度将影响常数d。因此，假如我们标准化法向量，我们必须重新计算d.注意
${d \over ||n||} = -{n \over ||n||} \cdot P_0$
　　因此，我们有下边公式来标准化平面(n, d)的法向量：

我们能用下面的D3DX函数来标准化一个平面：
```C++
D3DXPLANE *D3DXPlaneNormalize(
	D3DXPLANE *pOut, // Resulting normalized plane.
	CONST D3DXPLANE *pP // Input plane.
);
```
变换平面
我们能够通过如下处理来变换一个面（n, d），就象一个4D向量通过乘以它渴望得到变换的变换矩阵的逆矩阵一样来达到变换目的。注意平面的法向量必须首先被标准化。
我们能用下面的D3DX函数来完成操作：
```C++
D3DXPLANE *D3DXPlaneTransform(
	D3DXPLANE *pOut, // Result
	CONST D3DXPLANE *pP, // Input plane.
	CONST D3DXMATRIX *pM // Transformation matrix.
);
```
示例代码：
```C++
D3DXMATRIX T(...); // Init. T to a desired transformation.
D3DXMATRIX inverseOfT;
D3DXMATRIX inverseTransposeOfT;
D3DXMatrixInverse( &inverseOfT, 0, &T );
D3DXMatrixTranspose( &inverseTransposeOfT, &inverseOfT );
D3DXPLANE p(...); // Init. Plane.
D3DXPlaneNormalize( &p, &p ); // make sure normal is normalized.
D3DXPlaneTransform( &p, &p, &inverseTransposeOfT );
```
点到平面上最近的点
　　假如我们在空间中有一个点p并且想找到在平面( n, d)上的与p最接近一个点q。注意如果平面的法向量是单位长度，这将简化问题。

图13
　　从图13我们能看出q = p + (–k_n)，k是有符号之分的从点p到平面的距离，也就是点p和q之间的有向距离。假如平面的法向量n是单位长度，那么n·p + d 就是从平面到点p有向距离.

### 射线
　　一条射线能用一个起点和方向来描述。射线的参数方程是：
　　$p(t)=p_{0} + tu$
　　{% asset_img 14.jpg %}
　　图14
　　p0 是射线的起点，u是射线的方向，t是参数。通过赋予不同的t值，我们能计算出在射线上不同的点。要描述一条射线，参数t范围就必须在[0, ∞)之间。实际上，假如我们让t∈(–∞, ∞),那么我们就能得到一条3维空间直线。
### 线/面相交
　　假设一条射线p(t) = p0 + tu 和 一个平面n·p + d = 0，我们想知道射线是否与平面相交，以及相交的交点信息（如果相交的话）。照这样做，我们把射线代入平面方程并且求满足平面方程的参数t，解答出来的参数就是相交的点。

　　
　　假如t 不在[0, ∞)之间，那么射线与平面不相交。
　　假如t 在[0, ∞)之间，那么射线与平面相交。且把参数代入射线方程就能找到交点：
　　
# 初始化Direct3D (Direct3D Initialization)
## Direct3D概述
　　Direct3D是一种底层图形API，它能让我们利用3D硬件加速来渲染3D世界。我们可以把Direct3D看作是应用程序和图形设备之间的中介。例如通知图形设备清空屏幕，应用程序将调用Direct3D的IDirect3DDevice9::Clear方法。图1.1显示了应用程序、Direct3D和图形设备之间的关系。
　　{% asset_img 11.jpg 图1.1 %}
　　图1.1中Direct3D所表示的是Direct3D中已定义的，供程序员使用的Direct3D接口和函数的集合。这些接口和函数代表了当前版本的Direct3D所支持的全部特性。注意：仅仅因为Direct3D支持某种特性，并不意味着你所使用的图形硬件（显卡）也能支持它。
　　如图1.1所示，在Direct3D和图形设备之间有一层中介——叫做硬件抽象层（HAL，Hardware Abstraction Layer）。Direct3D不能直接作用于图形设备，因为现在市面上的显卡种类实在是太多了并且每种显卡都有不同的性能和处理事件的方式。例如，两种不同的显卡实现清屏的方式也可能是不同的。因此，Direct3D要求设备制造商实现HAL。HAL是一组指示设备执行某种操作的特殊设备代码的集合。用这种方法，Direct3D避免了必须去了解某个设备的特殊细节，使它能够独立于硬件设备。
　　设备制造商在HAL中实现他们的产品所支持的所有特性。HAL将不会实现那些Direct3D支持但硬件产品不支持的特性。调用一个HAL中没有实现的Direct3D的函数将会出错，除非它是顶点处理操作，因为这个功能可以由软件模拟来实现。因此当使用某些仅由市面上少数显卡所支持的高级特性时，必须检测一下设备是否支持。（设备的功能将在1.3.8节中讲解）
### REF设备
　　你也许想把一些你的设备不支持的Direct3D函数写入程序。为了达到这个目的，Direct3D提供了REF设备,它用软件模拟了所有的Direct3D API。这允许你写并测试那些你的显卡不支持的Direct3D特性的代码。例如在本书的第四部分，某些人的显卡可能会不支持顶点和像素着色器。如果你的显卡不支持着色器，你仍然能够使用RE设备测试示例代码。懂得RE设备仅仅是为了发展，这是很重要的。它只会和DirectX SDK一起被装载，而不会发布给最终用户。 另外，RE设备实在是太慢了，除了测试以外它没有任何利用价值。
### D3DDEVTYPE
　　在代码中，我们用D3DDEVTYPE_HAL来定义HAL设备，它是D3DDEVTYPE枚举类型的一个成员。同样的，REF设备则由D3DDEVTYPE_REF来定义，它也属于D3DDEVTYPE枚举类型。记住这些类型很重要，因为在创建设备的时候我们需要指定我们将要使用的类型。
## COM
　　组件对象模型（COM, Component Object Model）是一种能使DirectX独立于编程语言和具有向下兼容性的技术。我们通常把COM对象作为一个接口，你可以把它当作达到某种目的的C++类来使用它。当使用C++写DirectX程序的时候，COM的大部分细节对我们来说是透明。但是有一件事，我们必须知道，那就是我们通过某个特殊的COM接口的函数或指针获得了另一个COM接口指针，而不是通过C++的新关键字来创建它。当我们使用完某个接口后，调用它的Release方法比直接Delete它更好。COM对象具有它们自己的内存管理。
　　对COM来说还有很多细节可以了解，但是掌握这些细节对于我们有效的使用DirectX不是必须的。
注意：COM接口都具有前缀大写字母“I”，例如表示一个表面的COM接口叫做IDirect3DSurface9。
## 一些准备工作
　　Direct3D的初始化过程要求我们对图形学基础知识和Direct3D类型有一定了解。本节将介绍这些知识和类型，以确保下一节能把焦点集中在讨论Direct3D的初始化上。
### 表面
　　表面是一个像素点阵，在Direct3D中主要用来存储2D图形数据。图1.2指明了表面的一些成分。由图可以看出表面数据就像一个矩阵，像素数据实际上存储在线性数组里面。
　　{% asset_img 1-2.jpg 图1.2 %}
表面的Width和Height是按像素计算的。Pitch以字节为单位。而且Pitch有可能比Width大且依赖于低层硬件，所以不能单纯的认为Pitch = Width * sizeof (pixelFormat)。
　　在代码中，我们可以使用IDirect3DSurface9接口来描述表面。这个接口提供若干方法来直接读写表面数据并且还有一个方法用来返回表面信息。IDirect3DSurface9中最重要的方法是：
* LockRect——使用这个方法，我们将获得一个指向表面内存的指针，然后，通过一系列指针运算，我们可以对表面上任一个像素点进行读、写操作。
* UnlockRect——当你调用了LockRect和完成了对表面内存的访问后，你必须调用这个方法给表面解锁。
* GetDesc——这个方法将通过填充D3DSURFACE_DESC结构来返回表面的描述信息。
最初锁定表面和改写每一像素看来稍微有点迷茫。下面的代码表示锁定表面并将每一像素染成红色：
```C++
// 假定_surface是一个指向IDirect3DSurface9接口的指针
// 假定每个像素为：32-bit的像素格式

// 取得表面描述
D3DSURFACE_DESC surfaceDesc;
_surface->GetDesc(&surfaceDesc);

// 取得被锁定表面的像素数据的指针
D3DLOCKED_RECT lockedRect;
_surface->LockRect(
		&lockedRect, // 指向被锁定表面的数据
		0, // 0表示锁定全部表面
		0); // 0表示没有指定锁定标记

// 遍例表面上的每个像素，将它们设为红色
DWORD* imageData = (DWORD*)lockedRect.pBits;
for(int i = 0; i < surfaceDesc.Height; i++)
{
	for(int j = 0; j < surfaceDesc.Width; j++)
	{
		// 取得纹理索引, 注意我们用pitch 除以4是因为pitch的单位是像素，
		// 并且一个DWORD类型占为4 bytes空间
		int index = i * lockedRect.Pitch / 4 + j;
		imageData[index] = 0xffff0000; // 每个像素设为红色
	}
}

_surface->UnlockRect();
```
程序中D3DLOCKED_RECT结构的定义如下：
```C++
typedef struct _D3DLOCKED_RECT {
	INT Pitch; // 表面深度
	void *pBits; // 指向表面开始处的内存
} D3DLOCKED_RECT;
```
在这里有一些关于表面锁定代码的一些说明。32-bit像素格式这个设定很重要，我们把bits转换成DWORDs。这让我们能把每一个DWORD视为表示一个像素。

### Multisampling
由于使用像素矩阵来表示图像，在显示时会出现锯齿状，Multisampling就是使其变得平滑的技术。它的一种最普通的用法即为——全屏抗锯齿（看图1.3）。
{% asset_img 1-3.jpg 图1.3 %}　　
D3DMULTISAMPLE_TYPE枚举类型使我们可以指定全屏抗锯齿的质量等级：
- D3DMULTISAMPLE_NONE——不使用全屏抗锯齿。
- D3DMULTISAMPLE_1_SAMPLE…D3DMULTISAPLE_16_SAMPLE——设定1-16级的等级。

本书的示例程序中没有使用全屏抗锯齿的功能，因为它大大的降低了程序运行速度。如果你实在很想使用它的话，要记住使用IDirect3D9::CheckDeviceMultisampleType来检测你的显卡是否支持。

### 像素格式
当我们创建一个表面或纹理时，经常需要指定这些Direct3D资源的像素格式。它是由D3DFORMAT枚举类型的一个成员来定义的。这里例举一部分：
* D3DFMT_R8G8B8——表示一个24位像素，从左开始，8位分配给红色，8位分配给绿色，8位分配给蓝色。
* D3DFMT_X8R8G8B8——表示一个32位像素，从左开始，8位不用，8位分配给红色，8位分配给绿色，8位分配给蓝色。
* D3DFMT_A8R8G8B8——表示一个32位像素，从左开始，8位为ALPHA通道，8位分配给红色，8位分配给绿色，8位分配给蓝色。
* D3DFMT_A16B16G16R16F——表示一个64位浮点像素，从左开始，16位为ALPHA通道，16位分配给蓝色，16位分配给绿色，16位分配给红色。
* D3DFMT_A32B32G32R32F——表示一个128位浮点像素，从左开始，32位为ALPHA通道，32位分配给蓝色，32位分配给绿色，32位分配给红色。
想了解全部的像素格式请查看SDK文档中的D3DFORMAT部分。
注意：这前三种格式（D3DFMT_R8G8B8、D3DFMT_X8R8G8B8、D3DFMT_A8R8G8B8）是最常用并为大部分显卡所支持。但浮点像素格式或其它一些类型的支持并不是很广泛，在使用它们前请先检测你的显卡，看是否支持。

### 内存池
表面和其它一些Direct3D资源被放在多种内存池中。内存池的种类由D3DPOOL枚举类型的一个成员来指定。可用到的内存池有下列几种：
* D3DPOOL_DEFAULT——表示Direct3D将根据资源的类型和用途把它们放在最合适的地方。这有可能是显存、AGP内存或者系统内存中。值得注意的是，这种内存池中的资源必须要在IDirect3DDevice9::Reset被调用之前消毁掉，并且再次使用时必须重新初始化。
* D3DPOOL_MANAGED——资源将由Direct3D管理并且按设备的需要来指定放在显存还是放在AGP内存中。当应用程序访问和改变资源时它先把这些资源拷贝到系统内存中，当需要时Direct3D会自动把它们拷贝到显存里。
* D3DPOOL_SYSTEMMEM——指定资源放在系统内存中。
* D3DPOOL_SCRATCH——指定资源放在系统内存中，它与D3DPOOL_SYSTEMMEM不同之处在于使用这些资源不必受图形设备的限制。因此，参数使图形设备不能访问该内存池的资源，但资源可以相互拷贝。
1.3.5 交换链和页面切换
　　Direct3D通常创建2~3个表面组成一个集合，即为交换链，通常由IDirect3DSwapChain接口来表示。我们不必去了解它更详细的细节。我们也很少去管理它，通常Direct3D会自己去管理。所以我们只要大概的了解一下它就可以了。
　　交换链以及页面切换技巧被用在使两帧动画之间过度更平滑。图1.4展示的是一个有两个绘制表面的交换链。

图1.4
   如图1.4，在Front Buffer中的表面将用来在屏幕上显示。显示器不能即时显示Front Buffer中表示的图像；通常情况下，它是每六十分之一秒刷新显示一次，即刷新率为60赫兹。应用程序的帧率经常与监视器的刷新率不同步（比如应用程序的渲染帧速度可能比显示器的刷新速度快）。然而，我们不能在显示器显示完成当前帧之前就更新有下一帧动画的Front Buffer内容，但是我们又不想让程序停止渲染而去等待显示器显示。因此，我们渲染另一个屏幕表面Back Buffer。当监视器将Front Buffer显示出来后，Front Buffer就被放到交换链的末端，即变成图中的Back Buffer，而Back Buffer就会变成交换链中的Front Buffer。这个过程就叫做presenting。图1.5表示了交换的整个过程。

图1.5
　　因此，我们绘图代码的结构就会像下面这样：
1． Render to back buffer
2． Present the back buffer
3． Goto (1)
1.3.6 深度缓冲
　　深度缓冲也是一个表面，但它不是用来存储图像数据的，而是用来记录像素的深度信息。它将确定哪一个像素最后被绘制出来。所以，如果要绘制640*480分辨率的图片，那么就会有640*480个深度值。
　　
　　图1.6
图1.6展示了一个简单的场景，在这个场景里，一个物体把将另一个物体的一部分遮住了。为了使Direct3D能确定物体的前后关系并正确的绘制出来，我们使用一种深度缓冲，又叫做z-buffering的技术。
　　深度缓冲为每一个像素计算深度值，并进行深度测试。通过深度测试，我们可以比较出哪个像素离照相机更近，并将它画出来。这样就可以只绘制最靠近照相机的像素，被遮住的像素就不会被画出来。
　　深度缓冲的格式决定着深度测试的精确性。一个24位的深度缓冲比16位的深度缓冲更精确。通常，应用程序在24位深度缓冲下就能工作的很好，但是Direct3D也同时支持32位的深度缓冲。
* D3DFMT_D32——表示32位深度缓冲
* D3DFMT_D24S8——表示24位深度缓冲并保留8位模版缓冲（stencil buffer）
* D3DFMT_D24X8——表示24位深度缓冲
* D3DFMT_D24X4S4——表示24位深度缓冲并保留4位模版缓冲
* D3DFMT_D16——表示16位深度缓冲

注意：关于模版缓冲的问题将在第八章详细说明。
1.3.7 顶点处理
　　顶点是3D图形学的基础，它能够通过两种不同的方法被处理，一种是软件方式（software vertex processing），一种是硬件方式（hardware vertex processing），前者总是被支持且永远可用，后者必须要显卡硬件支持顶点处理才可用。
　　使用硬件顶点处理总是首选，因为它比软件方式更快，而且不占用CPU资源，这意味CPU至少可以有更多的空闲时间进行别的计算。
注意：如果一块显卡支持硬件顶点处理的话，也就是说它也支持硬件几何转换和光源计算。
1.3.8 设备能力
　　Direct3D支持的每一项特性都对应于D3DCAPS9结构的一个数据成员。初始化一个D3DCAPS9实例应该以你的设备实际支持特性为基础。因此，在我们的应用程序里，我们能够通过检测D3DCAPS9结构中相对应的某一成员来检测设备是否支持这一特性。
　　下面将举例说明，假设我们想要检测显卡是否支持硬件顶点处理（换句话说，就是显卡是否支持硬件几何转换和光源计算）。通过查阅SDK中的D3DCAPS9结构，可以得知数据成员D3DCAPS9::DevCaps中的D3DDEVCAPS_HWTRANSFORMANDLIGHT位表示硬件是否支持硬件顶点处理即硬件几何变换和光源计算。程序如下：
```C++
bool supportsHardwareVertexProcessing;

// 如果为真，意味着硬件设备支持它
if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
{
	// 支持
	supportsHardwareVertexProcessing = true;
}
else
{
	// 不支持
	hardwareSupportsVertexProcessing = false;
}
```
注意：DevCaps即为“device capabilities”
　　　下一节将学习怎样根据硬件的实际情况来初始化D3DCAPS9
      我们建议你阅读SDK中关于D3DCAPS9的结构，它完整的列出了Direct3D支持的特性。
1.4 初始化Direct3D
   下面几点说明怎样初始化Direct3D。根据下边的步骤你能初始化Direct3D：
1． 获得一个IDirect3D9接口指针。这个接口用于获得物理设备的信息和创建一个IDirect3DDevice9接口，它是一个代表我们显示3D图形的物理设备的C++对象。
2． 检查设备能力（D3DCAPS9），搞清楚主显卡是否支持硬件顶点处理。我们需要知道假如它能支持，我们就能创建IDirect3DDevice9接口。
3． 初始化一个D3DPRESENT_PARAMETERS结构实例，这个结构包含了许多数据成员允许我们指定将要创建的IDirect3DDevice9接口的特性。
4． 创建一个基于已经初始化好的D3DPRESENT_PARAMETERS结构的IDirect3DDevice9对象。它是一个代表我们显示3D图形的物理设备的C++对象。
请注意，本书使用主显示设备绘制3D图形，如果你的机子只有一块显卡，那它就是主显示设备。如果你有多个显卡，那么你当前使用的显卡将会成为主显示设备（如：用来显示Windows桌面的显卡）。
1.4.1获得IDirect3D9接口
　　Direct3D的初始化是从获得一个IDirect3D9接口指针开始的。使用一个专门的Direct3D函数来完成这个工作是非常容易的，代码如下：
IDirect3D9* _d3d9;
_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
   Direct3DCreate9的唯一一个参数总是D3D_SDK_VERSION，这可以保证应用程序通过正确的头文件被生成。如果函数调用失败，那么它将返回一个空指针。
   IDirect3D9对象通常有两个用途：设备列举和创建IDirect3DDevice9对象。设备列举即为查明系统中显示设备的技术特性，显示模式、格式，以及其它每一种显卡各自支持的特性。创建代表物理设备的IDirect3DDevice9对象，我们需要利用这个物理设备的显示模式结构和格式来创建它。为了找到一个工作配置，我们必须使用IDirect3D9的列举方法。
　　然而，设备列举实在太慢了，为了使Direct3D运行得尽可能快，我们通常不使用这个测试，除了下一节所谈到的一项测试。为了安全跳过它，我们可以选择总是被所有显卡都支持的“安全”配置。
1.4.2 检测硬件顶点处理
　　当我们创建一个IDirect3DDevice9对象来表示主显示设备时，必须要设定其顶点处理的类型。如果可以的话，当然要选用硬件顶点处理，但是由于并非所有显卡都支持硬件顶点处理，因此我们必须首先检查显卡是否支持。
　　首先我们要根据主显示设备的技术特性来初始化D3DCAPS9实例。可以使用如下方法：
HRESULT IDirect3D9::GetDeviceCaps(
	UINT Adapter,
	D3DDEVTYPE DeviceType,
	D3DCAPS9 *pCaps
);
* Adapter——指定要获得哪个显示适配器的特性
* DeviceType——指定设备类型（硬件设备（D3DDEVTYPE_HAL），软件设备（D3DDEVTYPE_REF））
* PCaps——返回一个已初始化的D3DCAPS9结构

　　然后，我们就可以象1.3.8部分那样检测显卡的能力了。下面就是代码片段：
// 填充主显示设备的能力（D3DCAPS9结构）
```C++
D3DCAPS9 caps;
d3d9->GetDeviceCaps(
	D3DADAPTER_DEFAULT, // 主显示设备
	deviceType, // 设备类型，一般是D3DDEVTYPE_HAL.
	&caps); // 返回填充后的D3DCAPS9 结构，包含主显示设备的能力

// 是否可以使用硬件顶点处理?
int vp = 0;
if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
{
	// 是，支持硬件顶点处理
	vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
}
else
{
	// 不，只能用软件顶点处理
	vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
}
```
观察代码，我们使用变量vp来存储顶点处理类型。这是因为在稍后创建IDirect3DDevice9对象时要求指定其顶点处理的类型。
注意：标识符D3DCREATE_HARDWARE_VERTEXPROCESSING和D3DCREATE_SOFTWARE_VERTEXPROCESSING是预定义的值，它们分别代表硬件顶点处理和软件顶点处理。

技巧：若我们开发有一些新的，高级特性的程序，在使用前我们总是先检查硬件是否支持这些特性。
注意：如果一个应用程序在你的机子上不能运行，说明它用到的一些特性可能你的显卡并不支持，可以试试把设备类型换成REF。
1.4.3 填充D3DPRESENT_PARAMETERS结构
初始化过程的下一步是填充一个D3DPRESENT_PARAMETERS结构的实例。这个结构用于设定我们将要创建的IDirect3DDevice9对象的一些特性，它的定义如下：
```C++
typedef struct _D3DPRESENT_PARAMETERS_ {
	UINT BackBufferWidth;
	UINT BackBufferHeight;
	D3DFORMAT BackBufferFormat;
	UINT BackBufferCount;
	D3DMULTISAMPLE_TYPE MultiSampleType;
	DWORD MultiSampleQuality;
	D3DSWAPEFFECT SwapEffect;
	HWND hDeviceWindow;
	BOOL Windowed;
	BOOL EnableAutoDepthStencil;
	D3DFORMAT AutoDepthStencilFormat;
	DWORD Flags;
	UINT FullScreen_RefreshRateInHz;
	UINT PresentationInterval;
} D3DPRESENT_PARAMETERS;
```
下面介绍其比较重要的数据成员，至于更详细的信息，请查阅SDK：
BackBufferWidth——后备缓冲表面的宽度（以像素为单位）
BackBufferHeight——后备缓冲表面的高度（以像素为单位）
BackBufferFormat——后备缓冲表面的像素格式（如：32位像素格式为D3DFMT——A8R8G8B8）
BackBufferCount——后备缓冲表面的数量，通常设为“1”，即只有一个后备表面
MultiSampleType——全屏抗锯齿的类型，详情请看SDK
MultiSampleQuality——全屏抗锯齿的质量等级，详情看SDK
SwapEffect——指定表面在交换链中是如何被交换的，取D3DSWAPEFFECT枚举类型中的一个成员。其中D3DSWAPEFFECT_DISCARD是最有效的
hDeviceWindow——与设备相关的窗口句柄，你想在哪个窗口绘制就写那个窗口的句柄
Windowed——BOOL型，设为true则为窗口模式，false则为全屏模式
EnableAutoDepthStencil——设为true，D3D将自动创建深度/模版缓冲
AutoDepthStencilFormat——深度/模版缓冲的格式
Flags——一些附加特性，设为0或D3DPRESENTFLAG类型的一个成员。下列两个最常用的标志
全部的标志请查阅SDK：
　　D3DPRESENTFLAG_LOCKABLE_BACKBUFFER——设定后备表面能够被锁定，这会降低应用程序的性能
　　D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL——深度/模版缓冲在调用IDirect3DDevice9::present方法后将被删除，这有利于提升程序性能
FullScreen_RefreshRateInHz——刷新率，设定D3DPRESENT_RATE_DEFAULT使用默认刷新率
PresentationInterval——属于D3DPRESENT成员，又有两个常用标志，其余请查SDK：
         D3DPRESENT_INTERVAL_IMMEDIATE——立即交换
         D3DPRESENT_INTERVAL_DEFAULT——D3D选择交换速度，通常等于刷新率
填充示例如下：
```C++
D3DPRESENT_PARAMETERS d3dpp;
d3dpp.BackBufferWidth = 800;
d3dpp.BackBufferHeight = 600;
d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; //像素格式
d3dpp.BackBufferCount = 1;
d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
d3dpp.MultiSampleQuality = 0;
d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
d3dpp.hDeviceWindow = hwnd;
d3dpp.Windowed = false; // fullscreen
d3dpp.EnableAutoDepthStencil = true;
d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; // depth format
d3dpp.Flags = 0;
d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
```
### 1.4.4 创建IDirect3DDevice9对象
　　在填充完了D3DPRESENT_PARAMETERS结构后，我们就可以用下面的方法创建一个IDirect3DDevice9对象了：
HRESULT IDirect3D9::CreateDevice(
	UINT Adapter,
	D3DDEVTYPE DeviceType,
	HWND hFocusWindow,
	DWORD BehaviorFlags,
	D3DPRESENT_PARAMETERS *pPresentationParameters,
	IDirect3DDevice9** ppReturnedDeviceInterface
);
* Adapter——指定对象要表示的物理显示设备
* DeviceType——设备类型，前面说过
* hFocusWindow——同我们在前面d3dpp.hDeviceWindow的相同
* BehaviorFlags——设定为D3DCREATE_SOFTWARE_VERTEXPROCESSING或者D3DCREATE_HARDWARE_VERTEXPROCESSING
* pPresentationParameters——指定一个已经初始化好的D3DPRESENT_PARAMETERS实例
* ppReturnedDeviceInterface——返回创建的设备
例子：
```C++
IDirect3DDevice9* device = 0;
hr = d3d9->CreateDevice(
	D3DADAPTER_DEFAULT, // primary adapter
	D3DDEVTYPE_HAL, // device type
	hwnd, // window associated with device
	D3DCREATE_HARDWARE_VERTEXPROCESSING, // vertex processing type
	&d3dpp, // present parameters
	&device); // returned created device
if( FAILED(hr) )
{
	::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
	return 0;
}
```
1.5 初始化Direct3D实例
在本章的例程中，初始化了一个Direct3D应用程序并用黑色填充显示窗口（如图1.7）。

图1.7
   本书所有的应用程序都包含了d3dUtility.h和d3dUtility.cpp这两个文件，它们所包含的函数实现了所有Direct3D应用程序都要去做的一些常见的功能。例如：创建一个窗口、初始化Direct3D、进入程序的消息循环等。将这些功能封装在函数中能使示例程序更加突出该章的主题。另外，在我们学习本书的过程中还会在这两个文件中加上一些通用的代码。
## 1.5.1 d3dUtility.h/cpp
　　在开始本章的例程之前，让我们先熟悉一下d3dUtility.h/cpp所提供的函数。d3dUtility.h如下：
// 包含主要的Direct3DX头文件，这将包含我们需要的另外的Direct3D头文件
```C++
#include <d3dx9.h>
namespace d3d
{
	bool InitD3D(
		HINSTANCE hInstance, // [in] 应用程序实例
		int width, int height, // [in] Back buffer尺寸
		bool windowed, // [in] 是否全屏
		D3DDEVTYPE deviceType, // [in] HAL 或 REF
		IDirect3DDevice9** device); // [out] 创建的设备

	int EnterMsgLoop(
		bool (*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);

	template<class T> void Release(T t)
	{
		if( t )
		{
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t)
	{
		if( t )
		{
			delete t;
			t = 0;
		}
	}
}
```
InitD3D——初始化一个应用程序主窗口并进行Direct3D的初始化。如果成功，则输出IDirect3DDevice9接口指针。从它的参数我们可以发现，我们能够设置窗口的大小和以窗口模式运行还是全屏模式运行。要知道它实现的细节，请看示例代码。
EnterMsgLoop——这个函数封装了应用程序的消息循环。它需要输入一个显示函数的函数指针，显示函数为程序中绘制图形的代码块，这样做是为了使显示函数能够在空闲的时候被调用并显示场景，它的实现如下：
```C++
int d3d::EnterMsgLoop( bool (*ptr_display)(float timeDelta) )
{
    MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
　　static float lastTime = (float)timeGetTime();
 
   	while(msg.message != WM_QUIT)
   	{
	    if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	    {
		    ::TranslateMessage(&msg);
		    ::DispatchMessage(&msg);
	    }
		else
        {
		     float currTime = (float)timeGetTime();
		     float timeDelta = (currTime - lastTime)*0.001f;
		     ptr_display(timeDelta); // call display function
		     lastTime = currTime;
        }
	}
    return msg.wParam;
}
```
与“time”有关的代码用于计算每次调用显示函数的时间间隔，即是每帧的时间。
Release——这个模版函数能方便的释放COM接口并将它们的值设为NULL
Delete——这个模版函数能方便的删除一个对象并将指向其的指针设为NULL
WndProc——应用程序主窗口的回调函数
1.5.2 实例框架
　　通过实例框架，我们形成了一种通用的方法去构造本书的示例程序。每一个例程都含有三个函数的实现，当然这不包括回调函数和WinMain主函数。这三个函数用特定的代码实现特定的功能。这三个函数是：
* bool Setup()——在这个函数里，我们将准备一切该程序需要用到的东西，包括资源的分配，检查设备能力，设置应用程序的状态
* void Clearup()——这个函数将释放Setup()中分配的资源，如分配的内存。
* bool Display(float timeDelta)——这个函数包含所有与我们绘图和显示有关的代码。参数timeDelta为每一帧的间隔时间，用来控制每秒的帧数。
1.5.3 D3D Init实例
　　这个示例程序将创建并初始化一个Direct3D应用程序，并用黑色填充屏幕。注意，我们使用了通用函数简化了初始化过程。
　　首先，我们要包含d3dUtility.h头文件，并为设备声明一个全局变量：
#include "d3dUtility.h"

IDirect3DDevice9* Device = 0;
　　然后实现我们的框架函数：
```C++
bool Setup()
{
	return true;
}

void Cleanup()
{

}
```
　　在这个程序中，我们不需要使用任何资源或触发任何事件，所以这两个函数都为空。
```C++
bool Display(float timeDelta)
{
	if( Device )
	{
　　　　Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
　　　　　0x00000000, 1.0f, 0);
		Device->Present(0, 0, 0, 0); // 页面切换
	}
	return true;
}
```
　　Display方法调用了IDirect3DDevice::Clear方法，分别用黑色和1.0填充后备表面和深度/模版缓冲。如果应用程序不停止的话，我们会一直执行这个操作。IDirect3DDevice::Clear声明如下：
```C++
HRESULT IDirect3DDevice9::Clear(
	DWORD Count,
	const D3DRECT* pRects,
	DWORD Flags,
	D3DCOLOR Color,
	float Z,
	DWORD Stencil
);
```
* Count——pRects组中的矩形的个数
* pRects——将要清除的屏幕矩形的数组，这使我们可以清除屏幕的某一部分
* Flags——指定在哪些表面上执行清除表面的操作
         D3DCLEAR_TARGET——目的表面，通常为后备表面
         D3DCLEAR_ZBUFFER——深度缓冲
         D3DCLEAR_STENCIL——模版缓冲
* Color——使用什么颜色填充清除的表面
* Z——设置深度缓冲的值
* Stencil——设置模版缓冲的值
   屏幕被填充后，要调用IDirecte3DDevice9::Present方法进行后备表面的交换。
   
Windows 回调函数为一组事件集，即，我们可按ESC键让程序退出。
```C++
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
		case WM_DESTROY:
		::PostQuitMessage(0);
		break;

		case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
```
最后，WinMain按如下步骤运行：
1. 初始化主显示窗口和Direct3D
2. 调用Setup进行程序的准备工作
3. 使用Display函数作为参数进入消息循环
4. 清除应用程序最后释放IDirecte3DDevice9对象
```C++
int WINAPI WinMain(HINSTANCE hinstance,	HINSTANCE prevInstance,	PSTR cmdLine,int showCmd)
{
	if(!d3d::InitD3D(hinstance,	800, 600, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();
	return 0;
}
```
就像你所看到的，我们的例程的模板结构是相当简洁的：有效的操作窗口函数、 Direct3D的初始化过程。这本书中的大部分程序，都是通过执行Setup, Cleanup, 和Display这三个函数来实现。
   
注意：不要忘了在你的工程中加入d3d9.lib、d3dx9.lib、winmm.lib 这三个库！


第二章 渲染管线
(The Rendering Pipeline)
　　本章的主题是渲染管线。它是用来创建为3D世界进行几何描述的2D图形并设定一个虚拟照相机确定这个世界中哪一部分将被透视投影到屏幕上。
　　
　　图2.1
目标
* 要弄清楚我们怎样在Direct3D中表示3D物体
* 学习怎样模拟虚拟照相机
* 弄懂渲染管线——这个过程是用几何学来表现3D场景和用它来产生2D图象。
2.1表现模型
　　一个场景是多个物体或模型的集合。一个物体可以用三角形网格（triangle mesh）来近似表示，如图2.2所示。由三角形网格建立一个物体，我们称之为建模。3D世界中最基本的图元就是三角形，但是Direct3D也支持点图元和线图元但我们都不常用到。不过在学到第14章的粒子系统的时候，将会用到点图元。

图2.2
   一个多边形的两边相交的点叫做顶点。为了描述一个三角形，我们通常指定三个点的位置来对应三角形的三个顶点（如图2.3），这样我们就能够很明确的表示出这个三角形了。

图2.3
2.1.1 顶点格式
　　我们以前定义的点在数学上来说是正确的，但是当我们在Direct3D环境中使用它的时候就会觉得很不完善。这是因为在Direct3D中的顶点包含了许多附加的属性，而不再单纯的只有空间位置的信息了。例如：一个顶点可以有颜色和法线向量属性（这两个属性分别在第四章和第五章介绍）。Direct3D让我们可以灵活的构造自己的顶点格式。换句话说，我们可以自己定义顶点的成员。
　　为了创建一个自定义的顶点结构，我们首先要创建一个包含能存放我们选择的顶点数据的结构。例如，下面我们举出两种不同顶点数据类型的例子，一种包含了位置和颜色信息，第二种则包含了位置，法线向量，纹理坐标信息（“纹理”见第六章）。
```C++
struct ColorVertex
{
	float _x, _y, _z; // 位置
	DWORD _color; // 颜色
};

struct NormalTexVertex
{
	float _x, _y, _z; // 位置
	float _nx, _ny, _nz; // 法线向量
	float _u, _v; // 纹理坐标
};
```
   一旦我们有了完整的顶点格式，我们就要使用灵活顶点格式（FVF）的组合标志来描述它。例如第一个顶点结构，我们要使用如下的顶点格式：
#define FVF_COLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)
   上面的顶点结构表明它包含位置和颜色属性。

   而第二种结构则要使用：
#define FVF_NORMAL_TEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
   上面的顶点结构表明它包含了位置，法线向量，纹理坐标的属性（这些常量是D3D内置的）。
   
　　有一点要注意，你的标志的顺序必须要和你的顶点结构的顺序一一对应。如果想知道所有的D3DFVF标志，请查阅SDK文档。
2.1.2 三角形
　　三角形是构建3D物体的基本图形。为了构造物体，我们创建了三角形列表（triangle list）来描述物体的形状和轮廓。三角形列包含了我们将要画的每一个三角形的数据信息。例如为了构造一个矩形，我们把它分成两个三角形，如图2.4所示，最后指定每个三角形的顶点。
　　
　　图2.4
Vertex rect[6] = {v0, v1, v2, // 三角形0
				v0, v2, v3}; // 三角形1
注意：指定三角形顶点的顺序是很重要的，将会按一定顺序环绕排列，这会在2.3.4节学习相关的内容。
2.1.3 索引
　　3D物体中的三角形经常会有许多共用顶点。如图2.4所表示的矩形。虽然现在仅有两个点被重复使用，但是当要表现一个更精细更复杂的模型的时候，重复的顶点数将会变得很大。例如图2.5所示的立方体，仅有八个顶点，但是当用三角形列表示它的时候，所有的点都被重复使用。

图2.5
　　为了解决这个问题，我们引入索引（indices）这个概念。它的工作方式是：我们创建一个顶点列表和一个索引列表（index list）。顶点列表包含所有不重复的顶点，索引列中则用顶点列中定义的值来表示每一个三角形的构造方式。回到那个矩形的示例上来，它的顶点列表的构造方式如下：
Vertex vertexList[4] = {v0, v1, v2, v3};
   
   索引列表则定义顶点列中的顶点是如何构造这两个三角形的：
WORD indexList[6] = {0, 1, 2, //三角形0
				0, 2, 3}; //三角形1
   也就是说，用顶点列表中的0（vertexList[0]）、1（vertexList[1]）和2（vertexList[2]）顶点构成三角形0；用顶点列表中的0（vertexList[0]）、2（vertexList[2]）和3（vertexList[3]）顶点构成三角形1。
## 2.2虚拟照相机
　　照相机确定3D世界中的哪部分是可见的，因而需要将哪部分转换为2D图形。在3D世界中照相机被放置和定向，并且定义其可视体，图2.6展示了我们的照相机模型。
　　{% asset_img 2.6.png %}
　　图2.6
　　可视体是由可视角度和前裁剪面（Near Plane）与后裁剪面（Far Plane）定义的一个平截头体。之所以要选择平截头体构造可视体，是因为我们的显示器都是矩形的。在可视体中不能被看见的物体都会被删除，删除这种数据的过程就叫做“裁剪”。
　　投影窗口（Projection Window）是可视体内的3D几何图形投影生成的用来显示3D场景的2D图像的2D区域。重要的是要知道，我们使用min=(-1,-1)和max=(1,1)来定义投影窗口的大小。
　　为了简化本书接下来的部分绘制，我们使前裁剪面与投影窗口在同一平面上。并且，注意Direct3D中定义的投影平面（即投影窗口所在的平面）是Z = 1的平面。
## 2.3 渲染管线

一旦我们描述几何学上的3D场景和设置了虚拟照相机，我们要把这个场景转换成2D图象显示在显示器上。这一系列必须完成的操作就叫做渲染管线。图2.7展示了一个简化的渲染管线，随后将详细解释图中的每一部分。
　　{% asset_img 2.7.png %}
　　图2.7
渲染管线中的许多级都是从一个坐标系到另一个坐标的几何变换。这些变换都通过矩阵变换来实现。Direct3D为我们进行变换计算并且如果显卡支持硬件变换的话那就更有利了。使用Direct3D进行矩阵变换，我们唯一要做的事就是提供从一个系统变换到另一个系统的变换矩阵就可以了。我们使用IDirect3DDevice9::SetTranform方法提供变换矩阵。它输入一个表示变换类型的参数和一个变换矩阵。如图2.7所示，为了进行一个从自身坐标系到世界坐标系的变换，我们可以这样写：
`Device->SetTransform(D3DTS_WORLD, &worldMatrix);`
   
### 2.3.1自身坐标系（Local Space）
　自身坐标系又叫做建模空间，这是我们定义物体的三角形列的坐标系。自身坐标系简化了建模的过程。在物体自己的坐标系中建模比在世界坐标系中直接建模更容易。例如，在自身坐标系中建模不像在世界坐标系中要考虑本物体相对于其他物体的位置、大小、方向关系。
　　{% asset_img 2.8.png %}
　　图2.8
2.3.2世界坐标系（World Space）
　　一旦我们构造了各种模型，它们都在自己的自身坐标系中，但是我们需要把它们都放到同一个世界坐标系中。物体从自身坐标系到世界坐标系中的换叫做世界变换。世界变换通常是用平移、旋转、缩放操作来设置模型在世界坐标系中的位置、大小、方向。世界变换就是通过各物体在世界坐标系中的位置、大小和方向等相互之间的关系来建立所有物体。
　　
　　图2.9
   世界变换由一个矩阵表示，并且在Direct3D中调用IDirect3DDevice9::SetTransform方法设置它，记住将转换类型设为D3DTS_WORLD。例如我们要在世界坐标系中放置一个立方体定位在（-3，2，6）和一个球体定位在（5，0，-2），我们可以这样写程序：
//创建立方体的世界矩阵（一个平移矩阵）
D3DXMATRIX cubeWorldMatrix;
D3DXMatrixTranslation(&cubeWorldMatrix, -3.0f, 2.0f, 6.0f);

//创建球体的世界矩阵（一个平移矩阵）
D3DXMATRIX sphereWorldMatrix;
D3DXMatrixTranslation(&sphereWorldMatrix, 5.0f, 0.0f, -2.0f);


// 变换立方体，然后绘制它
Device->SetTransform(D3DTS_WORLD, &cubeWorldMatrix);
drawCube(); // draw the cube

// 因为球体使用一个不同的世界变换，我们必须更改世界矩阵为球体的～，
// 如果不更改，球体将绘制在上一个世界矩阵的位置上（立方体的世界矩阵）
Device->SetTransform(D3DTS_WORLD, &sphereWorldMatrix);
drawSphere(); // 绘制球体
   这是个非常简单的实例，没有用到矩阵的旋转和缩放。但是一般很多物体都需要进行这些变换，不过这个例子也还是展示了世界变换是怎样进行的。
2.3.3视图坐标系（View Space）
　　世界坐标系中的几何图与照相机是相对于世界坐标系而定义的，如图2.10所示。然而在世界坐标系中当照相机是任意放置和定向时，投影和其它一些操作会变得困难或低效。为了使事情变得更简单，我们将照相机平移变换到世界坐标系的源点并把它的方向旋转至朝向Z轴的正方向，当然，世界坐标系中的所有物体都将随着照相机的变换而做相同的变换。这个变换就叫做视图坐标系变换（view space transformation）。
　　
　　图2.10
   视图坐标的变换矩阵可以通过如下的D3DX函数计算得到：
D3DXMATRIX *D3DXMatrixLookAtLH(
	D3DXMATRIX* pOut, // 指向返回的视图矩阵
	CONST D3DXVECTOR3* pEye, // 照相机在世界坐标系的位置
	CONST D3DXVECTOR3* pAt, // 照相机在世界坐标系的目标点
	CONST D3DXVECTOR3* pUp // 世界坐标系的上方向(0, 1, 0)
);
   pEye参数指定照相机在世界坐标系中的位置，pAt参数指定照相机所观察的世界坐标系中的一个目标点，pUp参数指定3D世界中的上方向，通常设Y轴正方向为上方向，即取值为（0，1，0）。
　　例如：假设我们要把照相机放在点（5，3，-10），并且目标点为世界坐标系的中点（0，0，0），我们可以这样获得视图坐标系变换矩阵：
D3DXVECTOR3 position(5.0f, 3.0f, –10.0f);
D3DXVECTOR3 targetPoint(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 worldUp(0.0f, 1.0f, 0.0f);

D3DXMATRIX V;
D3DXMatrixLookAtLH(&V, &position, &targetPoint, &worldUp);
   视图坐标系变换也是通过IDirect3DDevice9::SetTransform来实现的，只是要将变换类型设为D3DTS_VIEW，如下所示：
Device->SetTransform(D3DTS_VIEW, &V);
2.3.4背面拣选（Backface Culling）
　　一个多边形有两个表面，我们将一个标为正面，一个为背面。通常，后表面总是不可见的，这是因为场景中大多数物体是密封的。例如盒子、圆柱体、箱子、characters等，并且我们也不能把照相机放入物体的内部。因此照相机永不可能看到多边形的背面。这是很重要的，如果我们能看背面，那么背面拣选就不可能工作。
　　图2.11表示了一个物体在视图坐标系中的正面。一个多边形的边都是面向照相机叫正面多边形，而一个多边形的边都背对照相机叫背面多边形。
　　
　　图2.11
   由图2.11可知，正面多边形挡住了在它后面的背面多边形，Direct3D将通过拣选（即删除多余的处理过程）背面多边形来提高效率，这种方法就叫背面拣选。图2.12展示了背面拣选之后的多边形，从照相机的观察点来看，仍将绘制相同的场景到后备表面，那些被遮住的部分无论如何都永远不会被看见的。

　　图2.12
   当然，为了完成这项工作，Direct3D需要知道哪个多边形是正面，哪个是背面。Direct3D中默认顶点以顺时针方向（在观察坐标系中）形成的三角形为正面，以逆时针方向形成的三角形为背面。
如果我们不想使用默认的拣选状态，我们可以通过改变D3DRS_CULLMODE来改变渲染状态：
Device->SetRenderState(D3DRS_CULLMODE, Value);
Value可以是如下一个值：
* D3DCULL_NONE——完全不使用背面拣选
* D3DCULL_CW——拣选顺时针方向环绕的三角形
* D3DCULL_CCW——拣选逆时针方向环绕的三角形，这是默认值。

### 2.3.5光源（Lighting）
　　光源定义在世界坐标系中然后被变换到视图坐标系中。视图坐标系中光源给物体施加的光照大大增加了场景中物体的真实性，至于光照的相关函数的细节将会在第五章学习。在本书的第四部分，我们将使用可编程管线实现自己的光照。
### 2.3.6裁剪（Clipping）

我们拣选那些超出了可视体范围的几何图形的过程就叫做裁剪。这会出现三种情况：

* 完全包含——三角形完全在可视体内，这会保持不变，并进入下一级
* 完全在外——三角形完全在可视体外部，这将被拣选
* 部分在内（部分在外）——三角形一部分在可视体内，一部分在可视体外，则三角形将被分成两部分，可视体内的部分被保留，可视体之外的则被拣选

图2.13展示了上面三种情况：
{% asset_img 2.13.png %}
　　图2.13

### 2.3.7投影（Projection）

视图坐标系的主要任务就是将3D场景转化为2D图像表示。这种从n维转换成n-1维的过程就叫做投影。投影的方法有很多种，但是我们只对一种特殊的投影感兴趣，那就是透视投影。因为透视投影可以使离照相机越远的物体投影到屏幕上后就越小，这可以使我们把3D场景更真实的转化为2D图像。图2.14展示了一个3D空间中的点是如何通过透视投影到投影窗口上去的。

　　图2.14
   投影变换的实质就是定义可视体，并将可视体内的几何图形投影到投影窗口上去。投影矩阵的计算太复杂了，这里我们不会给出推导过程，而是使用如下的Direct3D函数通过给出平截头体的参数来求出投影矩阵。

　　图2.15
```c
D3DXMATRIX *D3DXMatrixPerspectiveFovLH(
	D3DXMATRIX* pOut, // 返回的投影矩阵
	FLOAT fovY, // 用弧度表示的视野角度vertical field of view angle in radians
	FLOAT Aspect, // 宽高比
	FLOAT zn, // 前裁剪面距离
	FLOAT zf // 后裁剪面距离
);
```
　　（fovY定义镜头垂直观察范围，以弧度为单位。对于这个参数，下面是我的理解：如果定义为D3DX_PI/4（90度角），那么就是表示以摄像机的观察方向为平分线，上方45度角和下方45度角就是摄像机所能看到的垂直范围了。嗯，可以想象一下自己的眼睛，如果可以把自己眼睛的fovY值设为D3DX_PI/2（180度角），那么我们就可以不用抬头就看得见头顶的东西了。如果设为D3DX_PI的话。。。我先编译一下试试（building…）。哈哈，结果啥也看不见。很难想象如果自己能同时看到所有方向的物体，那么将是一个怎样的画面啊）
　　Aspect参数为投影平面的宽高比例值，由于最后都为转换到屏幕上，所以这个比例一般设为屏幕分辨率的宽和高的比值（见2.3.8节）。如果投影窗口是个正方形，而我们的显示屏一般都是长方形的，这样转换后就会引起拉伸变形。

　　我们还是通过调用IDirect3DDevice9::SetTranform方法来进行投影变换，当然，要把第一个投影类型的参数设为D3DTS_PROJECTION。下面的例子基于一个90度视角、前裁剪面距离为1、后裁剪面距离为1000的平截头体创建投影矩阵：
D3DXMATRIX proj;
D3DXMatrixPerspectiveFovLH(
	&proj, PI * 0.5f, (float)width / (float)height, 1.0, 1000.0f);
Device->SetTransform(D3DTS_PROJECTION, &proj);
2.3.8视口变换（Viewport Transform）
　　视口变换主要是转换投影窗口到显示屏幕上。通常一个游戏的视口就是整个显示屏，但是当我们以窗口模式运行的时候，也有可能只占屏幕的一部分或在客户区内。视口矩形是由它所在窗口的坐标系来描述的，如图2.16。
　　
　　图2.16
　　在Direct3D中，视口矩形通过D3DVIEWPORT9结构来表示。它的定义如下：
typedef struct _D3DVIEWPORT9 {
	DWORD X;
	DWORD Y;
	DWORD Width;
	DWORD Height;
	DWORD MinZ;
	DWORD MaxZ;
} D3DVIEWPORT9;
   前四个参数定义了视口矩形与其所在窗口的关系。MinZ成员指定最小深度缓冲值，MaxZ指定最大深度缓冲值。Direct3D使用的深度缓冲的范围是0~1，所以如果不想做什么特殊效果的话，将它们分别设成相应的值就可以了。
　　一旦我们填充完D3DVIEWPORT9结构后，就可以如下设视口：
D3DVIEWPORT9 vp{ 0, 0, 640, 480, 0, 1 };
Device->SetViewport(&vp);
这样，Direct3D就会自动为我们处理视口变换。现在还是给出视口变换矩阵作为参考：

### 光栅化（Rasterization）
在把三角形每个顶点转换到屏幕上以后，我们就画了一个2D三角形。光栅化是计算需要显示的每个三角形中每个点颜色值（如图2.17）。
{% asset_img 2.17.jpg %}
图2.17
光栅化过程是非常繁重的计算，它应该通过硬件图形处理来完成。它的处理结果就是把2D图象显示在显示器上。
