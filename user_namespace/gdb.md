# 实时跟踪进程执行
> 1. gdb [进程启动文件]
> 2. gdb窗口前台模式启动进程：r -F [进程启动参数]
## gdb 环境打印变量
```
// f 1 中 1是堆栈行编号，即切换到程序运行到 1 处时的环境；
(gdb) f 1
(gdb) p [变量指针]->[变量成员]
(gdb) p *[变量指针]->[变量成员指针]
打印结构体大小
(gdb) p sizeof(struct struct_name)
由地址查看源码行信息：
(gdb) l* [addr]
也可以通过addr2line指令(非gdb环境，shell指令)：
addr2line -e [程序路径] [addr] -f -p
输出内容： [函数名] at 源码文件路径:line
设置gdb打印窗口无限大（默认是打印一页按enter键后继续打印下一页）
(gdb)set height 0
gdb界面打印内存都会保存到gdb.log文件中
(gdb)set logging on
打印addr对应地址内存，a是打印格式
(gdb) x/[len]a [addr]
以二进制打印addr1~addr2的内存
(gdb) dump binary memory result.bin [addr1] [addr2]
```
# [gdb手册](https://sourceware.org/gdb/onlinedocs/gdb/Threads.html)
# [gdb使用技巧](https://wizardforcel.gitbooks.io/100-gdb-tips/content/print-threads.html)
# 强制生成core文件
> gcore [pid]
# 修改变量
> gdb attach [pid]   
> p [全局变量名]=[值]   
# 分析进程堆内存分布gdb脚本，可分析coredump文件
```shell
define my_malloc_stats
  set $in_use = mp_.mmapped_mem
  set $system = mp_.mmapped_mem
  set $arena = &main_arena

  set $arena_n = 0

  # mALLINFo
  while $arena
    set $system = $system + $arena->system_mem

    set $avail = (($arena->top)->size & ~(0x1 | 0x2 | 0x4))

    set $fastavail = 0
    set $i = 0

    # traverse fastbins
    while $i < 10
      set $p = (($arena)->fastbinsY[$i])
      while $p
        set $fastavail = $fastavail + (($p)->size & ~(0x1 | 0x2 | 0x4))
        set $p = $p->fd
      end
      set $i = $i + 1
    end

    set $avail = $avail + $fastavail

    # traverse regular bins
    set $i = 1
    while $i < 128
      set $b = (mbinptr) (((char *) &(($arena)->bins[(($i) - 1) * 2])) - 16)
      set $p = $b->bk
      while $p != $b
        set $avail = $avail + (($p)->size & ~(0x1 | 0x2 | 0x4))
        set $p = $p->bk
      end
      set $i = $i + 1
    end

    printf "Arena: %u\nsystem bytes     = %10lu\nin use bytes     = %10lu\n", $arena_n, $arena->system_mem, ($arena->system_mem - $avail)

    set $in_use = $in_use + ($arena->system_mem - $avail)

    set $arena = $arena->next
    if $arena == &main_arena
      loop_break
    end
    set $arena_n = $arena_n + 1
  end

  printf "Total:\nsystem bytes     = %10lu\nin use bytes     = %10lu\n", $system, $in_use
end
```
