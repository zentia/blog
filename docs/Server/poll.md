Poll就是监控文件是否可读的一种机制，作用与select一样。
应用程序的调用如下：

	int poll(struct pollfd *fds, nfds_t nfds, int timeout)
Poll机制会判断fds中的文件是否可读，如果可读则立即返回，返回的就是可读fd的数量，如果不可读，那么进程就会休眠。

# 内核实现流程：
当应用程序调用poll函数的时候，系统会调用sys_poll函数，该函数最终调用do_poll函数，do_poll函数中有一个死循环，在里面又会利用