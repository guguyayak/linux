#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <linux/gpio.h>

#include "mmap_kernel.h"

#define DEVICE_NAME	"dfsmap"

#define FUN_START	printk("%s start\n", __func__)
#define FUN_END       printk("%s END\n", __func__)

static struct file *g_map_file = NULL;
struct mmap_buf g_mmap_buf;

struct file *get_dfs_map_file(void)
{
	return g_map_file;
}

static int dfs_map_open(struct inode *inode, struct file *file)
{
	FUN_START;
	file->private_data = 0;
	g_map_file = file;
	FUN_END;

	return 0;
}

static vm_fault_t dfs_mmap_fault(struct vm_fault *vmf)
{
	int *index = (int *)vmf->vma->vm_private_data;
	struct page *page;
	printk("%s index %d\n", __func__, *index);
	page = virt_to_page(g_mmap_buf.page_array[*index]);
	get_page(page);
	vmf->page = page;

	return 0;
}

static const struct vm_operations_struct dfs_mmap_vmops = {
	.fault = dfs_mmap_fault,
};

static int dfs_map_mmap3(struct file *filp, struct vm_area_struct *vma)
{
	unsigned int *index;

	index = (unsigned int *)kmalloc(PAGE_SIZE, GFP_KERNEL);

	spin_lock(&g_mmap_buf.lock);
	if (g_mmap_buf.map_index >= BUF_ARRAY_LEN)
	{
		g_mmap_buf.map_index = 0;
	}

	printk("map_endex: %d\n", g_mmap_buf.map_index);

	vma->vm_ops = &dfs_mmap_vmops;
	*index = g_mmap_buf.map_index;
	vma->vm_private_data = (void *)index;
	vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	g_mmap_buf.map_index++;
	spin_unlock(&g_mmap_buf.lock);

	return 0;
}

static long dfs_map_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return 0;
}

static long dfs_compat_ptr_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
        return 0;
}

static struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.open = dfs_map_open,
	.mmap = dfs_map_mmap3,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

int dfs_map_dev_init(void)
{
	int ret = 0;
	int i = 0;
	int order = 0;
	void *data_buf = NULL;

	FUN_START;

	ret = misc_register(&misc);
	if (ret)
	{
		goto out;
	}

	g_mmap_buf.map_index = 0;
	g_mmap_buf.init_index = 0;

	spin_lock_init(&g_mmap_buf.lock);

	for (i = 0; i < BUF_ARRAY_LEN; i++)
	{
		order = get_order(MMAP_BUF_SIZE);
		data_buf = (void *)__get_free_pages(GFP_KERNEL | __GFP_ZERO, order);
		if (data_buf == NULL)
		{
			ret = -ENOMEM;
			goto out_err;
		}
		else
		{
			memset(data_buf, 'a', MMAP_BUF_SIZE);
			g_mmap_buf.buf_array[i] = data_buf;
			g_mmap_buf.init_index++;
		}

		printk("mmap buf index : %d addr: %p\n", i, g_mmap_buf.buf_array[i]);
		clear_bit(i, g_mmap_buf.bit_map);
	}

	ret = 0;
	goto out;

out_err:
	for (i = 0; i < g_mmap_buf.init_index; i++)
	{
		int order = get_order(MMAP_BUF_SIZE);
		free_pages((unsigned long)g_mmap_buf.buf_array[i], order);
	}
	g_mmap_buf.init_index = 0;

out:
	FUN_END;
	return ret;
}

void dfs_map_dev_exit(void)
{
	int i = 0;
	
	FUN_START;

	misc_deregister(&misc);

	spin_lock(&g_mmap_buf.lock);
	for (i=0; i < g_mmap_buf.init_index; i++)
	{
		int order = get_order(MMAP_BUF_SIZE);
                free_pages((unsigned long)g_mmap_buf.buf_array[i], order);
	}
	g_mmap_buf.init_index = 0;
	g_mmap_buf.map_index = 0;
	spin_unlock(&g_mmap_buf.lock);

	FUN_END;
}

static int __init mmap_example_init(void)
{
	dfs_map_dev_init();
	return 0;
}

static void __exit mmap_example_exit(void)
{
	dfs_map_dev_exit();
	printk("unloaded mmap_example module\n");
}

MODULE_LICENSE("GPL");
module_init(mmap_example_init);
module_exit(mmap_example_exit);
