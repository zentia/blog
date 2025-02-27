# 摘要
提出了一种简单、快速的多边形纹理坐标和阴影参数插值方法。该方法可应用于z-buffer等扫描转换算法和painter算法中，对纹理坐标、颜色、法向量等阴影参数进行屏幕空间插值。以前的一些方法在屏幕空间中执行线性插值，但这是一种可旋转的变体，在纹理映射的情况下，会导致不可靠的“橡胶片”效果。为了正确地计算屏幕空间和参数空间之间的非线性投影变换，我们在多边形上使用有理线性插值，在每个像素上执行多个分割。我们提供了更简单的公式来设置这些插值计算，将每个多边形的设置成本降低到零，并将每个顶点的成本降低到几个除法。
# 介绍
我们首先定义了我们的术语，然后总结了一种简单的线性方法，用于在扫描转换期间插值着色参数。通过对线性插值的缺陷进行分析，描述了该方法，并证明了其正确性。
## 定义
我们定义了以下坐标系:对象空间是三维坐标系，其中定义了每个多边形。可以有多个对象空间。世界空间是一个坐标系统，通过三维建模转换(平移、旋转和缩放)与每个对象空间相关。三维屏幕空间是显示器的三维坐标系统，是一个像素坐标(x,y)和深度z的“透视空间”。相机参数与世界空间有关。最后，二维屏幕空间(简称“屏幕空间”)是没有z的三维屏幕空间的二维子空间。
为了方便仿射和射影(透视)变换，我们使用齐次符号[Maxwell46]，例如，二维实点(x,y)由三维齐次向量p = (xw,yw,w)表示，其中w是一个任意的非零数。我们将漫不经心地处理w = 0的情况。在齐次表示法中，二维点用3向量表示，三维点用4向量表示。
我们使用下面符号：

|COORDINATE SYSTEM|REAL|HOMOGENEOUS|
|--|--|--|
|3-D object space |$(x_0,y_0,z_0)$|$P_0=(x_0w_0,y_0w_0,z_0w_0,w_0)$|
|3-D screen space |$(x,y,z)$|$P_\delta=(xw,yw,zw,w)$|
|2-D screen space |$(x,y)$|$P_s=(xw,yw,w)$|
## 投影和仿射映射
We will use two classes of mapping (transformation): affine and projective.