# page shift mask
```c
define PAGE_MASK      (~(PAGE_SIZE-1))

/*
 * The page cache can done in larger chunks than
 * one page, because it allows for more efficient
 * throughput (it can then be mapped into user
 * space in smaller chunks for same flexibility).
 *
 * Or rather, it _will_ be done in larger chunks.
 */
#define PAGE_CACHE_SHIFT	PAGE_SHIFT
#define PAGE_CACHE_SIZE		PAGE_SIZE
#define PAGE_CACHE_MASK		PAGE_MASK
#define PAGE_CACHE_ALIGN(addr)	(((addr)+PAGE_CACHE_SIZE-1)&PAGE_CACHE_MASK)

#define page_cache_get(page)		get_page(page)
#define page_cache_release(page)	put_page(page)
```
# 内核slab使用
```c
static struct kmem_cache *file_slab;

void
nfsd4_free_slabs(void)
{
	...
	kmem_cache_destroy(file_slab);
	...
}

int
nfsd4_init_slabs(void)
{
	...
	file_slab = kmem_cache_create("nfsd4_files",
			sizeof(struct nfs4_file), 0, 0, NULL);
	if (file_slab == NULL)
		goto out_free_lockowner_slab;
	...
}

static struct nfs4_file *nfsd4_alloc_file(void)
{
	return kmem_cache_alloc(file_slab, GFP_KERNEL);
}

static void nfsd4_free_file_rcu(struct rcu_head *rcu)
{
	struct nfs4_file *fp = container_of(rcu, struct nfs4_file, fi_rcu);

	kmem_cache_free(file_slab, fp);
}
```

# [unable to find new object create with kmem_cache_create() in /proc/slabinfo](https://stackoverflow.com/questions/24858424/unable-to-find-new-object-create-with-kmem-cache-create-in-proc-slabinfo/24872230)

# [slab介绍](https://blog.simowce.com/2015/11/01/linux-memory-managent-slab-allocator-and-kmalloc/)
> 为什么使用slab？   
1. linux系统的内存管理使用伙伴系统，最小单元是 page，4k。当要申请小内存时，会造成浪费。slab可解决此问题，因为slab中对象是固定大小的，方便组织管理；
2. 对于系统频繁申请释放的对象（如task_struct），使用slab可提升性能、减少内存碎片；
3. slab会预分配内存，提高获取对象的效率；
4. 内核对象中相当一部分成员需要某些特殊的初始化（例如队列头部）而并非简单地清成全 0。如果能充分重用已被释放的对象使得下次分配时无需初始化，那么可以提高内核的运行效率。
> 需要深入学习的点
1. slab对象对齐导致只会映射到几个特定的hash，加剧cache miss。为了使slab对象均匀映射到所有的hash空间中，加入了着色偏移的方法。着色即为添加偏移。
# numa相关接口函数
```c
/*
 * Bitmasks that are kept for all the nodes.
 */
enum node_states {
	N_POSSIBLE,		/* The node could become online at some point */
	N_ONLINE,		/* The node is online */
	N_NORMAL_MEMORY,	/* The node has regular memory */
#ifdef CONFIG_HIGHMEM
	N_HIGH_MEMORY,		/* The node has regular or high memory */
#else
	N_HIGH_MEMORY = N_NORMAL_MEMORY,
#endif
#ifdef CONFIG_MOVABLE_NODE
	N_MEMORY,		/* The node has memory(regular, high, movable) */
#else
	N_MEMORY = N_HIGH_MEMORY,
#endif
	N_CPU,		/* The node has one or more cpus */
	NR_NODE_STATES
};

#define num_memory_nodes()	num_node_state(N_HIGH_MEMORY)
/*
 * Determine the per node value of a stat item. This function
 * is called frequently in a NUMA machine, so try to be as
 * frugal as possible.
 */
static inline unsigned long node_page_state(int node,
				 enum zone_stat_item item)
#if LINX_VERSION_CODE >= kernel_verdion(4,4,0)
#define node_free_pages(nid)	g_kallsyms.node_page_state(nid, NR_FREE_PAGES)
#else
#define node_free_pages(nid)	node_page_state(nid, NR_FREE_PAGES)
#endif
```
