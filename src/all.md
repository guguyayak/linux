# 堆栈打印
> dump_stack();
# 函数名
> strcmp(current->comm, "nfsd");
# 内核版本
> /usr/include/linux/version.h  
> #define LINUX_VERSION_CODE 263213  
> #define KERNEL_VERSION(a,b,c) (((a)<<16)+((b)<<8)+(c))  
# 原子变量读取
> kref_read(&freeme->ref)  
# 定时器任务初始化
> struct ceph_osd_client {...; struct delayed_work timeout_work; ...}  
> schedule_delayed_work(&timeout_work, timeout);  
初始化定时器任务及任务处理方法：INIT_DELAYED_WORK(&timeout_work, handle_timeout);  
把任务交给kworker线程处理：schedule_delayed_work(&timeout_work, timeout);  
由work找到ceph_osd_client：container_of(work, struct ceph_osd_client, timeout_work.work);  
让任务定时循环执行：在handle_timeout函数末尾重新调用schedule_delayed_work(&timeout_work, timeout);  
# procfs
## procfs 创建
```c
static const struct file_operations lockd_end_grace_operations = {
	.write		= nlm_end_grace_write,
	.read		= nlm_end_grace_read,
	.llseek		= default_llseek,
	.release	= simple_transaction_release,
	.owner		= THIS_MODULE,
};

int __init
lockd_create_procfs(void)
{
	struct proc_dir_entry *entry;

	entry = proc_mkdir("fs/lockd", NULL);
	if (!entry)
		return -ENOMEM;
	entry = proc_create("nlm_end_grace", S_IRUGO|S_IWUSR, entry,
				 &lockd_end_grace_operations);
	if (!entry) {
		remove_proc_entry("fs/lockd", NULL);
		return -ENOMEM;
	}
	return 0;
}
```
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
