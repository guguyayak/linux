# Linux 下如何产生core文件（core dump设置）

1、先用#ulimit -a可以查看系统core文件的大小限制（第一行），core文件大小设置为0, 即没有打开core dump设置；

2、接下来使用#ulimit -c [kbytes]可以设置系统允许生成的core文件大小；

ulimit -c 0 不产生core文件

ulimit -c 100 设置core文件最大为100k

ulimit -c unlimited 不限制core文件大小

执行#ulimit -c unlimited，然后#ulimit -a查看结果如下（第一行）

永久生效办法：

       #vi /etc/profile 然后，在profile中添加：

ulimit -c 1073741824

  (但是，若将产生的转储文件大小大于该数字时，将不会产生转储文件)

或者

　　ulimit -c unlimited

这样重启机器后生效了。 或者， 使用source命令使之马上生效。

 　　#source /etc/profile

三、指定内核转储的文件名和目录

        修改完内核转储设置后，当程序core dump后发现确实在本地目录产生了core文件，但是如果程序多次core dump时，core文件会被覆盖，原因是每次core dump后生成的文件名默认都叫core，接下来就分享下如果想在每次core dum时产生的core文件都带上进程号怎么操作，或者你想把内核转储文件保存到其他目录怎么办？

1、core dump文件名自动加上进程ID

    #echo 1 > /proc/sys/kernel/core_uses_pid

最后生成的core dump文件名会加上进程ID.

2、另外可以通过修改kernel的参数，指定内核转储所生成的core文件的路径和文件名。

       可以通过在/etc/sysctl.conf文件中，对sysctl变量kernel.core_pattern的设置。

       #vim /etc/sysctl.conf 然后，在sysctl.conf文件中添加下面两句话：

               kernel.core_pattern = /var/core/core_%e_%p

               kernel.core_uses_pid = 0

       保存后退出。

 注：如果/proc/sys/kernel/core_uses_pid 这个文件的内容被配置成1，即使core_pattern中没有设置%p，最后生成的core dump文件名仍会加上进程ID。

这里%e, %p分别表示：

%c 转储文件的大小上限

%e 所dump的文件名

%g 所dump的进程的实际组ID

%h 主机名

%p 所dump的进程PID

%s 导致本次coredump的信号

%t 转储时刻(由1970年1月1日起计的秒数)

%u 所dump进程的实际用户ID

可以使用以下命令，使修改结果马上生效。

#sysctl –p /etc/sysctl.conf

请在/var目录下先建立core文件夹，然后执行a.out程序，就会在/var/core/下产生以指定格式命名的内核转储文件。

# 让系统主动宕机
> echo  c  > /proc/sysrq-trigger

# core文件分析

分析vmcore文件需要安装对应版本内核的kernel-debuginfo-common和kernel-debuginfo两个RPM包

安装之后生成debug内核文件/usr/lib/debug/lib/modules/<kernel>/vmlinux

crash /usr/lib/debug/lib/modules/<kernel>/vmlinux vmcore

```c
  struct 命令用于查看数据结构的定义原型。命令截图如下：

crash> struct -o vm_struct

struct vm_struct {

  [0] struct vm_struct *next;

  [8] void *addr;

  [16] long unsigned int size;

  [24] long unsigned int flags;

  [32] struct page **pages;

  [40] unsigned int nr_pages;

  [48] phys_addr_t phys_addr;

  [56] void *caller;

}

SIZE: 64

struct 命令用于查看数据结构的具体信息（需要获取到该结构体的地址）

struct dentry ffff97584a3d9680（地址值）

反汇编函数：dis -l nfsd_create_locked

函数调用参数传递：在x86系统上，ebx, ecx, edx, esi和edi按照顺序存放前五个参数。
```
# 堆栈分析
 1、IP: [] ceph_mdsc_handle_mdsmap+0x96/0x630 [ceph]-----------------------------0x96是一个偏移量，从函数起始地址开始+150

2、找到对应版本的.o文件
```c
gdb ceph.o

disassemble ceph_mdsc_handle_mdsmap

(gdb) disassemble ceph_mdsc_handle_mdsmap

Dump of assembler code for functionceph_mdsc_handle_mdsmap:

…

0x0000000000006d8f<+127>:     pop    %r15

0x0000000000006d91<+129>:    pop    %rbp

0x0000000000006d92<+130>:    retq

0x0000000000006d93<+131>:    mov    (%rax),%rsi

0x0000000000006d96<+134>:    mov    %rdi,%rbx

0x0000000000006d99<+137>:    add    $0x10,%rax

0x0000000000006d9d<+141>:    mov    -0x8(%rax),%rdi

0x0000000000006da1<+145>:    mov    %rax,0x58(%rsp)

0x0000000000006da6<+150>:    mov    (%rbx),%rax



(gdb) info line *0x0000000000006da6

Line 3815 of"/project/V100R002B01D007SP02/kernel4.4.0/fs/ceph/mds_client.c"

starts at address 0x6da6 and ends at 0x6da9.
```
