# 内核版本
> /usr/include/linux/version.h  
> #define LINUX_VERSION_CODE 263213  
> #define KERNEL_VERSION(a,b,c) (((a)<<16)+((b)<<8)+(c))  
# 原子变量读取
> kref_read(&freeme->ref)  

# get_fs set_fs
> get_ds获得kernel的内存访问地址范围（IA32是4GB）  
> set_fs是设置当前的地址访问限制值  
> get_fs是取得当前的地址访问限制值。  
> 进程由用户态进入核态，linux进程的task_struct结构中的成员addr_limit也应该由0xBFFFFFFF变为0xFFFFFFFF(addr_limit规定了进程有用户态核内核态情况下的虚拟地址空间访问范围，在用户态，addr_limit成员值是0xBFFFFFFF也就是有3GB的虚拟内存空间，在核心态，是0xFFFFFFFF,范围扩展了1GB)。  
> 使用这三个函数是为了安全性。为了保证用户态的地址所指向空间有效，函数会做一些检查工作。
如果set_fs(KERNEL_DS),函数将跳过这些检查。  
```c
mm_segment_t fs;
fs = get_fs();
set_fs(KERNEL_DS);
...
set_fs(fs);

int kernel_sendmsg(struct socket *sock, struct msghdr *msg,
		   struct kvec *vec, size_t num, size_t size)
{
	mm_segment_t oldfs = get_fs();
	int result;

	set_fs(KERNEL_DS);
	/*
	 * the following is safe, since for compiler definitions of kvec and
	 * iovec are identical, yielding the same in-core layout and alignment
	 */
	msg->msg_iov = (struct iovec *)vec;
	msg->msg_iovlen = num;
	result = sock_sendmsg(sock, msg, size);
	set_fs(oldfs);
	return result;
}
EXPORT_SYMBOL(kernel_sendmsg);
```
# 内核open函数
```c
/**
 * filp_open - open file and return file pointer
 *
 * @filename:	path to open
 * @flags:	open flags as per the open(2) second argument
 * @mode:	mode for the new file if O_CREAT is set, else ignored
 *
 * This is the helper to open a file from kernelspace if you really
 * have to.  But in generally you should not do this, so please move
 * along, nothing to see here..
 */
struct file *filp_open(const char *filename, int flags, umode_t mode)
{
	struct filename *name = getname_kernel(filename);
	struct file *file = ERR_CAST(name);
	
	if (!IS_ERR(name)) {
		file = file_open_name(name, flags, mode);
		putname(name);
	}
	return file;
}
EXPORT_SYMBOL(filp_open);
```
# snprintf
```c
static int rpc_ntop4(const struct sockaddr *sap,
                     char *buf, const size_t buflen)
{
        const struct sockaddr_in *sin = (struct sockaddr_in *)sap;

        return snprintf(buf, buflen, "%pI4", &sin->sin_addr);
}
```
# kallsyms_lookup_name
```
kallsyms_lookup_name本身也是一个内核符号，如果这个符号被导出了，
那么就可以在内核模块中调用kallsyms_lookup_name("do_page_fault")来获得do_page_fault的符号地址。

这种方法的局限性在于kallsyms_lookup_name本身不一定被导出。
```
