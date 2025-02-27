# 平滑处理
“平滑处理”（smoothing）也称“模糊处理”（bluring），是一项简单且使用频率很高的图像处理方法。
平滑处理的用途很多，最常见的是用来减少图像上的噪点或者失真。在涉及到降低图像分辨率时，平滑处理是非常好用的方法。
# 图像滤波和滤波器
图像滤波：即在尽量保留图像细节特征的条件下对目标图像的噪声进行抑制，是图像预处理中不可缺少的操作，其处理效果的好坏将直接影响到后续图像处理和分析的有效性和可靠性。
消除图像中噪声成为叫做图像的平滑化或滤波操作。信号或图像的能量大部分几种在幅度谱的低频和中频段是很常见的，而在较高频段，信息经常被噪声淹没。因此一个能减低高频成分幅度的滤波器就能够减弱噪声的影响。
图像滤波的目的有两个：一个是抽出对象的特征作为图像识别的特征模式；另一个是为适应图像处理的要求，消除图像数字化时所混入的噪声。
而对滤波处理的要求也有两条：一是不能损坏图像的轮廓及边缘等重要信息；二是使图像清晰视觉效果好。

平滑滤波是低频增强的空间域滤波技术。它的目的有两类：一类是模糊；另一类是消除噪音。

# 滤波和模糊

滤波可分为低通滤波和高通滤波两种。而高斯滤波是指用高斯函数作为滤波函数的滤波操作，至于是不是模糊，要看是高斯低通还是高斯高通，低通就是模糊，高通就是锐化。
高斯滤波是指用高斯函数作为滤波函数的滤波操作。
高斯模糊就是高斯低通滤波。

# 高斯滤波

高斯滤波是一种线性平滑滤波，适用于消除高斯噪声，广泛应用于图像处理的减噪过程。