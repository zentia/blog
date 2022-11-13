---
title: 深入理解Dart虚拟机启动
---
# 概述
## Dart虚拟机概述
Dart虚拟机拥有自己的Isolate，完全由虚拟机自己管理的，Flutter引擎也无法直接访问。Dart的UI相关操作，是由Root Isolate通过Dart的C++调用，或者是发送消息通知的方式，将UI渲染相关的任务提交到UIRunner执行，这样就可以跟Flutter引擎线管模块进行交互。

Isolate之间内存并不共享，而是通过Port方式通信，每个Isolate是有自己的内存以及相应的线程载体。从字面上理解，Isolate是“隔离”，isolate之间的内存内是逻辑隔离的。Isolate中的代码也是顺序执行，因为Dart没有共享内存的并发，没有竞争的可能性，故不需要加锁，也没有死锁风险。对于Dart程序的并发则需要依赖多个isolate来实现。

## Dart虚拟机启动工作
文章在介绍Flutter引擎启动过程，有两个环节没有展开讲解，那就是DartVM和Isolate的创建过程。
Dart与Isolate的启动过程是在FlutterActivity的onCreate()过程触发，在引擎启动的过程有3个环节会跟Dart与Isolate的启动相关，如下所示：
## DartVM启动工作
AndroidShellHolder对象的创建过程，会调用到DartVMRef::Create()，进行DartVM创建，主要是为DartVM解析数据DartVMData，注册一系列Native方法，创建名为专属vm的Isolate对象，初始化虚拟机内存、堆、线程、StoreBuffer等大量对象，工作内容如下：
1. 同一个进程只有一个Dart虚拟机，所有的Shell共享该进程中的Dart虚拟机，当leak_vm为false则会在最后一个Shell对象退出时会回收dart虚拟机，当leak_vm为true则即便Shell对象全部退出也不会回收dart虚拟机，这是为了优化再次启动的速度；
2. 创建的IsolateNameServer对象，里面有一个重要的成员port_mapping_，以端口名为key，端口号为value的map结构，记录所有注册的port端口；可通过RegisterIsolatePortWithName()注册Isolate端口，通过LookupIsolatePortByName()根据端口名来查询端口号；
3. 创建DartVMData对象，从settings中解析出vm_snapshot，isolate_snapshot这两个DartSnapshot对象，DartSnapshot里有data_和instructions_两个字段；
4. 创建ListeningSocketRegistry对象，其中有两个重要的成员变量sockets_by_port_（记录以端口为key的socket集合），socket_by_fd_（记录以fd为key的socket集合）；
5. 通过pthread_create创建名为"dart io EventHandler"的线程，然后进入该线程进入poll轮询方法，一旦收到时间则执行HandleEvents()
6. 执行DartUI::InitForGlobal():执行相关对象的RegisterNatives()注册Native方法，用于Dart代码调用C++代码。
    - 创建DartLibraryNatives类型的g_natives对象，作为静态变量，其成员变量entries_和symbols_分别用于记录NativeFunction和Symbol信息；通过该对象的Register()，注册dart的native方法，用于Dart调用C++代码；通过GetNativeFunction()，GetSymbol()来查询Native方法和符号信息。
    - Canvas、DartRuntimeHooks、Paragraph、Scene、Window等大量对象都会执行RegisterNatives(g_natives)来注册各种Native方法。
7. 执行Dart::Init()：传递的参数params记录isolate和文件等相关callback，用于各种对象的初始化
    - 初始化VirtualMemory、OSThread、PortMap、FreeListElement、ForwardingCorpse、Api、NativeSymbolResolver、SemiSpace、StoreBuffer、MarkingStack对象
    - 初始化ThreadPool线程池对象
    - 创建名"vm-isolate"的Isolate对象，注意此处不允许混肴符号，将新创建的isolate添加到isolate_list_head_链表；
    - 为该isolate创建Heap、ApiState等对象；
    - 创建IsolateMessageHandler，其继承于MessageHandler，MessageHandler中有两个比较重要的成员变量queue_和oob_queue_，用于记录普通消息和oob消息的队列。
    - isolate需要有一个Port端口号，通过PortMap::CreatePort()随机数生成一个整形的端口号，PortMap的成员变量map_是一个记录端口entry的HashMap，每一个entry里面有端口号，handler，以及端口状态。