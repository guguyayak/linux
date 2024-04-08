# gdb切换线程
1. info threads 显示所有线程信息；
2. thread <thread_id> 切换指定线程
# gdb设置运行参数
1. 在终端中进入程序所在的目录。
2. 启动gdb，输入命令：`gdb <可执行文件名>`。例如：`gdb myprogram`。
3. 输入以下命令设置程序执行参数：
   ```
   set args <参数列表>
   ```
   其中 `<参数列表>` 是以空格分隔的所有程序执行参数。例如：`set args arg1 arg2 arg3`。
4. 输入命令 `run` 或者缩写形式 `r` 来运行程序。例如：`run`。
5. 如果想要在程序运行时设置断点，可以输入 `break <函数名>` 来设置断点。例如：`break main`。
6. 输入 `continue` 或者缩写形式 `c` 来继续运行程序，直到遇到断点停止。例如：`continue`。
7. 在程序暂停时，可以使用多种命令来检查程序状态和变量值。例如：`print <变量名>` 用于打印变量的当前值。
8. 继续按照需要调试的逻辑进行操作，最后退出gdb。
# 使用GDB来打断点并单步执行程序
1. `break`：在代码中设置断点。
2. `run`：开始执行程序。
3. `step`：单步执行程序，并进入到函数内部。
4. `next`：单步执行程序，但不进入函数内部。
5. `finish`：执行完当前函数并返回到上一层函数调用处。
6. `info`：查看程序状态信息，如当前栈帧、寄存器等。
7. `backtrace`（缩写为`bt`）：查看当前调用栈。
例如，要在某个函数开头设置断点并开始单步执行程序，可以按照以下步骤进行操作：
1. 使用`break`命令在函数开头位置设置断点，例如：`break main`。
2. 使用`run`命令开始执行程序。
3. 当程序执行到断点处停止时，使用`step`命令进入函数内部，并继续单步执行程序。可以重复使用`step`或`next`命令来逐步查看函数内部执行情况。
4. 当需要返回到上一层函数时，可以使用`finish`命令。
5. 使用`backtrace`命令查看当前调用栈，了解函数调用链情况。
6. 使用 continue 命令（或缩写形式的 c）来继续执行程序直到下一个断点处停止。如果没有其他断点，程序将一直运行直到结束。
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
(gdb) x/[N]c [addr]  打印 N 个字符
(gdb) x/1s [addr]  打印字符串
以二进制打印addr1~addr2的内存
(gdb) dump binary memory result.bin [addr1] [addr2]

可以用“disas /m fun”（disas是disassemble命令缩写）命令将函数代码和汇编指令映射起来
如果想在调试一个线程时，让其它线程暂停执行，可以使用“set scheduler-locking on”命令
在使用gdb时，如果想让断点只生效一次，可以使用“tbreak”命令（缩写为：tb）
当gdb启动时，会读取HOME目录和当前目录下的的配置文件，执行里面的命令。这个文件通常为“.gdbinit”。
# 保存历史命令
set history filename ~/.gdb_history
set history save on
# 每行打印一个结构体成员
set print pretty on
```
# gdb指令：[info locals](https://visualgdb.com/gdbreference/commands/info_locals)
```
The info locals command displays the local variable values in the current frame. You can select frames using the frame, up and down commands.
Note that the info locals command does not display the information about the function arguments. Use the info args command to list function arguments.
```
# [gdb手册](https://sourceware.org/gdb/onlinedocs/gdb/Threads.html)
# [gdb使用技巧](https://wizardforcel.gitbooks.io/100-gdb-tips/content/)
# 强制生成core文件
> gcore [pid]   
> 该指令会执行 gdb attach [pid]，生成core的过程中会阻塞进程
# 修改变量
> gdb attach [pid]   
> p [全局变量名]=[值]   
# [分析进程堆内存分布gdb脚本，可分析coredump文件](https://gist.github.com/darnaut/1d39b74a2a3311fb795e)
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
# 用户态core遍历malloc_chunk
```sh
[root@c75n81p64 lmm]# cat pChunk.gdb
set $total=0
set $alloc=0
set $free=0
set $seq=0
set $chk=(struct malloc_chunk *)(mp_->sbrk_base)
while ($chk != 0)
    set $sz=$chk->size & (~(0x7))
    if ($chk == main_arena->top)
        set $free=$free+$sz
        set $seq=$seq+$sz
        printf "%p, %ld, %d, %ld, %ld, %ld, %ld\n", $chk, $sz, 0, $alloc, $seq, $free, $total
        loop_break
    end
    set $nxt_chk=(struct malloc_chunk *)((char *)$chk + $sz)
    set $used=$nxt_chk->size & 0x1
    if ($used)
        set $alloc=$alloc+$sz
        set $seq=0
    else
        set $free=$free+$sz
        set $seq=$seq+$sz
    end
    printf "%p, %ld, %d, %ld, %ld, %ld, %ld\n", $chk, $sz, $used, $alloc, $seq, $free, $total
    set $chk=$nxt_chk
end
```
# 查看函数地址或者由地址查找函数名
```
debug时候，经常需要查找函数地址，在gdb中可以用 "info address function_name"

(gdb) info address test
Symbol "test" is a function at address 0x9e250

有时候，也会有知道函数地址，反找函数名的需求，在gdb中可以用 "info symbol func_addr"
(gdb) info symbol 0x9e250
test in section .text
```
# 多个断点，if判断不同断点
## 断点信息(断点函数是ganesha.nfsd中函数)
```bash
(gdb) b svc_vc_xprt_setup
Breakpoint 7 at 0x7fe58354d615: file /tmp/NFS-Ganesha-3.2/src/libntirpc/src/svc_vc.c, line 138.
(gdb) b svc_ref_it if xprt->xp_type == 4
Breakpoint 8 at 0x7fe5835452b7: svc_ref_it. (8 locations)
(gdb) b svc_release_it if xprt->xp_type == 4
Breakpoint 9 at 0x7fe58352eab0: svc_release_it. (15 locations)
Breakpoint 1, svc_vc_xprt_setup (sxpp=0x7f5d13f71fe8) at /tmp/NFS-Ganesha-3.2/src/libntirpc/src/svc_vc.c:138
138     /tmp/NFS-Ganesha-3.2/src/libntirpc/src/svc_vc.c: No such file or directory.
Breakpoint 2, svc_ref_it (xprt=0x7f5d84c2c700, flags=0, tag=0x7f5d9565f660 <__func__.12320> "svc_rqst_rearm_events_locked", line=745)
    at /tmp/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/svc.h:540
540     /tmp/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/svc.h: No such file or directory.
Breakpoint 3, svc_release_it (xprt=0x7f5d43fe1800, flags=0, tag=0x7f5d95661791 <__func__.12217> "svc_ioq_write", line=399)
    at /tmp/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/svc.h:563
563     in /tmp/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/svc.h
```
## python 脚本信息
```bash
[root@node131 lmm]# cat p_bt_c.py
import gdb

def LogAndC():
    c_bp = gdb.selected_frame().find_sal().line
    thread = gdb.selected_thread().ptid[1]
    func = gdb.selected_frame().function().name

    if c_bp == 554 or c_bp == 577:
        xprt_ref_v = gdb.parse_and_eval('xprt->xp_refcnt')
        print("func {} xprt->xp_refcnt: {} tid {}".format(func, int(xprt_ref_v), int(thread)))
    if c_bp == 1276:
        print("func {} tid {}".format(func, int(thread)))
        gdb.execute("p stat")

    gdb.execute("bt")
    gdb.execute("c")

class p_bt_c(gdb.Command):
    def __init__(self):
        super(p_bt_c, self).__init__("p_bt_c_func", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        LogAndC()

p_bt_c()
```
## 脚本执行
```bash
(gdb) source ./p_bt_c.py
(gdb) p_bt_c
func svc_ref_it xprt->xp_refcnt: 2 tid 3156343
#0  svc_ref_it (xprt=0x7f58a8824100, flags=0, tag=0x7f58fb66a010 <__func__.23556> "alloc_nfs_request", line=1444)
    at /tmp/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/svc.h:540
#1  0x00007f58fb50b0ad in alloc_nfs_request (xprt=0x7f58a8824100, xdrs=0x7f58a4c58180)
    at /tmp/NFS-Ganesha-3.2/src/MainNFSD/nfs_rpc_dispatcher_thread.c:1444
#2  0x00007f58fb29828d in svc_request (xprt=0x7f58a8824100, xdrs=0x7f58a4c58180) at /tmp/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1284
#3  0x00007f58fb29c495 in svc_vc_recv (xprt=0x7f58a8824100) at /tmp/NFS-Ganesha-3.2/src/libntirpc/src/svc_vc.c:896
#4  0x00007f58fb298237 in svc_rqst_xprt_task_recv (wpe=0x7f58a8824338) at /tmp/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1272
#5  0x00007f58fb298e08 in svc_rqst_epoll_loop (wpe=0x7f58eab70318) at /tmp/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1659
#6  0x00007f58fb2a53e8 in work_pool_thread (arg=0x7f58a800e480) at /tmp/NFS-Ganesha-3.2/src/libntirpc/src/work_pool.c:184
#7  0x00007f58f9407ea5 in start_thread () from /lib64/libpthread.so.0
#8  0x00007f58efaa0b8d in clone () from /lib64/libc.so.6
```
另：
```bash
[root@node131 lmm]# cat nsm_break.gdb
set $p = ((struct cx_data *) nsm_clnt)->cx_rec
set logging off
set logging file 0109_outfile_ccc_svc_0
set logging on
p nsm_clnt
b svc_ref_it if xprt == $p
b svc_release_it if xprt == $p
b insert_nlm_xprtlist
```
# readelf 导出结构体大小
```
使用 readelf -wi   带debug信息的二进制文件 导出的信息里面会包含以下相关的字段  aaabbb代表对应的结构体名称   DW_AT_byte_size   : 96表示aaabbb结构体的大小为96   你们看下可不可以用这个方式导一下  
 <1><73>: Abbrev Number: 4 (DW_TAG_structure_type)
    <74>   DW_AT_name        : (indirect string, offset: 0x92): aaabbb
    <78>   DW_AT_byte_size   : 96
    <79>   DW_AT_decl_file   : 1
    <7a>   DW_AT_decl_line   : 2
    <7b>   DW_AT_sibling     : <0xa8>
```
# 获取线程号
```
(gdb) t f 971861
Thread 6 has target id 'Thread 0x7f3cfb692700 (LWP 971861)'
```
# 打印结构体定义
> ptype struct struct_name   
# [gdb magic](http://luajit.io/posts/gdb-black-magics/)
# 打印sockaddr 地址
```
(gdb) set $addr_buf = (char*)malloc(400)
(gdb) call (void)inet_ntop(10, (void *)&((struct sockaddr_in6 *)0x7fcb2ec8e010)->sin6_addr, $addr_buf, 400)
(gdb) p $addr_buf
$13 = 0x7fcbd7e4c6c0 "::ffff:10.121.29.127"

1. IPv4: struct sockaddr_in, 16个字节

 1 struct sockaddr_in {
 2      sa_family_t sin_family;             /* AF_INET */
 3      in_port_t sin_port;                 /* Port number.  */
 4      struct in_addr sin_addr;            /* Internet address.  */
 5 
 6      /* Pad to size of `struct sockaddr'.  */
 7      unsigned char sin_zero[sizeof (struct sockaddr) -
 8                             sizeof (sa_family_t) -
 9                             sizeof (in_port_t) -
10                             sizeof (struct in_addr)];
11 };
12 typedef uint32_t in_addr_t;
13 struct in_addr  {
14     in_addr_t s_addr;                    /* IPv4 address */
15 };

2. IPv6: struct sockaddr_in6, 28个字节

 1 struct sockaddr_in6 {
 2     sa_family_t sin6_family;    /* AF_INET6 */
 3     in_port_t sin6_port;        /* Transport layer port # */
 4     uint32_t sin6_flowinfo;     /* IPv6 flow information */
 5     struct in6_addr sin6_addr;  /* IPv6 address */
 6     uint32_t sin6_scope_id;     /* IPv6 scope-id */
 7 };
 8 struct in6_addr {
 9     union {
10         uint8_t u6_addr8[16];
11         uint16_t u6_addr16[8];
12         uint32_t u6_addr32[4];
13     } in6_u;
14 
15     #define s6_addr                 in6_u.u6_addr8
16     #define s6_addr16               in6_u.u6_addr16
17     #define s6_addr32               in6_u.u6_addr32
18 };

3. 通用结构体1: struct sockaddr, 16个字节

1 struct sockaddr { 
2      sa_family_t sa_family;       /* Address family */
3      char sa_data[14];            /* protocol-specific address */
4 };
4. 通用结构体2: struct sockaddr_storage,128个字节

 1 /* Structure large enough to hold any socket address 
 2 (with the historical exception of AF_UNIX). 128 bytes reserved.  */
 3 
 4 #if ULONG_MAX > 0xffffffff
 5 # define __ss_aligntype __uint64_t
 6 #else
 7 # define __ss_aligntype __uint32_t
 8 #endif
 9 #define _SS_SIZE        128
10 #define _SS_PADSIZE     (_SS_SIZE - (2 * sizeof (__ss_aligntype)))
11 
12 struct sockaddr_storage
13 {
14     sa_family_t ss_family;      /* Address family */
15     __ss_aligntype __ss_align;  /* Force desired alignment.  */
16     char __ss_padding[_SS_PADSIZE];
17 };

struct sockaddr_storage addr;
memset(&addr, 0, sizeof(struct sockaddr_storage));
if (isIPv6 == TRUE)
{
    struct sockaddr_in6 *addr_v6 = (struct sockaddr_in6 *)&addr;
    addr_v6->sin6_family = AF_INET6;
    addr_v6->sin6_port = 1234;
    inet_pton(AF_INET6, “2001:3211::1”, &(addr_v6->sin6_addr));
}
else
{
    struct sockaddr_in *addr_v4 = (struct sockaddr_in *)&addr;
    addr_v4->sin_family = AF_INET;
    addr_v4->sin_port = 1234;
    inet_aton(“192.168.1.228”, &(addr_v4->sin_addr));
}

sendto(sock, buf, len, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_storage));
```
