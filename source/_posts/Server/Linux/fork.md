---
title: fork
date: 2017-10-25 09:38:36
tags:
- Server
- Linux
Categories:
- Server
- Linux
---
通过系统调用创建一个与原来进程几乎完全相同的进程，也就是说两个进程可以做完全相同的事情，但如果初始参数或者传入的变量不同，两个进程也可以做不同的事。
一个进程调用fork()函数之后，系统先给新的进程分配资源，例如存储数据和代码的空间。然后吧原来的进程的所有值都复制到新的进程中，只有少数值与原来进程的值不同。相当于克隆了一个自己。
```c
int whdaemon_init(bool bAutoCloseFD)
{
	printf("Transform to daemon ... ");

	pid_t pid;

	if( (pid=fork()) < 0)
	{
		printf("Failed!%s", WHINEED);
		return -1;
	}
	else if(pid !=0)
	{
		//parent goes bye-bye
		printf("SUCCESS, pid might be:%d!%s", pid+1, WHLINEEND);
		exit(0);
	}

	// 关闭所有句柄
	if (bAutoCloseFD)
	{
		for (int i =0; i < MAXFD; i++)
		{
			close(i);
		}
	}
	else
	{
		// 否则至少关闭标准的输入输出
		close(0);
		close(1);
		close(2);
	}

	// 变成seeion leader
	setsid();

	signal(SIGHUP, SIG_IGN);

	// 结束第一个紫禁城
	if ((pid=for() != 0))
	{
		exit(0);
	}

	umask(0);

	return 0;
}
```