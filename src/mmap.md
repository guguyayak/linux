# 映射大页
要映射大页，可以按照以下步骤进行操作：

1. 在内核模块中定义一个大页的大小，例如2MB或1GB。可以使用`PAGE_SIZE`宏来获取系统默认的页面大小，然后将其乘以相应的倍数。

```c
#define LARGE_PAGE_SIZE (512 * PAGE_SIZE) // 2MB
```

2. 在`my_mmap`函数中，检查映射区域的大小是否符合大页的要求。

```c
if (vma->vm_end - vma->vm_start > LARGE_PAGE_SIZE) {
    printk(KERN_ERR "my_mmap: invalid mmap size\n");
    return -EINVAL;
}
```

3. 使用`__get_free_pages`函数来分配大页的物理内存。

```c
unsigned long addr = __get_free_pages(GFP_KERNEL, get_order(LARGE_PAGE_SIZE));
if (!addr) {
    printk(KERN_ERR "my_mmap: failed to allocate large page\n");
    return -ENOMEM;
}
```

4. 使用`remap_pfn_range`函数将大页映射到用户空间。

```c
if (remap_pfn_range(vma, vma->vm_start, addr >> PAGE_SHIFT, LARGE_PAGE_SIZE, vma->vm_page_prot)) {
    printk(KERN_ERR "my_mmap: failed to remap large page\n");
    return -EAGAIN;
}
```

5. 保存大页的物理地址到`vma->vm_private_data`，以便在`my_mmap`函数返回后可以释放大页。

```c
vma->vm_private_data = (void *)addr;
```

6. 在`my_mmap`函数返回前，将大页的物理地址保存到`vma->vm_private_data`。

```c
vma->vm_private_data = (void *)addr;
```

7. 在`my_munmap`函数中，释放大页的物理内存。

```c
unsigned long addr = (unsigned long)vma->vm_private_data;
free_pages(addr, get_order(LARGE_PAGE_SIZE));
```

这样，你就可以在自定义的`mmap`函数中映射大页了。请注意，映射大页可能需要特权访问权限，并且需要确保硬件和操作系统支持大页功能。

# 内核程序：largepage_mmap.c
```c
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/cdev.h>

#define DEVICE_NAME "largepage"
#define LARGE_PAGE_SIZE (PAGE_SIZE * 512) // 2MB
#define LARGE_PAGE_ORDER get_order(LARGE_PAGE_SIZE)

static dev_t devno;
static struct cdev largepage_cdev;
static struct class *largepage_class;

static int largepage_open(struct inode *inode, struct file *file)
{
    pr_info("largepage_open\n");
    return 0;
}

static int largepage_release(struct inode *inode, struct file *file)
{
    pr_info("largepage_release\n");
    return 0;
}

static void largepage_vma_close(struct vm_area_struct *vma)
{
    unsigned long addr = (unsigned long)vma->vm_private_data;
    if (addr) {
        pr_info("largepage_vma_close: free pages at %lx\n", addr);
        free_pages(addr, LARGE_PAGE_ORDER);
    }
}

static const struct vm_operations_struct largepage_vm_ops = {
    .close = largepage_vma_close,
};

static int largepage_mmap(struct file *file, struct vm_area_struct *vma)
{
    unsigned long addr;
    unsigned long size = vma->vm_end - vma->vm_start;

    pr_info("largepage_mmap: requested size = %lu bytes\n", size);

    if (size != LARGE_PAGE_SIZE) {
        pr_err("only support exactly %lu bytes mapping\n", LARGE_PAGE_SIZE);
        return -EINVAL;
    }

    addr = __get_free_pages(GFP_KERNEL | __GFP_ZERO, LARGE_PAGE_ORDER);
    if (!addr) {
        pr_err("failed to allocate large page\n");
        return -ENOMEM;
    }

    if (remap_pfn_range(vma, vma->vm_start,
                        page_to_pfn(virt_to_page((void *)addr)),
                        size, vma->vm_page_prot)) {
        pr_err("remap_pfn_range failed\n");
        free_pages(addr, LARGE_PAGE_ORDER);
        return -EAGAIN;
    }

    vma->vm_private_data = (void *)addr;
    vma->vm_ops = &largepage_vm_ops;
    return 0;
}

static const struct file_operations largepage_fops = {
    .owner   = THIS_MODULE,
    .open    = largepage_open,
    .release = largepage_release,
    .mmap    = largepage_mmap,
};

static int __init largepage_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret < 0)
        return ret;

    cdev_init(&largepage_cdev, &largepage_fops);
    ret = cdev_add(&largepage_cdev, devno, 1);
    if (ret < 0) {
        unregister_chrdev_region(devno, 1);
        return ret;
    }

    largepage_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(largepage_class)) {
        cdev_del(&largepage_cdev);
        unregister_chrdev_region(devno, 1);
        return PTR_ERR(largepage_class);
    }

    device_create(largepage_class, NULL, devno, NULL, DEVICE_NAME);

    pr_info("largepage: loaded, device /dev/%s major=%d minor=%d\n",
            DEVICE_NAME, MAJOR(devno), MINOR(devno));
    return 0;
}

static void __exit largepage_exit(void)
{
    device_destroy(largepage_class, devno);
    class_destroy(largepage_class);
    cdev_del(&largepage_cdev);
    unregister_chrdev_region(devno, 1);
    pr_info("largepage: unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("H3C Demo");
MODULE_DESCRIPTION("Large Page mmap demo with auto /dev");

module_init(largepage_init);
module_exit(largepage_exit);
```
# 用户态测试代码 test_largepage.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>

#define DEVICE_PATH "/dev/largepage"
#define LARGE_PAGE_SIZE (4096 * 512) // 2MB

int main()
{
    int fd;
    char *map;

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    map = mmap(NULL, LARGE_PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    printf("Mapped address: %p\n", map);
    strcpy(map, "Hello Large Page from user space!\n");

    printf("Data in mapped memory: %s", map);

    if (munmap(map, LARGE_PAGE_SIZE) == -1)
        perror("munmap");

    close(fd);
    return 0;
}
```
# Makefile
```
obj-m := largepage_mmap.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```
