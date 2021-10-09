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
