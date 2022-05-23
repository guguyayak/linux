# 新增模块示例
## c文件 & Makefile
```c
[root@dk4 kmallock_test]# cat kmalloc_test.c
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>

void *ptr[10005];

static int __init kmalloc_test_init(void) {
        int i;
        for (i = 0; i < 10000; i++) {
                ptr[i] = kmalloc(80, GFP_KERNEL);
        }
        return 0;
}

void __exit kmalloc_test_exit(void)
{
        int i;
        for (i = 0; i < 10000; i++) {
                kfree(ptr[i]);
        }
}

module_init(kmalloc_test_init)
module_exit(kmalloc_test_exit)
MODULE_LICENSE("GPL");

[root@dk4 kmallock_test]# cat Makefile
obj-m := kmalloc_test.o

KDIR := /lib/modules/$(shell uname -r)/build
all:
        make -C $(KDIR) M=$(PWD) modules
clean:
        rm -f *.ko *.o *.mod.o *.mod.c .*.cmd *.symvers modul* *.unsigned
```
# modprobe 加载模块传参
```c
/* upcall via usermodehelper */
static char cltrack_prog[PATH_MAX] = "/sbin/nfsdcltrack";
module_param_string(cltrack_prog, cltrack_prog, sizeof(cltrack_prog),
			S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(cltrack_prog, "Path to the nfsdcltrack upcall program");

static bool cltrack_legacy_disable;
module_param(cltrack_legacy_disable, bool, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(cltrack_legacy_disable,
		"Disable legacy recoverydir conversion. Default: false");
```
> nfsd 模块加载参数查看路径：/sys/module/nfsd/parameters/   
# modprobe 加载模块传参配置文件
> /etc/modprobe.d/lockd.conf   
