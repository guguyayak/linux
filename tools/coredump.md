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
[get_kernel_core_dump](https://github.com/huataihuang/cloud-atlas-draft/blob/master/os/linux/kernel/tracing/get_kernel_core_dump.md)
> 保证kdump服务在正常运行：systemctl status kdump   
> echo 1 > /proc/sys/kernel/sysrq #打开sysrq   
> echo  c  > /proc/sysrq-trigger   
> 可能因为core dump内存不够而无法生成vmcore文件   
```
# 查看core dump内存
[root@node2 ~]# kdumpctl showmem
Reserved 1024MB memory for crash kernel
# 改变core dump内存
/etc/default/grub文件中
GRUB_CMDLINE_LINUX="crashkernel=auto 改为 crashkernel=1G
/etc/grub2-efi.cfg 文件中：
linuxefi /vmlinuz-4.14.0-49.hl28.x86_64 root=UUID=3ae788ab-5e25-4e75-ada7-a1f4d8f2ccb5 ro crashkernel=auto rd.md.uuid=a838a0ba:7b610187:b37b664a:03d8edc0 rd.md.uuid=aadd136e:cf397b79:e42a7072:5be2d944 rhgb quiet LANG=en_US.UTF-8
crashkernel=auto 改为 crashkernel=1G

重启主机生效
```
# 基础指令
```
打印所有栈：
foreach bt
```
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
# 一个crash分析实例
```c
[root@c65n50p66 ws]# crash ../vmlinux 

crash 6.1.0-5.el6
Copyright (C) 2002-2012  Red Hat, Inc.
Copyright (C) 2004, 2005, 2006, 2010  IBM Corporation
Copyright (C) 1999-2006  Hewlett-Packard Co
Copyright (C) 2005, 2006, 2011, 2012  Fujitsu Limited
Copyright (C) 2006, 2007  VA Linux Systems Japan K.K.
Copyright (C) 2005, 2011  NEC Corporation
Copyright (C) 1999, 2002, 2007  Silicon Graphics, Inc.
Copyright (C) 1999, 2000, 2001, 2002  Mission Critical Linux, Inc.
This program is free software, covered by the GNU General Public License,
and you are welcome to change it and/or distribute copies of it under
certain conditions.  Enter "help copying" to see the conditions.
This program has absolutely no warranty.  Enter "help warranty" for details.
 
GNU gdb (GDB) 7.3.1
Copyright (C) 2011 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-unknown-linux-gnu"...

      KERNEL: ../vmlinux
    DUMPFILE: /dev/crash
        CPUS: 32
        DATE: Mon Jun 21 16:00:38 2021
      UPTIME: 2 days, 23:52:46
LOAD AVERAGE: 0.08, 0.05, 0.00
       TASKS: 1093
    NODENAME: c65n50p66
     RELEASE: 2.6.32-431.el6.x86_64
     VERSION: #1 SMP Fri Nov 22 03:15:09 UTC 2013
     MACHINE: x86_64  (1796 Mhz)
      MEMORY: 63.9 GB
         PID: 3281
     COMMAND: "crash"
        TASK: ffff880455e2e040  [THREAD_INFO: ffff880455ec2000]
         CPU: 8
       STATE: TASK_RUNNING (ACTIVE)

crash> mod -S .
     MODULE       NAME                     SIZE  OBJECT FILE
ffffffffa137a800  kfnccli                211132  ./kfnccli.ko 
mod: cannot find or load object file for aesni_intel module
ffffffffa15fdde0  knal_without_rdma    12339633  ./knal_without_rdma.ko 
ffffffffa1fe5860  dlm                    371966  ./dlm.ko 
ffffffffa2284b80  parastor              2982706  ./parastor.ko 
ffffffffa23232a0  rctld                  192012  ./rctld.ko 
crash>           
crash> 
crash> kmem -s ffff8805f269ec80
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff881072da29c0 parastor_fsc_sb          384          2        10      1     4k
SLAB              MEMORY            TOTAL  ALLOCATED  FREE
ffff8805f269e000  ffff8805f269e080     10          2     8
FREE / [ALLOCATED]
  [ffff8805f269ec80]
crash> p (fsc_sb_t *)0xffff8805f269ec80
$3 = (fsc_sb_t *) 0xffff8805f269ec80

crash> p *$3 > fsb

crash> p &$3->s_inodes
$5 = (struct list_head *) 0xffff8805f269ec90

crash> list 0xffff8805f269ec90
ffff8805f269ec90
ffff880489c4cd70

crash> kmem -s ffff880489c4cd70
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff8810657d35c0 parastor_lcli_inode_t   1088          7        21      3     8k
SLAB              MEMORY            TOTAL  ALLOCATED  FREE
ffff880489c4c000  ffff880489c4c080      7          2     5
FREE / [ALLOCATED]
  [ffff880489c4cd40]

crash> p (fsc_inode_t *)0x ffff880489c4cd40
$6 = (fsc_inode_t *) 0xffff880489c4cd40
crash> p *$6 >finode
crash> p &$6->i_dentry
$8 = (struct list_head *) 0xffff880489c4cd80
crash> list 0xffff880489c4cd80
ffff880489c4cd80
ffff8805f2aaccb0
crash> p (fsc_dentry_t *)0x ffff8805f2aaccb0
$9 = (fsc_dentry_t *) 0xffff8805f2aaccb0
crash> p *$9> fdentry
crash> kmem -s ffff8805f2aaccb0
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff88106c9d2980 parastor_fsc_dentry      256          7        45      3     4k
SLAB              MEMORY            TOTAL  ALLOCATED  FREE
ffff8805f2aac000  ffff8805f2aac080     15          3    12
FREE / [ALLOCATED]
  [ffff8805f2aacc80]
crash> p (fsc_dentry_t *)0x ffff8805f2aacc80
$11 = (fsc_dentry_t *) 0xffff8805f2aacc80
crash> p *$11 > fdentry
crash> p (lcli_inode_t *)0xffff880489c4cd40
$13 = (lcli_inode_t *) 0xffff880489c4cd40
crash> p &$13->vinode
$14 = (struct inode *) 0xffff880489c4cef8
crash> p *$14 > vinode
crash> p &$14->i_dentry
$16 = (struct list_head *) 0xffff880489c4cf28
crash> list 0xffff880489c4cf28
ffff880489c4cf28
ffff88048d114570
crash> kmem -s ffff88048d114570
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff880877f40b40 dentry                   192      56969     59140   2957     4k
SLAB              MEMORY            TOTAL  ALLOCATED  FREE
ffff88048d114000  ffff88048d114080     20          7    13
FREE / [ALLOCATED]
  [ffff88048d114500]
crash> p (struct dentry *)0x ffff88048d114500
$17 = (struct dentry *) 0xffff88048d114500
crash> p *$17 >vdentry
crash> p /x 2147483676
$19 = 0x8000001c
crash> 
```
# mount 信息
```c
crash> mount
       mount       SUPERBLK        TYPE         DEVNAME     DIRNAME
       ffff9bxxxxxx ffffxxxxxxx   .......
       crash> p (struct mount *)0xffff9bxxxxxx
       $1=(struct mount *)ffff9bxxxxxx
       crash> p &$1->mnt
       $2 = (struct vfsmount *)0xffff9bxxxxxxx20
       
       vfsmount结构经常在struct path结构中，search后强转成struct path结构
       
       crash> search 0xffff9bxxxxxxx20
       ffffxxxxxxx38:0xffff9bxxxxxxx20
       crash> p (struct path *)0xffffxxxxxxx38
       $3 = (struct path *)0xffffxxxxxxx38
       p *$3
       $4 = {
          mnt = 0xffffxxxxxxxx20,
          dentry = 0xffffxxxxxc0
       }
       crash> files -d 0xffffxxxxxc0
       
       foreach files
       kmem -s | grep file
```
# 打印全局变量
> p [全局变量]   
# sym打印函数名
```sh
crash> struct notifier_block lockd_inetaddr_notifier
struct notifier_block {
  notifier_call = 0xffffffffc0820fe0 <create_lockd_listener+112>, 
  next = 0xffffffffc1f67bd0, 
  priority = 0
}
crash> struct notifier_block 0xffffffff8e9046e0^C
crash> sym 0xffffffffc0820fe0
ffffffffc0820fe0 (t) lockd_inetaddr_event [lockd] /usr/src/debug/kernel-3.10.0-862.el7/linux-3.10.0-862.el7.x86_64/fs/lockd/svc.c: 199
```
# vtop 查看虚拟地址和物理地址映射
```sh
crash> vtop ffffffffc1f67bd0
VIRTUAL           PHYSICAL        
ffffffffc1f67bd0  (not mapped)

PML4 DIRECTORY: ffffffff8e80e000
PAGE DIRECTORY: e0ba12067
   PUD: e0ba12ff8 => e0ba14067
   PMD: e0ba14078 => 40e106067
   PTE: 40e106b38 => 0

crash> vtop ffffffffc1f11bd0
VIRTUAL           PHYSICAL        
ffffffffc1f11bd0  4faf32bd0       

PML4 DIRECTORY: ffffffff8e80e000
PAGE DIRECTORY: e0ba12067
   PUD: e0ba12ff8 => e0ba14067
   PMD: e0ba14078 => 40e106067
   PTE: 40e106888 => 80000004faf32063
  PAGE: 4faf32000

      PTE         PHYSICAL   FLAGS
80000004faf32063  4faf32000  (PRESENT|RW|ACCESSED|DIRTY|NX)

      PAGE        PHYSICAL      MAPPING       INDEX CNT FLAGS
ffffd76693ebcc80 4faf32000                0        0  1 6fffff00000000
```
# 打印内存数据
> 在crash里执行x/1024x  0xffff97212eaf1000   
> 把后面4K的内存信息dump出来
# [oops分析教程](https://www.opensourceforu.com/2011/01/understanding-a-kernel-oops/)
# [Another way of reading page data](https://linuxcrash.readthedocs.io/en/latest/crash.html)
## 
```
Directly read the page
crash> kmem -p 0xffffea000d973f70
PAGE        PHYSICAL      MAPPING       INDEX CNT FLAGS
ffffea000d973f70 3e2122000 ffff88042731f1f8        0  1 400000000800d8
You need to find the virtual address of the page
crash> ptov 3e2122000
VIRTUAL           PHYSICAL
ffff8803e2122000  3e2122000
Read the data at the virtual address
crash> rd -8 ffff8803e2122000 100
ffff8803e2122000:  23 69 6e 63 6c 75 64 65 20 3c 66 63 6e 74 6c 2e   #include <fcntl.
ffff8803e2122010:  68 3e 0a 23 69 6e 63 6c 75 64 65 20 3c 73 74 64   h>.#include <std
ffff8803e2122020:  69 6f 2e 68 3e 0a 23 69 6e 63 6c 75 64 65 20 3c   io.h>.#include <
ffff8803e2122030:  75 6e 69 73 74 64 2e 68 3e 0a 23 69 6e 63 6c 75   unistd.h>.#inclu
ffff8803e2122040:  64 65 20 3c 73 79 73 2f 74 79 70 65 73 2e 68 3e   de <sys/types.h>
ffff8803e2122050:  0a 23 69 6e 63 6c 75 64 65 20 3c 73 79 73 2f 73   .#include <sys/s
ffff8803e2122060:  74 61 74 2e                                       tat.
crash> rd -8 ffff8803e2122000 1000
ffff8803e2122000:  23 69 6e 63 6c 75 64 65 20 3c 66 63 6e 74 6c 2e   #include <fcntl.
ffff8803e2122010:  68 3e 0a 23 69 6e 63 6c 75 64 65 20 3c 73 74 64   h>.#include <std
ffff8803e2122020:  69 6f 2e 68 3e 0a 23 69 6e 63 6c 75 64 65 20 3c   io.h>.#include <
ffff8803e2122030:  75 6e 69 73 74 64 2e 68 3e 0a 23 69 6e 63 6c 75   unistd.h>.#inclu
ffff8803e2122040:  64 65 20 3c 73 79 73 2f 74 79 70 65 73 2e 68 3e   de <sys/types.h>
ffff8803e2122050:  0a 23 69 6e 63 6c 75 64 65 20 3c 73 79 73 2f 73   .#include <sys/s
ffff8803e2122060:  74 61 74 2e 68 3e 0a 0a 69 6e 74 20 6d 61 69 6e   tat.h>..int main
ffff8803e2122070:  20 28 29 20 7b 0a 09 69 6e 74 20 66 64 31 2c 20    () {..int fd1,
ffff8803e2122080:  66 64 32 2c 20 66 64 33 2c 20 66 64 34 2c 20 66   fd2, fd3, fd4, f
ffff8803e2122090:  64 35 3b 0a 09 0a 09 66 64 31 3d 6f 70 65 6e 28   d5;....fd1=open(
ffff8803e21220a0:  22 31 22 2c 20 4f 5f 43 52 45 41 54 29 3b 0a 09   "1", O_CREAT);..
ffff8803e21220b0:  66 64 32 3d 6f 70 65 6e 28 22 32 22 2c 20 4f 5f   fd2=open("2", O_
ffff8803e21220c0:  43 52 45 41 54 29 3b 0a 09 66 64 33 3d 6f 70 65   CREAT);..fd3=ope
ffff8803e21220d0:  6e 28 22 33 22 2c 20 4f 5f 43 52 45 41 54 29 3b   n("3", O_CREAT);
ffff8803e21220e0:  0a 09 66 64 34 3d 6f 70 65 6e 28 22 34 22 2c 20   ..fd4=open("4",
ffff8803e21220f0:  4f 5f 43 52 45 41 54 29 3b 0a 09 66 64 35 3d 6f   O_CREAT);..fd5=o
ffff8803e2122100:  70 65 6e 28 22 35 22 2c 20 4f 5f 43 52 45 41 54   pen("5", O_CREAT
ffff8803e2122110:  29 3b 0a 09 77 68 69 6c 65 20 28 31 29 20 7b 0a   );..while (1) {.

//自己测试
日志：page fffffd27a024eec0 page_address (pages) = 0x ffff92b0493bb000 flags = 2fffff00000000

crash> kmem -p 0x fffffd27a024eec0
PAGE             PHYSICAL           MAPPING         INDEX CNT FLAGS
fffffd27a024eec0 8093bb000          0                 0    1  2fffff000000000

crash> ptov 8093bb000
VIRTUAL                  PHYSICAL
ffff92b0493bb000         8093bb000
```
# 把文件导入crash指令界面
```
crash> < h_gdb > hash_list
crash>

h_gdb 中是待执行的指令
[root@Anolis86n81p41 127.0.0.1-2023-06-08-03:12:30]# cat h_gdb
list nlm_file.f_list -s nlm_file 0xffff8b68e4ae1f00
list nlm_file.f_list -s nlm_file 0xffff8b6d81f71e00
list nlm_file.f_list -s nlm_file 0xffff8b6c7aa65600
```
# crash加载带debug信息的ko
```
[root@node114 lmm]# rpm -ql kernel-debuginfo|grep fuse
/usr/lib/debug/lib/modules/5.10.0-136.12.0.86.4.hl202.x86_64/kernel/fs/fuse/fuse.ko-5.10.0-136.12.0.86.4.hl202.x86_64.debug

mod -s fuse fuse.ko-5.10.0-136.12.0.86.4.hl202.x86_64.debug
```
# crash vmlinux显示版本不匹配(这个明确vmlinux和内核版本是匹配的)
```
crash /usr/lib/debug/usr/lib/modules/5.10.0-136.12.0.86.4.hl202.x86_64/vmlinux
This program has absolutely no warranty.  Enter "help warranty" for details.

WARNING: /usr/lib/debug/usr/lib/modules/5.10.0-136.12.0.86.4.hl202.x86_64/vmlinux
         and /proc/version do not match!

WARNING: /proc/version indicates kernel version: 5.10.0-136.12.0.86.4.hl202.x86_64

crash: please use the vmlinux file for that kernel version, or try using
       the System.map for that kernel version as an additional argument.

用vi打开那个vmlinux文件 搜索一下/proc/version那部分  里面会显示字符串相关信息的   把那个编译时间相关的字段改成和/proc/version的时间字段一样
示例：
[root@cnode106 127.0.0.1-2025-03-17-16:17:28]# cat /proc/version
Linux version 3.10.0-1160.71.1.el7.x86_64 (mockbuild@kbuilder.bsys.centos.org) (gcc version 4.8.5 20150623 (Red Hat 4.8.5-44) (GCC) ) #1 SMP Tue Jun 28 15:37:28 UTC 2022
搜索 "Linux version 3.10.0-1160.71.1.el7.x86_64" 找到对应字符串
```
# 锁分析锁持有者
> 读写锁如果是写者持有，只有一个持有者，可以根据锁信息分析出持有进程；
>  读写锁如果是读者持有，可能是多进程同时持有，无法分析持有锁的进程信息；
> 如果没有加额外的编译参数，spin_lock锁不会记录持有锁进程信息；
```c
//内核判断是否是本进程在持有锁的判断方法——遍历本进程持有的锁，和要判断的锁比较是否是一把锁
static int __lock_is_held(struct lockdep_map *lock, int read)
{
	struct task_struct *curr = current;
	int i;

	for (i = 0; i < curr->lockdep_depth; i++) {
		struct held_lock *hlock = curr->held_locks + i;

		if (match_held_lock(hlock, lock)) {
			if (read == -1 || hlock->read == read)
				return 1;

			return 0;
		}
	}

	return 0;
}
```
