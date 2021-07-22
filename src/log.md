# 日志打印技巧
## 由dentry打印文件名
```c
printk(KERN_ERR
	"nfsd_create: parent %pd2 not locked!\n",
	dentry);
```
## 打印调用栈
> dump_stack();
