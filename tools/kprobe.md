# kprobe使用
> function_enter中入参regs保存寄存器值，如regs->di就是第一个函数入参；   
> RDI, RSI, RDX, RCX, R8, R9分别表示当前函数的第1-6个传递参数，RDI对应第一个参数，RSI对应第二个参数，如此顺序对应。   
> 但是不同操作系统可能不同，具体看struct pt_regs定义；   
```c
// kernel_func_param.c

#include<linux/module.h>
#include<linux/mm.h>
#include<linux/kprobes.h>
#include<linux/fs.h>

#define FUNCTION_NAME "nfs_lookup"

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp;

//struct dentry *nfs_lookup(struct inode *dir, struct dentry * dentry, unsigned int flags)
static int function_enter(struct kprobe *p, struct pt_regs *regs)
{
	struct inode * parent = (struct inode *)regs->di;
	struct dentry * dentry = (struct dentry *)regs->si;

	printk("nfs_lookup parent(%ld) file(%s)\n", parent->i_ino, dentry->d_name.name);
	return 0;
}

static int __init kprobe_init(void)
{
	int ret = 0;

	/*function enter*/
	kp.symbol_name = FUNCTION_NAME;
	kp.pre_handler = function_enter;
	ret = register_kprobe(&kp);
	if (ret < 0) {
		printk("register_kprobe failed, returned %d\n", ret);
		return ret;
	}

	printk("register kprobe at %s\n", FUNCTION_NAME);
	return 0;
}

void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	printk("kprobe at %s unregistered\n", FUNCTION_NAME);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");

// Makefile

bj-m := kernel_func_param.o

CROSS_COMPILE=''

KDIR := /lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(PWD) modules 
clean:
	rm -f *.ko *.o *.mod.o *.mod.c .*.cmd *.symvers  modul* *.unsigned
```
