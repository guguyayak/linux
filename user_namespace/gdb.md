# 实时跟踪进程执行
> 1. gdb [进程启动文件]
> 2. gdb窗口前台模式启动进程：r -F [进程启动参数]
## gdb 环境打印变量
```
// f 1 中 1是堆栈行编号，即切换到程序运行到 1 处时的环境；
(gdb) f 1
(gdb) p [变量指针]->[变量成员]
(gdb) p *[变量指针]->[变量成员指针]
```
# [gdb手册](https://sourceware.org/gdb/onlinedocs/gdb/Threads.html)
# [gdb使用技巧](https://wizardforcel.gitbooks.io/100-gdb-tips/content/print-threads.html)
# 强制生成core文件
> gcore [pid]
# 修改变量
> gdb attach [pid]   
> p [全局变量名]=[值]   
