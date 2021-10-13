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

## 一次向一个文件写多个数字处理方式
```c
/**
 * write_pool_threads - Set or report the current number of threads per pool
 *
 * Input:
 *			buf:		ignored
 *			size:		zero
 *
 * OR
 *
 * Input:
 * 			buf:		C string containing whitespace-
 * 					separated unsigned integer values
 *					representing the number of NFSD
 *					threads to start in each pool
 *			size:		non-zero length of C string in @buf
 * Output:
 *	On success:	passed-in buffer filled with '\n'-terminated C
 *			string containing integer values representing the
 *			number of NFSD threads in each pool;
 *			return code is the size in bytes of the string
 *	On error:	return code is zero or a negative errno value
 */
static ssize_t write_pool_threads(struct file *file, char *buf, size_t size)
{
	/* if size > 0, look for an array of number of threads per node
	 * and apply them  then write out number of threads per node as reply
	 */
	char *mesg = buf;
	int i;
	int rv;
	int len;
	int npools;
	int *nthreads;
	struct net *net = netns(file);

	mutex_lock(&nfsd_mutex);
	npools = nfsd_nrpools(net);
	if (npools == 0) {
		/*
		 * NFS is shut down.  The admin can start it by
		 * writing to the threads file but NOT the pool_threads
		 * file, sorry.  Report zero threads.
		 */
		mutex_unlock(&nfsd_mutex);
		strcpy(buf, "0\n");
		return strlen(buf);
	}

	nthreads = kcalloc(npools, sizeof(int), GFP_KERNEL);
	rv = -ENOMEM;
	if (nthreads == NULL)
		goto out_free;

	if (size > 0) {
		for (i = 0; i < npools; i++) {
			rv = get_int(&mesg, &nthreads[i]);
			if (rv == -ENOENT)
				break;		/* fewer numbers than pools */
			if (rv)
				goto out_free;	/* syntax error */
			rv = -EINVAL;
			if (nthreads[i] < 0)
				goto out_free;
		}
		rv = nfsd_set_nrthreads(i, nthreads, net);
		if (rv)
			goto out_free;
	}

	rv = nfsd_get_nrthreads(npools, nthreads, net);
	if (rv)
		goto out_free;

	mesg = buf;
	size = SIMPLE_TRANSACTION_LIMIT;
	for (i = 0; i < npools && size > 0; i++) {
		snprintf(mesg, size, "%d%c", nthreads[i], (i == npools-1 ? '\n' : ' '));
		len = strlen(mesg);
		size -= len;
		mesg += len;
	}
	rv = mesg - buf;
out_free:
	kfree(nthreads);
	mutex_unlock(&nfsd_mutex);
	return rv;
}
```
