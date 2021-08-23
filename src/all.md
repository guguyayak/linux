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
