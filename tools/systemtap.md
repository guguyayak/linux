# 环境配置
1. 下载3个依赖的包，这里包的版本需要与CentOS内核的版本一致  
> 下载kernel-devel-[版本号]、kernel-debuginfo-[版本号]、kernel-debuginfo-common-[版本号] 
 
注：直接使用yum下载上述3个包，默认使用最新的安装包，所以需要手动从网上下载与CentOS内核版本一致的安装包  
2.使用yum安装systemtap systemtap-runtime  
> yum -y install systemtap systemtap-runtime  

# 文档：/usr/share/systemtap/

# [官方实例](https://sourceware.org/systemtap/examples/)

> general/callgraph.stp - Callgraph Tracing  
keywords: SIMPLE TRACE CALLGRAPH  
Print a timed per-thread microsecond-timed nested callgraph. The first parameter names the function probe points to trace.  
sample usage in general/callgraph.txt  
[root@node1 systemtap]# stap callgraph.stp 'module("nfsd").function("nfsd3_proc_write")'  
  0 nfsd(6098):->nfsd3_proc_write  
  356 nfsd(6098):<-nfsd3_proc_write  

# 运行脚本参数
> $1 ... $<NN>把参数解释成数字  
  @1 ... @<NN>把参数解释成字符串  
       In addition, script arguments given at the end of the command line may be inserted.  Use $1 ... $<NN> for insertion unquoted, @1 ...
       @<NN>  for  insertion as a string literal.  The number of arguments may be accessed through $# (as an unquoted number) or through @#
       (as a quoted number).  These may be used at any place a token may begin, including within the preprocessing stage.  Reference to  an
       argument number beyond what was actually given is an error.

# @cast()解释指针内容

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
