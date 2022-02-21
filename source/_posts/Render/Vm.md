---
title: VMware12中CentOS7网络设置
date: 2017-10-18 10:00:36
tags:
  - VMware
  - CentOS7
---

VMware提供了三种将虚拟网卡和物理网卡捆绑起来的方式，即桥接(Bridge）模式，网络地址转换(Network Address Transformation, NAT)模式和主机(Host Only)模式。
三种模式区别，参见
关于三种模式的区别，简单而言，Bridge模式直接将虚拟机添加到局域网中，使虚拟机看起来像网内的一台真实计算机，虚拟网卡和宿主物理网卡之间不存在名义上的从属关系，因而需要局域网内具有多余的IP地址能够提供给虚拟机网卡，如果局域网内严格给每台计算机分配固定的IP，那这种Bridge模式就基本失效。在Bridge模式下虚拟机既可以访问到Internet也可以同局域网内的其他真实计算机通信；NAT模式下宿主的物理网卡就充当了路由器或交换机的角色，这时VMware会根据宿主的真实IP提供很多Subset IP供虚拟机使用，这样所有的虚拟机都是通过宿主的网络端口进行对Internet的访问，但看起来这些虚拟计算机都合法地访问到了局域网或者Internet，因为他们的IP地址通过NAT技术之后看起来是合法的。Host Only模式下虚拟机之间可以相互通信，但虚拟机并不能同局域网内的其他真实计算机直接通信，但该模式下通过宿主访问Internet还是可以的。 
<img src="http://oxcvfpext.bkt.clouddn.com/vm1.gif">
<img src="http://oxcvfpext.bkt.clouddn.com/vm2.gif">
<img src="http://oxcvfpext.bkt.clouddn.com/vm3.gif">
VM的虚拟网卡可以被设置成上述的三种网络连接模式，默认情况下，VMnet0被设置成为Bridge模式，VMnet1被设置为Host Only模式，而VMnet8的默认连接方式为NAT模式。VMnet2-VMnet7和VMnet9这七块虚拟网卡用户可以自定义，但是在所有的虚拟网卡中仅有一块能被设置为NAT模式，默认情况就是VMnet8。用户可以在VMware workstation->编辑->虚拟网络编辑器 中查看这些信息。
<img src="http://oxcvfpext.bkt.clouddn.com/vm4.png">
<img src="http://oxcvfpext.bkt.clouddn.com/vm5.png">
这里已NAT模式配置
1. 打开“网络和共享中心”选择“VMware Virtual Ethernet Adapter for VMnet8”网卡右键属性，选择VMware Bridge Protocol,同时设置ip自动获取
<img src="http://oxcvfpext.bkt.clouddn.com/vm6.png">
<img src="http://oxcvfpext.bkt.clouddn.com/vm7.png">
2. 将物理网卡网络分享给VMware Virtual Ethernet Adapter for VMnet8
<img src="http://oxcvfpext.bkt.clouddn.com/vm8.png">
3. 虚拟机网络连接设置为NAT模式，选择虚拟机，右键》设置》网络适配器》NAT 模式
<img src="http://oxcvfpext.bkt.clouddn.com/vm9.png">
<img src="http://oxcvfpext.bkt.clouddn.com/vm10.png">
4. 检验网络是否正常，不正常，检测ifcfg-e****(每台可能不一样)中的bootproto是不是dhcp，如果不是则修改为dhcp，此外还需要设置onboot = yes; 然后用命令service network restart重启网络连接，就可以连上网络了
<img src="http://oxcvfpext.bkt.clouddn.com/vm11.png">
