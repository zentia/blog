---
title: Linux
date: 2017-10-15 12:08:04
tags:
  - Linux
---
例子：将目录A重命名为B

mv A B

例子：将/a目录移动到/b下，并重命名为c

mv /a /b/c

1. yum install wget
2. yum install gcc
3. yum install gcc-c++
4. wget http://ftp.gnu.org/pub/gnu/ncurses/ncurses-5.6.tar.gz
5. tar -zxvf 
6. ./configure -prefix=/usr/local -with-shared-without-debug
7. make > t.log
8. make install

- no termcap library found
wget http://ftp.gnu.org/gnu/termcap/termcap-1.3.1.tar.gz
tar -zxvf termcap-1.3.1.tar.gz
cd termcap
./configure --help
set t.log
vim t.log
/MACHTYPE
=x86_64-redhat-linux-gnu
yum install gdb (这尼玛坑爹呀)

# du查看某个文件或目录占空间大小
	du -ah
	du -sh
# linux下修改.bash_profile立即生效的方法
1. source .bash_profile
2. exec bash --login

# linux中文乱码
查看当前系统默认的字符集
locale
安装中文包
yum -y groupinstall chinese-support

生效
export LANG="zh_CN.UTF-8"
重新载入
. /etc/profile
# 解决中文乱码
export LC_ALL="zh_CN.utf8"
# Linux中安装gcc g++ yum方式
```javascript
yum -y install gcc
yum -y install gcc-c++
yum intall make
```
# Linux注销用户
who 列出登陆的用户列表
 who
root@localhost:~/work_code/src/svr> who
root     pts/0        2017-10-20 06:33 (192.168.171.1)
root@localhost:~/work_code/src/svr> pkill -kill -t pts/0
# cannot find -lgcc_s
在64位Centos7上编译32位C程序，因为是编译32位的目标程序，所以使用gcc编译的时候需要加上-m32选项，但是编译的话会报错，如下：
```
root@localhost:~/work_code> gcc -g -m32 -o hello hello.c
/usr/bin/ld: skipping incompatible /usr/lib/gcc/x86_64-redhat-linux/4.8.5/libgcc_s.so when searching for -lgcc_s
/usr/bin/ld: cannot find -lgcc_s
collect2: 错误：ld 返回 1
```
getconf LONG_BIT #查看自己版本是32位还是64位
32位的库
yum install glibc-devel.i686
yum install libstdc++-devel.i686
# kill -2 和 kill -9的区别
-2类似Ctrl + C 是程序在结束之前，能够保存相关数据，然后在退出。
-9直接强制结束程序。

# 硬链接(hard link)和软链接(符号链接，fsoft link或symbolic link)
我们知道文件都有文件名和数据，在Linux上被分成两个部分：用户数据(user data)和元数据(metadata)。用户数据，即文件数据块(data block)，数据块时记录文件真实内容的地方；而元数据则是文件的附加属性，如文件大小、创建时间、所有者等信息。在Linux中，元数据中的inode号(inode是文件元数据的一部分但其并不包含文件名，inode号即索引节点号)才是文件的唯一标识而非文件名。文件名仅是为了方便人们的记忆和使用，系统或程序通过inode号寻找正确的文件数据块。
若一个inode号对应多个文件名，则称这些文件为硬链接。换言之，硬链接就是同一个文件使用了多个别名。硬链接可由命令link或ln创建。
	link oldfile newfile
	ln oldfile newfile
由于硬链接有着相同inode号仅文件名不同的文件，因此硬链接存在以下几点特性：
- 文件有相同的inode及data block；
- 只能对已存在的文件进行创建；
- 不能交叉文件系统进行硬链接的创建；
- 不能对目录进行创建，只可对文件创建；
- 删除一个硬链接文件并不影响其它有相同inode号的文件。
软连接与硬链接不同，若文件用户数据块存放的内容是另一文件的路径名的指向，则该文件就是软连接。软链接就是普通文件，只是数据块内容有点特殊。软链接有着自己的inode号以及用户数据块
usage: ln -s 源文件 目标文件

# ssh登录方法
    ssh -l username ip
# 永久性获取ROOT权限
    sudo passwd
    su

# 64位linux机器上编译32位openssl库
    setarch i386 ./config -m32
    wget http://zlib.net/zlib-1.2.11.tar.gz
    tar -zxvf zlib-1.2.111.tar.gz
    cd zlib-1.2.11
    ./configure --prefix=/usr/local
    make && make install
	find ./ -name "*.o" -exec rm {} \;
	make: ***[install_docs] Error 1
	vim /usr/bin/pod2man 注释69行#

i386 几乎所有的X86平台
i686 只是i386的一个子集

# Linux内存管理
<% img asset_img Linux1.webp %>
在linux中，每一个进程都被抽象为task_struct结构体中，成为进程描述符，存储着进程各方面的信息；例如打开的文件，信号以及内存等等；然后task_struct中的一个属性mm_struct管理着进程的所有虚拟内存，成为内存描述符。在mm_struct结构体中，存储着进程各个内存段的开始以及结尾，如上图所示；进程使用的物理内存，即常驻内存RSS页数，这个内存使用的虚拟地址空间VSZ页数，还有进程虚拟内存区域集合和页表。
从上图可以看出，进程有代码段Text segment,数据段（已初始化的全局，静态变量），BSS段（未初始化的全局，静态变量），堆，内存映射区以及栈；
每一块虚拟内存区（VMA）都是由一块连续的虚拟地址组成，这些地址从不覆盖。一个vm_area_struct实例描述了一块内存区域，包括这块内存区域开始以及结尾地址；flags标志决定了这块内存的访问权限和行为；vm_file决定这块内存是由哪个文件映射的，如果没有文件映射，则这块内存为匿名的(anonymous)。上图中提到的每个内存段，都对应一个vm_area_struct结构。如下图所示
<% asset_img Linux2.webp %>
上图即为/bin/gonzo进程的内存布局。程序的二进制文件映射到代码段和数据段，代码段为只读只执行，不可更改；全局以及静态的未初始化的变量映射到BSS段，为匿名映射，堆和栈也是匿名映射，因为没有相对应的文件映射；内存映射区可以映射共享库，映射文件以及匿名映射，所以这块内存段可以使文件映射也可以是匿名映射。而且不同的文件，映射到不同的vm_area_struct区。
这些vm_area_struct集合存储在mm_struct中的一个单向链表和红黑树中；当输出/proc/pid/maps文件时，只需要遍历这个链表即可。红黑树主要是为了快速定位到某一个内存块，红黑树的跟存储在mm_rb域。
线性地址需要通过页表才能转换为物理地址。每个进程的内存描述符也保存了这个进程页表指针pgd,每一块虚拟内存页都和页表的某一项对应。
虚拟内存是不存储任何数据的，它只是将地址空间映射到物理内存。物理内存有内核伙伴系统分配，如果一块物理内存没有被映射，就可以被伙伴系统分配给虚拟内存。刚分配的物理内存可能是匿名的，存储进程数据，也可能是缓存，存储文件或块设备的数据。一块虚拟内存vm_area_struct块是由连续的虚拟内存也组成的，而这些虚拟内存块映射的物理内存却不一定连续，如下图所示：
<% asset_img Linux3.webp  %>

# 挂载
`mount -t cifs -o username=administrator,passwd=,uid="1000",gid="1000",vers=2.0 //192.168.0.102/workspace /home/develop`