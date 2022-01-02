---
title: 第十四章 粒子系统(Particle Systems)
mathjax: true
date: 2019-04-11 15:24:20
tags:
  - Direct3D
categories: Direct3D
---

   许多自然现象是由很多小的小颗粒组成的，它们有相似的行为。（例如，雪花落下，闪烁的火焰，冲出枪管的“子弹”），粒子系统用来模拟这种现象。
目标：
* 学习我们给定的粒子属性，如何描述D3D中的粒子。
* 设计一个灵活的粒子基系统的基类，包括一般的粒子系统都有的属性和方法。
* 模拟3个具体的粒子系统，雪、爆炸、粒子枪。
14.1 粒子和点精灵（Point Sprite）
   粒子是一个很小的对象，它通常用来模拟数学中的一个点。点元是用来显示粒子的很好的方案。可是点元被光栅化成一个简单的像素。这没给我们多少灵活性，因为我们想有各种大小不同的粒子，并且把整个纹理平滑映射到这些粒子上。在Direct3D 8.0,以前，因为点元方法的局限性而完全不使用他们。代替的方法是，程序员将使用公告板去显示粒子，一个板是一个方格，世界矩阵用它来确定方向，使它总是朝向照相机。
   Direct3D 8.0引入一个特殊的点元叫点精灵，多数时候被应用在粒子系统中。与一般的点元不同的是，点精灵有纹理映射并能改变大小。与公告板不同的是，能用一个简单的点描述一个点精灵，节省内存和处理时间，因为我们只是必须保存和处理一个点，而公告板则是四个。

14.1.1 结构的格式
   我们使用下面的顶点结构来描述粒子的位置和颜色：
struct Particle
{
     D3DXVECTOR3 _position;
     D3DCOLOR    _color;
     static const DWORD FVF;
};
const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
   这个结构只保存粒子的位置和颜色，这取决于你程序的需要，你能够用同样的结构去保存一套纹理坐标，我们在下一节讨论给点精灵赋予纹理。
   增加一个浮点变量给Particle结构去指定粒子的大小是可能的。我们必须增加一个D3DFVF_PSIZE标记给我们的灵活的顶点格式，以反映这个变化。每个粒子维护自己的大小很有用，因为它允许我们以具体情况指定并改变粒子的大小。可是，大多数的图形卡不支持控制粒子的大小，因此我们不使用它。（检查D3DFVFCAPS_PSIZE在D3 DCAPS9结构的FVFCaps成员）代替的方法是：用渲染状态（render states）去控制粒子的大小，就像你很快看到的，有尺寸成员的顶点结构的例子：
strict Particle
{
     D3DXVECTOR3 _position;
     D3DCOLOR    _color;
     float       _size;
     static const DWORD FVF;
};
const DWORD Particle::FVF = D3DFVF XYZ | D3DFVF DIFFUSE |
  D3DFVF_PSIZE;
   注意：通过vertex shader，能够获取每个粒子的大小，即使你的硬件不支持D3DFVFCAPS_PSIZE。Vertex shaders的内容在本书的第IV部分。
   
14.1.2点精灵（Point Sprite）渲染状态
   点精灵的行为大部分由渲染状态（render states）来控制，现在让我们来看一下这些渲染状态：
D3DRS_POINTSPRITEENABLE—A Boolean value. The default value is false.
True表示将当前的纹理全部映射到点精灵上。
False 表示用指定的纹理坐标映射到点精灵的点（图素）上。
_device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

D3DRS_POINTSCALEENABLE—A Boolean value. The default value is false.
True表示用视图空间单位来解释点的大小。视图空间单位的3D空间点在照相机中，点精灵将会自动缩放，这取决到它有多远, 像其他对象一样，离照相机近的粒子比离照相机远的粒子要小。
False 表示点的大小将用屏幕空间单位来解释。屏幕空间单位是屏幕上的像素单位。. 因此如果你指定false, 例如, 设置点精灵的尺寸为3, 则点精灵在屏幕区域中的尺寸为3×3像素。.
_device->SetRenderState(D3DRS_POINTSCALEENABLE, true);

D3DRS_POINTSIZE—表示点精灵的尺寸. 这个值可以任意指定视图空间或屏幕空间的点精灵的尺寸, 取决于D3DRS_POINTSCALEENABLE 状态如何设置. 下面的代码段设置点的尺寸为2.5个单位。:
_device->SetRenderState( D3DRS_POINTSIZE, d3d::FtoDw(2.5f) );

d3d::FtoDw 是我们新加进 d3dUtility.h/cpp 文件中的一个函数，它将float型转换为 DWORD型。 我们必须这么做是因为所有的IDirect3DDevice9::SetRenderState 都要一个 DWORD 型的值而不是float型。
DWORD d3d::FtoDw(float f)
{
     return *((DWORD*)&f);
}



D3DRS_POINTSIZE_MIN—表示点精灵的最小尺寸。例子，将设置最小值为0.2：
_device->SetRenderState(D3DRS_POINTSIZE_MIN, d3d::FtoDw(0.2f));

D3DRS_POINTSIZE_MAX—表示点精灵的最大尺寸。例子，将设置最大值为5.0:
_device->SetRenderState(D3DRS_POINTSIZE_MAX, d3d::FtoDw(5.0f));

D3DRS_POINTSCALE_A, D3DRS_POINTSCALE_B, D3DRS_POINTSCALE_C—这3个常量表示如何根据距离控制点精灵的尺寸—这个距离是点精灵到照相机的距离。

D3D用以下的公式去计算点精灵的最终尺寸，这取决于距离和这3个常量。

其中：
FinalSize：距离计算后，点精灵的最后尺寸。
ViewportHeight：视口的高度。
Size：分别为D3DRS_POINTSCALE_A, D3DRS_POINTSCALE_B, and D3DRS_POINTSCALE_C值。
D：在视图空间中点精灵与照相机的距离。因为照相机被放置在视图空间中的原点，这个值是：，也是点精灵所在的位置。

   下面代码设置点精灵的距离常量，因此远处的点精灵将变小。
_device->SetRenderState(D3DRS_POINTSCALE_A, d3d::FtoDw(0.0f));
_device->SetRenderState(D3DRS_POINTSCALE_B, d3d::FtoDw(0.0f));
_device->SetRenderState(D3DRS_POINTSCALE_C, d3d::FtoDw(1.0f));

14.1.3 粒子和他们的属性
    一个粒子系统是由除了位置、颜色以外的更多的属性组成，例如，一个粒子有某些速度。然而，这些额外的属性对于渲染粒子来说不是必须的。因此，我们在单独的结构中保存渲染粒子所必须的数据和属性。当我们创建、显示或更新粒子时，我们使用属性来工作。当我们准备渲染时，我们从Particle（粒子）结构中COPY位置和颜色。
   对于我们模拟的具体粒子系统，粒子的属性也是不同的。因此我们能够归纳一些通用的属性，一面的结构例子中包含一些通用的属性，大多数系统用不上这么多，一些系统需要的属性这里可能还没有。
struct Attribute
{
     D3DXVECTOR3 _position;
     D3DXVECTOR3 _velocity;
     D3DXVECTOR3 _acceleration;
     float       _lifeTime;
     float       _age;
     D3DXCOLOR   _color;
     D3DXCOLOR   _colorFade;
     bool        _isAlive;
};
_position—粒子在世界空间中的位置
_velocity—粒子的速度，每秒多少个单位。
_acceleration—粒子的加速度, 每秒多少个单位。
_lifeTime—粒子的生命周期. 例如,当一个时间段后，我们可以杀死一个激光柱的粒子.
_age—粒子的当前年龄。
_color—粒子的颜色。
_colorFade—粒子随时间的变化而褪去的颜色。
_isAlive—True 表示粒子活着;false 表示粒子死了。 
   
14.2 粒子系统的组成
   粒子系统是粒子的集合，用来保存和显示这些粒子。粒子系统维护所有粒子的全部属性，影响系统中的所有粒子：粒子的尺寸，起始的位置及应用在粒子上的纹理等。粒子系统的方法负责更新、显示、杀死和创建粒子。
   虽然不同的具体（与抽象是相对的）粒子系统有不同的行为，我们归纳并找到一些所有的粒子系统共有的基本属性，我们把这些公共的属性放到一个抽象的Psystem基类，它是我们所有的具体粒子系统的父类，现在让我们看一下Psystem类：
class PSystem
{
public:
     PSystem();
     virtual ~PSystem();

     virtual bool init(IDirect3DDevice9* device, char* texFileName);
     virtual void reset();
     virtual void resetParticle(Attribute* attribute) = 0;
     virtual void addParticle();
     virtual void update(float timeDelta) = 0;

     virtual void preRender();
     virtual void render();
     virtual void postRender();

     bool isEmpty();
     bool isDead();
protected:
     virtual void removeDeadParticles();

protected:
     IDirect3DDevice9*       _device;
     D3DXVECTOR3             _origin;
     d3d::BoundingBox        _boundingBox;
     float                   _emitRate;
     float                   _size;
     IDirect3DTexture9*      _tex;
     IDirect3DVertexBuffer9* _vb;
     std::list<Attribute>    _particles;
     int                     _maxParticles;

     DWORD _vbSize;
     DWORD _vbOffset;
     DWORD _vbBatchSize;
};

   一些数据成员：
* _origin—粒子系统的原点， 这是粒子系统产生时的位置。
* _boundingBox—创建粒子系统使用的边界盒，用于限制粒子的活动范围。例如，假如我们让雪系统只落在一个围绕高山的峰顶的体积内； 我们会定义一个包括这个体积的边界盒, 出界的粒子将会被杀死。
* _emitRate—新增加到系统中的粒子的速度。 通常的标准是每秒。
* _size—系统中所有粒子的尺寸。
* _particles—系统中粒子属性的一个列表。 我们用这个列表创建，释放及更新粒子。 当我们准备画粒子时, 我们COPY列表节点的一部分到顶点缓存并画粒子。 当我们COPY另外一批时绘制这批粒子，然后重复这一过程直到绘制完所有粒子。 这有点太简单了，我们将在section 14.2.1节详细的解释绘制的过程。
* _maxParticles—在给定的时间内，系统中允许的粒子最大数。例如, 如果创建粒子的速度比释放快的话, 随着时间的增长粒子的数量将会是巨大的，这个成员将避免出现这样的问题。
* _vbSize—在给定的时间内顶点缓存中能够保存的粒子的数量，这个值与实际的粒子系统中的粒子数量无关。
注意：member _vbOffset和_vbBatchSize数据成员在渲染粒子系统时使用，我们在稍后的section 14.2.1节讨论。

方法：
* PSystem/ ~PSystem—用来初始化默认值的构造器/用来释放设备接口的析构器 (vertex buffer, texture)。
* init—这个方法做与设备无关的初始化工作, 比如创建用来保存点精灵的顶点缓存或创建纹理。 顶点缓存的创建包括一些标记，现在我们都已经讨论过了，但还没有用:
hr = device->CreateVertexBuffer( 
     _vbSize * sizeof(Particle),
     D3DUSAGE DYNAMIC | D3DUSAGE POINTS | D3DUSAGE WRITEONLY,
     Particle::FVF,
     D3DPOOL_DEFAULT,
     &_vb,
     0) ;
* 
o 注意： 我们使用动态的顶点缓存（D3DUSAGE DYNAMIC）。 因为我们需要在每帧中更新我们的粒子,意思是我们将会去存取顶点缓存的内存，回想一下，访问一个静态的顶点缓存慢得不可接受， 所以我们使用动态的顶点缓存。
o 查看我们用过的 D3DUSAGE_POINTS 标记,它说明顶点缓存将保存点精灵。
o 顶点缓存的尺寸是由_vbSize预先确定的，而且与系统中粒子的数量无关。 也就是说, _vbSize 将小于等于系统中粒子的数量。 这是因为渲染粒子系统是一批一批的，不是一次渲染全部。 我们将在section 14.2.1节中解释渲染过程。
o 我们使用默认的内存池(pool)代替通常使用的托管内存池，因为动态顶点缓存不能用在托管内存池中。
* reset—这个方法重新设置系统中每个粒子的属性:
void PSystem::reset()
{
     std::list<Attribute>::iterator i;
     for(i = _particles.begin(); i != _particles.end(); i++)
     {
          resetParticle( &(*i) );
     }
}
* resetParticle—这个方法重新设置粒子的属性。如何重设粒子的属性，这依赖于具体粒子系统的特性。因此我们定义这个方法为虚拟的，等待子类去实现。
* addParticle—这个方法用来在系统中增加一个粒子。在增加它到粒子列表之前，使用resetParticle 方法先初始化粒子:
void PSystem::addParticle() 
{
     Attribute attribute;
     resetParticle(&attribute);
     _particles.push_back(attribute);
}
void PSystem::addParticle()
* update—这个方法更新系统中所有的粒子。因为这个的方法的执行取决于具体粒子系统的特性, 因此我们定义这个方法为抽象的，等待子类去实现。
* render—这个方法用来显示系统中所有的粒子。 执行起来很复杂，我们将在14.2.1 节讨论。
* preRender—用它来初始化渲染状态， 在渲染前设置。 因为系统与系统之间是不同的,所以我们定义它为虚拟的。 默认将执行下列代码:
void PSystem::preRender()
{
  _device->SetRenderState(D3DRS_LIGHTING, false);
  _device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
  _device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
  _device->SetRenderState(D3DRS_POINTSIZE, d3d::FtoDw( size));
  _device->SetRenderState(D3DRS_POINTSIZE MIN, d3d::FtoDw(0.0f));

  // control the size of the particle relative to distance
  _device->SetRenderState(D3DRS_POINTSCALE A, d3d::FtoDw(0.0f));
  _device->SetRenderState(D3DRS_POINTSCALE B, d3d::FtoDw(0.0f));
  _device->SetRenderState(D3DRS_POINTSCALE C, d3d::FtoDw(1.0f));

  // use alpha from texture
  _device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_
     TEXTURE);
  _device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_
     SELECTARG1);

  _device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
  _device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
* 注意：我们使用alpha混合渲染，以便设置纹理的alpha通道，来设置纹理像素的透明度。 用它产生多种效果；一种特殊的情况是：获得象纹理那样的非矩形的粒子。例如， 获得一个圆形“雪球形”的粒子, 我们使用一个简单的带有alpha通道的纹理， 它看上去是背景为黑色的带有白色圆形的样子。因此，显示出来时只是一个白圆，这比白色的矩形纹理要好。
* postRender—用它去保存所有渲染状态，它是一个特殊的粒子系统可能有的设置。因为系统与系统间是不同的,所以我们定义它为虚拟的。默认将执行下列代码:
```c++
void PSystem::postRender()
{
    _device->SetRenderState(D3DRS_LIGHTING,          true);
    _device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
    _device->SetRenderState(D3DRS_POINTSCALEENABLE,  false);
    _device->SetRenderState(D3DRS_ALPHABLENDENABLE,  false);
}
```
* isEmpty—如果为True 则在当前的系统中没有粒子， 否则为false.
* isDead—如果为True 则系统中的所有粒子都是死的，否则为false。 注意： 系统中所有粒子状态为idDead时并不意味着isEmpty. 空意思着系统中没有粒子。 Dead的意思是系统中有粒子，但都是死的。.
* removeDeadParticles—搜索属_particle性表，从表中杀死并删除粒子。
14.2.1 绘制粒子系统
   因为粒子系统是动态的，在每一个帧中我们需要更新系统中的粒子，对于渲染粒子系统的一种直观但效率低下的方法如下:
* 创建一个足够大的顶点缓存保存最大数量的粒子。
         每一帧里执行：
A. 更新所有粒子。
B. COPY所有活着的粒子到顶点缓存。
C. 绘制顶点缓存。
   这个方法正确，不过不是最有效率的。第一，顶点缓冲必须足够大以保存系统中所有粒子。但是非常重要的是，当我们从列表拷贝所有粒子到顶点缓冲（步骤B）时，显卡却什么也不做。举个例子，假设我们系统有10,000个粒子，首先我们需要一个能容纳10,000个粒子的顶点缓冲，这是一个很大的内存。另外显卡将停着什么也不做直到列表中的10,000个粒子拷到顶点缓冲，并且我们调用DrawPrimitive。这个特定情况是CPU与显卡不同时工作的一个很好的例子。
   
    更好的办法（SDK中点精灵例程中用到的方法）就象这样：
   提示：这是一个简单的描述，但它说明了这一思想。它假定我们总是有500个粒子以填充一个缓存片段，但是这是不可能发生的，因为我们经常杀死并创建粒子，所以从一帧到另一帧粒子数量是变化的。举个例子，假设我们只剩下200个粒子要在当前帧拷贝并渲染。因为200个粒子不能填充整个缓存片段，我们用代码处理这个特定情形。这个特定情形只有在最后的缓存片段中才会出现，因为如果不是最后的片断，就意味着必然有500个粒子将被移到下一缓存片段。
   
   创建一个合适尺寸的顶点缓存（能够保存2000个粒子），然后我们划分顶点缓存为几个小的块，就像这个例子，我们设置每个缓存片断的尺寸为500个粒子。
   

* 然后创建一个全局变量 i = 0 ，用来记录片段。
         每一帧里执行:
A. 更新所有粒子。
B. 直到所有粒子渲染完毕。:
1. 如果顶点缓存没有满：
a 用D3DLOCK_NOOVERWRITE标记锁定缓存片段i
b COPY 500个粒子到片段i
2. 如果顶点缓存满了：
a 从起始的地方开始顶点缓冲: i=0
b 用D3DLOCK_NOOVERWRITE标记锁定缓存段i
c COPY 500个粒子到片段i 

3. 渲染片段i. 
4. 下一片段： i+ +
备注：顶点缓存是动态的， 因此我们能利用动态锁定标记D3DLOCK_NOOVERWRITE 和 D3DLOCK_DISCARD。这两个标记允许我们锁定顶点缓存的某一部分。当顶点缓存中的其他部分被渲染时，它是不能渲染的。例如，假如我们正在使用D3DLOCK_NOOVERWRITE标记渲染片段0时， 当渲染片段0的时候我们能锁定并填充片段1。这样可以防止渲染的延迟。

   这个方法更有效率。首先，我们减少顶点缓存的尺寸；然后， CPU与显卡在协调的工作。也就是说，当我们绘制一小批粒子时(graphics card work)，同时拷贝另一小批粒子到顶点缓存 (CPU work)。这个动作是连续执行的，直到所有的粒子都被渲染完毕，就像你了解的一样， 显卡在全部顶点缓存被填充的时候是不用处于空闲状态的。
   我们现在将注意力转向这一个渲染方案的实现，为了方便使用这个粒子系统的渲染方案, 我们使用 PSystem 类中的下列数据成员:
* _vbSize—在给定时间内我们的顶点缓存能够保存的粒子数量。这个值与实际的粒子系统中的粒子数无关。
* _vbOffset—这个变量是在顶点缓存中的偏移，在顶点缓存里我们将用它开始COPY下一批粒子，例如，如果第一批在缓存中是0到499，偏移到第二批COPY的开始处将是500。
* _vbBatchSize—定义一批缓存中的粒子数量。
* 
我们现在介绍渲染方法的代码：
void PSystem::render()
{
  if( !_particles.empty() )
  {
     // set render states
     preRender();
     _device->SetTexture(0, _tex);
     _device->SetFVF(Particle::FVF);
     _device->SetStreamSource(0, _vb, 0, sizeof(Particle));



// start at beginning if we're at the end of the vb
     if(_vbOffset >= _vbSize)
          _vbOffset = 0;

     Particle*v =0;

     _vb->Lock(
          _vbOffset    * sizeof( Particle ),
          _vbBatchSize * sizeof( Particle ),
          (void**)&v,
          _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

     DWORD numParticlesInBatch = 0;

     //
     // Until all particles have been rendered.
     //
     std::list<Attribute>::iterator i;
     for(i = _particles.begin(); i != _particles.end(); i++)
     {
          if( i->_isAlive )
          {
               //
               // Copy a batch of the living particles to the
               // next vertex buffer segment
               //
               v->_position = i->_position;
               v->_color = (D3DCOLOR)i->_color;
               v++; // next element;

               numParticlesInBatch++; //increase batch counter

               // is this batch full?
               if(numParticlesInBatch == _vbBatchSize)
               {
                    //
                    // Draw the last batch of particles that was
                    // copied to the vertex buffer.
                    //
                    _vb->Unlock();
                    _device->DrawPrimitive(
                         D3DPT_POINTLIST,
                         _vbOffset,
                         _vbBatchSize);

               //
               // While that batch is drawing, start filling the
               // next batch with particles.
               //

               // move the offset to the start of the next batch
               _vbOffset += _vbBatchSize;

               // don't offset into memory thats outside the vb's
               // range. If we're at the end, start at the beginning.
               if(_vbOffset >= _vbSize)
                    _vbOffset = 0;

               _vb->Lock(
                    _vbOffset    * sizeof( Particle ),
                    _vbBatchSize * sizeof( Particle ),
                    (void**)&v,
                    _vbOffset ? D3DLOCK_NOOVERWRITE :
                       D3DLOCK_DISCARD);

               numParticlesInBatch = 0; // reset for new batch
          }//end if
       }//end if
     }//end for

     _vb->Unlock();

     // it's possible that the LAST batch being filled never
     // got rendered because the condition
     // (numParticlesInBatch == _vbBatchSize) would not have
     // been satisfied.  We draw the last partially filled batch now.

     if( numParticlesInBatch )
     {
           _device->DrawPrimitive(
                D3DPT_POINTLIST,
                _vbOffset,
                numParticlesInBatch);
     }

     // next block
     _vbOffset += _vbBatchSize;

     postRender();
     }//end if
}// end render()

14.2.2 随机
   这有一个随机的粒子系统。例如，如果我们模拟雪花，不能让所有雪花以完全相同的方式落下。我们要让它们按相似的方式落下而不是完全相同的方式。为了使粒子系统的随机功能更简单，我们增加了下列两个函数到d3dUtility.h/cpp文件。
   第一个函数在[lowBound, highBound]区间内随机的返回一个Float类型值：
float d3d::GetRandomFloat(float lowBound, float highBound)
{
     if( lowBound >= highBound ) // bad input
          return lowBound;

     // get random float in [0, 1] interval
     float f = (rand() % 10000) * 0.0001f;

     // return float in [lowBound, highBound] interval.
     return (f * (highBound - lowBound)) + lowBound;
}
   第二个函数在边界盒的范围内，输出一个随机的向量。
void d3d::GetRandomVector(
       D3DXVECTOR3* out,
       D3DXVECTOR3* min,
       D3DXVECTOR3* max)
{
       out->x = GetRandomFloat(min->x, max->x);
       out->y = GetRandomFloat(min->y, max->y);
       out->z = GetRandomFloat(min->z, max->z);
}
   
   注意：记得用srand()去seed随机数生成器。
14.3 具体的粒子系统：雪、火、粒子枪
现在让我们用Psystem类开始一个具体的粒子系统，为了说明用意，这些系统的设计很简单，没有用到Psystem类所提供的所有灵活性。我们实现雪、火、粒子枪系统。这些系统的名字基本上概括了他们的模型。雪系统模拟下落的雪花，火系统模拟看上去像火焰的爆炸，粒子枪系统从照相机位置向对面发射出粒子（用键盘）。
注意：照例，用全部的工程代码来说明这些系统，你能够在本章找到这些文件。

14.3.1 例子程序：雪

图14.2 雪系统例子的屏幕截图

雪系统类定义如下：
class Snow : public PSystem
{
public:
     Snow(d3d::BoundingBox* boundingBox, int numParticles);
     void resetParticle(Attribute* attribute);
     void update(float timeDelta);
};
备注：因为父类做了大部分的工作，所以雪系统的接口非常简单。事实上，我们在这一节中实现的这三个粒子系统，接口简单并相对容易实现。
   构造器提供一个点给边界盒结构，边界盒是粒子系统的成员。边界盒描述雪花在哪个范围内（体积范围）下落，如果雪花出了边界盒，它将被杀死并再生。这样，雪系统始终能保存有同样数量的激粒子，构造器的实现：
Snow::Snow(d3d::BoundingBox* boundingBox, int numParticles)
{
     _boundingBox   = *boundingBox;
     _size          = 0.8f;
     _vbSize        = 2048;
     _vbOffset      = 0;
     _vbBatchSize   = 512;
     for(int i = 0; i < numParticles; i++)
          addParticle();
}
   同样注意：我们指定顶点缓存的尺寸，每一批的尺寸和开始的偏移。
   ResetParticle方法创建一个雪花，在x、z轴随机的位置并在边界盒的范围内。设置y轴高度为边界盒的顶部。如果给雪花一个速度，以便让雪花下落时稍稍向左倾斜。雪花是白色的。
void Snow::resetParticle(Attribute* attribute)
{
     attribute->_isAlive = true;

     // get random x, z coordinate for the position of the snowflake.
     d3d::GetRandomVector(
          &attribute->_position,
          &_boundingBox._min,
          &_boundingBox._max);

     // no randomness for height (y-coordinate). Snowflake
     // always starts at the top of bounding box.
     attribute->_position.y = _boundingBox._max.y;

     // snowflakes fall downward and slightly to the left
     attribute->_velocity.x = d3d::GetRandomFloat(0.0f, 1.0f)*-3.0f;
     attribute->_velocity.y = d3d::GetRandomFloat(0.0f, 1.0f)*-10.0f;
     attribute->_velocity.z = 0.0f;

     // white snowflake
     attribute->_color = d3d::WHITE;
}
   Update方法更新粒子和粒子间的位置，并且测试粒子是否在系统的边界盒之外，如果它已经跳出边界盒，就再重新创建。
void Snow::update(float timeDelta)
{
     std::list<Attribute>::iterator i;
     for(i = _particles.begin(); i != _particles.end(); i++)
     {
          i->_position += i->_velocity * timeDelta;

          // is the point outside bounds?
          if( _boundingBox.isPointInside( i->_position ) == false )
          {
               // nope so kill it, but we want to recycle dead
               // particles, so respawn it instead.
               resetParticle( &(*i) );
          }
     }
}
   
   
14.3.2 例子程序：火

图14.3 火粒子系统例子的屏幕截图
火系统类定义如下：
class Firework : public PSystem
{
public:
     Firework(D3DXVECTOR3* origin, int numParticles);
     void resetParticle(Attribute* attribute);
     void update(float timeDelta); void preRender();
     void postRender();
};
   构造器需要提供一个点作为粒子系统中的原点，和系统中的粒子数，原点是火焰爆发的那个点。
ResetParticle方法在原点位置初始化粒子系统，并在边界球内创建一个随机的速度，粒子系统中的每个例子有一个随机的颜色，我们定义粒子只能存活2秒。
void Firework::resetParticle(Attribute* attribute)
{
     attribute->_isAlive = true;
     attribute->_position = _origin;

     D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
     D3DXVECTOR3 max = D3DXVECTOR3( 1.0f,  1.0f,  1.0f);

     d3d::GetRandomVector(
          &attribute->_velocity,
          &min,
          &max);
     // normalize to make spherical
     D3DXVec3Normalize(
          &attribute->_velocity,
          &attribute->_velocity);

     attribute->_velocity *= 100.0f;

     attribute->_color = D3DXCOLOR(
          d3d::GetRandomFloat(0.0f, 1.0f),
          d3d::GetRandomFloat(0.0f, 1.0f),
          d3d::GetRandomFloat(0.0f, 1.0f),
          1.0f);

     attribute->_age      = 0.0f;
     attribute->_lifeTime = 2.0f; // lives for 2 seconds
}
   
   Update方法更新每个粒子的位置，并在粒子超出自己的生活周期时杀死它。注意：这个系统不能移除死掉的粒子，这么做是因为我们想产生一个新的火焰的时候，我们只要简单的重新设置已经存在的死了的火焰系统就可以了。这样为我们不必频繁的去产生和释放粒子。
void Firework::update(float timeDelta)
{
     std::list<Attribute>::iterator i;

     for(i = _particles.begin(); i != _particles.end(); i++)
     {
          // only update living particles
          if( i->_isAlive )
          {
               i->_position += i->_velocity * timeDelta;

               i->_age += timeDelta;

               if(i->_age > i->_lifeTime) // kill
                    i->_isAlive = false;
          }
     }
}
当渲染时，火系统使用不同的方法渲染像素。进一步讲，它不写深度缓存，我们可以简单的改变混合像素，通过重写PSystem::preRender方法和PSystem::postRender方法，下面是重写的实现：
void Firework::update(float timeDelta)
{
     std::list<Attribute>::iterator i;
void Firework::preRender()
{
     PSystem::preRender();

     _device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
     _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
     // read, but don't write particles to z-buffer
     _device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void Firework::postRender()
{
     PSystem::postRender();

     _device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
注意：这两个方法调用父类版本，这样，我们仍能重新使用父类的一些功能，做一些小的改变就变成了火焰系统。
14.3.3 例子程序：粒子枪

图14.4 激光枪系统的截图
下面是粒子枪系统的定义：
class ParticleGun : public PSystem
{
public:
     ParticleGun(Camera* camera);
     void resetParticle(Attribute* attribute);
     void update(float timeDelta);

private:
     Camera* _camera;
};
   构造器需要提供一个照相机的位置点，这是因为系统需要知道照相机的位置及朝向，以决定在哪创建一个粒子。
   ResetParticle方法设置粒子的位置为当前照相机的位置，并且设置方向上的速度，在照像机视角的100个单位。这样，子弹将射向我们正在看的方向，粒子颜色为绿色。
void ParticleGun::resetParticle(Attribute* attribute)
{
     attribute->_isAlive  = true;

     D3DXVECTOR3 cameraPos;
     _camera->getPosition(&cameraPos);

     D3DXVECTOR3 cameraDir;
     _camera->getLook(&cameraDir);

     // change to camera position
     attribute->_position = cameraPos;
     attribute->_position.y -= 1.0f; // slightly below camera so it's
                                     // like we're carrying gun

     // travels in the direction the camera is looking
     attribute->_velocity = cameraDir * 100.0f;

     // green
     attribute->_color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

     attribute->_age      = 0.0f;
     attribute->_lifeTime = 1.0f; // lives for 1 seconds
}

   
   Update方法更新粒子的位置，并且杀死超过其生命周期的粒子，然后，我们搜索粒子列表删除已经死了的粒子。
{
     std::list<Attribute>::iterator i;

     for(i = _particles.begin(); i != _particles.end(); i++)
     {
          i->_position += i->_velocity * timeDelta;

          i->_age += timeDelta;

          if(i->_age > i->_lifeTime) // kill
               i->_isAlive = false;
     }
     removeDeadParticles();
}
   
14.4 摘要
* 用点精灵来显示一个粒子是方便且灵活的，它可能改变粒子尺寸、给粒子赋予纹理。此外，能够使用简单的顶点（vertex）来描述它们。
* 粒子系统维护一个粒子的集合，并负责创建、释放、更新和显示粒子。
* 还有一些其他粒子系统的概念，是你能够实现的：烟，火箭的轨迹，喷泉/河水车效果，火，光，爆炸，和雨。


