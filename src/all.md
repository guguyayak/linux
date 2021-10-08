# kthread_run 内核起线程做一件事
```c
#include <linux/kthread.h>

/*
 * Reclaim all locks on server host. We do this by spawning a separate
 * reclaimer thread.
 */
void
nlmclnt_recovery(struct nlm_host *host)
{
	struct task_struct *task;

	if (!host->h_reclaiming++) {
		nlm_get_host(host);
		task = kthread_run(reclaimer, host, "%s-reclaim", host->h_name);
		if (IS_ERR(task))
			printk(KERN_ERR "lockd: unable to spawn reclaimer "
				"thread. Locks for %s won't be reclaimed! "
				"(%ld)\n", host->h_name, PTR_ERR(task));
	}
}
```
# 内核执行用户态进程
```c
static int
nfsd4_umh_cltrack_upcall(char *cmd, char *arg, char *env0, char *env1)
{
	char *envp[3];
	char *argv[4];
	int ret;

	if (unlikely(!cltrack_prog[0])) {
		dprintk("%s: cltrack_prog is disabled\n", __func__);
		return -EACCES;
	}

	dprintk("%s: cmd: %s\n", __func__, cmd);
	dprintk("%s: arg: %s\n", __func__, arg ? arg : "(null)");
	dprintk("%s: env0: %s\n", __func__, env0 ? env0 : "(null)");
	dprintk("%s: env1: %s\n", __func__, env1 ? env1 : "(null)");

	envp[0] = env0;
	envp[1] = env1;
	envp[2] = NULL;

	argv[0] = (char *)cltrack_prog;
	argv[1] = cmd;
	argv[2] = arg;
	argv[3] = NULL;

	ret = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
	/*
	 * Disable the upcall mechanism if we're getting an ENOENT or EACCES
	 * error. The admin can re-enable it on the fly by using sysfs
	 * once the problem has been fixed.
	 */
	if (ret == -ENOENT || ret == -EACCES) {
		dprintk("NFSD: %s was not found or isn't executable (%d). "
			"Setting cltrack_prog to blank string!",
			cltrack_prog, ret);
		cltrack_prog[0] = '\0';
	}
	dprintk("%s: %s return value: %d\n", __func__, cltrack_prog, ret);

	return ret;
}
```
## log
```
Sep 13 03:41:10 dk4 kernel: nfsd4_umh_cltrack_upcall: cmd: create
Sep 13 03:41:10 dk4 kernel: nfsd4_umh_cltrack_upcall: arg: 4c696e7578204e465376342e30206c6f63616c686f73742e6c6f63616c646f6d61696e2f3139322e3136382e3138322e323034
Sep 13 03:41:10 dk4 kernel: nfsd4_umh_cltrack_upcall: env0: NFSDCLTRACK_CLIENT_HAS_SESSION=N
Sep 13 03:41:10 dk4 kernel: nfsd4_umh_cltrack_upcall: env1: NFSDCLTRACK_GRACE_START=1631243448
Sep 13 03:41:10 dk4 kernel: nfsd4_umh_cltrack_upcall: /sbin/nfsdcltrack return value: 0
```
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
## 示例
```c
struct delayed_work {
	struct work_struct work;
	struct timer_list timer;

	/* target workqueue and CPU ->timer uses to queue ->work */
	struct workqueue_struct *wq;
	int cpu;
};

struct lockd_net {
	...
	struct delayed_work grace_period_end;
	...
};

static void grace_ender(struct work_struct *grace)
{
	struct delayed_work *dwork = container_of(grace, struct delayed_work,
						  work);
	struct lockd_net *ln = container_of(dwork, struct lockd_net,
					    grace_period_end);

	locks_end_grace(&ln->lockd_manager);
}

static void set_grace_period(struct net *net)
{
	unsigned long grace_period = get_lockd_grace_period();
	struct lockd_net *ln = net_generic(net, lockd_net_id);

	locks_start_grace(net, &ln->lockd_manager);
	cancel_delayed_work_sync(&ln->grace_period_end);
	schedule_delayed_work(&ln->grace_period_end, grace_period);
}

static int lockd_init_net(struct net *net)
{
	struct lockd_net *ln = net_generic(net, lockd_net_id);

	INIT_DELAYED_WORK(&ln->grace_period_end, grace_ender);
	...
	return 0;
}

static int
lockd(void *vrqstp)
{
...
	cancel_delayed_work_sync(&ln->grace_period_end);
	locks_end_grace(&ln->lockd_manager);
	return 0;
}
```
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
## 文件读写
```c
static ssize_t
fault_disconnect_read(struct file *filp, char __user *user_buf,
		      size_t len, loff_t *offset)
{
	char *buffer = (char *)filp->private_data;
	size_t size;

	size = sprintf(buffer, "%u\n", rpc_inject_disconnect);
	return simple_read_from_buffer(user_buf, len, offset, buffer, size);
}

static ssize_t
fault_disconnect_write(struct file *filp, const char __user *user_buf,
		       size_t len, loff_t *offset)
{
	char buffer[16];

	if (len >= sizeof(buffer))
		len = sizeof(buffer) - 1;
	if (copy_from_user(buffer, user_buf, len))
		return -EFAULT;
	buffer[len] = '\0';
	if (kstrtouint(buffer, 10, &rpc_inject_disconnect))
		return -EINVAL;
	return len;
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
# snprintf
```c
static int rpc_ntop4(const struct sockaddr *sap,
                     char *buf, const size_t buflen)
{
        const struct sockaddr_in *sin = (struct sockaddr_in *)sap;

        return snprintf(buf, buflen, "%pI4", &sin->sin_addr);
}
```
