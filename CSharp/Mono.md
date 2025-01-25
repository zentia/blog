Mono主要包含了C#的编译器，CLI(Common Language Infrastructure)实现和一系列相关的开发工具。
# 源码结构
msc:
	msc: Mono实现的基于Ecma标准的C#编译器。
	class:CLI的C#的实现。类似于Android中的Java蹭，应用程序看到的是这一层提供的接口。这一层是平台无关的。
	ilasm:反汇编器，将Native code反汇编成bytecode。
mono:
	mini:JIT编译器，将bytecode编译成native code。
	metadata:Mono的runtime,CLI的Native级的下实现。
	io-layer:与操作系统的接口实现，像socket,thread,mutex这些。
liggc:GC实现的一部分。
# Mono主要工作框架
```
mini/main.c:main()
	mono_main_with_options()
		mono_mian()
			mini_ini()
			mono_assembly_open()
			main_thread_handler()//assembly(也就是bytecode)的编译执行
			mini_cleanup()
main_thread_handler()
	mono_jit_exec()
		mono_assembly_get_image()//得到image信息，如"test.ext"
		mono_image_get_entry_point()//得到类，方法信息
		mono_runtime_run_main(method,argc,argv,NULL)
			mono_thread_set_main(mono_thread_current());//把当前线程设为主线程
			mono_assembly_set_main()
			mono_runtime_exec_main() //编译及调用目标方法
mono_runtime_exec_main()
	mono_runtime_invoke(method, NULL, pa, exc)//要调用的方法，如"ClassName::Main()"
		default_mono_runtime_invoke()//实际上是调用了mono_jit_runtime_invoke()
			info->compiled_method = mono_jit_compile_method_with_opt(method)//编译目标函数
			info->runtime_invoke = mono_jit_compile_method()//编译目标函数的runtime wrapper
				mono_jit_compile_method_with_opt(method,default_opt,&ex)
			runtime_invoke=info->runtime_invoke
			runtime_invoke(obj,pararms,exc,info->compiled_method)//调用wrapper,wrapper会调用目标方法
mono_jit_compile_method_with_opt()
	mono_jit_compile_method_inner()
		mini_method_compile(method, opt, target_domain, TRUE, FALSE, 0)//通过JIT编译给定方法
		mono_runtime_class_init_full()//初始化方法所在对象
			method=mono_class_get_cctor()//得到类的构造函数
			if(do_initialization)//对象需要初始化
				mono_runtime_invoke()//调用相应构造函数来构造对象，如"System.console:.cctor()"
					mono_jit_runtime_invoke()
```

# 垃圾回收

Mono在2.10版本前都是使用了BOEHM的垃圾回收器。Unity目前使用的也是BOEHM，BOEHM采用的是Mark-Sweep，也就是先通过一个Root指针来遍历所有被引用的对象，并标记。直到遍历完所有的指针。然后将未引用的对象释放掉。

# 线程池

应用程序或者Mono runtime中的一些异步恩物可以交由单独线程完成。Mono中提供了两个线程池:async_tp和async_io_tp。往线程池加线程函数为threadpool_append_jobs()，当第一次试图往里边加线程时，会进行初始化，起一个"Monitor"线程(该线程执行monotor_thread())。现在假设

# Mono内存

内存占用：

1. 库代码：Unity库、第三方库
2. Native堆：资源、Unity逻辑、第三方逻辑
3. Mono堆：C#代码

Unity游戏逻辑代码所使用的语言为C#，我们知道C#代码所占用的内存又称为mono内存，由于Unity它是通过mono来跨平台解析并运行C#代码的，它在Android系统上面，因此游戏的lib目录下存在的libmono.so文件，就是mono在Android系统上的实现。

C#的代码是通过mono来解析执行的，所需要的内存字然也是由mono来进行分配管理，下面来介绍一个mono的内存管理策略以及内存泄漏分析。

# Mono内存管理的基本策略

Mono通过垃圾回收机制(GarbageCollect，简称GC)堆内存进行管理。Mono内存分为两部分，已用内存(used)和堆内存(heap)，已用内存它指的是mono实际需要使用的内存，堆内存指的是mono向操作系统申请的内存，两者的差值就是mono的空闲内存。

当mono需要分配内存时，首先是会查看空闲内存是否足够，若足够的话，则是直接在空闲内存中分配，否则mono会进行一次GC以释放更多的内存，如果GC之后仍然没有足够的空闲内存，则mono会向操作系统申请内存，并扩充堆内存。

C/C++ Mono运行时 托管代码(CIL)
