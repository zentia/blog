---
title: 操作记录
date: 2017-10-19 20:15:12
tags:
  - server
---
pwd: ~/work_code
mkdbg_all
vim src/ini/Jenkins_Script/compile.sh
 ** IN DIR:BNMSGamePlay, make all with Makefile_debug.mk all **
115 make[3]: 进入目录“/root/work_code/src/svr/BNMSGamePlay”
116 ==== making DLL file ... ====
117 g++ -DLINUX -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -Wall -Wno-multichar -g -D_DEBUG -DPG_MEMORY_DISABLE_POOL -march=core2 -m32 -rdynamic -std=c++0x -o ../../bin/debug/svr/BNMSGamePlay/BNMSGamePlay_d.so ../../temp/debug/svr/BNMSGamePlay/BNMSGamePlay.s.o_d  /root/work_code/src/lib/libBNMSGamePlay_Lib_d.s.a /root/work_code/src/lib/lib    PNGS_d.s.a /root/work_code/src/lib/libWHNET_d.s.a /root/work_code/src/lib/libWHCMN_d.s.a   -lpthread -ldl -lrt -march=prescott -shared  -L/root/work_code/3rd/lib -L/root/work_code/src/lib
118 /usr/bin/ld: skipping incompatible /usr/lib/gcc/x86_64-redhat-linux/4.8.5/libstdc++.so when searching for -lstdc++
119 /usr/bin/ld: cannot find -lstdc++
120 /usr/bin/ld: skipping incompatible /usr/lib/gcc/x86_64-redhat-linux/4.8.5/libgcc_s.so when searching for -lgcc_s
121 /usr/bin/ld: cannot find -lgcc_s
122 collect2: 错误：ld 返回 1
123 make[3]: *** [../../bin/debug/svr/BNMSGamePlay/BNMSGamePlay_d.so] 错误 1
124 make[3]: 离开目录“/root/work_code/src/svr/BNMSGamePlay”

```shell
ldconfig #更新缓冲
```

 g++ -DLINUX -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -Wall -Wno-multichar -g -D_DEBUG -DPG_MEMORY_DISABLE_POOL -march=core2 -m32 -rdynamic -std=c++0x -o ../../bin/debug/xxsy/ScnSvr_AllInOne/ScnSvr_AllInOne_d ../../temp/debug/xxsy/ScnSvr_AllInOne/ScnSvr_AllInOne.o_d  /root/work_code/src/lib/libGSDB_Lib_d.a /root/work_code/src/lib/li      bScnSvr_d.a /root/work_code/src/lib/libGZSGamePlay_Lib_d.a /root/work_code/src/lib/libMCE4Chat_d.a /root/work_code/src/lib/libMCE_d.a /root/work_code/src/lib/libDIA4DBI_d.a /root/work_code/src/lib/libDIA_d.a /root/work_code/src/lib/libPNGS_d.a /root/work_code/src/lib/libWHNET_d.a /root/work_code/src/lib/libWHCMN_d.a /root/work_code/src/lib/libxxsy_cmn_d.a       /root/work_code/src/lib/libpgcmn_d.a -lpathfind_d -lcJSON_d -lcryptlib_d -llua_d -lzlib_d -ltss_sdk_d  -lz -ldl -lssl -lpthread -ldl -lrt -march=prescott  -L/root/work_code/3rd/lib -L/root/work_code/src/lib
g++ -DLINUX -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -Wall -Wno-multichar -g -D_DEBUG -DPG_MEMORY_DISABLE_POOL -march=core2 -m32 -rdynamic -std=c++0x -o ../../bin/debug/xxsy/ScnSvr_AllInOne/ScnSvr_AllInOne_d ../../temp/debug/xxsy/ScnSvr_AllInOne/ScnSvr_AllInOne.o_d  /root/work_code/src/lib/libGSDB_Lib_d.a /root/work_code/src/lib/libScnSvr_d.a /root/work_code/src/lib/libGZSGamePlay_Lib_d.a /root/work_code/src/lib/libMCE4Chat_d.a /root/work_code/src/lib/libMCE_d.a /root/work_code/src/lib/libDIA4DBI_d.a /root/work_code/src/lib/libDIA_d.a /root/work_code/src/lib/libPNGS_d.a /root/work_code/src/lib/libWHNET_d.a /root/work_code/src/lib/libWHCMN_d.a /root/work_code/src/lib/libxxsy_cmn_d.a /root/work_code/src/lib/libpgcmn_d.a -lpathfind_d -lcJSON_d -lcryptlib_d -llua_d -lzlib_d -ltss_sdk_d  -lz -ldl -lssl -lpthread -ldl -lrt -march=prescott  -L/root/work_code/3rd/lib -L/root/work_code/src/lib -pthread
/root/work_code/3rd/lib/liblua_d.a(core_profiler.o_d)：在函数‘lprofP_init_core_profiler’中：
/root/work_code/3rd/lua/profiler/core_profiler.c:154: 警告：the use of `tmpnam' is dangerous, better use `mkstemp'
/usr/bin/ld: /root/work_code/src/lib/libScnSvr_d.a(RegScn.o_d): undefined reference to symbol 'EVP_sha256@@libcrypto.so.10'
/lib/libcrypto.so.10: error adding symbols: DSO missing from command line
