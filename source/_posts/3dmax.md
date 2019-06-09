---
title: 3DMAX快捷键操作
mathjax: true
date: 2019-05-11 10:10:32
tags:
    - 3DMax
categories: 3DMax
---
# 工具栏
主工具栏（Main Toolbars）:按ALT+6可以进行隐藏/显示，其他工具栏可以选择菜单
Customize\Show UI(自定义\显示用户界面)进行显示在屏幕上。
# 工作视图
1. 视图切换可直接按快捷键的有： T--Top（顶视图）、B--Bottom（底视图）、F--Front（前视图）
L--Left（左视图）、U--User（用户视图）、P--Perspective（透视图）
C--Camara（摄像机视图，只有建立摄像机后才有用）
其他不能按快捷键的操作如下：
a.激活视图，按V键，从快捷菜单中进行选择
b.在视图名称处按右键选择Views(查看)命令后面的相应视图
2. 视图中的物体显示方式
Wireframe（网格）：以物体颜色显示网格，材质不显示
Smooth+HighLights（实体）
以上两种可以按F3进行切换
Other后面的有
Smooth（光滑）：只进行光滑显示，不会显示高光
Facets+Highlights：显示面状+高光，不进行光滑显示
Facets:只按面状显示
Lit Wireframe：以材质标准方式显示网格，与物体颜色设置无关
Bounding Box：边界盒子，按物体最大体积边界方盒显示，这种方式视图刷新最快
Facets Edges:按网格和实体同时显示，可按F4进行切换
Alt+X--将物体按透明方式显示
3. 改变视图大小
将鼠标置于视图窗口交界出拖动即可任意改变视图大小，还原时在窗口边界处按右键，选择Rest Layout
（重新恢复布局）命令即可。
单独放大某个视图：按Alt+W键，再次使用还原。
4. 隐藏/显示视图网格--按G键
5. 视图布局
选择菜单Customize\Viewport Configration(自定义\视图配置)命令，选择Layout（布局）选项
6. 视图布局风格
选择菜单Customize\Lode Custom UI Scheme(自定义\装载自定义用户界面方案)命令
# 鼠标操作:
直接按"中键"--平移视图
Alt+"中键"--按任意角度旋转视图
Ctrl+Alt+"中键"--缩放视图
Shift+Alt+"中键"--水平或垂直旋转视图
五、命令面板（Command Panal）
最顶上一排图标分别表示Create(创建)、Modify（修改）、Hiaracher（层级）、Motion（运动）、Display（显示）、Utilities（实用程序）
六、其他（轨迹栏、状态栏、提示栏、动画播放区等）略
先左键后右键，取消当前选择。
复制，整列复制。
Ctrl+V：复制，实例，参考。
复制：复制出来的物体与原物体没有任何关系
实例：相互影响，不管是修改实例。
参考：参考物体时不具有属性的，无法修改参考属性。
Z键：将选择物体最大化显示
修改器列表:
Bend
中心:修改弯曲轴
弯曲
角度,弯曲多少度
方向:
弯曲轴:以哪个方向为弯曲轴
限制:某一部分不进行弯曲.
上限:轴心0点以上
下线:轴心0点以下
显示最终结果打开
Show end result
锥化命令(Taper)
数量(Amount)如果为整数上面放大
曲线(Curve)中间凸起
锥化轴(Taper Axis)
主轴(Primary)
效果(Effect)
限制(Limits)
限制效果(Limit Effect)
# 扭曲()-制作冰激凌


晶格(Lattice)-制作纸篓:运用边生成三维模型
应用整个对象(Apply to Entire Object)
忽略隐藏边(Ignore Hidden Edges)
末端封口(End Caps)
基点面类型(Geodeisic Base Type)
四面体(Tetra)
八面体(Octa)
二十面体(Icosa)

# 选择操作
1、多选物体：
A:按Ctrl键增加物体选择，按Alt键减少物体选择
B：框选，有矩形、圆形、多边形、套索四种范围
2、编辑菜单中的几个选择命令：
Select Invert(反选)（ctrl+i）、Select by Color(按颜色选择)、Select by name(按名称选择)
3、工具菜单中的选择命令：
Isolate Selection:单独显示被选择的物体（alt+q ），自动隐藏其他物体（非选择）
关闭Isolate Selection警告对话框进行还原。
4、命名选择集合
实例：台球桌物体的操作
# 复制操作
1. 使用菜单Edit\Clone（编辑\克隆）命令。
2、使用变换工具进行复制（移动、旋转、缩放）
3、使用镜向工具复制
三、变换操作
1、移动：移动控制轴由两两垂直的三个单向轴组成，显示为红、绿、篮颜色的三个轴向分别代表X、Y、Z方向，
操作时既可单独锁定控制轴进行单轴方向上的移动，也可将


鼠标定在靠近两轴交点的黄色平面区域
内按平面的方式进行任意移动。
2、旋转：旋转物体时，其外围会出现三个球形控制轴，显示为红、绿、蓝圆弧线条的为单向旋转轴，
分别代表X、Y、Z三个轴向，当前操纵的轴向会显示为黄色；内圈的灰色圆弧可以进行空间上的旋转，
可将物体在三个轴向上同时进行旋转；外圈的灰白色圆弧可以在当前视图角度的平面上进行旋转。
3、缩放：5.0版对缩放控制轴做了很大改进，可以直接在控制轴上实现等比和非等比操作，
其控制轴呈三角形，显示为红、绿、蓝箭头轴向为单向缩放轴，分别代表X、Y、Z三个轴向。
操作时拖动内部中心的三角区域可以进行等比例缩放；拖动单个轴向，可以进行单方向上缩放；
拖动外侧的三角平面可以进行双方向上的同时缩放。
四、实例：
1、使用局布（Local）坐标系统移动斜波物体。
2、复制西餐椅子（可使用移动、旋转、镜向工具完成）
3、复制石凳（使用旋转工具）
要点：A：移动和对齐轴心点；B：捕捉角度

第三讲 
一、复习
二、群组操作
Group（成组）：将多个选择的物体组成一个组
Ungroup（解组）：将群组解散，一次只能解散一个组
Open（打开组）：打开的组仍然存在，可以对组中的单个成员进行编辑
Close（关闭组）：将打开的组关闭，保持原来的组名
Detach（分离组）：将选择的组从当前的组中分离出去

Attach（合并组）：对两个或两个以上的组组合成一个新的组。
Explode（炸开组）：对当前选择的组进行炸开操作，可将多个组一次解散。
三、阵列操作
1、阵列对话框参数解释
Incremental（增量）：表示每两个物体之间的变换量，使用Move表示为距离，使用Rotate则表示角度，
使用Scale表示缩放变比。
Tatols（总量）：表示所有物体之和的总量，使用Move表示为距离，使用Rotate则表示角度，
使用Scale表示缩放变比。
Uniform:勾选此项，将锁定Scale变比设置，只能等比缩放。
1D、2D、3D分别表示在一维、二维、三维空间上产生阵列，Count表示物体的复制数目。
Incremental Row Offsets:制作二维、三维阵列时相对于一维阵列X、Y、Z轴上的偏移量。
Reset all parameters可以恢复对话框中的默认参数设置
2、实例：
A：制作圆形环绕的石凳

B：制作旋转台阶（特别注意台阶物体的轴心点位置）
四、对齐操作
先选择被对齐的原物体，单击工具栏中的对齐按钮，在视图中点击目标对齐物体
， 依据对话框可以实现原物体与目标物体以下几种对齐方式：
Minimum（最小对齐）:在正交视图物体的最左端和最下端分别定义X、Y轴的最小值。
Center(中心)：以物体的重心位置进行对齐
Pivot Point（轴心点）：以物体的坐标轴位置进行对齐
Maximum（最大值）：在正交视图物体的最右端和最上端分别定义X、Y轴的最大值。
第四讲 
1. Box（方体）
Length、Width和Height分别定义方体的长度、宽度和高度，Length Segs、

Width Segs和Height Segs分别定义长度、宽度和高度的分段划分，增加它们的值则产生栅格方体，
配合修改器的使用可以使方体更易于变形，从而产生形态各异的几何物体
2.Sphere(球体）
Radius（半径）：定义球体的大小。
Segments（分段数）：该值越大，球越圆滑。
Smooth（光滑）：控制球体表面是否光滑。
Hemisphere（半球）：值域为0-1，值为0时，表示一个完整的球体，值为1时，整个球体收缩成一个点，
值介于0-1之间，球体变为球冠，仅当值为0.5时为半球。
Chop（剪切）：在半球或球冠时，将整个球体的水平分段数切掉一半
Squash（挤压）：在半球或球冠时，可将下半球被切掉的球体水平分段数挤压到上半球，仍保持完整的分段数。
Slice On（切片）：打开它可以在下面的设置中调节球体局部切片大小。
Slice From/Slice To（切片开始/切片结束）：分别设置切片两端切除的幅度
Base To Pivot：设置球体的轴心点在球体的底部还是在球心位置。

Generate Mapping Coordinates（产生贴图坐标）：自动产生贴图坐标。
3.Geosphere(几何球体）
Tetra（四面体）/Octa（八面体）/Icosa（二十面体）：这三个参数分别控制该球体的基本类型，它们与Segments
参数值的平方值相乘正好是几何球体的所有三角面的个数，直
接在选择的物体上按数字键7，视图中会显示物体所有面的数量
4. Cylinder(柱体)
Radius（半径）：控制圆柱体的粗细。
Height（高度）：可以控制圆柱体的高矮。
Height Segments（高度分段数）：分段越多，物体精度越高，表面则越光滑
Cap Segments（端面分段数）：定义圆柱体截面圆形的分段数，从顶端截面看就是一个个同心圆。
Sides（边数）：定义圆柱体侧面的分段划分，值越大，圆柱体越圆。
5. Cone(锥体)
Radius 1/Radius 2分别控制锥体的上下两个端面的半径大小，当其中的一个值为0时即为锥体，
当两个半径值不为0且不相等时为圆台物体，当两个半径不为0且相等时则变为柱体。
6. Tube(圆管)
Radius 1/Radius 2分别控制圆管的内径和外径的大小。
7. Torus(圆环)
Radius 1：表示自圆环的中心至截面正多边形的中心距离。
Radius 2：表示圆环截面正多边形的内径大小，用它可定义圆环的粗细。
Rotation（旋转）：设置每一片段截面沿圆环轴旋转的角度
Twist（扭曲）：设置每个截面扭曲的角度，产生扭曲的表面。
扭曲角度,扭曲偏移Bias
扭曲轴,限制,上下限制扭曲.
Segments：沿着圆环长度上的分段数，值越大，得到的圆形越光滑。
Sides：圆环截面的边数，值越大，圆环越圆滑。
Smooth：设置圆环光滑范围，有All（全部光滑）、Sides（边光滑）、None（所有不光滑）和Segments（沿着长度光滑）四种类型。
8. Teaport（茶壶）
Radius（半径）：用于控制茶壶的大小。
Segments（分段数）：增加此值，可使茶壶变得更加圆滑。
Teapot Parts：该选项中的四个参数分别控制是否要茶壶的Body（壶身）、Handle（壶柄）、Spout（壶嘴）和Lid（壶盖）
9. Pyramid（四棱锥）

产生类似金字塔状的四棱锥体，调节Width、 Depth和 Height参数可以改变四棱锥的宽度、深度和高度。
10. Plane(平面)
Length和Width确定平面的大小。
Scale（比例）:控制平面在渲染时可以按一定的比例倍数进行放大或缩小； 
Density（密度）:参数可以使平面增加更多面的划分，当加大此值时，可以从Total Faces
（总面）参数中获得相关信息。
FFD-制作苹果
通过控制点
FFD2*2*2
控制点(Control Points)
晶格(Lattice)
设置体积(Set Volume):这个晶格点是放在哪个位置上

第五讲 
1. Hedra（异面体）
Family（类型）：即Tetra（四面体）、Cube/Octa（立方体/八面体）、Dodec/Octa（十二面体/二十面体）、Star1（星形1）和Star2（星形2）。
Family Parameters（类型参数）：参数P和Q，取值范围从0.0到1.0，修改它们的值可以控制组成异面体的多边形的形状。
Axis Scaling（轴向比率）：P、Q、R分别调节各种多边形的轴向比率。如果异面体只有一种或两种类型的面，那么轴向比率参数也只有一项或两项有效
无效的比率不产生效果。
Reset（重设置）:单击此按钮恢复轴向的初始设置。
Vertices（顶点）：确定异面体内部顶点的分布情况，从而决定异面体的内部结构。其中Basic表示超过最小值的面不再进行细划分；Center表示在面的中心位置添加一顶点，
按中心点到面的各个顶点所形成的边进行细划分；Center & Sides表示在面的中心位置添加一枯点，按中心点到面的各个顶点和边中心所形成的边进行细划分，所产生的面比
Center方式多一倍。
2. Turus Knot（环形节）
Base Curve（基本曲线）：提供两种基本曲线类型：Knot（节）和Circle（圆形）。当选择Knot类型时，激活其下的P和Q参数；当选择Circle类型时，激活Warp Count和Warp 
Height参数。
Radius：定义环形节物体的半径大小，在Cross Section子面板中也有一个Radius，它是定义环形节物体的截面半径大小。
P和Q：表示在水平方向和垂直方向产生打节的数目，当这两个值相等时，均为一个不打节的圆环，当其中的一个值为小数时，是一个断裂的圆环。
Warp Count（扭曲数目）：表示在环形物体上突出的扭曲角的数目，最大值为100。
Warp Height（扭曲高度）：表示扭曲角突出的程度，最大值为4。
Cross Section（横截面）：定义环形节物体横截面的形态。
Sides（边）：环形节截面的段数，值越大越圆滑。
Eccentricity（偏心率）：设置环形节压扁的程度，该值越接近1，其截面就越接近圆形。
Twist（扭曲）：设置截面沿路径扭曲旋转的程度，一般取消Smooth（光滑）选项后观察比较明显。
Lumps（肿块）：设置此值，可以使环形节产生一定数量的肿块效果。
Lumps Height：设置肿块的高度，最大值为4。
Lumps Offset：设置肿块在路径上移动的偏移量。
Smooth（光滑）：设置环形节表面按哪种方式进行光滑，All、Sides和None分别表示对整个造型进行光滑、沿路径方向的面进行光滑和不进行光滑处理。
Mapping Coordinates（贴图坐标）：设置贴图坐标及贴图图像在当前物体U、V方向的偏移量和平辅次数。
3. ChamferBox（倒角方体）
有两个参数用于设置倒角值，Fillet设置倒角的大小，当值为0时，就变为Box了；Fillet Segments用于设置倒角的分段数。这两个值要配合使用，如果Fillet Segments值设置
过小，很难体现倒角的圆滑度。 
4. ChamferCyl（倒角圆柱体）
与标准几何体中的圆柱体相似，所不同的是在圆柱体的两个端面与侧面之间可以产生圆滑的倒角效果,Fillet和Fillet Segments，含义与ChamferBox中的参数含义相同
5. OilTank（油罐）、Capsule（胶囊体）、Spindle（纺锤体）
这三个扩展几何体工具属于同类型的，共同点是中间都是圆柱体，只是顶端形状不一样，控制参数也大同小异，大部分参数项与圆柱体相同。
Blend（融合）参数设置油罐圆柱部分与其顶盖部分的倒角程度，使其互相交融，产生圆滑效果。Overall与Center用来定义油罐的高度，使用Overall包括上下两个顶盖的高度
和油罐中间圆柱的高度，使用Center仅指中间圆柱部分的高度。
6. Gengon（多边形棱体）
使用Gengon（多边形棱体）命令可以创建多边形柱体，修改Fillet参数值还可以使多边形柱体的每个侧面交界处产生光滑的倒角。
7. L-Ext（L型物体）与C-Ext（C型物体）
使用L-Ext和C-Ext命令可以创建表现L型和C型实体模型，主要用来表现建筑中的墙体效果
8. RingWave（环形波）
Radius（半径）：设置环形波的外沿半径。
Radius Segs（半径分段数）：设置内沿半径与外沿半径之间的分段数。
Radius Width（环形宽度）：设置从外沿半径向内的环形宽度的平均值。
RingWave Timing（环形波定时）：主要用于设置环形波的运动形式，主要有三种：
No Growth（没有增长）：环形波在生成过程始终以静态方式显示出来，不能形成动画。
Grow and Stay（增长并保持）：环形波生长过程中逐渐放大，到达最大时就停止放大，直至运动到最后一帧，其放大及停留的时间可以通过Start Time和Grow Time参数控制。
Cyclic Growth（循环增长）：将使环形波在运动过程中以循环方式进行放大显示，循环的次数可以通过Start Time和Grow Time参数进行设置，例如要在100帧的动画中循环5次
，可将Start Time设置为0，Grow Time设置为20。
Outer Edge Breakup与Inner Edge Breakup：它们的参数项是一样的，修改这些参数可以设置环形波内部和外部波浪的变化形状和大小幅度。分别有一个On复选项，控制其下参
数设置是否有效，系统默认激活Inner Edge Breakup控制项，取消该复选项，环形波内部为一圆形，主要参数含义如下。
Major Cycles（主圆周期）：定义环形波内部或外部边缘产生波纹的数目。
Width Flux（宽度流束）：定义波纹伸展的幅度，以百分比的形式表示。
Crawl Time（运动时间）：该值为正值时，环形波按顺时针方向运动，为负值时，按逆时针方向运动。
Minor Cycles（次圆周期）：设置此值可以在主圆的外围再产生若干个次圆波纹。
9. Hose（软管）
Hose是一种可以连接两个物体之间的可变形物体，它会随着两端物体的运动而做出相应的反应，从外形看很像一根软管，有圆形、矩形和D形三种外观。软管分自由软管和绑定
软管两种，作为捆绑软管可以在其顶部和底部与任何物体进行绑定操作，被绑定物体的轴心点将是软管模型两端的定位点。

第六讲 复合物体(COMPOUND OBJECTSOBJECTS)) 
*注意: 1.要有二个或二个以上的物体才能用其命令
一.BOOLEAN(布尔计算)
1.PICK OPERAND B（拾取操作对象B）
1)REFERENCE(将对象B参考复制一个,来进行布尔计算)
2)COPY (将对象B复制一个,来进行布尔计算)
3)MOVE (将对象B直接来进行布尔计算)
4)INSTANCE (将对象B关联复制一个,来进行布尔计算) 
2.Operation（操作）
Union（相加）
Intersection（相交）
Subtraction（A-B）/Subtraction（B-A）:相减
Cut（剪切）
Refine（细化，插入一条对象B与对象A相交部分的轮廓线）
Split（割裂，把相交部分分为单独的一个次元素对象）
Remove Inside（出除里面，删除相交部分，使对象A里面挖空）
Remove Outside（出除外面，删除对象A与对象B不相交的部分，
相交部分只保留一个面，里面挖空）
3.DISPLAY
RESULT（显示结果）
OPERAND（显示操作对象）
RESULT+HIDDEN OPERAND（结果+隐藏的操作对象）
4.如何修改原物体参数
在OPERAND中选原始物体A或B(MODIFY-OPERANDS A或B)
5.几个物体同时挖
要求：几个物体用ATTACH(合并)命令使它们作为一个物体
第1种：EDIT MESH（编辑网格）-ATTACH LIST
第2种：按右键-CONVERT TO-CONVERT TO EDITABLE MESH
-ATTACH LIST）
例:烟灰缸，TWO-17，TWO-7，TWO-8，TWO-10
三.MORPH(变形)
四.SCATTER(离散)
五.CONFORM(包裹)
六.CONNECT(连接)
七.SHAPEMERGE(形体合并)
八.BOOLEAN(布尔运算)
九.TERRAIN(地形)
十.LOFT(放样)（针对于二维图形SHAPES）
十一.MESHER(网格化)

第七讲 创建二维图形（SHAPES） 
一.二维图形
1.LINE（线）
1)直线
2）曲线
（1）点的类型（CORNER角点
SMOOTH圆滑
BAZIER贝赛尔
BAZIER CORNER贝赛尔角点）
3)画一条笔直的线(SHIFT+LINE画)
4)线的连接(用2D捕捉命令)
2.RECTANGLE（矩形）
3.CIRCLE（圆）
二.二维渲染
RENDERING-RENDERABLE（可渲染的）THICHNESS（线变粗变细）
渲染圆滑如何提高
INTERPOLATION（插补值）
STEPS（插入多少个点）-点越多越圆滑
OPTIMIZE（优化圆滑）
ADAPTIVE（适配圆滑，自动生成圆滑，不需要你填插入多少个点）
三.二维变成面(extrude)例如:楼梯,圆形台阶(BEND)
四.ARC（弧）
1.PIE SLICE(将圆心与弧的两端点连接)
2.REVERSE(端点对调)
五.DONUT（双圆）(双形在一起为复合形)
1.双形在一起为复合形,单形为一形
2.START NEW SHAPE(开始新形)
六.STAR（星形）
七.NGON（多边形）
八.ELLIPSE（椭圆）
九.TEXT(文本)
(多行文字,按回车键)
十.HELIX（螺旋线）
(motion-trajectories使球沿着螺旋线运动)
十一.SECTION(剖面)
截取一个三维物体的剖面
挤出(Extrude)
Amount:挤出多少

第八讲 二维编辑命令（EDIT SPLINE(样条曲线) 
EDIT SPLINE(编辑样条曲线)
注：EDIT SPLINE是斜体--针对了多个物体
EDIT SPLINE是正体--针对单个物体
EDIT SPLINE包括
VERTEX(点)
SEGMENT(线段)
SPLINE(曲线)
三个次对象
(一)点,线段,曲线所共有的命令
1.CREATE LINE（创建线）
2.ATTACH(合并)没有对话框合并
使二个或多个独立的二维图形变成复合形）
3.ATTACH MULT(合并)有对话框合并
使二个或多个独立的二维图形变成复合形）
4.INSERT（插入）
(二)VERTEX(点)
1.点的不同类型
（CORNER，SMOOTH，BAZIER，BAZIER CORNER）
LOCK HANDLES（锁定控制柄）
ALIKE （选定相似的手柄，才移动）
ALL （所有选定了的手柄都移动）
例：雨伞（SQUEEZE(挤压)(1.5,-2,50,1)）
2.点的编辑命令
1）REFINE（细化，增加点）不改变原线的位置
2）INSERT（插入点）会改变原线的位置
3）BREAK（断开点）
4）CONNECT（将断开的点连接起来）
5）WELD(焊接)
先选定点,再点击WELD(不考虑点是否断开)
6）CROSSINSERT(在交叉处插入点)
(注意:CROSSINSERT只针对复合形)
7）FILLET（倒圆角）
8）CHAMFER（倒直角）
9）HIDE (隐藏)
10）UNHIDE ALL（取消隐藏）
11）DELETE （删除）
12）MAKE FIRST（使某一个做为起点）
（1）是一个封闭型的二维图形，
任何一个都可做为起点
（2）如果不是一个封闭型的二维图形
只有断开的两个端点才可以做为起点
(三)SEGMENT(线段)
1.REFINE
2.BREAK
3.DIVIDE(匀分)（先选定线段，后填数学，点DIVIDE）
4.DETACH(分离)
1）SAME SHAPES(同一图形)使分离出来的部分仍是源对象的一部分
2）REORIENT(重定向)使分离出来的部分独立
3）COPY(复制)创建分离次对象的一个新的复制品
5.线段 焊接（针对复合形）
（选定一段，移动回来，如有多段要移动多次）
6.HIDE
7.UNHIDE
8.DELETE
(四)SPLINE(曲线)
1.BOOLEAN(只针对复合形)
（如果不是复合形，请用ATTACH或
ATTACH MULT做成复合形）
2.OUTLINE
例:茶杯（LATHE）
3.TRIM （修剪）（一般最好针对复合形
4.MIRROR
5.EXTEND
6.HIDE
7.UNHIDE
8.DELETE
9.SPLINE 焊接（针对复合形）
（选定一段，移动回来，如有多段只要移动一次）

第九讲 LOFT(放样) 
一.基本概念
一个截面(SHAPE)沿着一条路径(PATH)不断的复制
注意:1.路径(PATH)只能是一条
（可以是LINE，CIRCLE或者其它的二维图形）
2.截面(SHAPE)可是一个,也可以是多个
(但是多个截面都要是同一类型的（单形，复合形）) 3.路径(PATH)和截面(SHAPE)只能是二维图形
例:镜框,吧台,沙发
二.LOFT的参数
1.SURFACE PARAMETERS(表面参数)
(主要针对LENGTH,WIDTH是否光滑)
*** 2.PATH PARAMETERS (路径参数)
(主要针对在路径什么地方放什么形)
3.SKIN PARAMETERS (表皮参数)
(主要针对加盖,增减分格数,是否显示表皮)
4.DEFORMATIONS (变形)
(SCALE,TWIST,TEETER,BEVEL,FIT) 
三.LOFT的修改
1.PATH（可以使路径变长或变短）
2.SHAPE（可以改变形的位置，旋转，缩放）
例:盆,窗帘
3.SHAPE对齐COMPARE
(1)MODIFIERS-SHAPE-COMPARE-PICK SHAPE
(2)进入次对象:SPLINE进行旋转
例:上面是一个圆,下面是一个矩形的放样
放样如果图形放反了怎么办
1.如果路径是不封闭的
我们用MAKE FIRST改变路径起点的位置
点是断开的两个端点
2.如果路径是封闭的
我们是改变SHAPE(形状)的位置用ROTATE命令
第十讲 LOFT(放样)的修改 
一.DEFORAMTIONS(变形)
1.SCALE(缩放)
1）截面形放大,缩小
2）界面介绍
例:FOUR-8,FOUR-9,蹋陷的易拉罐,FOUR-16,SEVEN-2
2.TWIST(扭曲)
1）截面扭曲
例:FOUR-11
3.TEETER(倾斜)
2）界面介绍
例:圆珠笔
4.BEVEL(倒角)
1）截面倒角
2）界面介绍
3）优点:圆滑
4）缺点:值不能大
5）修改器中的BEVEL
(1)优点:能很大
(2)缺点:不圆滑
5.FIT(拟合,适配)
1）界面介绍
2）物体的厚度感,深度感,高度感用Y轴（LEFT的截面）
3）物体的宽度用X轴 （TOP的截面）
例:FOUR-18,FOUR-17, FOUR-19,FOUR-20
二.模型精度不能太高(因为会影响渲染时间)

第十一讲 CAMERA（照相机） 
一.改变物体大小1.CAMERA离物体远,物体小,离物体近,物体大2.PARAMETERS(参数)1)LENS(焦距)单位：MM（毫米）(1)焦距值大----离物体近----物体大---物体越清晰相应的视野也就越小 ----看到的物体也就越少(2)焦距值小----离物体远----物体小---物体也不很清晰相应的视野也就越大 ----看到的物体也就越多2)FOV(视野)（FIELD OF VIEW）单位：DEGREE（度）(1)视野大----离物体远----物体小 ----看到的物体也就越多(2)视野小----离物体近----物体大----看到的物体也就越少3）LENS(焦距)与FOV(视野)之间是相反的。二.SHOW CONE（显示锥形）（无论是否选定照相机，照相机的视野都会显示）三.SHOW HORIZON(显示水平线)照相机的水平线四.ENVIRONMENT RANGES(环境范围)要以环境挂上钩1.RENDERING--ENVIRONMENT--ATMOSPHERE(大气)--FOG(雾)1)ACTIVE(激活雾)2) MERGE(合并另外一个场景的特效,如没特效,只是合并场景)3)TYPE(类型)STANDARD(标准雾)(均匀分布,沿水平方向分布)LAYERED(层雾)(沿垂直方向分布)2.NEAR RANGE(近距离范围)(弱,密度没有)3.FAR RANGE(远距离范围)(强,密度最大)4.NEAR RANGE到FAR RANGE之间的雾越来越多五.CLIPPING PLANES(剪切平面)1.CLIPPING MANUALLY(从NEAR到FAR两者间的物体显示,不在两者间的物体不显示)第十二讲 MATERIAL EDITOR（材质编辑器） 一.概念1.材质:是指物体外表信息的表现(包括:颜色,高光,光泽度,透明度等等)2.贴图:是指带有图案,纹理的图像信息,它附于材质之上注意:物体一定有材质,可以没有贴图3.热材质（同步材质）:赋给了物体的材质球(边有白框)4.冷材质（不同步材质）:没有赋给物体的材质球二.SHADER(阴影) BASIC（基础） PARAMETERS1.渲染模式2.WIRE（网格）1)网格与物体的分格数（SEGMENTS）有关2)用一步修改网格(1)加粗(EXTENDED PARAMETERS-WIRE-PIXELS(远近一样粗细)UNITS(远细,近粗 )3）二维物体网格--没有分格数插入EDIT SPLINE-SEGMENT-DEVIDE（注：顶盖，顶底没有）3.2-SIDED（内外所赋的材质是一样）4.FACE MAP5.FACETED三.不同渲染模式的基本参数( XXXXXX BASIC PARAMETERS)1.AMBIENT(背光区)2.DIFFUSE(主光区)3.SPECULAR(高光区,高光在什么地方与灯有必然的联系)1)SPECULAR LEVEL (高光级别)2)GLOSSINESS (光泽度,控制高光的面积)3)SOFTEN (柔和0~1,高光与非高光之间的过渡)4.SELF-ILLUMINATION (自发光,可以用数字表示,也可以用颜色表示)5.OPACITY (不透明度)1)值越低,越透明,物体越不亮2)可以进一步对透明进行修改EXTENDED PARAMETERS-ADVANCED TRANSPARENCYFALL OFF(衰减) TYPE(1)FILTER(过滤,在物体上留下一种颜色)(2)SUBTRACTIVE(当前物体颜色减去背景色)使物体更暗(3)ADDITIVE(当前物体颜色加上背景色)使物体更亮例:杯子,矿泉水瓶注意:玻璃作透明不能太厚3)INDEX OF REFRACTION(折射率)水(1.3)玻璃(1.5)高光固体(2.0)四.改变背景色的颜色(RENDERING-ENVIRONMENT)第十三讲 材质(二) 一.渲染模式*1.METAL(金属)1)SPECULAR(高光度) 最佳的取值范围（60~70）2)GLOSSINESS(光泽度) 最佳的取值范围（60~70）2.ANISOTROPIC(各向异形)主要控制高光的形状1)ANISOTROPIC(0,圆形高光,50,椭圆高光,100,线形高光)2)ORIENTATION(控制高光的方向)3.MULTI-LAYER(多层)在ANISOTROPIC基础上再增加一层来控制高光4.STRAUSS(金属加强)(金属渲染方式一种)渲染粗糙,不光滑,有凹凸感**5.BLINN(反射)柔和一点,低光照射的效果6.PHONG(多面)与BLINN相似7.OREN-NAYER-BLINN(明暗处理器)适合于制作表现物体模糊的"材质"二.图标介绍1.GET MATERIAL(获取材质)(注:只能获取材质,不能获取贴图)1)MTLIBARY (从材质库中获取材质)(C:\3DMAX4.0\MATLIBS\*.MAT)2)MTL EDITOR (从材质编辑器中获取材质)(一共有24个)3)ACTIVE (从激活的样本球(白框)中获取材质)4)SELECTED (从选定的物体中获取材质)5)SCENCE (从场景中获取材质)6)NEW (获取新的材质)2.PUT MATERIAL TO SCENE (将修改了的材质,重新赋给场景)3.ASSIGN MATERIAL TO SELECTION (将指定的材质赋给选定的对象)4.RESET MAP/MTL TO DEFAULT SETTINGS (恢复到默认的设置)5.MAKE MATERIAL COPY (将热材质进行复制)(注:1)当是热材质时,才有用 2)热材质变成冷材质,没有热材质标志)6.PUT TO LIBRARY(将编辑好的材质保存到材质库中,从材质库中可获取此材质)第十四讲 材质（三） 一.贴图（是指带有图案,纹理的图像信息,它附于材质之上）1.如何贴图1)通过选择MAPS(贴图)中的通道来贴图例:DIFFUSE COLOR(主光区颜色通道)2.图的来源方式1)自带的程序图(例如:BRICK,CELLULAR等等)**2)拾取外面的图(通过BITMAP)（注意：当你拾取外面的图时，注意图片格式图片格式是PSD的话是能拾取的）3.图的修改1)COORDINATES(贴图极坐标)(偏移,平铺,旋转)2)BITMAP PARAMETERS(位图参数)（只有拾取外面的图才有）4.有些图贴不上去,是因为贴图坐标的问题1)创建一个物体都带有GENERATE MAPPING COORDS(贴图坐标)(1)BOX5.贴图坐标的修改器MODIFIERS--UV COORDINATE--UVW MAP（针对于解决MISSING MAP COORDINATES（丢失贴图坐标）的问题）1)PLANAR (平面)2)CYLINDRICAL(圆柱体)3)SPHERICAL (球体) 4)SHRINK WRAP(收缩包裹) 5)BOX (盒子)6)FACE (面状)（与分格数有关）7)XYZ TO UVW (物体本身具有的坐标 到 图贴的方向)6.对齐 **1)FIT （适配） 2)CENTER （使图居中）3)BITMAP FIT （根据你所选的位图对齐）4)NORMAL ALIGN（法线对齐，只能在透视图中进行可任意移动图片在任意面上**5)VIEW ALIGN （以当前正交视图对齐）** 
6)REGION FIT （画一个区域进行贴，会产生重复贴， 想不重复贴回到材质编辑器，取消TILE钩）7)RESET （恢复原始值）8)ACQUIRE （从其它物体获取一个贴图坐标）（相对，绝对）（1-W的值）/2=U的值（图水平放在中间）（1-H的值）/2=V的值（图垂直放在中间）.jpg .bmp .tif .tga .png .psd（在3DMAX5.0中支持）

g键关闭网格，右键对象菜单，选择对象属性，勾选以灰色显示冻结。
摁住Shift可以画出直线。

# Bones和Biped的区别
用bones需要自己创建骨骼系统（比如人、动物、怪物等的骨骼），会使用到很多的约束、参数关键等；如果只是简单的机械臂，或者简单的几根骨骼动画，用这个还是比较方便的。而biped是系统自带的适合于两足动物的骨骼系统（当然也可以调节适合马、虎等动物），各种骨骼约束关联创建好了，直接可以蒙皮使用。

蒙皮： http://yuntv.letv.com/bcloud.html?uu=ejtjj0ntox&vu=4b9dd28064&pu=75346142bf&auto_play=0&gpcflag=1&width=100%&height=100%&isPlayerAd=0
先确定旋转方向，然后在确定高度。
视图操作栏，命令面板，创建面板，
四大键：Q,W,E,R
Q选择工具，点选，点击视图中的物体。按住鼠标左键不放，框选。
减选：Alt键，
加选：Ctrl键，点击物体或者框选物体。
Alt+A全选
Q：改变框选方式。
窗口和交叉：如果交叉，表示必须将物体完全的框住。
W:是一个双重工具。选择并且移动。X键可以放大或者缩小。
E：旋转功能
R：缩放功能，放在中心区域，三个轴同时缩放。
Zomm Extends All Selected
Alt+W：最大化视图
Orbit Subobject:旋转视图。