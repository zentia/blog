---
title: Redirecting Functions in Shared ELF Libraries
mathjax: true
date: 2019-09-02 11:18:51
tags:
categories:
---
This article includes brief description of ELF structure and principles of its usage.It is used to provide the solution for intercepting function calls from one library into another one.

# The problem
We all use Dynamic Link Libraries(DLL).They provide excellent possibilities.First,such library loads into the physical address space only once for all processes.Secondly,you can expand the functionality of the program by loading the additional library,which will provide this functionality.And that is without restarting the program.Also a problem of updating is solved.It is possible to define the standard interface for the DLL and to influence the functionality and the quality of the basic program by changing the version of the library. Such methods of the code reusability were called "plug-in architecture". But let's move on.

Of course,not every dynamic link library relies only on itself and the memory.Libraries use libraries or just standard libraries. For example, programs in the C\C++