# 环境配置
1. 下载3个依赖的包，这里包的版本需要与CentOS内核的版本一致  
> 下载kernel-devel-[版本号]、kernel-debuginfo-[版本号]、kernel-debuginfo-common-[版本号] 
2. 查看vmlinux版本
> [root@c76n81p85 lmm]# strings /usr/lib/debug/lib/modules/3.10.0-957.el7.x86_64/vmlinux| grep 'Linux version'   
> Linux version 3.10.0-957.el7.x86_64 (mockbuild@kbuilder.bsys.centos.org) (gcc version 4.8.5 20150623 (Red Hat 4.8.5-36) (GCC) ) #1 SMP Thu Nov 8 23:39:32 UTC 2018   

注：直接使用yum下载上述3个包，默认使用最新的安装包，所以需要手动从网上下载与CentOS内核版本一致的安装包  
2.使用yum安装systemtap systemtap-runtime  
> yum -y install systemtap systemtap-runtime  
## 基本测试
> stap -e 'probe begin{printf("Hello, World\n"); exit();}'  

# 文档：/usr/share/systemtap/

# [文档介绍](https://systemtap.bruceding.me/variable.html)

# [官方实例](https://sourceware.org/systemtap/examples/)

> general/callgraph.stp - Callgraph Tracing  
keywords: SIMPLE TRACE CALLGRAPH  
Print a timed per-thread microsecond-timed nested callgraph. The first parameter names the function probe points to trace.  
sample usage in general/callgraph.txt  
[root@node1 systemtap]# stap callgraph.stp 'module("nfsd").function("nfsd3_proc_write")'  
  0 nfsd(6098):->nfsd3_proc_write  
  356 nfsd(6098):<-nfsd3_proc_write  
## sk_stream_wait_memory.stp
```c
#! /usr/bin/env stap
# Simple probe to detect when a process is waiting for more socket send
# buffer memory. Usually means the process is doing writes larger than the
# socket send buffer size or there is a slow receiver at the other side.
# Increasing the socket's send buffer size might help decrease application
# latencies, but it might also make it worse, so buyer beware.
#
# Typical output: timestamp in microseconds: procname(pid) event
#
# 1218230114875167: python(17631) blocked on full send buffer
# 1218230114876196: python(17631) recovered from full send buffer
# 1218230114876271: python(17631) blocked on full send buffer
# 1218230114876479: python(17631) recovered from full send buffer

probe kernel.function("sk_stream_wait_memory")
{
	printf("%u: %s(%d) blocked on full send buffer\n",
		gettimeofday_us(), execname(), pid())
}

probe kernel.function("sk_stream_wait_memory").return
{
	printf("%u: %s(%d) recovered from full send buffer\n",
		gettimeofday_us(), execname(), pid())
}
```
## /usr/share/systemtap/examples/general/para-callgraph.stp
```
#!/usr/bin/stap

function trace(entry_p, extra) {
  %( $# > 1 %? if (tid() in trace) %)
  printf("%s%s%s %s\n",
         thread_indent (entry_p),
         (entry_p>0?"->":"<-"),
         ppfunc (),
         extra)
}


%( $# > 1 %?
global trace
probe $2.call {
  trace[tid()] = 1
}
probe $2.return {
  delete trace[tid()]
}
%)

probe $1.call   { trace(1, $$parms) }
probe $1.return { trace(-1, $$return) }

[使用示例](https://sourceware.org/systemtap/examples/general/para-callgraph.txt)：
$ stap para-callgraph.stp 'process("/bin/ls").function("*")' \
'process("/bin/ls").function("main")' -c "/bin/ls > /dev/null"
```
# 脚本执行时长设定
> probe timer.s(60) { exit(); }   
> -T TIME    terminate the script after TIME seconds   
# 正则匹配
```
name = kernel_string($pathname->name)
if (name =~ ".*vdb_f.*")
        start4[pid()] = gettimeofday_us()
```
# 运行脚本参数
> $1 ... $<NN>把参数解释成数字  
  @1 ... @<NN>把参数解释成字符串  
       In addition, script arguments given at the end of the command line may be inserted.  Use $1 ... $<NN> for insertion unquoted, @1 ...
       @<NN>  for  insertion as a string literal.  The number of arguments may be accessed through $# (as an unquoted number) or through @#
       (as a quoted number).  These may be used at any place a token may begin, including within the preprocessing stage.  Reference to  an
       argument number beyond what was actually given is an error.

# @cast()解释指针内容
```
TYPECASTING
Once  a pointer (see the CONTEXT VARIABLES section of stapprobes(3stap)) has been saved into a script integer variable, the transla‐
tor loses the type information necessary to access members from that pointer.  Using the @cast() operator tells the  translator  how
to interpret the number as a typed pointer.

       @cast(p, "type_name"[, "module"])->member

This  will  interpret p as a pointer to a struct/union named type_name and dereference the member value.  Further ->subfield expres‐
sions may be appended to dereference more levels. Note that for direct dereferencing of a  pointer  {kernel,user}_{char,int,...}($p)
should  be  used.  (Refer to stapfuncs(5) for more details.)   NOTE: the same dereferencing operator -> is used to refer to both di‐
rect containment or pointer indirection.  Systemtap automatically determines which.  The optional module tells the translator  where
to look for information about that type.  Multiple modules may be specified as a list with : separators.  If the module is not spec‐
ified, it will default either to the probe module for dwarf probes, or to "kernel" for functions and all other probes types.

The translator can create its own module with type information from a header surrounded by angle brackets, in case normal  debuginfo
is  not  available.   For  kernel headers, prefix it with "kernel" to use the appropriate build system.  All other headers are built
with default GCC parameters into a user module.  Multiple headers may be specified in sequence to resolve a codependency.

       @cast(tv, "timeval", "<sys/time.h>")->tv_sec
       @cast(task, "task_struct", "kernel<linux/sched.h>")->tgid
       @cast(task, "task_struct",
             "kernel<linux/sched.h><linux/fs_struct.h>")->fs->umask
stap -d kernel -ve 'probe module("fuse").function("fuse_perform_write") {printf("big_writes %d\n", @cast($mapping->host->i_sb->s_fs_info, "fuse_conn")->big_writes);}'
s_fs_info 在 i_sb 中是 void 类型指针。
printf("xprt sc_cm_id: %p\n", @cast($xprt, "svcxprt_rdma", "kernel<linux/sunrpc/svc_rdma.h>")->sc_cm_id)
```
# @defined 判断成员是否存在
```c
  # cat inode-watch.stp
probe kernel.function ("vfs_write"),
      kernel.function ("vfs_read")
{
  if (@defined($file->f_path->dentry)) {
    dev_nr = $file->f_path->dentry->d_inode->i_sb->s_dev
    inode_nr = $file->f_path->dentry->d_inode->i_ino
  } else {
    dev_nr = $file->f_dentry->d_inode->i_sb->s_dev
    inode_nr = $file->f_dentry->d_inode->i_ino
  }
  if (dev_nr == ($1 << 20 | $2) # major/minor device
      && inode_nr == $3)
    printf ("%s(%d) %s 0x%x/%u\n",
      execname(), pid(), ppfunc(), dev_nr, inode_nr)
}
```
# 函数
```shell
tid(),      当前线程id
pid(),      当前进程id
uid(),      当前用户id
execname(),    当前进程名称
cpu(),         当前cpu编号
gettimeofday_s(),      秒时间戳
get_cycles(),     硬件周期计数器快照
pp(),             探测点事件名称
ppfunc(),         探测点触发的函数名称
$$var, 上下文中存在 $var， 可以使用该变量
print_backtrace(),         打印内核栈
print_ubacktrace(),        打印用户空间栈
thread_indent()，打印N个空格，常用于打印多级函数时的缩进(配合target()选择目标进程)
```
# 变量
```shell
$#是什么意思？（表示的是命令行中给出的参数的个数）

如果$#>1, 紧接着判断if(tid() in trace)

%( CONDITION %? TRUE-TOKENS %)

%( CONDITION %? TRUE-TOKENS %: FALSE-TOKENS %)                          -> condition    ?   true  : false           %：预编命令

stap -e '%( CONFIG_UTRACE == "y" %? probe begin {printf("true\n"); exit();} %: probe begin {printf("false\n"); exit();} %)'

获取指定地址的字节：kernel_char(address)

kernel_short(address)

kernel_int(address)

kernel_long(address)

kernel_string(address)

kernel_string_n(address,n)

为了方便获取参数，systemtap提供了一个方法
直接使用$$var就可以打印函数参数和变量。
如果使用$$locals,就是打印局部变量
$$parms就只打印函数阐述
$$return 打印返回值，只在返回探针能用。

打印入参是int类型指针值：kernel_long($ppos)
probe module("ceph").function("ceph_generic_file_splice_read") {
        printf("inode:%p ino %ld pos %ld isize %ld\n",
                $in->f_mapping->host, $in->f_mapping->host->i_ino, kernel_long($ppos), $in->f_mapping->host->i_size);
}
```
# 执行文件
```c
[root@c75n50p9 lmm]# cat nfs-setfattr.stp
probe begin
{
        time_s = gettimeofday_s();
        datetime_str = tz_ctime(time_s);
        print(datetime_str."\n");
}

probe kernel.function("setxattr")
{
        printf("file %s\n", user_string($name))
}

probe kernel.function("vfs_setxattr").call
{
        printf("vfs_setxattr name=%s\n", kernel_string($name));
        printf("setxattr: %s, getattr: %s\n", symname($dentry->d_inode->i_op->setxattr), symname($dentry->d_inode->i_op->getattr))
}

probe module("nfsv3").function("nfs3_setxattr").call
{
                printf("nfs3_setxattr dentry %p, name=%s, flags = %d\n", $dentry, kernel_string($name), $flags);
}

probe timer.s(1)
{
        time_s = gettimeofday_s();
        datetime_str = tz_ctime(time_s);
        print(datetime_str."\n");
}
```
# 输出
> file user.user1  
vfs_setxattr name=user.user1  
setxattr: nfs3_setxattr, getattr: 0xffffffffc0bacbc0  
nfs3_setxattr dentry 0xffffa041df21a540, name=user.user1, flags = 0  
# 函数接口
```c
int nfs3_setxattr(struct dentry *dentry, const char *name,
	     const void *value, size_t size, int flags);
vfs_setxattr(struct dentry *dentry, const char *name, const void *value,
		size_t size, int flags);
static long
setxattr(struct dentry *d, const char __user *name, const void __user *value,
	 size_t size, int flags);
```

# 还可以在函数末尾加上“.call”或“.return”，分别表示函数被调用和返回时probe：
# 在调用build_open_flags时probe，handle是：打印rbp寄存器的值
```c
probe kernel.function("build_open_flags").call {

        printf("rbp=%p\n", register("rbp"));

}
```

# 统计函数调用次数及平均时延

[源代码](https://sourceware.org/systemtap/examples/general/func_time_stats.stp)

```c
[root@xjlm_sg001_oStor1 tmp]# cat func_time_stats.stp
#! /usr/bin/env stap
/*
 * func_time_stats.stp
 * usage: func_time_stats.stp function_probe
 */
global start, intervals, start2, intervals2, start3, intervals3

probe $1 { start[pid()] = gettimeofday_us() }
probe $1.return
{
  t = gettimeofday_us()
  old_t = start[pid()]
  if (old_t) intervals <<< t - old_t
  delete start[pid()]
}

probe $2 { start2[pid()] = gettimeofday_us() }
probe $2.return
{
  t2 = gettimeofday_us()
  old_t2 = start2[pid()]
  if (old_t2) intervals2 <<< t2 - old_t2
  delete start2[pid()]
}

probe $3 { start3[pid()] = gettimeofday_us() }
probe $3.return
{
  t3 = gettimeofday_us()
  old_t3 = start3[pid()]
  if (old_t3) intervals3 <<< t3 - old_t3
  delete start3[pid()]
}

probe end
{
  printf("intervals min:%dus avg:%dus max:%dus count:%d variance:%d sum:%d\n",
         @min(intervals), @avg(intervals), @max(intervals),
         @count(intervals), @variance(intervals, 3), @sum(intervals))
  print(@hist_log(intervals));

  printf("intervals2 min:%dus avg:%dus max:%dus count:%d variance:%d sum:%d\n",
          @min(intervals2), @avg(intervals2), @max(intervals2),
          @count(intervals2), @variance(intervals2, 3), @sum(intervals2))
  print(@hist_log(intervals2));

  printf("intervals3 min:%dus avg:%dus max:%dus count:%d variance:%d sum:%d\n",
          @min(intervals3), @avg(intervals3), @max(intervals3),
          @count(intervals3), @variance(intervals3, 3), @sum(intervals3))
  print(@hist_log(intervals3));
}
```
## 使用举例
> stap -v func_time_stats.stp 'module("nfsd").function("nfsd_write")' 'module("nfsd").function("nfsd3_proc_commit")' 'module("nfsd").function("nfsd_open")'

# 打印结构体
```c
struct file_ra_state {
	pgoff_t start;			/* where readahead started */
	unsigned int size;		/* # of readahead pages */
	unsigned int async_size;	/* do asynchronous readahead when
					   there are only # of pages ahead */

	unsigned int ra_pages;		/* Maximum readahead window */
	unsigned int mmap_miss;		/* Cache miss stat for mmap accesses */
	loff_t prev_pos;		/* Cache last read() position */
};

probe kernel.function ("ondemand_readahead").call
{
	if (execname() == "dd")
		printf ("%s ra$: %s\n", ppfunc(), $ra$); // 多层打印： $ra$$
}
```
# 加载指定ko
> stap -v -d /home/parastor/tools/client/knal.ko lcli.stp   
# 一个用过的脚本
```c
probe begin
{
        time_s = gettimeofday_s();
        datetime_str = tz_ctime(time_s);
        print(datetime_str."\n");
}

probe module("exportfs").function("exportfs_decode_fh").call
{
        printf("exportfs_decode_fh fid: %p, ino = %ld\n", $fid, $fid->i32->ino)
        printf("--------------------------------------------------------\n");
}

probe module("exportfs").function("exportfs_decode_fh").return
{
        printf("exportfs_decode_fh return: %p\n", $return)
        if ($return)
                printf("exportfs_decode_fh ret: %p, flag: %x, inode: %p, ino = %d\n",
                        $return, $return->d_flags, $return->d_inode, $return->d_inode->i_ino)
        printf("--------------------------------------------------------\n");
}

probe module("nfsd").function("fh_verify").call
{
        printf("fh_verify fhp: %p\n", $fhp)
        printf("------------------------------call--------------------------\n");
}

probe module("nfsd").function("nfsd_unlink").call
{
        printf("nfsd_unlink fname: %s\n", kernel_string($fname))
        printf("------------------------------call--------------------------\n");
}

probe kernel.function("d_alloc_pseudo")
{
        printf("d_alloc_pseudo sb: %p\n", $sb)
        printf("--------------------------------------------------------\n");
        print_backtrace();
        printf("--------------------------------------------------------\n");
}

probe kernel.function("d_obtain_alias").call
{
        printf("d_obtain_alias inode: %p, ino = %ld\n", $inode, $inode->i_ino)
        printf("--------------------------------------------------------\n");
        print_backtrace();
        printf("--------------------------------------------------------\n");
}

probe kernel.function("d_obtain_alias").return
{
        printf("d_obtain_alias return: %p\n", $return)
        if ($return)
                printf("d_obtain_alias ret: %p, flag: %x, dname = %s\n",
                        $return, $return->d_flags, kernel_string($return->d_name->name))
        printf("--------------------------------------------------------\n");
}

probe kernel.function("vfs_unlink").call
{
        if ($dentry)
        printf("vfs_unlink dentry %p, d_count =%d, flags = %d, d_name = %s\n",
                $dentry, $dentry->d_count->counter, $dentry->d_flags, kernel_string($dentry->d_name->name))
}

probe kernel.function("d_delete").call
{
        if ($dentry)
        printf("d_delete dentry %p, d_count =%d, flags = %d\n",
                $dentry, $dentry->d_count->counter, $dentry->d_flags)
}

probe kernel.function("dput").call
{
        if ($dentry)
        printf("d_put dentry %p, d_count =%d, flags = %d\n",
                $dentry, $dentry->d_count->counter, $dentry->d_flags)
}

probe kernel.function("dentry_iput").call
{
        if ($dentry)
        printf("dentry_iput dentry %p, d_count =%d, flags = %d\n",
                $dentry, $dentry->d_count->counter, $dentry->d_flags)
}

probe kernel.function("vfs_getattr").call
{
        if ($dentry)
        printf("vfs_getattr dentry %p, d_count =%d, flags = %d, dname = %s\n",
                $dentry, $dentry->d_count->counter, $dentry->d_flags, kernel_string($dentry->d_name->name))
}

probe timer.s(1)
{
        time_s = gettimeofday_s();
        datetime_str = tz_ctime(time_s);
        print(datetime_str."\n");
}
```
# 跟踪某个文件下的函数
> 跟踪一个函数：stap -ve 'probe kernel.function("remove_proc_entry@proc/generic.c") {printf("%s\n", kernel_string($name))}'   
> 跟踪所有函数：stap -ve 'probe kernel.function("*@proc/generic.c") {printf("%s\n", kernel_string($name))}'   
## 用户态程序跟踪
> stap callgraph.stp 'process("/bin/ls").function("*")'   
# 修改函数返回值
> stap -vge 'probe module("/home/parastor/tools/client/parastor.ko").function("dataio_read_sort").return {print_backtrace(); $return=-20410011;}'   
> 改变返回值，需要加 -g 参数   
# 跟踪nfsd读写时延
```sh
probebegin
{
        time_s = gettimeofday_s();
        datetime_str = tz_ctime(time_s);
        print(datetime_str."\n");
}

probemodule("nfsd").function("nfsd_read").return
{
        printf("read ino=%ld, pages=%ld,latency=%ldms\n", @cast(@entry($fhp), "structsvc_fh")->fh_dentry->d_inode->i_ino, @entry($vlen),gettimeofday_ms()-@entry(gettimeofday_ms()));
}

probemodule("nfsd").function("nfsd_write").return
{
        printf("write ino=%ld, pages=%ld,latency=%ldms\n", @cast(@entry($fhp), "structsvc_fh")->fh_dentry->d_inode->i_ino, @entry($vlen),gettimeofday_ms()-@entry(gettimeofday_ms()));
}

probetimer.s(1)
{
        time_s = gettimeofday_s();
        datetime_str = tz_ctime(time_s);
        print(datetime_str."\n");
}
```
# 列出函数入参
```
stap -L 'module("nfsd").function("nfsd_write")'
```
