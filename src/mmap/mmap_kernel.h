#ifndef _MMAP_KERNEL_H
#define _MMAP_KERNEL_H

#define BUF_ARRAY_LEN	1024
#define MMAP_BUF_SIZE	512 * 1024

struct mmap_buf {
	spinlock_t lock;
	int init_index;
	int map_index;
	DECLARE_BITMAP(bit_map, BUF_ARRAY_LEN);
	void *buf_array[BUF_ARRAY_LEN];
	struct page *page_array[BUF_ARRAY_LEN][MMAP_BUF_SIZE / PAGE_SIZE];
};

int dfs_map_dev_init(void);
void dfs_map_dev_exit(void);
struct file *get_dfs_map_file(void);

#endif
